#include "HawkAppGateway.h"
#include "HawkAppFrame.h"

#define TGW_BUFFER_SIZE 80

namespace Hawk
{
	PVoid hawk_AppFrameGatewayRoutine(void* pArgs)
	{
		HawkAppGateway* pGateway = (HawkAppGateway*)pArgs;
		if (pGateway)
			pGateway->Run();
		
		return 0;
	}

	HawkAppGateway::HawkAppGateway(HawkAppFrame* pApp) : m_pApp(pApp), m_pThread(0)
	{
	}

	
	HawkAppGateway::~HawkAppGateway()
	{
		HAWK_RELEASE(m_pThread);
	}

	Bool HawkAppGateway::Init()
	{
		const HawkAppFrame::AppCfg& sCfg = *m_pApp->GetAppCfg();

		if (!HawkGateway::InitCfg(sCfg.Gateway.GwCfg, sCfg.Gateway.GwSecurity))
		{
			HawkFmtPrint("AppGateway Init Error, GwCfg: %s, GwSecurity: %s", sCfg.Gateway.GwCfg.c_str(), sCfg.Gateway.GwSecurity.c_str());
			return false;
		}	

		//开启网关服务器线程
		if (!m_pThread)
		{
			m_pThread = new HawkThread(hawk_AppFrameGatewayRoutine);
			m_pThread->Start(this);
		}

		return true;
	}

	Bool HawkAppGateway::Stop()
	{
		if (HawkGateway::Stop())
		{
			if (m_pThread)
			{
				m_pThread->Close();
				HAWK_RELEASE(m_pThread);
			}
		}
		return true;
	}
}
