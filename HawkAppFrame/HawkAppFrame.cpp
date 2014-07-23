#include "HawkAppFrame.h"

namespace Hawk
{
	HawkAppFrame::HawkAppFrame(const XID& sXid) : HawkApp(sXid)
	{
		m_pLogProxy	  = 0;	
		m_pGateProxy  = 0;
		m_pGateway	  = 0;
		m_iElapseTime = 0;
		m_bBreakLoop  = false;
		m_iSysTime	  = (UInt32)HawkOSOperator::GetSysTime();
		m_iTickTime	  = HawkOSOperator::GetTickCount();
	}

	HawkAppFrame::~HawkAppFrame()
	{
		m_mSidAddr.Clear();

		if (m_mDbHdl.size())
		{
			DBHdlMap::iterator it = m_mDbHdl.begin();
			for (;it!=m_mDbHdl.end();it++)
			{
				P_DBManager->CloseDatabase(it->second->GetDBName());
			}
			m_mDbHdl.clear();
		}		

		HAWK_RELEASE(m_pLogProxy);
		HAWK_RELEASE(m_pGateProxy);
		HAWK_RELEASE(m_pGateway);
	}	

	HawkMysql* HawkAppFrame::GetSafeDbHdl()
	{
		UInt32 dwThreadId = HawkOSOperator::GetThreadId();
		DBHdlMap::iterator it = m_mDbHdl.find(dwThreadId);
		if (it != m_mDbHdl.end())
			return it->second;

		return 0;
	}

	Bool HawkAppFrame::BreakLoop()
	{
		m_bBreakLoop = true;
		return true;
	}

	const HawkAppFrame::AppCfg*	HawkAppFrame::GetAppCfg() const
	{
		return &m_sAppCfg;
	}

	UInt32	HawkAppFrame::GetSvrId() const
	{
		return m_sAppCfg.SvrId;
	}

	UInt32 HawkAppFrame::GetSysTime() const
	{
		return m_iSysTime;
	}

	UInt32 HawkAppFrame::GetTickTime() const
	{
		return m_iTickTime;
	}
	
	UInt32 HawkAppFrame::GetElapseTime() const
	{
		return m_iElapseTime;
	}

	AString	HawkAppFrame::GetSidAddr(SID iSid)
	{
		AString sAddr;
		if (m_mSidAddr.Find(iSid, &sAddr))
			return sAddr;

		return "";
	}	

	Bool HawkAppFrame::Init(const AppCfg& sCfg)
	{
		m_sAppCfg = sCfg;
		HawkAssert(m_sAppCfg.Threads && m_sAppCfg.SvrId);
		if (HawkApp::Init(m_sAppCfg.Threads))
		{
			//日志管理器控制台打印
			P_LoggerManager->EnableConsole(m_sAppCfg.Console);

			//日志对象初始化
			if (m_sAppCfg.LogAddr.size())
			{
				if (!m_pLogProxy)
					m_pLogProxy = new HawkLogProxy;

				if (!m_pLogProxy->Init(m_sAppCfg.LogAddr, m_sAppCfg.SvrId))
				{
					HawkFmtPrint("LogProxy Init Error, Addr: %s.", m_sAppCfg.LogAddr.c_str());
					return false;
				}
				m_pLogProxy->EnableConsole(m_sAppCfg.Console);
			}			

			//内嵌网关模式
			if (sCfg.Gateway.GwCfg.size())
			{
				m_pGateway = new HawkAppGateway(this);
				if (!m_pGateway->Init())
					return false;
			}

			//网关代理器
			m_pGateProxy = new HawkAppGateProxy(this);
			if (!m_pGateProxy->Init())
				return false;

			//数据库对象初始化
			if (sCfg.DBConn.IsValid())
			{
				vector<UInt32> vThreads;
				vThreads.push_back(HawkOSOperator::GetThreadId());
				for (Int32 i=0;i<m_pThreadPool->GetThreadNum();i++)
				{
					vThreads.push_back((UInt32)m_pThreadPool->GetThreadId(i));
					HawkFmtPrint("AppThread: %d", m_pThreadPool->GetThreadId(i));
				}

				for (Size_t i=0; i<vThreads.size(); i++)
				{
					Char sDBName[DEFAULT_SIZE] = {0};
					sprintf(sDBName,"AppDB_%u", vThreads[i]);
					HawkMysql* pDBHdl = (HawkMysql*)P_DBManager->OpenDatabase(sDBName, m_sDbConn);
					if(!pDBHdl) 
						return false;

					m_mDbHdl[ vThreads[i] ] = pDBHdl; 	
				}
			}
			return true;
		}
		return false;
	}
	
	Bool HawkAppFrame::Run()
	{
		if (HawkApp::Run())
		{
			UInt32 iTickTime = 0;
			while (m_bRunning && !m_bBreakLoop)
			{
				m_iSysTime	  = (UInt32)HawkOSOperator::GetSysTime();
				iTickTime	  = HawkOSOperator::GetTickCount();
				m_iElapseTime = iTickTime - m_iTickTime;
				m_iTickTime   = iTickTime;

				//Hawk体系帧更新
				HawkUtil::Tick(m_iElapseTime);
			
				//网关代理器更新
				if (m_pGateProxy && !m_pGateProxy->Update())
				{
					HawkSleep(DEFAULT_SLEEP);
				}

				//逻辑帧更新
				OnTick(m_iElapseTime);
			}

			//服务器关闭异常
			OnException(0);

			//关闭服务器
			Stop();
			
			return true;
		}
		return false;
	}
	
	Bool HawkAppFrame::SendProtocol(SID iSid, Protocol* pProto)
	{
		HawkAssert(iSid && pProto);

		if (!iSid || !pProto || !m_pGateProxy || !m_pGateProxy->IsConnect())
			return false;

		return m_pGateProxy->SendProtocol(iSid, pProto);
	}

	Bool HawkAppFrame::CloseSession(SID iSid)
	{
		if (iSid)
		{
			m_mSidAddr.Erase(iSid);

			if (m_pGateProxy)
				return m_pGateProxy->CloseSession(iSid);
		}
		return false;
	}

	Bool HawkAppFrame::BroadcastMsg(ObjMan* pObjMan, HawkMsg* pMsg)
	{
		if (pObjMan && pMsg)
		{
			XIDVector vXID;
			pObjMan->CollectObjKey(vXID);

			if (vXID.size())
				PostMsg(vXID, pMsg);

			return true;
		}		
		return false;
	}

	Bool HawkAppFrame::BroadcastProto(Protocol* pProto, const vector<SID>& vSid)
	{
		if (!pProto || !m_pGateProxy || !m_pGateProxy->IsConnect())
			return false;

		if (!vSid.size())
		{
			vector<SID> vAllSid;
			m_mSidAddr.CollectKey(vAllSid);
			return m_pGateProxy->BroadcastProto(vAllSid, pProto);
		}

		return m_pGateProxy->BroadcastProto(vSid, pProto);
	}

	Bool HawkAppFrame::FormatMsg(const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtMsg(0, "", pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::FormatWarn(const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtWarn(0, "", pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::FormatError(const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtError(0, "", pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::FormatMsg(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtMsg(iUsrId, pUsrName, pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::FormatWarn(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtWarn(iUsrId, pUsrName, pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::FormatError(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...)
	{
		if (m_pLogProxy)
		{
			va_list vArgs;
			Char sMsg[LOG_DEFAULT_SIZE+1] = {0};
			va_start(vArgs, (const Char*)pFmt);
			_vsnprintf((Char*)sMsg, LOG_DEFAULT_SIZE, (Char*)pFmt, vArgs);
			va_end(vArgs);

			return m_pLogProxy->FmtError(iUsrId, pUsrName, pKey, sMsg);
		}

		return false;
	}

	Bool HawkAppFrame::OnGateProxyConnected()
	{
		m_mSidAddr.Clear();		
		return true;
	}

	Bool HawkAppFrame::OnSessionStart(SID iSid, const AString& sAddr)
	{
		m_mSidAddr.Set(iSid, sAddr);
		FormatMsg("SessionStart", "Sid: %u, IpAddr: %s", iSid, sAddr.c_str());
		return true;
	}

	Bool HawkAppFrame::OnSessionProtocol(SID iSid, Protocol* pProto)
	{
		if (iSid && pProto)
		{
			PostProtocol(iSid, pProto);
			return true;
		}
		return false;
	}

	Bool HawkAppFrame::OnSessionClose(SID iSid, const AString& sAddr)
	{
		if (m_mSidAddr.Erase(iSid))
		{
			FormatMsg("SessionClose", "Sid: %u, IpAddr: %s", iSid, sAddr.c_str());
			return true;
		}
		return false;
	}

	Bool HawkAppFrame::OnGateProxyDisconnect()
	{
		//网关断开连接异常
		OnException(0);

		m_mSidAddr.Clear();
		return true;
	}

	Bool HawkAppFrame::OnAcceptConnect(const HawkSocket& sSocket, const SocketAddr& sAddr)
	{
		return true;
	}

	HawkAppObj*	HawkAppFrame::AppCreateObj(const XID& sXid)
	{
		return HawkApp::AppCreateObj(sXid);
	}

	Bool HawkAppFrame::OnTick(UInt32 iPeriod)
	{
		return true;
	}

	Bool HawkAppFrame::Stop()
	{
		if (m_bRunning)
		{
			m_bRunning = false;

			//网关服务器线程关闭
			if (m_pGateway)
				m_pGateway->Stop();

			//等待逻辑线程任务执行结束
			if (m_pThreadPool)
				m_pThreadPool->Close(true);
		}
	
		return HawkApp::Stop();
	}	
}
