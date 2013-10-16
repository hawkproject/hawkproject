#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	Char sFile[256]   = {0};
	AString sHawkHead = "HawkUtil.h";

	if (argc < 2)
	{
		if (HawkOSOperator::ExistFile("Protocol.xml"))
		{
			strcpy(sFile, "Protocol.xml");
		}
		else if (HawkOSOperator::ExistFile("HsProtocol.xml"))
		{
			strcpy(sFile, "HsProtocol.xml");
		}
		else
		{
			HawkPrint("Input Protocol XmlFile: ");
			scanf("%s",sFile);
		}
	}
	else
	{
		//读取协议文件
		if (argc >= 2)
			memcpy(sFile, argv[1], strlen(argv[1]));

		//读取通用头文件
		if (argc >= 3)
			sHawkHead = argv[2];
	}
	
	if(!P_ProtocolManager->GenProtocols(sFile, sHawkHead))
	{
		HawkPrint("\r\nGen Protocols Failed.");
	}
	else
	{
		HawkPrint("\r\nGen Protocols Success.");
#ifdef PLATFORM_WINDOWS
		if (argc != 2)
		{
			HawkOSOperator::WaitKeyboardInput();
		}
#endif
	}

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
