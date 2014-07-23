#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	Char sFile[256]   = {0};
	AString sArgs     = "cpp";

	//默认协议文件
	if (HawkOSOperator::ExistFile("Protocol.xml"))
	{
		strcpy(sFile, "Protocol.xml");
	}
	else if (HawkOSOperator::ExistFile("HawkProtocol.xml"))
	{
		strcpy(sFile, "HawkProtocol.xml");
	}
	else if (HawkOSOperator::ExistFile("HsProtocol.xml"))
	{
		strcpy(sFile, "HsProtocol.xml");
	}
	else if (HawkOSOperator::ExistFile("HawkSysProtocol.xml"))
	{
		strcpy(sFile, "HawkSysProtocol.xml");
	}
	
	if (!strlen(sFile))
	{
		//提示输入
		if (argc < 2)
		{
			HawkPrint("Input Protocol XmlFile: ", false);
			scanf("%s",sFile);
		}
		else
		{
			//读取协议文件
			memcpy(sFile, argv[1], strlen(argv[1]));
			
			//读取生成模式参数
			for (Int32 i=2;i< argc;i++)
			{
				sArgs += " ";
				sArgs += argv[i];
			}
		}
	}
	else
	{
		//读取生成模式参数
		for (Int32 i=1;i<argc;i++)
		{
			sArgs += " ";
			sArgs += argv[i];
		}
	}
	
	if(!P_ProtocolManager->GenProtocols(sFile, sArgs))
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
