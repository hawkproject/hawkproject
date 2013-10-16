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

	HawkAppGateway::HawkAppGateway(HawkAppFrame* pApp) : m_pApp(pApp), m_pThread(0), m_iPlatform(GATE_PF_NONE)
	{
	}

	
	HawkAppGateway::~HawkAppGateway()
	{
		HAWK_RELEASE(m_pThread);
	}

	Bool HawkAppGateway::Init()
	{
		const HawkAppFrame::AppCfg& sCfg = *m_pApp->GetAppCfg();
		if (!HawkGateway::Init(sCfg.Gateway.Address,sCfg.Gateproxy.Address, sCfg.Gateway.Threads, sCfg.Gateway.Mode))
		{
			HawkFmtPrint("AppGateway Init Error, Addr: %s, Threads: %d, Mode: %d", sCfg.Gateway.Address.c_str(), sCfg.Gateway.Threads, sCfg.Gateway.Mode);
			return false;
		}

		//加密模式
		if (sCfg.Gateway.Security)
			TurnGateSecurity();

		//性能监视
		if (sCfg.Gateway.Profiler.size())
			TurnOnProfiler(sCfg.Gateway.Profiler);

		//设置超时
		SetSessionTimeout(sCfg.Gateway.Timeout);

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

	Bool HawkAppGateway::TrustSession(void* pSession, OctetsStream* pIBuffer)
	{
		if (!pSession || !pIBuffer)
			return false;
		
		//QQ平台网关校验信息过滤
		if (m_iPlatform == GATE_PF_QQ)
		{
			if (pIBuffer->AvailableSize() >= TGW_BUFFER_SIZE)
			{
				AString sTgw = (Char*)pIBuffer->AvailableData();

				#ifdef _DEBUG
					HawkFmtPrint("TGW: %s", sTgw.c_str());
				#endif

				Bool bStart = HawkStringUtil::BeginWith<AString>(sTgw, "tgw_l7_forward");
				Bool bEnd   = HawkStringUtil::EndWith<AString>(sTgw, "\r\n\r\n");
				if (bStart && bEnd)
				{
					UInt32 iCrc  = HawkOSOperator::CalcCrc((const UChar*)sTgw.c_str(),(UInt32)sTgw.size());
					UInt32* pCrc = (UInt32*)((Char*)pIBuffer->AvailableData() + (TGW_BUFFER_SIZE - sizeof(UInt32)));
					if (pCrc && iCrc == *pCrc)
					{
						pIBuffer->MoveNonius(TGW_BUFFER_SIZE);
						return true;
					}
				}
			}
			return false;
		}

		return true;
	}
}
