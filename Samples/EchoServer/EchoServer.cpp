#include "EchoProto.h"
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

	REGISTER_PROTO(ProtoA);
	REGISTER_PROTO(ProtoB);
	REGISTER_PROTO(ProtoC);

	if (argc >= 3)
	{
		HawkOSOperator::ClearConsole();
		HawkOSOperator::Gotoxy(0, 0);

		HawkPrint("==============================================================");
		HawkPrint("================EchoServer, Author: Daijunhua=================");
		HawkPrint("==============================================================");

		HawkGateway* pGateway = new HawkGateway;
		AString sAddr = argv[1];
		Int32 iThreads = HawkStringUtil::StringToInt<AString>(argv[2]);

		if (pGateway->Init(sAddr, "0.0.0.0:0", iThreads, 0))
		{
#ifdef _DEBUG
			g_DbgThread = new HawkThread(DoKeyDownExit);
			if (g_DbgThread)
				g_DbgThread->Start(pGateway);
#endif	

			if (argc >= 4)
			{
				Bool bSecurity = HawkStringUtil::StringToBool<AString>(argv[3]);
				if (bSecurity)
					pGateway->TurnGateSecurity();
			}

			pGateway->Run();
		}

		HAWK_RELEASE(g_DbgThread);
		HAWK_RELEASE(pGateway);
	}
	else
	{
		printf("usage: echoserver <ip:port> <threads> <security>.\r\n");
		HawkOSOperator::WaitKeyboardInput();
		return -1;
	}

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
