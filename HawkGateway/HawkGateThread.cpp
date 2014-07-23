#include "HawkGateThread.h"
#include "HawkGateway.h"
#include "event.h"
#include "zmq.h"

namespace Hawk
{
	//�߳���ں���
	PVoid hawk_GateThreadRoutine(void* pArgs)
	{
		HawkGateThread* pThread = (HawkGateThread*)pArgs;
		HawkAssert(pThread);
		if (pThread)
			pThread->OnThreadLoop();

		return 0;
	}

	//�Ự���¼�������
	void hawk_GateSessionRead(bufferevent* pEvent, void* pArgs) 
	{
		HawkGateThread::Session* pSession = (HawkGateThread::Session*)pArgs;
		HawkAssert(pSession);
		if (pSession && pSession->GThread)
			pSession->GThread->OnSessionEvent(HEVENT_READ, pSession);
	}

	//�Ựд�¼�������
	void hawk_GateSessionWrite(bufferevent* pEvent, void* pArgs) 
	{
		HawkGateThread::Session* pSession = (HawkGateThread::Session*)pArgs;
		HawkAssert(pSession);
		if (pSession && pSession->GThread)
			pSession->GThread->OnSessionEvent(HEVENT_WRITE, pSession);
	}

	//�Ự�����¼�������
	void hawk_GateSessionError(bufferevent* pEvent, Short iWhat, void* pArgs) 
	{
		HawkGateThread::Session* pSession = (HawkGateThread::Session*)pArgs;
		HawkAssert(pSession);
		if (pSession && pSession->GThread)
			pSession->GThread->OnSessionEvent(HEVENT_EXCEPT, pSession, iWhat);
	}

	//////////////////////////////////////////////////////////////////////////

	HawkGateThread::HawkGateThread(HawkGateway* pGateway)
	{
		m_pThread	= 0;
		m_pBase		= 0;
		m_pZmq		= 0;
		m_iBaseSid	= 0;
		m_iSidStep	= 0;
		m_iCurSid	= 0;
		m_pOctets	= 0;
		m_bIdle	    = true;
		m_bRunning  = false;
		m_pGateway	= pGateway;
	}

	HawkGateThread::~HawkGateThread()
	{
		//�ͷŻỰ�б�
		FreeSessionMap();

		//ɾ�������¼�
		if (m_pBase)
		{
			event_base_free((event_base*)m_pBase);
			m_pBase = 0;
		}

		//�ر���Ϣ����
		P_ZmqManager->CloseZmq(m_pZmq);

		//�ͷ���Ϣ����
		HAWK_RELEASE(m_pOctets);

		//�ͷ��̺߳���
		HAWK_RELEASE(m_pThread);		
	}

	Bool HawkGateThread::Init(UInt32 iIdentify, UInt32 iBaseSid, UInt32 iSidStep)
	{
		HawkAssert(!m_pThread && !m_pBase && !m_pZmq);

		m_iBaseSid = iBaseSid + iIdentify;
		m_iSidStep = iSidStep;

		//����ͨ�û���
		if (!m_pOctets)
			m_pOctets = new OctetsStream(m_pGateway->GetBufSize());

		//�����߳�
		if (!m_pThread)
			m_pThread = new HawkThread(hawk_GateThreadRoutine);

		//�����¼���������
		if (!m_pBase)
		{
			event_config* pCfg = event_config_new();
			if (!pCfg) 
			{
				HawkPrint("Create EventConfig Failed.");
				return false;
			}
			
#ifdef PLATFORM_LINUX
			event_config_require_features(pCfg, EV_FEATURE_ET);
#endif
			event_config_set_flag(pCfg, EVENT_BASE_FLAG_NOLOCK);

			m_pBase = (void*)event_base_new_with_config(pCfg);
			event_config_free(pCfg);
			if (!m_pBase)
			{
				HawkPrint("Create EventBase Failed.");
				return false;
			}

			static Bool show_kernel_event_mechanism = true;
			if (show_kernel_event_mechanism)
			{
				show_kernel_event_mechanism = false;

				const Char* pszMethod = event_base_get_method((event_base*)m_pBase);
				if (pszMethod && strlen(pszMethod))
				{
					HawkFmtPrint("LibEvent, Version: %s, Kernel Mechanism: %s", event_get_version(), pszMethod);
				}
			}			
		}

		//����ZMQ����
		if (!m_pZmq)
		{
			m_pZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_DEALER);

			UInt64 iZmqId = HawkZmq::GenValidZid(iIdentify);
			m_pZmq->SetIdentity(&iZmqId, (Int32)sizeof(iZmqId));
			
			m_pZmq->Connect(m_pGateway->GetThreadZmqAddr());
		}
		
		return true;
	}

	Bool HawkGateThread::Start()
	{
		if (m_pThread && m_pBase && m_pZmq && !m_bRunning)
		{
			m_bRunning = true;

			if (m_pThread)
				m_pThread->Start(this);

			return true;
		}
		return false;
	}

	Bool HawkGateThread::Close()
	{
		//�ȴ��߳̽���
		if (m_pThread)
			m_pThread->Close();

		//�ͷŵ�ǰ����
		FreeSessionMap();

		return true;
	}	

	Int32 HawkGateThread::GetThreadId() const
	{
		if (m_pThread)
			return m_pThread->GetThreadId();

		return 0;
	}

	SID  HawkGateThread::GenSessionId()
	{
		HawkAssert(m_pGateway);		
		SID iSid = m_iCurSid++ * m_iSidStep + m_iBaseSid;
		
		//����ĳЩ���Ի�ƽ̨��֧�ָ����Ựid
		if (iSid >= 0x80000000)
		{
			m_iCurSid = 0;
			iSid = m_iCurSid++ * m_iSidStep + m_iBaseSid;
		}
		
		return iSid;
	}

	HawkGateThread::Session* HawkGateThread::AllocSession(SOCKET hSocket, const SocketAddr& sAddr)
	{
		if (hSocket != INVALID_SOCKET)
		{			
			Session* pSession = (Session*)HawkMalloc(sizeof(Session));
			memset(pSession, 0, sizeof(Session));

			pSession->GThread		= this;
			pSession->Socket		= hSocket;
			pSession->Trust			= false;
			pSession->ProtoFreq		= 0;
			pSession->FreqTime		= 0;
			pSession->Sid			= GenSessionId();
			pSession->State			= SESSION_STATE_CLOSED;
			pSession->Addr			= new SocketAddr(sAddr);
			pSession->IBuffer		= new OctetsStream(m_pGateway->GetBufSize());			
			pSession->ProtoTypeCrc	= new ProtoCrcMap;

			HawkSocket::SetBlocking(pSession->Socket, false);
			m_pGateway->CreateISecurity(pSession->ISecurity);
			m_pGateway->CreateOSecurity(pSession->OSecurity);
	
			return pSession;
		}
		return 0;
	}

	Bool HawkGateThread::FreeSession(Session* pSession)
	{
		if (pSession)
		{
			//�ͷ��¼�
			if (pSession->Event)
			{
				bufferevent_free((bufferevent*)pSession->Event);
				pSession->Event = 0;
			}
	
			//�ͷ��ڴ�
			pSession->ProtoTypeCrc->clear();
			HAWK_DELETE(pSession->ProtoTypeCrc);
			HAWK_RELEASE(pSession->Addr);
			HAWK_RELEASE(pSession->IBuffer);
			HAWK_RELEASE(pSession->ISecurity);
			HAWK_RELEASE(pSession->OSecurity);			
			
			//�ͷŻỰ
			HawkFree(pSession);

			return true;
		}
		return false;
	}

	Bool HawkGateThread::FreeSessionMap()
	{
		SessionMap::iterator it = m_mSession.begin();
		for (;it!=m_mSession.end();it++)
		{
			Session* pSession = it->second;
			FreeSession(pSession);
		}
		m_mSession.clear();
		return true;
	}

	Bool HawkGateThread::OnThreadLoop()
	{
		if (m_pBase)
		{
			//��־��¼
			HawkFmtPrint("GateThread EventLoop, ThreadId: %u", HawkOSOperator::GetThreadId());	

			while (m_bRunning)
			{
				//����Ĭ�Ͽ���״̬
				m_bIdle = true;

				//��ʼ��Ϣѭ��
				event_base_loop((event_base*)m_pBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);

				//��ȡ���ط��͹�����������Ϣ
				OnGatewayEvent();

				//����״̬
				OnThreadIdle();
			}
			return true;
		}
		return false;
	}

	Bool HawkGateThread::StartSession(SOCKET hSocket, const SocketAddr& sAddr, Session** ppSession)
	{
		//�׽�����Ч
		if (hSocket == INVALID_SOCKET)
			return false;

		//���������¼�
		bufferevent* pEvent = bufferevent_socket_new((event_base*)m_pBase, hSocket, BEV_OPT_CLOSE_ON_FREE);
		if (!pEvent)
			return false;

		//�����Ự
		Session* pSession = AllocSession(hSocket, sAddr);
		if (!pSession)
		{
			bufferevent_free(pEvent);
			return false;
		}
		pSession->Event = pEvent;
		pSession->State = SESSION_STATE_ACTIVE;

		//���ûص�
		bufferevent_setcb(pEvent, hawk_GateSessionRead, hawk_GateSessionWrite, hawk_GateSessionError, pSession);

		//���ö���ʱ, ����Ϊ��������
		Int32 iTimeout = m_pGateway->GetSessionTimeout();
		if (iTimeout > 0)
		{
			struct timeval tv;
			tv.tv_sec  = iTimeout / 1000;
			tv.tv_usec = (iTimeout % 1000) * 1000; 
			bufferevent_set_timeouts(pEvent, &tv, NULL);
		}
		//������д�¼�
		bufferevent_enable(pEvent, EV_READ | EV_WRITE);

		//��ӵ��Ự�б�
		m_mSession[pSession->Sid] = pSession;
		if (ppSession)
			*ppSession = pSession;

		//�������ܼ�����
		if (m_pGateway->GetProfiler())
			m_pGateway->GetProfiler()->RegConnect(true);		

		//�����־
		HawkFmtLog("SessionStart, Sid: %u, Address: %s", pSession->Sid, pSession->Addr->ToString().c_str());				

		return true;
	}

	Bool HawkGateThread::CloseSession(SID iSid, Int32 iErrCode)
	{
		if (iSid)
		{
			SessionMap::iterator it = m_mSession.find(iSid);
			if (it != m_mSession.end())
			{
				//�޳��Ự�б�
				Session* pSession = it->second;
				m_mSession.erase(it);

				//�����־
				if (iErrCode)
					HawkFmtLog("SessionClose, Sid: %u, Address: %s, ErrCode: %d, SocketErr: %d", pSession->Sid, pSession->Addr->ToString().c_str(), iErrCode, HawkSocket::GetSocketErr());
				else
					HawkFmtLog("SessionClose, Sid: %u, Address: %s", pSession->Sid, pSession->Addr->ToString().c_str());

				//�ͷŻỰ
				FreeSession(pSession);

				//�������ܼ�����
				if (m_pGateway->GetProfiler())
					m_pGateway->GetProfiler()->RegConnect(false);				

				return true;
			}
		}		
		return false;
	}

	Bool HawkGateThread::RecvGateMsg(GateMsgHeader& sHeader, OctetsStream* pOctets)
	{
		if (m_pZmq && pOctets)
		{
			//����SIDͷ��Ϣ
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pZmq->Recv(pOctets->Begin(), iSize))
				return false;

			sHeader = *((GateMsgHeader*)pOctets->Begin());
			Bool bRecvMore = m_pZmq->IsWaitRecv();			
			HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
			if (iSize != sizeof(sHeader) || !bRecvMore)
				return false;

			//����������
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateThread::RecvGateExtData(OctetsStream* pOctets)
	{
		if (m_pZmq && pOctets)
		{
			//��Ϣ״̬У��
			Bool bRecvMore = m_pZmq->IsWaitRecv();
			HawkAssert(bRecvMore);
			if (!bRecvMore)
				return false;

			//����������
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateThread::SendGateMsg(const ThreadMsgHeader& sHeader, void* pData, Size_t iSize)
	{
		if (m_pZmq)
		{			
			//������Ϣͷ
			if (!m_pZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			//����������
			if (!m_pZmq->Send(pData, iSize))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkGateThread::SendProtocol(Session* pSession, Protocol* pProto)
	{
		if (pSession && pProto)
		{
			//����Э�鵽������
			m_pOctets->Clear();
			pProto->Encode(*m_pOctets);

			if (OnSessionEncode(pSession, m_pOctets))
			{
				//д��������
				if (bufferevent_write((bufferevent*)pSession->Event, m_pOctets->Begin(), m_pOctets->Size()) != HAWK_OK)
				{
					HawkFmtPrint("Event Buffer Write Error, Size: %u", m_pOctets->Size());
					return false;
				}

				//�������ܼ�����
				if (m_pGateway->GetProfiler())
					m_pGateway->GetProfiler()->RegSendProto(pProto->GetType(), m_pOctets->Size());

				return true;
			}
		}
		return false;
	}

	Bool HawkGateThread::OnGatewayEvent()
	{
		while (m_pZmq && m_pZmq->PollEvent(HEVENT_READ, 0))
		{
			//���÷ǿ���
			m_bIdle = false;

			//�������ط��͵�����
			GateMsgHeader sHeader;
			if (!RecvGateMsg(sHeader, m_pOctets))
				return false;

			//�ỰЭ��ת��
			if (sHeader.Sid)
			{
				SessionMap::iterator it = m_mSession.find(sHeader.Sid);
				if (it != m_mSession.end() && it->second)
				{
					Session* pSession = it->second;

					//��ȡЭ���������
					ProtoType iProtoType = *((ProtoType*)m_pOctets->Begin());
					ProtoSize iProtoSize = 0;
					ProtoCrc  iProtoCrc  = 0;
					P_ProtocolManager->AnalyseProtocolHeader(*m_pOctets, iProtoType, iProtoSize, iProtoCrc);

					//״̬����Э�����
					Bool bStateProtoFilter = false;
					ProtoTypeDriveMap::const_iterator ptd_it = m_mProtoTypeDrive.find(iProtoType);
					if (pSession->ProtoTypeCrc && ptd_it != m_mProtoTypeDrive.end() && ptd_it->second == Protocol::DRIVE_STATE)
					{
						ProtoCrcMap::const_iterator pc_it = pSession->ProtoTypeCrc->find(iProtoType);
						if (pc_it != pSession->ProtoTypeCrc->end() && pc_it->second == iProtoCrc)
							bStateProtoFilter = true;
						else
							(*pSession->ProtoTypeCrc)[iProtoType] = iProtoCrc;
					}

					//�������������
					if (!bStateProtoFilter && OnSessionEncode(pSession, m_pOctets))
					{
						//д��������
						if (bufferevent_write((bufferevent*)pSession->Event, m_pOctets->Begin(), m_pOctets->Size()) != HAWK_OK)
						{
							HawkFmtPrint("Event Buffer Write Error, Size: %u", m_pOctets->Size());
							OnSessionError(pSession, SE_BEV_WRITE);
							return false;
						}

						//�������ܼ�����
						if (m_pGateway->GetProfiler())
							m_pGateway->GetProfiler()->RegSendProto(iProtoType, m_pOctets->Size());
					}
				}
			}
			//ϵͳ��Ϣ֪ͨ
			else
			{
				HawkAssert(m_pOctets->Size() == sizeof(GateNotify));
				GateNotify* pNotify = (GateNotify*)m_pOctets->Begin();
				if (m_pOctets->Size() != sizeof(GateNotify))
					return false;
				
				//�����Ự����
				if (pNotify->Type == GateNotify::NOTIFY_SESSION_CONNECT)
				{
					if (pNotify->eConnect.Handle != INVALID_SOCKET && pNotify->eConnect.AddrLen)
					{
						Session* pSession = 0;
						SocketAddr sAddr((sockaddr*)pNotify->eConnect.Address, pNotify->eConnect.AddrLen);
						if (!StartSession(pNotify->eConnect.Handle, sAddr, &pSession))
						{
							closesocket(pNotify->eConnect.Handle);
						}
						else
						{
							//����󶨵ķ�����ID
							pSession->SvrId = pNotify->eConnect.SvrId;

							//֪ͨ��˷���Ự����
							if (pSession->SvrId)
							{
								SysProtocol::Sys_SessionStart sProto(pSession->Sid, pSession->Addr->ToString());
								m_pOctets->Clear();
								sProto.Encode(*m_pOctets);
								SendGateMsg(ThreadMsgHeader(pSession->SvrId, pSession->Sid, 0, sProto.GetType()), m_pOctets->Begin(), m_pOctets->Size());
							}							
						}
					}
				}
				//�رջỰ����
				else if (pNotify->Type == GateNotify::NOTIFY_SESSION_CLOSE)
				{
					Session* pSession = 0;
					SessionMap::iterator it = m_mSession.find(pNotify->eClose.Sid);					
					if (it != m_mSession.end())
						pSession = it->second;
		
					if (pSession)
					{
						HawkAssert(pSession->SvrId == pNotify->eClose.SvrId);
						struct evbuffer* pBuf = bufferevent_get_output((bufferevent*)pSession->Event);
						if (!pBuf || !evbuffer_get_length(pBuf)) 
							CloseSession(pNotify->eClose.Sid);
						else
							pSession->State = SESSION_STATE_CLOSING;
					}					
				}
				//��˷���ж��, Ĭ�ϹرջỰ
				else if (pNotify->Type == GateNotify::NOTIFY_SERVICE_DETACH)
				{
					//�������ر�Э��
					SysProtocol::Sys_ServerShutdown sProto;
					sProto.m_iSvrId = pNotify->eDetach.SvrId;
					sProto.m_iTime  = (UInt32)HawkOSOperator::GetSysTime();

					vector<SID> vCloseSid;
					//ɸѡ���ڵ�ǰ�رշ����µĻỰ
					SessionMap::const_iterator it = m_mSession.begin();
					for (;it != m_mSession.end(); it++)
					{
						if (it->second->SvrId == pNotify->eDetach.SvrId)
						{
							vCloseSid.push_back(it->first);

							//֪ͨǰ�˷������ر�
							SendProtocol(it->second, &sProto);
						}
					}

					if (!m_pGateway->IsHoldConnection())
					{
						//ͳһ�رջỰ
						for (Size_t i=0;i<vCloseSid.size();i++)
						{
							CloseSession(vCloseSid[i], SE_SHUTDOWN);
						}	
					}								
				}
				//�˳����ط���
				else if (pNotify->Type == GateNotify::NOTIFY_SERVICE_EXIT)
				{
					//�����˳����
					m_bRunning = false;
					HawkFmtPrint("Break EventLoop Success, ThreadId: %u", HawkOSOperator::GetThreadId());
				}
				//Э��ע��
				else if (pNotify->Type == GateNotify::NOTIFY_PROTO_REGISTER)
				{
					//֪ͨ��ϢЯ������
					if (RecvGateExtData(m_pOctets))
					{
						map<ProtoType, UInt8> mTypeDrive;
						*m_pOctets >> mTypeDrive;

						map<ProtoType, UInt8>::iterator it = mTypeDrive.begin();
						for (;it!=mTypeDrive.end();it++)
							m_mProtoTypeDrive[ it->first ] = it->second;

						//ϵͳЭ��
						vector<ProtoType> vSysType;
						P_ProtocolManager->GetRegProtoIds(vSysType);
						for (Size_t i=0; i<vSysType.size(); i++)
							m_mProtoTypeDrive[ vSysType[i] ] = Protocol::DRIVE_EVENT;
					}
				}
			}
		}
		return true;
	}

	Bool HawkGateThread::OnThreadIdle()
	{
		//���д���
		if (m_bIdle)
		{
			HawkSleep(DEFAULT_SLEEP);
		}		
		return true;
	}

	Bool HawkGateThread::OnSessionEvent(UInt32 iEvent, Session* pSession, Int32 iErrCode)
	{
		if (iEvent && pSession && pSession->State != SESSION_STATE_CLOSED)
		{
			//���÷ǿ���
			m_bIdle = false;

			//�Ự���¼�
			if (iEvent & HEVENT_READ)
				OnSessionRead(pSession);

			//�Ựд�¼�
			if (iEvent & HEVENT_WRITE)
				OnSessionWrite(pSession);

			//�Ự�쳣�¼�
			if (iEvent & HEVENT_EXCEPT)
				OnSessionError(pSession, iErrCode);

			return true;
		}
		return false;
	}

	Bool HawkGateThread::OnSessionRead(Session* pSession)
	{
		if (!pSession || pSession->Socket == INVALID_SOCKET || pSession->State != SESSION_STATE_ACTIVE)
			return false;		

		//������뻺����
		struct evbuffer* pBuf = bufferevent_get_input((bufferevent*)pSession->Event);
		if (!pBuf || !evbuffer_get_length(pBuf)) 
		{
			OnSessionError(pSession, SE_BEV_LENGTH);
			return false;
		}

		//ѭ�����������¼���Ե���������δ��ȡ����
		while (evbuffer_get_length(pBuf))
		{
			m_pOctets->Clear();

			//��ȡ����
			Size_t iBufCap  = (Size_t)m_pGateway->GetBufSize();
			HawkAssert(m_pOctets->Capacity() >= iBufCap);

			Int32 iReadSize = evbuffer_remove(pBuf, m_pOctets->Begin(), iBufCap);
			if (iReadSize <= 0)
			{
				OnSessionError(pSession, SE_BEV_REMOVE);
				return false;
			}
			m_pOctets->Resize(iReadSize);

			//�������ܼ�����(ֻ������, ����ǰ������ʵ��������)
			if (m_pGateway->GetProfiler())
				m_pGateway->GetProfiler()->RegRecvProto(0, iReadSize);

			//����
			if (!OnSessionDecode(pSession, m_pOctets))
			{
				OnSessionError(pSession, SE_DECIPHER);
				return false;
			}			

			//��䵽���뻺����
			UInt32 iFillPos = 0;
			UInt32 iBufSize = m_pOctets->Size();
			while(iBufSize)
			{
				//�����������뻺����
				UInt32 iFillSize = HawkMath::Min<UInt32>(pSession->IBuffer->EmptyCap(), iBufSize);
				if (iFillSize)
				{
					void* pData = (Char*)m_pOctets->Begin() + iFillPos;
					pSession->IBuffer->Insert(pSession->IBuffer->End(), pData, iFillSize);
					iFillPos += iFillSize;
					iBufSize -= iFillSize;
				}
				//���뻺��������, ����رջỰ
				else
				{
					HawkFmtError("IBuffer Full, BufCap: %u, ReadSize: %u", pSession->IBuffer->Capacity(), iBufSize);
					OnSessionError(pSession, SE_BUF_FULL);
					return false;
				}

				//�������μ��
				if (!pSession->Trust)
				{
					if (!m_pGateway->TrustSession(pSession, pSession->IBuffer))
						break;

					pSession->Trust = true;

					//�Ƴ����뻺���ǰ�οհ�
					pSession->IBuffer->RemoveBlank();
				}

				//�ӻ������н���Э��
				if (!DecodeIBuffer(pSession))
				{
					OnSessionError(pSession, SE_DECODE);
					return false;
				}
			}
		}
		return true;
	}

	Bool  HawkGateThread::DecodeIBuffer(Session* pSession)
	{
		if (pSession && pSession->IBuffer && pSession->IBuffer->Size())
		{
			//Э�������				
			ProtoType iProtoType = 0;
			ProtoSize iProtoSize = 0;
			ProtoCrc  iProtoCrc  = 0;

			//��ǰʱ��
			UInt32 iCurTime = (UInt32)HawkOSOperator::GetSysTime();

			//����Э��ͷ
			while (P_ProtocolManager->AnalyseProtocolHeader(*pSession->IBuffer, iProtoType, iProtoSize, iProtoCrc))
			{
				//��С���Ϸ�
				if (!P_ProtocolManager->CheckSizeLimit(iProtoSize))
				{
					HawkFmtError("Protocol Illicit, ProtoType: %u, ProtoSize: %u", iProtoType, iProtoSize);
					return false;
				}

				//���Э��Ϸ���
				if (m_mProtoTypeDrive.size() && m_mProtoTypeDrive.find(iProtoType) == m_mProtoTypeDrive.end())
				{
					HawkFmtError("Protocol Illicit, ProtoType: %d", iProtoType);
					return false;
				}

				UInt32 iChunkSize = 0;
				//Э����Ƿ���ȫ��ȡ����
				if (!P_ProtocolManager->CheckDecodeProtocol(*pSession->IBuffer, &iChunkSize))
					break;					

				//Э��Ƶ�ʿ���
				if (m_pGateway->GetSessionProtoFreq())
				{
					//Ƶ�ʹ��죬ֱ�Ӹɵ�
					pSession->ProtoFreq ++;
					if (pSession->ProtoFreq > m_pGateway->GetSessionProtoFreq())				
						return false;

					//ÿ��ָ�
					if (!pSession->FreqTime || iCurTime - pSession->FreqTime >= 1000)
					{					
						pSession->FreqTime  = iCurTime;
						pSession->ProtoFreq = 1;
					}
				}								
				
				//��¼����ǰλ��
				UInt32 iNoniusPos = pSession->IBuffer->GetPos();

				//�ȼ���Ƿ�ΪϵͳЭ��, �ٴ��ݸ�����, �ٷ�����˷�����
				if (!OnSysProtocol(pSession, iProtoType))
				{
					//�ѷ��������id�ĻỰ, �����ط�Э����Ϣ
					if (pSession->SvrId)
					{
						SendGateMsg(ThreadMsgHeader(pSession->SvrId, pSession->Sid, 0, iProtoType), pSession->IBuffer->AvailableData(), iChunkSize);
					}
				}

				//У����
				HawkAssert(iNoniusPos == pSession->IBuffer->GetPos() || (iNoniusPos + iChunkSize) == pSession->IBuffer->GetPos());
				//�ƶ������α�
				pSession->IBuffer->MoveNonius((iNoniusPos + iChunkSize) - pSession->IBuffer->GetPos());

				//�������ܼ�����(ֻ��Э����, ��������ͳ�Ʋ�׼ȷ)
				if (m_pGateway->GetProfiler())
					m_pGateway->GetProfiler()->RegRecvProto(iProtoType, 0);
			}			

			//�Ƴ����뻺���ǰ�οհ�
			pSession->IBuffer->RemoveBlank();
		}
		return true;
	}

	Bool HawkGateThread::OnSysProtocol(Session* pSession, ProtoType iProtoType)
	{
		//��Ӧ���ӵ�PingЭ��
		if (iProtoType == SysProtocol::SYS_CLT_PING ||
			iProtoType == SysProtocol::SYS_SEL_SERVER ||
			iProtoType == SysProtocol::SYS_SECURITY)
		{
			//����Ҫ����Э��
			if (iProtoType == SysProtocol::SYS_CLT_PING)
			{
				//�ظ�Pong��Ӧ
				SysProtocol::Sys_SvrPong sProto;
				sProto.m_iTime = (UInt32)HawkOSOperator::GetSysTime();
				SendProtocol(pSession, &sProto);			
				return true;
			}

			//����ϵͳЭ��
			Protocol* pProto = 0;
			try
			{
				pProto = P_ProtocolManager->Decode(*pSession->IBuffer);

				if (pProto)
				{
					//ѡ�������
					if(pProto->GetType() == SysProtocol::SYS_SEL_SERVER)
					{
						if (m_pGateway->GetGateMode() == HawkGateway::GATEMODE_MULTI_SERVICE_MANUAL)
						{
							//��һ�¼�ѡ��������ɹ�
							SysProtocol::Sys_SelServer* pCmd = (SysProtocol::Sys_SelServer*)pProto;
							if (!pSession->SvrId && pSession->SvrId != pCmd->m_iSvrId)
							{
								pSession->SvrId = pCmd->m_iSvrId;

								//֪ͨ��˷���Ự����
								if (pSession->SvrId)
								{
									SysProtocol::Sys_SessionStart sProto(pSession->Sid, pSession->Addr->ToString());
									m_pOctets->Clear();
									sProto.Encode(*m_pOctets);
									SendGateMsg(ThreadMsgHeader(pSession->SvrId, pSession->Sid, 0, sProto.GetType()), m_pOctets->Begin(), m_pOctets->Size());
								}
							}
						}
						else
						{
							//�Ƿ�Э�鼰ʱ�ر�����
							OnSessionError(pSession, SE_SEL_SERVER);
						}					
					}
					//��Կ����
					else if (pProto->GetType() == SysProtocol::SYS_SECURITY)
					{
						SysProtocol::Sys_Security* pCmd = (SysProtocol::Sys_Security*)pProto;

						if (pCmd->m_iSecurity != HawkGateway::GATESECURITY_NONE)
						{
							//�������
							HAWK_RELEASE(pSession->ISecurity);
							m_pGateway->CreateISecurity(pSession->ISecurity, pCmd->m_iSecurity, &pCmd->m_sKey);

							//�������
							HAWK_RELEASE(pSession->OSecurity);
							m_pGateway->CreateOSecurity(pSession->OSecurity, pCmd->m_iSecurity, &pCmd->m_sKey);
						}						
					}
				}
			}
			catch (HawkException& rhsExcep)
			{
				//Э������쳣�˳�
				HawkFmtError("Session Decode Protocol Error, Msg: %s", rhsExcep.GetMsg().c_str());

				//����Э��ʧ��
				OnSessionError(pSession, SE_DECODE);
			}

			P_ProtocolManager->ReleaseProto(pProto);
			return true;
		}

		return false;
	}

	Bool HawkGateThread::OnSessionWrite(Session* pSession)
	{
		if (!pSession || pSession->Socket == INVALID_SOCKET)
			return false;
		
		if (pSession->State == SESSION_STATE_CLOSING)
		{
			struct evbuffer* pBuf = bufferevent_get_output((bufferevent*)pSession->Event);
			if (!pBuf || !evbuffer_get_length(pBuf)) 
				CloseSession(pSession->Sid);
		}
		
		return true;
	}

	Bool HawkGateThread::OnSessionError(Session* pSession, Int32 iErrCode)
	{
		if (pSession)
		{
			//���ͻỰ�Ͽ�Э��
			if (pSession->Sid)
			{
				//�ѷ��������id��֪ͨ
				if (pSession->SvrId)
				{
					SysProtocol::Sys_SessionBreak sProto(pSession->Sid, pSession->Addr->ToString());
					m_pOctets->Clear();
					sProto.Encode(*m_pOctets);
					SendGateMsg(ThreadMsgHeader(pSession->SvrId, pSession->Sid, 0, sProto.GetType()), m_pOctets->Begin(), m_pOctets->Size());
				}				
			}

			//�رջỰ
			CloseSession(pSession->Sid, iErrCode);
			return true;
		}
		return false;
	}

	Bool HawkGateThread::OnSessionDecode(Session* pSession, OctetsStream* pBuffer)
	{
		if (pSession && pBuffer && pSession->ISecurity)
		{
			return pSession->ISecurity->Update(*pBuffer);
		}

		return true;
	}

	Bool HawkGateThread::OnSessionEncode(Session* pSession, OctetsStream* pBuffer)
	{
		if (pSession && pBuffer && pSession->OSecurity)
		{
			return pSession->OSecurity->Update(*pBuffer);
		}

		return true;
	}
}
