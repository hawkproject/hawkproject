#include "HawkApp.h"
#include "HawkRand.h"
#include "HawkScope.h"
#include "HawkException.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	HawkApp* g_HawkApp = 0;

	//////////////////////////////////////////////////////////////////////////

	class HawkApp_ProtoTask : public HawkTask
	{
	public:
		HawkApp_ProtoTask(const XID& sXid = XID(0,0), SID iSid = 0, Protocol* pProto = 0) : m_sXid(sXid), m_iSid(iSid), m_pProto(pProto)
		{
		};

		virtual ~HawkApp_ProtoTask()
		{
			P_ProtocolManager->ReleaseProto(m_pProto);
			m_pProto = 0;
		}

	public:
		virtual PVoid  Run(void* pArgs = 0)
		{
			if(m_pProto && g_HawkApp)
			{
				Bool bDispatch = g_HawkApp->DispatchProto(m_sXid, m_iSid, m_pProto);
				if (!bDispatch)
					HawkFmtError("Dispatch Protocol Failed, Xid: (%u, %u), ProtocolId: %u, Sid: %u", m_sXid.Type, m_sXid.Id, m_pProto->GetType(), m_iSid);
			}
			return 0;
		}

	protected:
		XID	      m_sXid;
		SID		  m_iSid;
		Protocol* m_pProto;
	};

	class HawkApp_MsgTask : public HawkTask
	{
	public:
		HawkApp_MsgTask(const XID& sXid = XID(0,0), HawkMsg* pMsg = 0) : m_sXid(sXid), m_pMsg(pMsg)
		{
		};		

		HawkApp_MsgTask(const XIDVector& vXid, HawkMsg* pMsg) : m_pMsg(pMsg)
		{
			for (Size_t i=0;i<vXid.size();i++)
			{
				m_vXid.push_back(vXid[i]);
			}
		};

		virtual ~HawkApp_MsgTask()
		{
			m_vXid.clear();
			P_MsgManager->FreeMsg(m_pMsg);
			m_pMsg = 0;
		}

	public:
		virtual PVoid  Run(void* pArgs = 0)
		{
			if(m_pMsg && g_HawkApp)
			{
				if (m_vXid.size())
				{
					for (Size_t i=0;i<m_vXid.size();i++)
					{
						m_pMsg->Target = m_vXid[i];
						Bool bDispatch = g_HawkApp->DispatchMsg(m_vXid[i], m_pMsg);
						if (!bDispatch)
							HawkFmtError("Dispatch Message Failed, Xid: (%u, %u), MsgId: %u", m_vXid[i].Id, m_pMsg->Msg);
					}
				}
				else
				{
					Bool bDispatch = g_HawkApp->DispatchMsg(m_sXid, m_pMsg);
					if (!bDispatch)
						HawkFmtError("Dispatch Message Failed, Xid: (%u, %u), MsgId: %u", m_sXid.Id, m_pMsg->Msg);
				}				
			}
			return 0;
		}

	protected:
		XID	      m_sXid;
		XIDVector m_vXid;
		HawkMsg*  m_pMsg;
	};

	class HawkApp_TickTask : public HawkTask
	{
	public:
		HawkApp_TickTask(const XID& sXid = XID(0,0)) : m_sXid(sXid)
		{			
		};

		HawkApp_TickTask(const XIDVector& vXid)
		{
			for (Size_t i=0;i<vXid.size();i++)
			{
				m_vXid.push_back(vXid[i]);
			}
		};

		virtual ~HawkApp_TickTask()
		{
			m_vXid.clear();
		}

	public:
		virtual void ResetXids(const XIDVector& vXid)
		{
			m_vXid.clear();
			for (Size_t i=0;i<vXid.size();i++)
			{
				m_vXid.push_back(vXid[i]);
			}
		}

		virtual PVoid  Run(void* pArgs = 0)
		{
			if (m_vXid.size())
			{
				for (Size_t i=0;i<m_vXid.size();i++)
				{
					g_HawkApp->DispatchTick(m_vXid[i]);
				}
			}
			else
			{
				g_HawkApp->DispatchTick(m_sXid);
			}
			return 0;
		}

	protected:
		XID			m_sXid;
		XIDVector	m_vXid;
	};

	//////////////////////////////////////////////////////////////////////////

	HawkApp::SafeObj::SafeObj(XID sXid) : ObjMan::SafeObj(g_HawkApp->GetObjMan(sXid.Type), sXid)
	{
	}

	HawkApp::SafeObj::~SafeObj()
	{
	}

	//////////////////////////////////////////////////////////////////////////

	HawkApp::HawkApp(const XID& sXid) : HawkAppObj(sXid)
	{
		HawkAssert(g_HawkApp == 0);
		g_HawkApp     = this;
		m_pThreadPool = 0;
		m_bRunning	  = false;		
		m_pSidXidLock = new HawkSpinLock;
		m_iMainTid    = HawkOSOperator::GetThreadId();		
	}

	HawkApp::~HawkApp()
	{
		Stop();		

		if (m_pThreadPool)
			m_pThreadPool->Close();

		ObjManMap::iterator it = m_mObjMan.begin();
		for (;it!=m_mObjMan.end();it++)
		{
			ObjMan* pMan = it->second;
			HAWK_RELEASE(pMan);
		}
		m_mObjMan.clear();
		m_mSidXid.clear();

		HAWK_RELEASE(m_pThreadPool);
		HAWK_RELEASE(m_pSidXidLock);

		g_HawkApp = 0;
	}

	HawkApp::ObjMan* HawkApp::CreateObjMan(UInt32 iType)
	{
		ObjMan* pObjMan = GetObjMan(iType);
		if (!pObjMan)
		{
			pObjMan = new ObjMan;
			m_mObjMan[iType] = pObjMan;
		}
		return pObjMan;
	}

	HawkApp::ObjMan* HawkApp::GetObjMan(UInt32 iType)
	{
		ObjManMap::const_iterator it = m_mObjMan.find(iType);
		if (it != m_mObjMan.end())
		{
			return (ObjMan*)it->second;
		}
		return 0;
	}

	HawkAppObj* HawkApp::CreateObj(const XID& sXid, SID iSid)
	{
		if(!sXid.IsValid())
			return 0;		

		ObjMan* pObjMan = GetObjMan(sXid.Type);
		HawkAssert(pObjMan);

		HawkAppObj* pObj = g_HawkApp->AppCreateObj(sXid);
		if (pObj)
		{
			if(pObjMan->AllocObject(sXid, pObj))	
			{
				if(iSid) 
					BindSidXid(iSid, sXid);	

				return pObj;
			}

			HAWK_RELEASE(pObj);
			return 0;
		}

		HawkAssert(false && "Create Obj Null.");
		return 0;
	}

	HawkAppObj* HawkApp::AppCreateObj(const XID& sXid)
	{
		return 0;
	}

	Bool  HawkApp::DeleteObj(const XID& sXid)
	{
		if (sXid.IsValid())
		{
			ObjMan* pObjMan = GetObjMan(sXid.Type);
			HawkAssert(pObjMan);

			return pObjMan->FreeObject(sXid);
		}
		return true;
	}

	XID HawkApp::GetXidBySid(SID iSid)
	{
		HawkAutoSpinLock(lock, m_pSidXidLock);
		SidXidMap::iterator it = m_mSidXid.find(iSid);
		if (it != m_mSidXid.end())
			return it->second;
		
		return XID(0,0);
	}

	void HawkApp::UnbindSidXid(SID iSid)
	{
		if(iSid) 
		{
			HawkAutoSpinLock(lock, m_pSidXidLock);
			SidXidMap::iterator it = m_mSidXid.find(iSid);
			if (it != m_mSidXid.end())
				m_mSidXid.erase(it);
		}
	}

	void HawkApp::BindSidXid(SID iSid,const XID& sXid)
	{
		if (iSid && sXid.IsValid())
		{
			HawkAutoSpinLock(lock, m_pSidXidLock);
			m_mSidXid[iSid] = sXid;
		}
	}

	Bool HawkApp::Init(Int32 iThread)
	{
		if (!m_pThreadPool)
		{
			m_pThreadPool = new HawkThreadPool;

			if (!m_pThreadPool->InitPool(iThread, false))
				return false;

			if(!m_pThreadPool->Start())
				return false;	

			return true;
		}
		
		return false;
	}

	Bool HawkApp::Run()
	{
		if (!m_bRunning)
		{
			m_bRunning = true;

			return true;
		}
		return false;
	}

	Bool HawkApp::Stop()
	{
		if (m_bRunning)
		{
			m_bRunning = false;

			if (m_pThreadPool)
				m_pThreadPool->Close();
		}
		return true;
	}

	Bool HawkApp::IsRunning() const
	{
		return m_bRunning;
	}

	Int32 HawkApp::GetThreadNum() const
	{
		if (m_pThreadPool)
			return m_pThreadPool->GetThreadNum();
		
		return 0;
	}

	Bool HawkApp::SendProtocol(SID iSid, Protocol* pProto)
	{
		return true;
	}

	Bool HawkApp::PostProtocol(SID iSid, Protocol* pProto)
	{
		if (m_bRunning && iSid && pProto)
		{
			XID sXid = GetXidBySid(iSid);
			if (sXid.IsValid())
			{
				Int32 iCount = GetThreadNum();
				HawkAssert(iCount > 0);
				Int32 iIdx = sXid.Id % iCount;

				HawkApp_ProtoTask* pTask = new HawkApp_ProtoTask(sXid, iSid, pProto);
				HawkScope::ObjPtr scope(pTask);
				return PostAppTask(pTask, iIdx);
			}			
		}

		P_ProtocolManager->ReleaseProto(pProto);
		return true;
	}

	Bool HawkApp::PostProtocol(const XID& sXid, SID iSid, Protocol* pProto)
	{
		if (m_bRunning && iSid && pProto && sXid.IsValid())
		{
			Int32 iCount = GetThreadNum();
			HawkAssert(iCount > 0);
			Int32 iIdx = sXid.Id % iCount;

			HawkApp_ProtoTask* pTask = new HawkApp_ProtoTask(sXid, iSid, pProto);
			HawkScope::ObjPtr scope(pTask);
			return PostAppTask(pTask, iIdx);		
		}

		P_ProtocolManager->ReleaseProto(pProto);
		return true;
	}

	Bool HawkApp::PostMsg(HawkMsg* pMsg)
	{
		if (m_bRunning && pMsg)
		{
			Int32 iCount = GetThreadNum();
			HawkAssert(iCount > 0);
			Int32 iIdx = pMsg->Target.Id % iCount;

			HawkApp_MsgTask* pTask = new HawkApp_MsgTask(pMsg->Target, pMsg);
			HawkScope::ObjPtr scope(pTask);
			return PostAppTask(pTask, iIdx);
		}

		P_MsgManager->FreeMsg(pMsg);
		return false;
	}

	Bool HawkApp::PostMsg(const XID& sXid, HawkMsg* pMsg)
	{
		HawkMsg* pRealMsg = (HawkMsg*)pMsg;
		if (pRealMsg && sXid.IsValid())
		{
			pRealMsg->Target = sXid;
			return PostMsg(pRealMsg);
		}
		
		P_MsgManager->FreeMsg(pRealMsg);
		return false;
	}

	Bool HawkApp::PostMsg(const XIDVector& vXID, HawkMsg* pMsg)
	{
		if (pMsg && vXID.size())
		{
			Int32 iCount = GetThreadNum();
			HawkAssert(iCount > 0);
			map<Int32, XIDVector> mThreadXid;
		
			for(Size_t i=0;i<vXID.size();i++)
			{
				Int32 iIdx = vXID[i].Id % iCount;
				if (mThreadXid.find(iIdx) == mThreadXid.end())
					mThreadXid[iIdx] = XIDVector();
				
				mThreadXid[iIdx].push_back(vXID[i]);
			}

			map<Int32, XIDVector>::iterator it = mThreadXid.begin();
			for (;it!=mThreadXid.end();it++)
			{
				HawkApp_MsgTask* pTask = new HawkApp_MsgTask(it->second, pMsg);
				HawkScope::ObjPtr scope(pTask);
				PostAppTask(pTask, it->first);
			}
		}

		P_MsgManager->FreeMsg(pMsg);
		return true;
	}

	Bool HawkApp::PostTick(const XIDVector& vXID)
	{
		if (vXID.size())
		{
			Int32 iCount = GetThreadNum();
			HawkAssert(iCount > 0);
			map<Int32, XIDVector> mThreadXid;

			for(Size_t i=0;i<vXID.size();i++)
			{
				Int32 iIdx = vXID[i].Id % iCount;
				if (mThreadXid.find(iIdx) == mThreadXid.end())
					mThreadXid[iIdx] = XIDVector();

				mThreadXid[iIdx].push_back(vXID[i]);
			}

			map<Int32, XIDVector>::iterator it = mThreadXid.begin();
			for (;it!=mThreadXid.end();it++)
			{
				static map<Int32, HawkApp_TickTask> mTickTask;

				HawkApp_TickTask* pTask = 0;
				map<Int32, HawkApp_TickTask>::iterator tit = mTickTask.find(it->first);
				if (tit == mTickTask.end())
				{
					mTickTask.insert(std::make_pair(it->first, HawkApp_TickTask(it->second)));
					pTask = &mTickTask.find(it->first)->second;
				}
				else
				{
					pTask = &tit->second;
					pTask->ResetXids(it->second);
				}

				if (pTask)
				{
					pTask->AddRef();
					PostAppTask(pTask, it->first);
				}				
			}
		}

		return true;
	}

	Bool HawkApp::PostAppTask(HawkTask* pTask, Int32 iThreadIdx)
	{
		if(m_bRunning && m_pThreadPool)
		{
			if(iThreadIdx < 0)
			{
				A_Exception(m_pThreadPool->GetThreadNum() > 0);
				iThreadIdx = HawkRand::RandInt(0, m_pThreadPool->GetThreadNum()-1);
			}

			return m_pThreadPool->AddTask(pTask, iThreadIdx);
		}
		return false;
	}

	Bool HawkApp::DispatchProto(const XID& sXid, SID iSid, Protocol* pProto)
	{
		if(sXid.IsValid() && pProto)
		{
			SafeObj obj(sXid);
			if (obj.IsObjValid())
				return obj->OnProtocol(iSid, pProto);
		}	
		return false;
	}

	Bool HawkApp::DispatchMsg(const XID& sXid, HawkMsg* pMsg)
	{
		if(sXid.IsValid() && pMsg && pMsg->Msg > 0 && pMsg->Target.IsValid())
		{
			SafeObj obj(pMsg->Target);
			if (obj.IsObjValid())
				return obj->OnMessage(*pMsg);		
		}
		return false;
	}

	Bool HawkApp::DispatchTick(const XID& sXid)
	{
		if(sXid.IsValid())
		{
			SafeObj obj(sXid);
			if (obj.IsObjValid())
				return obj->OnTick();		
		}
		return false;
	}
}
