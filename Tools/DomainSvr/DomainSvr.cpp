#include <HawkUtil.h>

Bool StartService(const AString& sSvrAddr)
{
	HawkSocket sSocket;
	if(!sSocket.Create())
	{
		HawkFmtPrint("Create Socket Error, Addr: %s",sSvrAddr.c_str());
		return false;
	}

	if (!sSocket.Bind(SocketAddr(sSvrAddr)))
	{
		HawkFmtPrint("Bind Address Error, Addr: %s",sSvrAddr.c_str());
		return false;
	}

	if (!sSocket.Listen())
	{
		HawkFmtPrint("Listen Socket Error, Addr: %s",sSvrAddr.c_str());
		return false;
	}

	Int32 iTimeout    = 500;
	Char sData[1024]  = {0};

	Size_t iRecv	  = 0;
	Size_t iSend	  = 0;
	Size_t iReal	  = 0;

	AString sReq	  = "<policy-file-request/>";
	AString sRep	  = "<cross-domain-policy> <allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy> ";

	while(true)
	{
		HawkSocket sPeer;
		SocketAddr sAddr;		

		if(sSocket.Accept(sPeer,sAddr))
		{
			AString sTime = HawkOSOperator::GetTimeString();

			HawkFmtPrint("<?>Verify Connect: %s£¬Time: %s",sAddr.ToString().c_str(),sTime.c_str());

			sPeer.SetOption(SO_RCVTIMEO, &iTimeout, sizeof(iTimeout), SOL_SOCKET);
			sPeer.SetOption(SO_SNDTIMEO, &iTimeout, sizeof(iTimeout), SOL_SOCKET);

			iRecv = 1024;
			memset(sData,0,iRecv);
			if(sPeer.Receive(sData, iRecv))
			{
				AString sQuery = sData;
				if (sQuery == sReq)
				{
					HawkFmtPrint("<!>Verify Request: %s",sReq.c_str());

					iSend = 1024;
					memset(sData,0,iSend);
					memcpy(sData,sRep.c_str(),sRep.size());
					iSend = sRep.size() + 1;
					iReal = iSend;

					if(sPeer.Send(sData, iReal) && iReal == iSend)
					{
						HawkFmtPrint("<!>Verify Response: %s\r\n",sRep.c_str());
					}
					else
					{
						HawkFmtPrint("<x>Socket Send Error, ErrCode: %s",sPeer.GetSocketErr());						
					}
				}
				else
				{
					HawkFmtPrint("<x>Receive Data Error, Data: %s",sData);
				}
			}
			else
			{
				HawkFmtPrint("<x>Socket Receive Error, ErrCode: %d",sPeer.GetSocketErr());
			}

			sPeer.Close();
		}
	}
}

int main(int argc, Char* argv[])
{
	HawkUtil::Init();	

	HawkPrint("==============================================================");
	HawkPrint("              Flash Domain Verify Server Running");
	HawkPrint("==============================================================");

	StartService("*:843");

	HawkOSOperator::WaitKeyboardInput("Please Press Any Key Exist......",true);

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
