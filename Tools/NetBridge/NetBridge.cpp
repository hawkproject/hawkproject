#include "HawkUtil.h"

struct ConnBridge 
{
	HawkSocket Peer;
	SocketAddr Addr;
	HawkSocket Bridge;
};

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	if (argc >= 3) 
	{
		AString sAddr   = argv[1];
		AString sBridge = argv[2];

		map<SOCKET, ConnBridge*> mBridgeMap;

		HawkSocket sAcceptor;
		if (!sAcceptor.InitTcpServer(SocketAddr(sAddr)))
			return false;

		while (true)
		{
			if (sAcceptor.UpdateEvent(HEVENT_READ))
			{
				ConnBridge* pBridge = new ConnBridge;
				if (sAcceptor.Accept(pBridge->Peer, pBridge->Addr) &&
					pBridge->Bridge.InitTcpClient(SocketAddr(sBridge)))
				{
					mBridgeMap[pBridge->Peer.Handle()] = pBridge;

					HawkFmtPrint("Bridge Connect, %s->%s->%s", pBridge->Addr.ToString().c_str(), sAddr.c_str(), sBridge.c_str());
				}
				else
				{
					HAWK_DELETE(pBridge);
				}
			}

			map<SOCKET, ConnBridge*>::iterator it = mBridgeMap.begin();
			for (;it!=mBridgeMap.end();)
			{
				ConnBridge* pBridge = it->second;
				if (pBridge->Peer.UpdateEvent(HEVENT_READ))
				{
					Char szBuff[DEFAULT_SIZE] = {0};
					Size_t iSize = DEFAULT_SIZE;
					if (!pBridge->Peer.Receive(szBuff, iSize) || !pBridge->Bridge.Send(szBuff, iSize))
					{
						HawkFmtPrint("Bridge Disconnect, %s->%s->%s", pBridge->Addr.ToString().c_str(), sAddr.c_str(), sBridge.c_str());

						pBridge->Peer.Close();
						pBridge->Bridge.Close();
						HAWK_DELETE(pBridge);
						mBridgeMap.erase(it++);
						continue;
					}					
				}

				if (pBridge->Bridge.UpdateEvent(HEVENT_READ))
				{
					Char szBuff[DEFAULT_SIZE] = {0};
					Size_t iSize = DEFAULT_SIZE;
					if (!pBridge->Bridge.Receive(szBuff, iSize) || !pBridge->Peer.Send(szBuff, iSize))
					{
						HawkFmtPrint("Bridge Disconnect, %s->%s->%s", pBridge->Addr.ToString().c_str(), sAddr.c_str(), sBridge.c_str());

						pBridge->Peer.Close();
						pBridge->Bridge.Close();
						HAWK_DELETE(pBridge);
						mBridgeMap.erase(it++);
						continue;
					}					
				}

				it ++;
			}
		}
	}
	else
	{
		printf("usage: netbridge <host-ip:port> <bridge-ip:port>.\r\n");
	}

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

