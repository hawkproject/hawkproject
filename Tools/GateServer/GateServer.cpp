#include "HawkUtil.h"
#include "HawkGateway.h"

HawkThread* g_DbgThread = 0;

PVoid DoKeyDownExit(void* pArg)
{
	HawkGateway* pGateway = (HawkGateway*)pArg;
	while (pGateway)
	{
		if(HawkOSOperator::WaitKeyboardInput("",false) == 'Q')
		{
			pGateway->Stop();
			return 0;
		}
	}
	return 0;
}

int main(int argc, Char* argv[])
{
	HawkUtil::Init();	

	if (argc >= 5)
	{
		HawkOSOperator::ClearConsole();
		HawkOSOperator::Gotoxy(0, 0);

		HawkPrint("==============================================================");
		HawkPrint("==================Gateway, Author: Daijunhua==================");
		HawkPrint("==============================================================");

		HawkGateway* pGateway = new HawkGateway;
		AString sFrontend = argv[1];
		AString sBackend  = argv[2];
		Int32 iThreads    = HawkStringUtil::StringToInt<AString>(argv[3]);
		Int32 iGateMode   = HawkStringUtil::StringToInt<AString>(argv[4]);

		if (pGateway->Init(sFrontend, sBackend, iThreads, iGateMode))
		{
#ifdef _DEBUG
			g_DbgThread = new HawkThread(DoKeyDownExit);
			if (g_DbgThread)
				g_DbgThread->Start(pGateway);
#endif	

			if (argc >= 6)
			{
				Bool bSecurity = HawkStringUtil::StringToBool<AString>(argv[5]);
				if (bSecurity)
					pGateway->TurnGateSecurity();
			}

			if (argc >= 7)
			{
				pGateway->TurnOnProfiler(argv[6]);
			}

			pGateway->Run();
		}

		HAWK_RELEASE(g_DbgThread);
		HAWK_RELEASE(pGateway);
	}
	else
	{
		printf("usage: gateserver <frontend-ip:port> <backend-ip:port> <threads> <gatemode> <security> <profiler-ip:port>.\r\n");
		HawkOSOperator::WaitKeyboardInput();
		return -1;
	}

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
