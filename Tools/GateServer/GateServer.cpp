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

	if (argc >= 2)
	{
		HawkOSOperator::ClearConsole();
		HawkOSOperator::Gotoxy(0, 0);

		HawkPrint("==============================================================");
		HawkPrint("==================Gateway, Author: Daijunhua==================");
		HawkPrint("==============================================================");

		HawkGateway* pGateway = new HawkGateway;
		AString sGwCfg		  = argv[1];
		AString sGwSecurity   = "";
		if (argc >= 3)
			sGwSecurity		  = argv[2];
	
		if (pGateway->InitCfg(sGwCfg, sGwSecurity))
		{
#ifdef _DEBUG
			g_DbgThread = new HawkThread(DoKeyDownExit);
			if (g_DbgThread)
				g_DbgThread->Start(pGateway);
#endif	
			
			pGateway->Run();
		}

		HAWK_RELEASE(g_DbgThread);
		HAWK_RELEASE(pGateway);
	}
	else
	{
		printf("usage: gateserver <gw.cfg> <gw-security.cfg>.\r\n");
		HawkOSOperator::WaitKeyboardInput();
		return -1;
	}

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
