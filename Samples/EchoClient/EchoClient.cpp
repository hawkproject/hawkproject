#include "HawkUtil.h"
#include "EchoProto.h"
#include "GsProtocol.h"

#define LOGIN_CLIENT 1
#define PRINT_PROTO  1
#define THREAD_TRUNK 10
#define PROTO_TIME	 200

volatile Bool		g_Running = true;
AString				g_IpAddr  = "";
Int32				g_Count   = 1;
vector<HawkThread*>	g_Threads;

PVoid hawk_EchoClientRoutine(void* pArg)
{
	HawkSession session;
	if (session.Init(g_IpAddr))
	{
#if LOGIN_CLIENT
		LoginGame login;
		login.m_sName = HawkStringUtil::RandomString<UString>(HawkRand::RandInt(8,16));
		login.m_sPwd  = HawkStringUtil::RandomString<UString>(HawkRand::RandInt(8,16));
		session.SendProtocol(&login);
#endif
		
		Bool bShutdown = false;

		while (g_Running)
		{
			//更新
			if (!session.Tick(PROTO_TIME))
				break;			

#if LOGIN_CLIENT
			if (bShutdown)
			{
				//重新登录
				LoginGame login;
				login.m_sName = HawkStringUtil::RandomString<UString>(HawkRand::RandInt(8,16));
				login.m_sPwd  = HawkStringUtil::RandomString<UString>(HawkRand::RandInt(8,16));
				session.SendProtocol(&login);

				HawkSleep(1000);
			}

			MsgNotify notify;
			notify.m_sMsg  = HawkStringUtil::RandomString<UString>(HawkRand::RandInt(32,128));
			notify.m_iMode = HAWK_ERROR;
			session.SendProtocol(&notify);	
#else
			//发送协议
			Int32 iType = HawkRand::RandInt(PROTO_A, PROTO_C);

			if (iType == PROTO_A)
			{
				ProtoA sCmd;
				session.SendProtocol(&sCmd);
			}
			else if (iType == PROTO_B)
			{
				ProtoB sCmd;
				session.SendProtocol(&sCmd);
			}
			else if (iType == PROTO_C)
			{
				ProtoC sCmd;
				session.SendProtocol(&sCmd);
			}
#endif

			//接收协议
			Protocol* pProto = 0;
			while (session.DecodeProtocol(pProto) && pProto)
			{
#if (PRINT_PROTO == 1)
				ProtoType iType = pProto->GetType();
				if (iType == PROTO_A)
				{
					ProtoA* pA = (ProtoA*)pProto;
					HawkFmtPrint("Msg: %s",pA->m_sMsg.c_str());
				}
				else if (iType == PROTO_B)
				{
					ProtoB* pB = (ProtoB*)pProto;
					HawkFmtPrint("Size: %d, Time: %d",pB->m_iSize, pB->m_iTime);
				}
				else if (iType == PROTO_C)
				{
					ProtoC* pC = (ProtoC*)pProto;
					HawkFmtPrint("X: %f, Y: %f, Z: %f",pC->m_fX, pC->m_fY, pC->m_fZ);
				}
				else if (iType == ProtocolId::MSG_NOTIFY)
				{
					MsgNotify* pNotify = (MsgNotify*)pProto;

					if (g_Count <= 1)
					{
						HawkFmtPrint("Msg: %s",pNotify->m_sMsg.c_str());
					}					

					bShutdown = false;
				}
				else if (iType == SysProtocol::SYS_SERVER_SHUTDOWN)
				{
					bShutdown = true;					
				}
#endif
				P_ProtocolManager->ReleaseProto(pProto);
			}

			HawkSleep(PROTO_TIME);			
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

	#include "ProtocolReg.inl"

	if (argc >= 2) 
		g_IpAddr = argv[1];

	if (!g_IpAddr.size())
		g_IpAddr = HawkOSOperator::ConsoleInput();

	if (argc >= 3)
		g_Count = HawkStringUtil::StringToInt<AString>(argv[2]);

	for (Int32 i=0;i<g_Count;i++)
	{
		HawkThread* pThread = new HawkThread(hawk_EchoClientRoutine);
		pThread->Start();
		g_Threads.push_back(pThread);

		HawkSleep(10);
	}	

	while (HawkOSOperator::WaitKeyboardInput("",false) != 'Q');
	
	g_Running = false;

	for (Size_t i=0;i<g_Threads.size();i++)
	{
		g_Threads[i]->Close();
		HAWK_RELEASE(g_Threads[i]);
	}

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

