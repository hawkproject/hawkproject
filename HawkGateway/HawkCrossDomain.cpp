#include "HawkGateway.h"

namespace Hawk
{
	PVoid hawk_CrossDomainRoutine(void* pArgs)
	{
		HawkGateway* pGateway = (HawkGateway*)pArgs;
		HawkAssert(pGateway);

		//检测跨域服务是否已经开启
		{
			HawkSocket sTest;
			if(sTest.Create() && sTest.Connect(SocketAddr("127.0.0.1:843")))
			{
				HawkFmtError("Cross Domain Security Port Busy.");
				return 0;
			}
		}

		HawkSocket sSocket;
		if(!sSocket.Create() || !sSocket.SetBlocking(false) || !sSocket.Bind(SocketAddr("*:843")) || !sSocket.Listen())
			return 0;

		Char szReq[] = "<policy-file-request/>";
		Char szRep[] = "<cross-domain-policy> <allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy> ";

		while (pGateway && pGateway->IsRunning())
		{
			if (!sSocket.UpdateEvent(HEVENT_READ, 1000))
				continue;

			HawkSocket sPeer;
			SocketAddr sAddr;
			if(sSocket.Accept(sPeer, sAddr))
			{
				Size_t iBufSize = DEFAULT_SIZE;
				Char szBufData[DEFAULT_SIZE] = {0};

				//设置非阻塞
				sSocket.SetNoDelay(true);
				sSocket.SetBlocking(false);

				//等待事件触发
				if(sPeer.UpdateEvent(HEVENT_READ, 1000) && sPeer.Receive(szBufData, iBufSize))
				{
					if (strcmp(szReq, szBufData) == 0)
					{
						memset(szBufData, 0, DEFAULT_SIZE);
						strcpy(szBufData, szRep);
						iBufSize = strlen(szBufData) + 1;
						sPeer.Send(szBufData, iBufSize);
					}				
				}
				sPeer.Close();
			}
		}
		return 0;
	}

	Bool HawkGateway::TurnOnCrossDomain()
	{
		if (!m_bRunning && !m_pCrossDomain)
		{
			m_pCrossDomain = new HawkThread(hawk_CrossDomainRoutine);
			return true;
		}
		return false;
	}
}
