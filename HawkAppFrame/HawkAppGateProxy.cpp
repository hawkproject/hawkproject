#include "HawkAppGateProxy.h"
#include "HawkAppFrame.h"

namespace Hawk
{
	HawkAppGateProxy::HawkAppGateProxy(HawkAppFrame* pApp) : m_pApp(pApp), m_bConnect(false)
	{
	}

	HawkAppGateProxy::~HawkAppGateProxy()
	{
	}

	Bool HawkAppGateProxy::Init()
	{
		const HawkAppFrame::AppCfg& sCfg = *m_pApp->GetAppCfg();
		if (!HawkGateProxy::Init(sCfg.Gateproxy.Address, sCfg.AppId, sCfg.Gateproxy.ConnLimit))
		{
			HawkFmtPrint("AppGateProxy Init Error, Addr: %s.", sCfg.Gateproxy.Address.c_str());
			return false;
		}

		//向网关服务器协议注册
		vector<ProtoType> vType;
		vector<UInt8>	  vDrive;				
		P_ProtocolManager->GetRegProtoIds(vType, &vDrive);

		map<ProtoType, UInt8> mTypeDrive;
		HawkAssert(vType.size() == vDrive.size());
		if (vType.size() == vDrive.size())
		{
			for (Size_t i=0;i<vType.size();i++)
				mTypeDrive[ vType[i] ] = vDrive[i];
		}
		RegisterProtoType(mTypeDrive);

		//注册网关代理器线程
		vector<UInt32> vThreads;
		vThreads.push_back(HawkOSOperator::GetThreadId());
		for (Int32 i=0;i<m_pApp->m_pThreadPool->GetThreadNum();i++)
		{
			vThreads.push_back((UInt32)m_pApp->m_pThreadPool->GetThreadId(i));
			HawkFmtPrint("AppThread: %d", m_pApp->m_pThreadPool->GetThreadId(i));
		}
		if (!RegisterThreads(vThreads))
		{
			HawkFmtPrint("GateProxy Register Threads Error.");
			return false;
		}

		return true;
	}

	Bool HawkAppGateProxy::OnProxyConnected()
	{
		m_bConnect = true;

		if (m_pApp)
			m_pApp->OnGateProxyConnected();

		return HawkGateProxy::OnProxyConnected();
	}

	Bool HawkAppGateProxy::OnProxyDisconnect()
	{
		m_bConnect = false;

		if (m_pApp)
			m_pApp->OnGateProxyDisconnect();

		return HawkGateProxy::OnProxyDisconnect();
	}

	Bool HawkAppGateProxy::IsConnect() const
	{
		return m_bConnect;
	}

	Bool HawkAppGateProxy::OnRecvProtocol(const GateMsgHeader& sHeader, HawkProtocol* pProto)
	{
		HawkAssert(pProto);
		if (!m_pApp || !pProto) 
			return false;

		//指定会话ID的是由用户发送过来的
		if (sHeader.Sid)
		{
			return m_pApp->OnSessionProtocol(sHeader.Sid, pProto);
		}

		//系统间的协议处理
		ProtoType iType = pProto->GetType();
		if (iType == SysProtocol::SYS_SESSION_START)
		{
			SysProtocol::Sys_SessionStart* pCmd = (SysProtocol::Sys_SessionStart*)pProto;
			if (pCmd->m_iSid)
				m_pApp->OnSessionStart(pCmd->m_iSid, pCmd->m_sAddr);
		}
		else if (iType == SysProtocol::SYS_SESSION_BREAK)
		{
			SysProtocol::Sys_SessionBreak* pCmd = (SysProtocol::Sys_SessionBreak*)pProto;
			if (pCmd->m_iSid)
				m_pApp->OnSessionClose(pCmd->m_iSid, pCmd->m_sAddr);
		}
		else
		{
			HawkAssert(false && "Exception Protocol.");
		}

		//最终释放协议
		P_ProtocolManager->ReleaseProto(pProto);

		return true;
	}
}
