#include "HawkGateway.h"
#include "HawkGateSecurity.h"
#include "event.h"
#include "zmq.h"

namespace Hawk
{
	HawkGateway::HawkGateway()
	{
		m_iThread		= 0;
		m_iTurnIdx		= 0;
		m_iBaseSid		= 0;
		m_ppThread		= 0;
		m_pCrossDomain	= 0;
		m_pProfiler		= 0;
		m_pThreadZmq	= 0;
		m_pServerZmq	= 0;
		m_pMonitorZmq	= 0;
		m_pOctets		= 0;
		m_iTimeout		= 0;	
		m_iProtoFreq	= 0;
		m_iIpPeerLimit	= 0;
		m_pControler	= 0;		
		m_bIdle			= true;
		m_bBlocking		= true;
		m_bWaitIdle		= false;
		m_bRunning		= false;	
		m_bSecurity		= false;
		m_bHoldConn		= false;
		m_iBufSize		= PAGE_SIZE * 2;
		m_iGateMode		= GATEMODE_SINGLE_SERVICE;
		m_iSecuType		= GATESECURITY_NONE;
	}

	HawkGateway::~HawkGateway()
	{
		//�ͷ��߳�
		for (Int32 i=0;m_ppThread && i<m_iThread;i++)
		{
			HAWK_RELEASE(m_ppThread[i]);
		}
		HAWK_DELETE_ARRAY(m_ppThread);

		//�رռ����׽���
		m_sSocket.Close();

		//�ͷ���Ϣ����
		HAWK_RELEASE(m_pOctets);
		
		//�ͷ����ܼ�����
		HAWK_RELEASE(m_pProfiler);

		//�ͷ����ؿ�����
		HAWK_RELEASE(m_pControler);

		//�ر���Ϣ����
		P_ZmqManager->CloseZmq(m_pMonitorZmq);
		P_ZmqManager->CloseZmq(m_pServerZmq);
		P_ZmqManager->CloseZmq(m_pThreadZmq);

		//�ͷſ����߳�
		HAWK_RELEASE(m_pCrossDomain);
	}

	void HawkGateway::SetBufSize(Int32 iBufSize)
	{
		m_iBufSize = iBufSize;
	}

	Int32 HawkGateway::GetBufSize() const
	{
		return m_iBufSize;
	}

	Bool HawkGateway::SetWaitIdle(Bool bWaitIdle)
	{
		m_bWaitIdle = bWaitIdle;
		return true;
	}

	Bool HawkGateway::IsWaitIdle() const
	{
		return m_bWaitIdle;
	}

	Bool HawkGateway::IsRunning() const
	{
		return m_bRunning;
	}

	void HawkGateway::SetHoldConnection(Bool bHold)
	{
		m_bHoldConn = bHold;
	}

	Bool HawkGateway::IsHoldConnection() const
	{
		return m_bHoldConn;
	}

	void HawkGateway::SetSessionTimeout(Int32 iTimeout)
	{
		m_iTimeout = iTimeout;
	}

	Int32 HawkGateway::GetSessionTimeout() const
	{
		return m_iTimeout;
	}

	void HawkGateway::SetIpPeerLimit(UInt32 iLimit)
	{
		m_iIpPeerLimit = iLimit;
	}

	UInt32 HawkGateway::GetIpPeerLimit() const
	{
		return m_iIpPeerLimit;
	}

	Int32 HawkGateway::GetGateMode() const
	{
		return m_iGateMode;
	}
	
	void HawkGateway::SetSessionProtoFreq(UInt32 iFreq)
	{
		m_iProtoFreq = iFreq;
	}

	UInt32 HawkGateway::GetSessionProtoFreq() const
	{
		return m_iProtoFreq;
	}

	Int32  HawkGateway::GetThreadNum() const
	{
		return m_iThread;
	}

	Int32  HawkGateway::GetThreadId(Int32 iIdx) const
	{
		if (iIdx >= 0 && iIdx < m_iThread && m_ppThread && m_ppThread[iIdx])
			return m_ppThread[iIdx]->GetThreadId();
		
		return 0;
	}

	Bool HawkGateway::SetBlocking(Bool bBlocking)
	{
		m_bBlocking = bBlocking;
		return true;
	}

	Bool HawkGateway::CreateGateThread(HawkGateThread*& pThread)
	{
		pThread = new HawkGateThread(this);
		return pThread != 0;
	}

	Bool HawkGateway::CreateISecurity(HawkSecurity*& pSecurity, UInt32 iSecurity, HawkOctets* pParam)
	{
		pSecurity = 0;
		if (m_bSecurity)
		{
			//Ĭ�Ͻ���ģʽ
			if (iSecurity == GATESECURITY_NONE && m_iSecuType != GATESECURITY_NONE)
			{
				OctetsStream xOS;
				xOS.Push(m_sSecuKey.c_str(), m_sSecuKey.size());
				return CreateISecurity(pSecurity, m_iSecuType, &xOS);
			}
			else if (iSecurity == GATESECURITY_RC4)
			{
				pSecurity = new HawkRC4Security();

				if (pParam)
					pSecurity->SetParameter(*pParam);
			}
			else if (iSecurity == GATESECURITY_XOR)
			{
				pSecurity = new HawkXorSecurity();

				if (pParam)
					pSecurity->SetParameter(*pParam);
			}
			else if (iSecurity == GATESECURITY_ZLIB)
			{
				pSecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_DECODE);
			}
		}

		return true;
	}

	Bool HawkGateway::CreateOSecurity(HawkSecurity*& pSecurity, UInt32 iSecurity, HawkOctets* pParam)
	{
		pSecurity = 0;
		if (m_bSecurity)
		{
			//Ĭ�ϼ���ģʽ
			if (iSecurity == GATESECURITY_NONE && m_iSecuType != GATESECURITY_NONE)
			{
				OctetsStream xOS;
				xOS.Push(m_sSecuKey.c_str(), m_sSecuKey.size());
				return CreateOSecurity(pSecurity, m_iSecuType, &xOS);
			}
			else if (iSecurity == GATESECURITY_RC4)
			{
				pSecurity = new HawkRC4Security();

				if (pParam)
					pSecurity->SetParameter(*pParam);
			}
			else if (iSecurity == GATESECURITY_XOR)
			{
				pSecurity = new HawkXorSecurity();

				if (pParam)
					pSecurity->SetParameter(*pParam);
			}
			else if (iSecurity == GATESECURITY_ZLIB)
			{
				pSecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_ENCODE);
			}
		}
		return true;
	}

	Bool HawkGateway::Init(const AString& sFrontend, const AString& sBackend, Int32 iThread, Int32 iGateMode)
	{
		HawkAssert(sFrontend.size() && sBackend.size() && iThread > 0);

		if (!sFrontend.size() || !sBackend.size() || iThread <= 0)
			return false;

		//������ֵ
		m_iThread   = iThread;
		m_iGateMode = iGateMode;

		//����ͨ�û���
		if (!m_pOctets)
			m_pOctets = new OctetsStream(m_iBufSize);

		//�����������׽���
		if (!m_sSocket.IsValid())
		{
			if (!m_sSocket.InitTcpServer(sFrontend))
			{
				HawkFmtPrint("Init TcpServer Error, Addr: %s, ErrCode: %d", sFrontend.c_str(), m_sSocket.GetSocketErr());
				return false;
			}
		}		

		//���������߳�ͨ�Ŷ���
		if (!m_pThreadZmq)
		{
			m_pThreadZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_ROUTER);
			if (!m_pThreadZmq->Bind(GetThreadZmqAddr()))
			{
				HawkFmtPrint("ThreadZmq Bind Error, Addr: %s", GetThreadZmqAddr().c_str());
				return false;
			}
		}

		//������˷���ͨ�Ŷ���
		if (!m_pServerZmq)
		{
			AString sAddr = sBackend;
			if (sAddr.find("tcp://") == AString::npos)
				sAddr = "tcp://" + sBackend;

			m_pServerZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_ROUTER);

			//����ָ���¼�������
			m_pServerZmq->StartMonitor(HawkZmq::HZMQ_EVENT_ACCEPTED | HawkZmq::HZMQ_EVENT_DISCONNECTED, GetServerZmqAddr());
			//�󶨵���ַ
			if (!m_pServerZmq->Bind(sAddr))
			{
				HawkFmtPrint("ServerZmq Bind Error, Addr: %s", sAddr.c_str());
				return false;
			}			
		}
		
		//�������������
		if (!m_pMonitorZmq && m_pServerZmq)
		{
			m_pMonitorZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_PAIR);
			//���ӵ�������
			if (!m_pMonitorZmq->Connect(GetServerZmqAddr()))
			{
				HawkFmtPrint("MonitorZmq Connect Error, Addr: %s", GetServerZmqAddr().c_str());
				return false;
			}
		}

		//���������߳�
		m_ppThread = new HawkGateThread*[m_iThread];
		memset(m_ppThread, 0, sizeof(HawkGateThread*) * m_iThread);

		tm xTM = HawkOSOperator::GetSysClock();
		//ǿ���趨��8λ, �����������»ỰID�ظ�������Э�鵼�����
		m_iBaseSid = (xTM.tm_sec << 24) & 0xFF000000;

		for (UInt32 i=0;i<(UInt32)m_iThread;i++)
		{
			//���������߳�
			if (!CreateGateThread(m_ppThread[i]))
			{
				HawkPrint("Create GateThread Failed.");
				return false;
			}

			//��ʼ�������߳�
			if (!m_ppThread[i]->Init(i+1, m_iBaseSid, (UInt32)m_iThread))
			{
				HawkPrint("Init GateThread Failed.");
				return false;
			}

			//���������߳�
			if (!m_ppThread[i]->Start())
			{
				HawkPrint("Start GateThread Failed.");
				return false;
			}
		}		
		return true;
	}

	Bool HawkGateway::InitCfg(const AString& sCfg, const AString& sSecurity)
	{
		HawkKVFile kvFile;
		if (!kvFile.Open(sCfg))
			return false;

		//��ʼ����������
		AString sFrontend, sBackend, sThread, sGateMode, sValue;
		if (!kvFile.QueryValue("FrontendAddr", sFrontend) || !sFrontend.size())
		{
			HawkFmtPrint("Gateway InitCfg Error, FrontendAddr: %s", sFrontend.c_str());
			return false;
		}

		if (!kvFile.QueryValue("BackendAddr", sBackend) || !sBackend.size())
		{
			HawkFmtPrint("Gateway InitCfg Error, BackendAddr: %s", sBackend.c_str());
			return false;
		}

		if (!kvFile.QueryValue("ThreadCount", sThread) || !sThread.size())
		{
			HawkFmtPrint("Gateway InitCfg Error, ThreadCount: %s", sThread.c_str());
			return false;
		}

		if (!kvFile.QueryValue("GateMode", sGateMode) || !sGateMode.size())
		{
			HawkFmtPrint("Gateway InitCfg Error, GateMode: %s", sGateMode.c_str());
			return false;
		}

		//������ʼ��
		if (Init(sFrontend, sBackend, HawkStringUtil::StringToInt<AString>(sThread), HawkStringUtil::StringToInt<AString>(sGateMode)))
		{
			HawkFmtPrint("Gateway InitCfg Ok.");	
		
			//��ӡ��ʼ������
			HawkFmtPrint("Gateway InitCfg, FrontendAddr: %s", sFrontend.c_str());
			HawkFmtPrint("Gateway InitCfg, BackendAddr: %s",  sBackend.c_str());
			HawkFmtPrint("Gateway InitCfg, ThreadCount: %s",  sThread.c_str());
			HawkFmtPrint("Gateway InitCfg, GateMode: %s",     sGateMode.c_str());
		}
		else
		{			
			HawkFmtPrint("Gateway InitCfg Failed, FrontendAddr: %s, BackendAddr: %s, ThreadCount: %s, GateMode: %s", 
				sFrontend.c_str(), sBackend.c_str(), sThread.c_str(), sGateMode.c_str());

			return false;
		}

		//���ؿ�����
		if (kvFile.QueryValue("ControlerAddr", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, ControlerAddr: %s", sValue.c_str());
			InitControler(sValue);
		}

		//���ܼ�����
		if (kvFile.QueryValue("ProfilerAddr", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, ProfilerAddr: %s", sValue.c_str());
			TurnOnProfiler(sValue);
		}

		//ѭ�����Ʊ���
		if (kvFile.QueryValue("WaitUntilIdle", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, WaitUntilIdle: %s", sValue.c_str());
			SetWaitIdle(HawkStringUtil::StringToBool<AString>(sValue));
		}

		//�Ự����
		if (kvFile.QueryValue("SecurityEnable", sValue) && sValue.size())
		{			
			Bool bEnable = HawkStringUtil::StringToBool<AString>(sValue);
			if (bEnable)
			{
				AString sSecuType, sSecuKey;
				if (kvFile.QueryValue("SecurityMode", sSecuType))
				{
					m_iSecuType = HawkStringUtil::StringToUInt<AString>(sSecuType);					
					if (m_iSecuType != GATESECURITY_NONE)
					{
						kvFile.QueryValue("SecurityKey", sSecuKey);
						EnableGateSecurity(bEnable, m_iSecuType, sSecuKey);						
						HawkFmtPrint("Gateway InitCfg, SecurityEnable: %s, SecurityMode: %s, SecurityKey: %s", sValue.c_str(), sSecuType.c_str(), sSecuKey.c_str());
					}					
				}				
			}			
		}

		//�Ự���г�ʱ
		if (kvFile.QueryValue("SessionTimeout", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, SessionTimeout: %s", sValue.c_str());
			SetSessionTimeout(HawkStringUtil::StringToInt<AString>(sValue));
		}

		//���߱���
		if (kvFile.QueryValue("HoldConnection", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, HoldConnection: %s", sValue.c_str());
			SetHoldConnection(HawkStringUtil::StringToBool<AString>(sValue));
		}

		//Э��Ƶ��
		if (kvFile.QueryValue("ProtocolFreq", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, ProtocolFreq: %s", sValue.c_str());
			SetSessionProtoFreq(HawkStringUtil::StringToInt<AString>(sValue));
		}

		//ip����������
		if (kvFile.QueryValue("IpPeerLimit", sValue) && sValue.size())
		{
			HawkFmtPrint("Gateway InitCfg, IpPeerLimit: %s", sValue.c_str());
			SetIpPeerLimit(HawkStringUtil::StringToInt<AString>(sValue));
		}

		//���ذ�ȫ����
		AXmlDocument xmlDoc;
		HawkXmlFile  xmlFile;
		if (xmlFile.Open<AString>(sSecurity, xmlDoc) && xmlDoc.GetRoot()->GetTag() == "GwSecurity")
		{
			//��ʼ��ip�ڰ���������
			AXmlElement* pIpSecurity = xmlDoc.GetRoot()->GetChildren("IpSecurity");
			if (pIpSecurity)
			{
				//������
				AXmlElement* pWhiteList = pIpSecurity->GetChildren("WhiteList");
				for (Int32 i=0; pWhiteList && i<pWhiteList->GetChildrenNum(); i++)
				{
					if (pWhiteList->GetChildren(i)->GetTag() == "Ip")
						AddWhiteIp(pWhiteList->GetChildren(i)->GetValue());
				}

				//������
				AXmlElement* pBlackList = pIpSecurity->GetChildren("BlackList");
				for (Int32 i=0; pBlackList && i<pBlackList->GetChildrenNum(); i++)
				{
					if (pBlackList->GetChildren(i)->GetTag() == "Ip")
						AddBlackIp(pBlackList->GetChildren(i)->GetValue());
				}
			}
		}

		return true;
	}

	Bool HawkGateway::Stop()
	{
		//֪ͨ�˳�ѭ��
		m_bRunning = false;

		//�ȴ����ؽ���
		if (!m_bBlocking)
			OnGatewayClose();

		return true;
	}

	AString HawkGateway::GetThreadZmqAddr() const
	{
		return "inproc://hawk-gateway-thread";
	}

	AString HawkGateway::GetServerZmqAddr() const
	{
		return "inproc://hawk-server-monitor";
	}

	HawkProfiler* HawkGateway::GetProfiler()
	{
		return m_pProfiler;
	}

	Bool HawkGateway::EnableGateSecurity(Bool bEnable, UInt32 iSecuType, const AString& sSecuKey)
	{
		m_bSecurity = bEnable;
		m_iSecuType = iSecuType;
		m_sSecuKey  = sSecuKey;
		return true;
	}

	Bool HawkGateway::TurnOnProfiler(const AString& sAddr)
	{
		if (!m_pProfiler && sAddr.size())
		{
			m_pProfiler = new HawkProfiler(sAddr);			
		}
		return true;
	}

	Bool HawkGateway::InitControler(const AString& sAddr)
	{
		if (!m_pControler)
		{
			m_pControler = new HawkGwControler(this);
			return m_pControler->Init(sAddr);
		}
		return false;
	}

	Bool HawkGateway::Run()
	{
		if (!m_bRunning)
		{
			m_bRunning = true;
			
			if (m_pCrossDomain)
				m_pCrossDomain->Start(this);

			if (m_pProfiler)
				m_pProfiler->Start();
		}
		
		while (m_bRunning)
		{
			//����Ĭ�Ͽ���״̬
			m_bIdle = true;

			//�������ؿ�����
			if (m_pControler && m_pControler->UpdateEvent())
				m_bIdle = false;

			//��������������
			OnSessionAccept();

			//���������¼�
			OnGateMonitorEvent();

			//��������߳��¼�
			OnGateThreadEvent();

			//����˷����¼�
			OnGateServerEvent();			
			
			//���θ����¼�
			if (!m_bBlocking)
				return true;

			//���ؿ��д���
			OnGateIdleEvent();
		}

		//�����ȴ�����
		OnGatewayClose();

		return true;
	}		

	Bool HawkGateway::RecvThreadMsg(UInt32& iThread, ThreadMsgHeader& sHeader, OctetsStream* pOctets)
	{
		if (m_pThreadZmq && pOctets)
		{
			//��ȡ�߳�ID��ʶ��Ϣ
			pOctets->Clear();			
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;

			UInt64 iThreadZmqId = 0;			
			iThreadZmqId	= *((UInt64*)pOctets->Begin());
			iThread			= HIGH_U32(iThreadZmqId);
			Bool bRecvMore = m_pThreadZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(iThreadZmqId) && bRecvMore);
			if (iSize != sizeof(iThreadZmqId) || !bRecvMore)
				return false;
		
			//��ȡ��Ϣͷ��Ϣ
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;
			
			sHeader	   = *((ThreadMsgHeader*)pOctets->Begin());
			bRecvMore = m_pThreadZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
			if (iSize != sizeof(sHeader) || !bRecvMore)
				return false;

			//��ȡ��Ϣ������
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::SendThreadMsg(UInt32 iThread, const GateMsgHeader& sHeader, void* pData, Size_t iSize, const OctetsStream* pExtData)
	{
		if (m_pThreadZmq && iThread)
		{
			//�߳�Ŀ��
			UInt64 iThreadZmqId = MAKE_UINT64(iThread, 1);
			if (!m_pThreadZmq->Send(&iThreadZmqId, sizeof(iThreadZmqId), HawkZmq::HZMQ_SNDMORE))
				return false;

			//��Ϣͷ��Ϣ
			if (!m_pThreadZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			//��������
			if (!m_pThreadZmq->Send(pData, iSize, pExtData ? HawkZmq::HZMQ_SNDMORE : 0))
				return false;

			//��������
			if (pExtData && !m_pThreadZmq->Send((void*)pExtData->Begin(), pExtData->Size()))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkGateway::RecvServerMsg(UInt32& iSvrId, SvrMsgHeader& sHeader, OctetsStream* pOctets)
	{
		if (m_pServerZmq && pOctets)
		{
			//������ID��ʶ��Ϣ
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			UInt64 iSvrZmqId	= *((UInt64*)pOctets->Begin());
			iSvrId				= HIGH_U32(iSvrZmqId);
			Bool bRecvMore		= m_pServerZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(iSvrZmqId) && bRecvMore);			
			if (iSize != sizeof(iSvrZmqId) || !bRecvMore)
				return false;

			//��ȡ��Ϣͷ��Ϣ
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			sHeader	   = *((SvrMsgHeader*)pOctets->Begin());
			bRecvMore = m_pServerZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
			if (iSize != sizeof(sHeader) || !bRecvMore)
				return false;

			//��Ϣ������
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::RecvServerExtData(OctetsStream* pOctets)
	{
		if (m_pServerZmq && pOctets)
		{
			//��Ϣ״̬У��
			Bool bRecvMore = m_pServerZmq->IsWaitRecv();
			HawkAssert(bRecvMore);
			if (!bRecvMore)
				return false;

			//��Ϣ������
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::SendServerMsg(UInt32 iSvrId, const GateMsgHeader& sHeader, void* pData, Size_t iSize)
	{
		if (m_pServerZmq && iSvrId)
		{
			//Ŀ��
			UInt64 iSvrZmqId = MAKE_UINT64(iSvrId, 1);
			if (!m_pServerZmq->Send(&iSvrZmqId, sizeof(iSvrZmqId), HawkZmq::HZMQ_SNDMORE))
				return false;

			//��Ϣͷ
			if (!m_pServerZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			//����
			if (!m_pServerZmq->Send(pData, iSize))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkGateway::TrustSession(void* pSession, OctetsStream* pIBuffer)
	{
		return true;
	}

	Bool HawkGateway::AddWhiteIp(const AString& sIp)
	{
		AString sTmpIp = sIp;
		HawkStringUtil::Trim<AString>(sTmpIp);

		if (sTmpIp.size())
			m_mWhiteIp[sTmpIp] = sTmpIp;
			
		return true;
	}

	Bool HawkGateway::DelWhiteIp(const AString& sIp)
	{
		AString sTmpIp = sIp;
		HawkStringUtil::Trim<AString>(sTmpIp);

		AStringMap::iterator it = m_mWhiteIp.find(sTmpIp);
		if (it != m_mWhiteIp.end())
		{
			m_mWhiteIp.erase(it);
			return true;
		}
		return false;
	}

	Bool HawkGateway::AddBlackIp(const AString& sIp)
	{
		AString sTmpIp = sIp;
		HawkStringUtil::Trim<AString>(sTmpIp);

		if (sTmpIp.size())
			m_mBlackIp[sTmpIp] = sTmpIp;

		return true;
	}

	Bool HawkGateway::DelBlackIp(const AString& sIp)
	{
		AString sTmpIp = sIp;
		HawkStringUtil::Trim<AString>(sTmpIp);

		AStringMap::iterator it = m_mBlackIp.find(sTmpIp);
		if (it != m_mBlackIp.end())
		{
			m_mBlackIp.erase(it);
			return true;
		}
		return false;
	}

	Bool HawkGateway::CloseSession(SID iSid, UInt32 iSvrId)
	{
		if (!iSvrId)
		{
			SidBindMap::const_iterator it = m_mSidBind.find(iSid);
			if (it != m_mSidBind.end())
				iSvrId = it->second.SvrId;
		}

		if (iSvrId && iSid > m_iBaseSid)
		{
			//�����Ự��Ӧ������������
			OnSessionDetachServer(iSid, iSvrId);

			//֪ͨ�����̹߳رջỰ
			GateNotify sNotify(GateNotify::NOTIFY_SESSION_CLOSE);
			sNotify.eClose.Sid   = iSid;
			sNotify.eClose.SvrId = iSvrId;
			UInt32 iIdx = (iSid-m_iBaseSid-1) % m_iThread + 1;
			SendThreadMsg(iIdx, GateMsgHeader(iSid), &sNotify, sizeof(sNotify));

			return true;
		}
		return false;
	}

	Bool HawkGateway::OnSessionAccept()
	{
		if (!m_sSocket.UpdateEvent(HEVENT_READ))
			return true;

		//����������
		HawkSocket sSocket;
		SocketAddr sAddr;
		while (m_sSocket.Accept(sSocket, sAddr))
		{
			//���÷ǿ���
			m_bIdle = false;

			//ע����������Ϣ
			UInt32 iSvrId	 = 0;
			Int32  iErrCode = 0;
			if (!CheckIpSecurity(sAddr.ToString(), iErrCode) || !RegConnection(sAddr, iSvrId, iErrCode))
				return OnGateRefuseConn(iErrCode, sSocket, &sAddr);

			//Ͷ������֪ͨ
			GateNotify sNotify(GateNotify::NOTIFY_SESSION_CONNECT);
			sNotify.eConnect.SvrId		= iSvrId;
			sNotify.eConnect.AddrLen	= sAddr.CopyAddr(sNotify.eConnect.Address);
			sNotify.eConnect.Handle		= sSocket.Handle();
			UInt32 iIdx = m_iTurnIdx++ % m_iThread + 1;
			SendThreadMsg(iIdx, GateMsgHeader(0), &sNotify, sizeof(sNotify));

			//���δ���ƽ������ʱ��
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::CheckIpSecurity(const AString& sIpPort, Int32& iErrCode)
	{
		Size_t iPos = sIpPort.find(":");
		if (iPos == AString::npos)
			return false;

		AString sIp = sIpPort.substr(0, iPos);
		//��ip����������
		if (!m_iIpPeerLimit)
			return true;

		//������
		if (m_mWhiteIp.find(sIp) != m_mWhiteIp.end())
			return true;

		//������
		if (m_mBlackIp.find(sIp) != m_mBlackIp.end())
		{
			HawkFmtPrint("BlackIpList Defense, IpAddr: %s", sIp.c_str());
			
			iErrCode = SysProtocol::GateErr::ERR_IP_BLACKLIST;
			return false;
		}

		//����������
		IpPeerMap::iterator it = m_mIpPeer.find(sIp);
		if (it != m_mIpPeer.end() && it->second >= (Int32)m_iIpPeerLimit)
		{
			HawkFmtPrint("IpPeerLimit Defense, IpAddr: %s", sIp.c_str());

			iErrCode = SysProtocol::GateErr::ERR_IP_PEERLIMIT;
			return false;
		}

		return true;
	}

	Bool HawkGateway::RegConnection(const SocketAddr& sAddr, UInt32& iSvrId, Int32& iErrCode)
	{
		//����ģʽ
		if (m_iGateMode == GATEMODE_ECHO)
		{
			iSvrId = (Size_t)(-1);
			return true;
		}
		
		//��ǰ�޹��ط�����
		if (!m_vService.size())
		{
			iErrCode = SysProtocol::GateErr::ERR_SERVICE_INVALID;
			return false;
		}

		//��һ������ģʽ, ��һ����
		if (m_iGateMode == GATEMODE_SINGLE_SERVICE)
		{			
			if (m_vService[0].ConnCap && m_vService[0].CurCap >= m_vService[0].ConnCap)
			{
				iErrCode = SysProtocol::GateErr::ERR_REFUSE_CONN;
				return false;
			}

			iSvrId = m_vService[0].SvrId;
			return true;
		}
		//�������ģʽ, �ֶ�ѡ��ģʽ
		else if (m_iGateMode == GATEMODE_MULTI_SERVICE_MANUAL)
		{
			iSvrId = 0;
			return true;
		}
		//�������ģʽ, ˳������ԭ��
		else if (m_iGateMode == GATEMODE_MULTI_SERVICE_FULLLOAD)
		{
			for (Size_t i=0;i<m_vService.size();i++)
			{
				if (!m_vService[i].ConnCap || m_vService[i].CurCap < m_vService[i].ConnCap)
				{
					iSvrId = m_vService[i].SvrId;
					return true;
				}
			}

			iErrCode = SysProtocol::GateErr::ERR_REFUSE_CONN;
			return false;
		}
		//�������ģʽ, ���ؾ���ԭ��
		else if (m_iGateMode == GATEMODE_MULTI_SERVICE_BALANCE)
		{
			Int32 iSvrIdx = -1;
			Int32 iSvrCap = -1;
			for (Size_t i=0;i<m_vService.size();i++)
			{
				if (!m_vService[i].ConnCap || m_vService[i].CurCap < m_vService[i].ConnCap)
				{
					if (iSvrCap == -1 || iSvrCap > (Int32)m_vService[i].CurCap)
					{
						iSvrIdx = i;
						iSvrCap = m_vService[i].CurCap;
					}
				}
			}

			//������Ч������
			if (iSvrIdx >= 0)
			{
				iSvrId = m_vService[iSvrIdx].SvrId;
				return true;
			}

			iErrCode = SysProtocol::GateErr::ERR_REFUSE_CONN;
			return false;
		}

		return false;
	}

	Bool HawkGateway::OnGateRefuseConn(Int32 iReason, HawkSocket& sSocket, const SocketAddr* pAddr)
	{
		//�ȷ��;ܾ�����Э���ٹر�		
		SysProtocol::Sys_RefuseConn sProto(iReason);		

		m_pOctets->Clear();
		sProto.Encode(*m_pOctets);
		Size_t iSize = (Size_t)m_pOctets->Size();
		sSocket.Send(m_pOctets->Begin(), iSize);
		sSocket.Close();

		return true;
	}

	Bool HawkGateway::OnSessionAttachServer(SID iSid, UInt32 iSvrId, const AString& sIpAddr)
	{
		//SID��ʱδʹ��, �ʴ˴���������SID��һ����ȷ
		for (Size_t i=0;i<m_vService.size();i++)
		{
			if (m_vService[i].SvrId == iSvrId)
			{
				m_vService[i].CurCap ++;				
				m_mSidBind[iSid] = BindInfo(iSvrId, sIpAddr);

				//���������
				IpPeerMap::iterator it = m_mIpPeer.find(sIpAddr);
				if (it != m_mIpPeer.end())
					it->second ++;
				else
					m_mIpPeer[sIpAddr] = 1;

				return true;
			}
		}
		return false;
	}

	Bool HawkGateway::OnSessionDetachServer(SID iSid, UInt32 iSvrId)
	{
		//SID��ʱδʹ��, �ʴ˴���������SID��һ����ȷ
		for (Size_t i=0;i<m_vService.size();i++)
		{		
			if (m_vService[i].SvrId == iSvrId)
			{
				if (m_vService[i].CurCap)
					m_vService[i].CurCap --;

				return true;
			}
		}

		//����ip������
		SidBindMap::iterator it = m_mSidBind.find(iSid);
		if (it != m_mSidBind.end())
		{			
			IpPeerMap::iterator ip_it = m_mIpPeer.find(it->second.IpAddr);
			if (ip_it != m_mIpPeer.end())
			{
				ip_it->second --;
				m_mSidBind.erase(it);
			}
		}

		return false;
	}

	Bool HawkGateway::OnGateThreadEvent()
	{
		while (m_pThreadZmq && m_pThreadZmq->PollEvent(HEVENT_READ, 0))
		{
			//���÷ǿ���
			m_bIdle = false;

			//���������߳���Ϣ
			UInt32	iThread = 0;
			ThreadMsgHeader sHeader;
			if (!RecvThreadMsg(iThread, sHeader, m_pOctets))
				return false;
			
			//���ݰ�����, �޶��⸽������
			if (m_pThreadZmq->IsWaitRecv())
				return false;

			//����id����У��
			Bool bSvrIdOk = false;
			for (Size_t i=0;i<m_vService.size();i++)
			{
				if (m_iGateMode == GATEMODE_ECHO || m_vService[i].SvrId == sHeader.SvrId)
				{
					bSvrIdOk = true;
					break;
				}
			}
			
			//�����ӵķ���id������
			if (m_iGateMode != GATEMODE_ECHO && !bSvrIdOk)
			{
#ifdef _DEBUG
				HawkFmtPrint("Unknown Service Id, SvrId: %u, Sid: %u", sHeader.SvrId, sHeader.Sid);
#endif
				continue;
			}

			//�������û�Э������У��
			if (sHeader.Sid)
			{
				if (!OnSessionProtocol(sHeader.Sid, m_pOctets))
				{
					CloseSession(sHeader.SvrId, sHeader.Sid);
					return true;
				}

				//Э�����ؼ��
				if (OnProtocolIntercept(sHeader.Sid, m_pOctets)) 
				{
					return true;
				}
			}
			
			//Echoģʽֱ�ӻظ�
			if (m_iGateMode == GATEMODE_ECHO)
			{
				//�û�Э��ֱ�ӷ��ͻ�ȥ
				if (sHeader.Args > 0 && sHeader.Sid)
					SendThreadMsg(iThread, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}
			//���͸���˷���
			else
			{
				//�Ự���غ�ж���ж�
				if (sHeader.Args == SysProtocol::SYS_SESSION_START)
				{
					AString sIpAddr = "";
					try
					{
						OctetsStream xOS(m_pOctets->Begin(), m_pOctets->Size());
						HawkProtocol* pProto = P_ProtocolManager->Decode(xOS);
						if (pProto && pProto->GetType() == SysProtocol::SYS_SESSION_START)
						{
							sIpAddr = ((SysProtocol::Sys_SessionStart*)pProto)->m_sAddr;
						}
					}
					catch (HawkException& rhsExcep)
					{
						//Э������쳣�˳�
						HawkFmtError("Gateway Decode Protocol Error, Msg: %s", rhsExcep.GetMsg().c_str());
					}
					
					//���ӻỰ��Ӧ������������
					OnSessionAttachServer(sHeader.Sid, sHeader.SvrId, sIpAddr);

					//������ϢͷЯ���ĻỰID, ������˷��������������Э��(���û�Э��)
					sHeader.Sid = 0;
				}
				else if (sHeader.Args == SysProtocol::SYS_SESSION_BREAK)
				{
					//�����Ự��Ӧ������������
					OnSessionDetachServer(sHeader.Sid, sHeader.SvrId);

					//������ϢͷЯ���ĻỰID, ������˷��������������Э��(���û�Э��)
					sHeader.Sid = 0;
				}

				//ת������˷�����
				if (sHeader.SvrId)
					SendServerMsg(sHeader.SvrId, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}

			//���δ���ƽ������ʱ��
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnSessionProtocol(SID iSid, const OctetsStream* pOctets)
	{
		if (iSid && pOctets)			
			return true;

		return false;
	}

	Bool HawkGateway::OnProtocolIntercept(SID iSid, const OctetsStream* pOctets)
	{
		return false;
	}

	Bool HawkGateway::OnGateServerEvent()
	{
		while (m_pServerZmq && m_pServerZmq->PollEvent(HEVENT_READ, 0))
		{
			//���÷ǿ���
			m_bIdle = false;
			
			//���պ�˷�����Ϣ
			UInt32 iSvrId = 0;
			SvrMsgHeader sHeader;
			if (!RecvServerMsg(iSvrId, sHeader, m_pOctets))
				return false;

			//�ỰЭ��ת��
			if (sHeader.Sid)
			{
				//���ݰ�����, �޶��⸽������
				if (m_pServerZmq->IsWaitRecv())
					return false;

				UInt32 iIdx = (sHeader.Sid-m_iBaseSid-1) % m_iThread + 1;
				SendThreadMsg(iIdx, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}
			//ϵͳ��Ϣ֪ͨ
			else
			{
				HawkAssert(m_pOctets->Size() == sizeof(GateNotify));
				GateNotify* pNotify = (GateNotify*)m_pOctets->Begin();
				if (m_pOctets->Size() != sizeof(GateNotify))
					return false;
				
				//֪ͨ�����̹߳رջỰ����
				if (pNotify->Type == GateNotify::NOTIFY_SESSION_CLOSE)
				{
					//���ݰ�����, �޶��⸽������
					if (m_pServerZmq->IsWaitRecv())
						return false;

					//����Ự��
					OnSessionDetachServer(pNotify->eClose.Sid, pNotify->eClose.SvrId);

					//ת���������̴߳���
					UInt32 iIdx = (pNotify->eClose.Sid-m_iBaseSid-1) % m_iThread + 1;
					SendThreadMsg(iIdx, GateMsgHeader(0), pNotify, sizeof(*pNotify));
				}
				//Э��ע��
				else if (pNotify->Type == GateNotify::NOTIFY_PROTO_REGISTER)
				{
					//֪ͨ��ϢЯ������
					if (RecvServerExtData(m_pOctets))
					{
						GateNotify sNotify(GateNotify::NOTIFY_PROTO_REGISTER);
						for (Int32 i=0; i<m_iThread; i++)
						{
							UInt32 iIdx = i + 1;
							SendThreadMsg(iIdx, GateMsgHeader(0), &sNotify, sizeof(sNotify), m_pOctets);
						}
					}
				}
				//Э��㲥
				else if (pNotify->Type == GateNotify::NOTIFY_PROTO_BROADCAST)
				{
					//֪ͨ��ϢЯ������
					if (RecvServerExtData(m_pOctets))
					{
						vector<SID> vSid;
						*m_pOctets >> vSid;

						for (Size_t i=0; i<vSid.size(); i++)
						{
							UInt32 iIdx = (vSid[i]-m_iBaseSid-1) % m_iThread + 1;
							SendThreadMsg(iIdx, GateMsgHeader(vSid[i]), m_pOctets->AvailableData(), m_pOctets->AvailableSize());						
						}
					}
				}
			}

			//���δ���ƽ������ʱ��
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnGateMonitorEvent()
	{
		while (m_pMonitorZmq && m_pMonitorZmq->PollEvent(HEVENT_READ, 0))
		{
			//���÷ǿ���
			m_bIdle = false;

			//����ͷ��Ϣ
			m_pOctets->Clear();
			Size_t iSize = (Size_t)m_pOctets->Capacity();
			if (!m_pMonitorZmq->Recv(m_pOctets->Begin(), iSize))
				return false;
			else
				m_pOctets->Resize(iSize);

#if (ZMQ_VERSION_MAJOR >= 4)
			//ZMQ-Version(4.0.1)
			//����״̬У��
			UInt16 iEvent  = 0;
			UInt32 iValue  = 0;
			Bool bRecvMore = m_pMonitorZmq->IsWaitRecv();

			//״̬����
			HawkAssert(iSize == sizeof(iEvent) + sizeof(iValue) && bRecvMore);
			if (iSize != sizeof(iEvent) + sizeof(iValue) || !bRecvMore)
				return false;

			//��ȡ�¼����ͺͲ���
			m_pOctets->Pop(iEvent);
			m_pOctets->Pop(iValue);
			
			//��ȡ��ַ��Ϣ
			m_pOctets->Clear();
			iSize = (Size_t)m_pOctets->Capacity();
			if (!m_pMonitorZmq->Recv(m_pOctets->Begin(), iSize))
				return false;

			//ת����ַ�ṹ
			AString sAddr = "";
			if (iSize > 0)
				sAddr.append((Char*)m_pOctets->Begin(), iSize);

			//���÷���ID�Ѿ������б�
			if (iEvent & HawkZmq::HZMQ_EVENT_ACCEPTED)
			{
				HawkFmtPrint("Service Connect Successful, Addr: %s, FD: %d", sAddr.c_str(), iValue);

				if (!OnServerConnected(iValue))
					return false;
			}
			else if (iEvent & HawkZmq::HZMQ_EVENT_DISCONNECTED)
			{
				HawkFmtPrint("Service Disconnect Exception, Addr: %s, FD: %d", sAddr.c_str(), iValue);

				if (!OnServerDisConnect(iValue))
					return false;				
			}
			else
			{
				HawkFmtPrint("Gateway Unhandle Event, Event: %d", iEvent);
			}
#elif (ZMQ_VERSION_MAJOR >= 3)
			//ZMQ-Version(3.2.2)
			//����У��
			HawkAssert(iSize == sizeof(zmq_event_t));
			if (iSize != sizeof(zmq_event_t))
				return false;

			//ת���¼��ṹ��
			zmq_event_t sZmqEvent = *((zmq_event_t*)m_pOctets->Begin());
			
			//���÷���ID�Ѿ������б�
			if (sZmqEvent.event & HawkZmq::HZMQ_EVENT_ACCEPTED)
			{
				HawkFmtPrint("Service Connect Successful, Socket: %d", sZmqEvent.data.accepted.fd);

				if (!OnServerConnected(sZmqEvent.data.accepted.fd))
					return false;
			}
			else if (sZmqEvent.event & HawkZmq::HZMQ_EVENT_DISCONNECTED)
			{
				HawkFmtPrint("Service Disconnect Exception, Socket: %d", sZmqEvent.data.accepted.fd);

				if (!OnServerDisConnect(sZmqEvent.data.disconnected.fd))
					return false;				
			}
			else
			{
				HawkFmtPrint("Gateway Unhandle Event, Event: %d", sZmqEvent.event);
			}
#endif

			//���δ���ƽ������ʱ��
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnGateIdleEvent()
	{
		//���д���
		if (m_bIdle)
		{
			HawkSleep(DEFAULT_SLEEP);
		}		
		return true;
	}

	Bool HawkGateway::OnGatewayClose()
	{
		//֪ͨ�����߳��˳�
		GateNotify sNotify(GateNotify::NOTIFY_SERVICE_EXIT);
		for (UInt32 i=0;m_pThreadZmq && i<(UInt32)m_iThread;i++)
		{
			SendThreadMsg(i+1, GateMsgHeader(0), &sNotify, sizeof(sNotify));
		}

		//�ȴ��߳̽���
		for (UInt32 i=0;m_ppThread && i<(UInt32)m_iThread;i++)
		{
			if (m_ppThread[i])
				m_ppThread[i]->Close();
		}

		//�ر����ܼ�����
		if (m_pProfiler)
			m_pProfiler->Stop();

		//�ر����ؿ�����
		if (m_pControler)
			m_pControler->Stop();

		//��������״̬
		m_bRunning = false;

		return true;
	}

	Bool HawkGateway::OnServerConnected(SOCKET hSocket)
	{
		if (hSocket != INVALID_SOCKET)
		{
			if (m_pServerZmq->PollEvent(HEVENT_READ, DEFAULT_TIMEOUT))
			{				
				//��������֪ͨ
				UInt32 iSvrId = 0;
				SvrMsgHeader sHeader;
				if (!RecvServerMsg(iSvrId, sHeader, m_pOctets))
					return false;

				//���ݰ�����, �޶��⸽������
				if (m_pServerZmq->IsWaitRecv())
					return false;

				//��������У��(���͹��ط��������һ����)
				HawkAssert(sHeader.Sid == 0 && iSvrId && m_pOctets->Size() == sizeof(GateNotify));
				GateNotify* pNotify = (GateNotify*)m_pOctets->Begin();
				if (sHeader.Sid || !iSvrId || iSvrId != pNotify->eAttach.SvrId || m_pOctets->Size() != sizeof(GateNotify))
					return false;

				//�������֪ͨ
				if (pNotify->Type == GateNotify::NOTIFY_SERVICE_ATTACH)
				{
					//�жϷ���ID�ظ�
					Size_t i = 0;
					for (;i<m_vService.size();i++)
					{
						if (iSvrId == m_vService[i].SvrId)
							break;
					}

					//��ӵ��б�
					if (i == m_vService.size())
					{						
						m_vService.push_back(Service(iSvrId, hSocket, pNotify->eAttach.ConnLimit));
						HawkFmtPrint("Service Attach, SvrId: %u", iSvrId);
					}

					return true;
				}
			}		
		}
		return false;
	}

	Bool HawkGateway::OnServerDisConnect(SOCKET hSocket)
	{
		if (hSocket != INVALID_SOCKET)
		{
			ServiceVec::iterator it = m_vService.begin();
			for (;it!=m_vService.end();it++)
			{				
				if (it->SvrFd == hSocket)
				{	
					//֪ͨ�����̷߳���ж��
					GateNotify sNotify(GateNotify::NOTIFY_SERVICE_DETACH);
					sNotify.eDetach.SvrId = it->SvrId;

					for (UInt32 j=0;m_pThreadZmq && j<(UInt32)m_iThread;j++)
					{
						SendThreadMsg(j+1, GateMsgHeader(0), &sNotify, sizeof(sNotify));
					}

					//�������Ͽ����ر�ǰ�����ӵ������, ����ɾ���Ự�󶨹�ϵ
					if (!IsHoldConnection())
					{
						//ɾ�����ϵĻỰID
						SidBindMap::iterator bit = m_mSidBind.begin();
						for (;bit != m_mSidBind.end();)
						{
							if (bit->second.SvrId == it->SvrId)
							{
								m_mSidBind.erase(bit++);
								continue;
							}
							bit ++;
						}

						//�������ӶϿ�, ����ip������
						m_mIpPeer.clear();
					}					

					//�ӷ����б�ɾ��
					HawkFmtPrint("Service Detach, SvrId: %u", it->SvrId);
					m_vService.erase(it);

					break;
				}
			}
		}
		return false;
	}
}
