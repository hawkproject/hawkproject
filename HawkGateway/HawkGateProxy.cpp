#include "HawkGateProxy.h"
#include "HawkGateDef.h"
#include "zmq.h"

namespace Hawk
{
	HawkGateProxy::HawkGateProxy()
	{
		m_pProxyZmq   = 0;
		m_pOctets     = 0;		
		m_pCollectZmq = 0;
		m_iConnLimit  = 0;
		m_iProxyState = HAWK_ERROR;
	}

	HawkGateProxy::~HawkGateProxy()
	{
		ThreadProxyMap::iterator it = m_mThreadProxy.begin();
		for (; it != m_mThreadProxy.end(); it++)
			P_ZmqManager->CloseZmq(it->second);
		
		m_mThreadProxy.clear();
		HAWK_RELEASE(m_pOctets);

		P_ZmqManager->CloseZmq(m_pCollectZmq);
		P_ZmqManager->CloseZmq(m_pProxyZmq);			
	}

	Bool HawkGateProxy::Init(const AString& sAddr, UInt32 iSvrId, UInt32 iConnLimit)
	{
		if (!m_pOctets)
			m_pOctets = new OctetsStream(PAGE_SIZE*2);

		m_iSvrId     = iSvrId;
		m_sGateAddr  = sAddr;
		m_iConnLimit = iConnLimit;
		if (m_sGateAddr.find("tcp://") == AString::npos)
			m_sGateAddr = "tcp://" + sAddr;	

		if (!m_pProxyZmq)
		{
			m_pProxyZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_DEALER);

			UInt64 iZmqId = HawkZmq::GenValidZid(m_iSvrId);
			if (!m_pProxyZmq->SetIdentity(&iZmqId, (Int32)sizeof(iZmqId)))
				return false;

			m_pProxyZmq->StartMonitor(HawkZmq::HZMQ_EVENT_CONNECTED | HawkZmq::HZMQ_EVENT_DISCONNECTED);
			if (!m_pProxyZmq->Connect(m_sGateAddr))
				return false;
		}

		if (!m_pCollectZmq)
		{
			m_pCollectZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_PULL);

			Char szCollectAddr[DEFAULT_SIZE] = {0};
			Long lCollectId = (Long)m_pCollectZmq->GetHandle();
			sprintf(szCollectAddr, "inproc://hawk-gateproxy-%ld", lCollectId);

			if (!m_pCollectZmq->Bind(szCollectAddr))
			{
				HawkFmtPrint("CollectZmq Bind Error, Addr: %s", szCollectAddr);
				return false;
			}			
		}

		return true;
	}

	Bool HawkGateProxy::RecvProtocol(GateMsgHeader& sHeader, HawkProtocol*& pProto, Int32 iTimeout)
	{
		if (m_pProxyZmq && m_pOctets)
		{
			if (m_pProxyZmq->PollEvent(HEVENT_READ, iTimeout))
			{
				//提取消息头
				m_pOctets->Clear();
				Size_t iSize = (Size_t)m_pOctets->Capacity();
				if (!m_pProxyZmq->Recv(m_pOctets->Begin(), iSize))
					return false;

				sHeader = *((GateMsgHeader*)m_pOctets->Begin());
				Bool bRecvMore = m_pProxyZmq->IsWaitRecv();			
				HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
				if (iSize != sizeof(sHeader) || !bRecvMore)
					return false;

				//提取协议内容
				m_pOctets->Clear();
				iSize = (Size_t)m_pOctets->Capacity();
				if (!m_pProxyZmq->Recv(m_pOctets->Begin(), iSize))
					return false;				

				//协议解析
				m_pOctets->Resize(iSize);
				try
				{
					pProto = P_ProtocolManager->Decode(*m_pOctets);
				}
				catch (HawkException& rhsExcep)
				{
					//协议解析异常退出
					HawkFmtError("Session Decode Protocol Error, Msg: %s", rhsExcep.GetMsg().c_str());
					//释放协议
					P_ProtocolManager->ReleaseProto(pProto);
					return false;
				}

				HawkAssert(!m_pProxyZmq->IsWaitRecv());

				return pProto != 0;
			}			
		}
		return false;
	}	

	Bool  HawkGateProxy::CloseSession(SID iSid)
	{
		if (iSid)
		{
			GateNotify sNotify(GateNotify::NOTIFY_SESSION_CLOSE);
			sNotify.eClose.SvrId = m_iSvrId;
			sNotify.eClose.Sid   = iSid;			
			return SendNotify(sNotify);
		}
		return false;
	}

	Bool HawkGateProxy::RegisterProtoType(const ProtoTypeDriveMap& mTypeDrive)
	{
		if (&mTypeDrive != &m_mProtoTypeDrive)
			m_mProtoTypeDrive = mTypeDrive;

		if (m_iProxyState == HAWK_OK && m_mProtoTypeDrive.size())
		{
			OctetsStream xOS;
			xOS << m_mProtoTypeDrive;
			
			GateNotify sNotify(GateNotify::NOTIFY_PROTO_REGISTER);			
			return SendNotify(sNotify, &xOS);
		}
		return true;
	}	

	Bool HawkGateProxy::RegisterThreads(const vector<UInt32>& vThreads)
	{
		Char szCollectAddr[DEFAULT_SIZE] = {0};
		Long lCollectId = (Long)m_pCollectZmq->GetHandle();
		sprintf(szCollectAddr, "inproc://hawk-gateproxy-%ld", lCollectId);

		for (Size_t i=0;i<vThreads.size();i++)
		{
			UInt32 iThreadId = vThreads[i];

			//支持多次调用
			if (m_mThreadProxy.find(iThreadId) != m_mThreadProxy.end())
			{
				HawkFmtError("GateProxy Thread Register Duplicate, ThreadId: %u", iThreadId);
				continue;
			}
			
			HawkZmq* pZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_PUSH);

			UInt64 iZmqId = HawkZmq::GenValidZid(iThreadId);
			if (!pZmq->SetIdentity(&iZmqId, (Int32)sizeof(iZmqId)))
			{
				P_ZmqManager->CloseZmq(pZmq);
				return false;
			}

			if (!pZmq->Connect(szCollectAddr))
			{
				P_ZmqManager->CloseZmq(pZmq);
				return false;
			}
			
			m_mThreadProxy[iThreadId] = pZmq;
		}

		return true;
	}

	HawkZmq* HawkGateProxy::GetThreadProxy()
	{
		UInt32 iThreadId = HawkOSOperator::GetThreadId();
		ThreadProxyMap::iterator it = m_mThreadProxy.find(iThreadId);
		if (it != m_mThreadProxy.end())
			return it->second;

		return 0;
	}

	Bool HawkGateProxy::OnRecvProtocol(const GateMsgHeader& sHeader, HawkProtocol* pProto)
	{
		return false;
	}

	Bool HawkGateProxy::SendProtocol(SID iSid, HawkProtocol* pProto)
	{
		SvrMsgHeader sHeader;
		sHeader.Sid = iSid;
		return SendProtocol(sHeader, pProto);
	}

	Bool HawkGateProxy::SendProtocol(const SvrMsgHeader& sHeader, HawkProtocol* pProto)
	{
		if (m_iProxyState == HAWK_OK)
		{
			HawkZmq* pProxyZmq = GetThreadProxy();
			HawkAssert(pProxyZmq);

			if (pProto && pProxyZmq)
			{
				if (!pProxyZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
					return false;

				if (!pProxyZmq->SendProtocol(pProto))
					return false;

				return true;		
			}
		}
		return false;
	}
	
	Bool HawkGateProxy::SendRawData(SID iSid, const OctetsStream* pData)
	{
		SvrMsgHeader sHeader;
		sHeader.Sid = iSid;
		return SendRawData(sHeader, pData);
	}

	Bool HawkGateProxy::SendRawData(const SvrMsgHeader& sHeader, const OctetsStream* pData)
	{
		if (m_iProxyState == HAWK_OK)
		{
			HawkZmq* pProxyZmq = GetThreadProxy();
			HawkAssert(pProxyZmq);

			if (pData && pProxyZmq)
			{
				if (!pProxyZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
					return false;

				if (!pProxyZmq->Send(pData->AvailableData(), pData->Size()))
					return false;

				return true;
			}
		}
		return false;
	}

	Bool HawkGateProxy::BroadcastProto(const vector<SID>& vSid, HawkProtocol* pProto)
	{
		if (m_iProxyState == HAWK_OK && vSid.size() && pProto)
		{
			OctetsStream xOS;
			xOS << vSid;
			pProto->Encode(xOS);

			GateNotify sNotify(GateNotify::NOTIFY_PROTO_BROADCAST);			
			return SendNotify(sNotify, &xOS);
		}		
		return false;
	}

	Bool HawkGateProxy::SendNotify(const GateNotify& sNotify, const OctetsStream* pExtData)
	{
		HawkZmq* pProxyZmq = GetThreadProxy();
		HawkAssert(pProxyZmq);

		if (pProxyZmq)
		{
			SvrMsgHeader sHeader;
			sHeader.Sid = 0;

			if (!pProxyZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			if (!pProxyZmq->Send((void*)&sNotify, sizeof(sNotify), pExtData ? HawkZmq::HZMQ_SNDMORE : 0))
				return false;

			if (pExtData && !pProxyZmq->Send((void*)pExtData->Begin(), pExtData->Size()))
				return false;

			return true;
		}

		return false;
	}

	Bool HawkGateProxy::SendRegsterMsg()
	{
		//先注册服务挂载
		if (m_iSvrId)
		{
			GateNotify sNotify(GateNotify::NOTIFY_SERVICE_ATTACH);
			sNotify.eAttach.SvrId     = m_iSvrId;
			sNotify.eAttach.ConnLimit = m_iConnLimit;
			SendNotify(sNotify);
		}

		//注册协议
		if (m_mProtoTypeDrive.size())
		{
			OctetsStream xOS;
			xOS << m_mProtoTypeDrive;

			GateNotify sNotify(GateNotify::NOTIFY_PROTO_REGISTER);
			SendNotify(sNotify, &xOS);
		}

		return true;
	}

	Bool HawkGateProxy::OnProxyConnected()
	{
		SendRegsterMsg();

		m_iProxyState = HAWK_OK;
		HawkFmtPrint("Gateway Connect Successful.");

		return true;
	}

	Bool HawkGateProxy::OnProxyDisconnect()
	{
		m_iProxyState = HAWK_ERROR;
		HawkFmtPrint("Gateway Disconnect Exception.");

		return true;
	}

	Bool HawkGateProxy::CheckProxyState()
	{
		if (m_pProxyZmq)
		{
			UInt32 iEvent = m_pProxyZmq->UpdateMonitor();
			if (iEvent)
			{
				if (iEvent & HawkZmq::HZMQ_EVENT_CONNECTED)
				{
					OnProxyConnected();
				}
				else if (iEvent & HawkZmq::HZMQ_EVENT_DISCONNECTED)
				{
					OnProxyDisconnect();
				}
			}
		}
		return m_iProxyState == HAWK_OK;
	}

	Bool HawkGateProxy::Update(Int32 iTimeout)
	{
		if (!CheckProxyState())
		{
			if (m_pCollectZmq)
				m_pCollectZmq->DiscardMsg();

			HawkSleep(DEFAULT_SLEEP);
			return true;
		}

		//接收网关来的协议并回调		
		Protocol* pProto = 0;
		GateMsgHeader sHeader;
		while (RecvProtocol(sHeader, pProto, iTimeout))
		{
			if (!OnRecvProtocol(sHeader, pProto))		
				P_ProtocolManager->ReleaseProto(pProto);

			//接收同时转发, 避免发送堆积
			P_ZmqManager->ProxyZmq(m_pCollectZmq, m_pProxyZmq, false, iTimeout, true);
		}

		//把服务器的协议转发给网关
		Bool bTransmit = P_ZmqManager->ProxyZmq(m_pCollectZmq, m_pProxyZmq, false, iTimeout, true);
		if (!bTransmit)
		{
			HawkAssert(bTransmit && "GateProxy Transmit Error.");
		}		
		
		return true;
	}
}
