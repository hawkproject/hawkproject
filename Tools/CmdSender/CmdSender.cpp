#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	AString sAddr = "192.168.0.109:8000";
	if (argc >= 2) sAddr = argv[1];
	
	HawkSocket sSocket;
	if (sSocket.InitTcpClient(sAddr))
	{
		while (true)
		{
			HawkPrint("input send command: ", false);
			
			Char szCmd[DEFAULT_SIZE] = {0};
			scanf("%s", szCmd);
			Size_t iSize = strlen(szCmd) + 1;
			if (iSize)
			{
				sSocket.Send(szCmd, iSize);
				HawkPrint("send ok.");
			}

			if (sSocket.UpdateEvent(HEVENT_READ, 1000))
			{
				Char szRecv[DEFAULT_SIZE] = {0};
				iSize = DEFAULT_SIZE;
				if (sSocket.Receive(szRecv, iSize))
				{
					HawkFmtPrint("recv cmd: %s", szRecv);
				}
			}
		}
	}

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

