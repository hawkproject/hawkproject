#include "HawkGwControler.h"
#include "HawkGateway.h"

namespace Hawk
{
	HawkGwControler::HawkGwControler(HawkGateway* pGateway) : m_pGateway(pGateway), m_pControler(0), m_pIdentify(0), m_pCommand(0)
	{
	}

	HawkGwControler::~HawkGwControler()
	{
		HAWK_RELEASE(m_pIdentify);
		HAWK_RELEASE(m_pCommand);
		P_ZmqManager->CloseZmq(m_pControler);
	}

	Bool HawkGwControler::Init(const AString& sAddr)
	{
		if (!m_pIdentify)
			m_pIdentify = new OctetsStream(PAGE_SIZE);

		if (!m_pCommand)
			m_pCommand = new OctetsStream(PAGE_SIZE);

		if (!m_pControler)
		{
			m_pControler = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_ROUTER);
			if (!m_pControler->Bind(sAddr))
			{
				HawkFmtPrint("GwControler Bind Error, Addr: %s", sAddr.c_str());
				return false;
			}
		}		
		
		return true;
	}

	Bool HawkGwControler::UpdateEvent()
	{
		if (m_pIdentify && m_pCommand && m_pControler && m_pControler->PollEvent(HEVENT_READ, 0))
		{
			//提取标识信息
			m_pIdentify->Clear();
			Size_t iSize = (Size_t)m_pIdentify->Capacity();
			if (!m_pControler->Recv(m_pIdentify->Begin(), iSize))
				return false;
			m_pIdentify->Resize(iSize);

			//提取命令数据
			m_pCommand->Clear();
			iSize = (Size_t)m_pCommand->Capacity();
			if (!m_pControler->Recv(m_pCommand->Begin(), iSize))
				return false;
			m_pCommand->Resize((UInt32)iSize);

			if (m_pGateway)
				OnCommand((GateCommand*)m_pCommand->Begin(), m_pIdentify);

			return true;
		}
		return false;
	}

	Bool HawkGwControler::OnCommand(const GateCommand* pCmd, const OctetsStream* pIdentify)
	{
		if (m_pGateway)
		{
			if (pCmd->Type == GateCommand::CMD_IPSECURITY)
			{
				m_pGateway->SetIpPeerLimit(pCmd->eIpSecurity.IpPeerLimit);
				m_pGateway->SetSessionProtoFreq(pCmd->eIpSecurity.ProtocolFreq);			
			}
			else if (pCmd->Type == GateCommand::CMD_WHITELIST)
			{
				if (pCmd->eWhiteList.AddIp)
					m_pGateway->AddWhiteIp(pCmd->eWhiteList.IpAddr);
				else
					m_pGateway->DelWhiteIp(pCmd->eWhiteList.IpAddr);
			}
			else if (pCmd->Type == GateCommand::CMD_BLACKLIST)
			{
				if (pCmd->eBlackList.AddIp)
					m_pGateway->AddBlackIp(pCmd->eBlackList.IpAddr);
				else
					m_pGateway->DelBlackIp(pCmd->eBlackList.IpAddr);
			}
			else if (pCmd->Type == GateCommand::CMD_PROF_REQ)
			{
				if (m_pGateway->GetProfiler())
				{
					GateCommand sCmd(GateCommand::CMD_PROF_INFO);
					
					sCmd.eProfInfo.TimeStamp = HawkOSOperator::GetTickCount();;
					sCmd.eProfInfo.CpuCount  = m_pGateway->GetProfiler()->GetProcessorNumber();
					sCmd.eProfInfo.TotalMem  = m_pGateway->GetProfiler()->GetTotalMem();
					sCmd.eProfInfo.CpuUsage  = m_pGateway->GetProfiler()->GetCpuUsage();
					sCmd.eProfInfo.MemUsage  = m_pGateway->GetProfiler()->GetMemUsage();
					sCmd.eProfInfo.Connect   = m_pGateway->GetProfiler()->GetPeerCount();
					sCmd.eProfInfo.RecvProto = m_pGateway->GetProfiler()->GetRecvProto();
					sCmd.eProfInfo.RecvSize  = m_pGateway->GetProfiler()->GetRecvSize();
					sCmd.eProfInfo.SendProto = m_pGateway->GetProfiler()->GetSendProto();
					sCmd.eProfInfo.SendSize  = m_pGateway->GetProfiler()->GetSendSize();

					//回应性能信息请求
					m_pControler->Send((void*)pIdentify->Begin(), pIdentify->Size(), HawkZmq::HZMQ_SNDMORE);
					m_pControler->Send(&sCmd, sizeof(sCmd));
				}
			}
		}		

		return true;
	}

	Bool HawkGwControler::Stop()
	{
		HAWK_RELEASE(m_pIdentify);
		HAWK_RELEASE(m_pCommand);
		P_ZmqManager->CloseZmq(m_pControler);
		
		return true;
	}
}
