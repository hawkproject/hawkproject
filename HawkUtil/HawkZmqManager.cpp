#include "HawkZmqManager.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"
#include "zmq.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(ZmqManager);

	HawkZmqManager::HawkZmqManager()
	{
		m_pZmqCtx = 0;
	}

	HawkZmqManager::~HawkZmqManager()
	{
		ZmqMap::iterator it = m_mZmq.begin();
		for (;it != m_mZmq.end();++it)
		{
			HawkZmq* pZmq = it->second;

			if (pZmq)
				pZmq->Close();
			
			HAWK_RELEASE(pZmq);
		}
		m_mZmq.clear();

		if(m_pZmqCtx)
		{
			zmq_ctx_destroy(m_pZmqCtx);
			m_pZmqCtx = 0;
		}
	}

	Bool  HawkZmqManager::SetupZmqCtx(Int32 iThreads)
	{
		if (!m_pZmqCtx)
		{
			m_pZmqCtx = zmq_ctx_new();
			if (m_pZmqCtx)
			{
				zmq_ctx_set(m_pZmqCtx, ZMQ_IO_THREADS, iThreads);

				HawkFmtPrint("LibZMQ, Version: %d.%d.%d, IOThreads: %d", ZMQ_VERSION_MAJOR, ZMQ_VERSION_MINOR, ZMQ_VERSION_PATCH, iThreads);
			}
		}
		return m_pZmqCtx != 0;
	}

	void* HawkZmqManager::GetZmqCtx()
	{
		return m_pZmqCtx;
	}

	HawkZmq* HawkZmqManager::CreateZmq(Int32 iType)
	{
		if (m_pZmqCtx || SetupZmqCtx())
		{
			HawkZmq* pZmq = new HawkZmq();
			if (pZmq->Init(iType))
			{
				m_mZmq[pZmq] = pZmq;
				return pZmq;
			}
			else
			{
				HAWK_RELEASE(pZmq);
			}
		}
		return 0;
	}

	Bool HawkZmqManager::CloseZmq(HawkZmq*& pZmq)
	{
		if (pZmq)
		{
			ZmqMap::iterator it = m_mZmq.find(pZmq);
			if (it != m_mZmq.end())
			{
				if (pZmq)
					pZmq->Close();

				m_mZmq.erase(it);
				HAWK_RELEASE(pZmq);
				return true;
			}
		}
		return false;
	}

	Bool HawkZmqManager::ProxyZmq(HawkZmq* pFrontend, HawkZmq* pBackend, Bool bBothway, Int32 iTimeout, Bool bOnce)
	{
		HawkAssert(pFrontend && pBackend);
		if (!pFrontend || !pBackend)
			return false;		

		zmq_pollitem_t items[] = 
		{
			{ pFrontend->GetHandle(), 0, 0, 0 },
			{ pBackend->GetHandle(),  0, 0, 0 }
		};
		
		items[0].events = ZMQ_POLLIN;
		if (bBothway)
			items[1].events = ZMQ_POLLIN;

		do
		{
			items[0].revents = 0;
			items[1].revents = 0;

			Int32 iEvents = zmq_poll(items, 2, iTimeout);
			if (iEvents < 0)
				return false;

			if (items[0].revents & ZMQ_POLLIN) 
			{
				zmq_msg_t sMsg;
				if (zmq_msg_init(&sMsg) != HAWK_OK)
					return false;

				while (true) 
				{
					if (zmq_recvmsg(items[0].socket, &sMsg, 0) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					Int32  iRecvMore = 0;
					Size_t iLen = sizeof(iRecvMore);
					if (zmq_getsockopt(items[0].socket, ZMQ_RCVMORE, &iRecvMore, &iLen) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					if (zmq_sendmsg(items[1].socket, &sMsg, iRecvMore? ZMQ_SNDMORE : 0) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					if (iRecvMore == 0)
						break;
				}

				//释放消息
				zmq_msg_close(&sMsg);				
			}
		
			if (items[1].revents & ZMQ_POLLIN) 
			{
				zmq_msg_t sMsg;
				if (zmq_msg_init(&sMsg) != HAWK_OK)
					return false;

				while (true) 
				{
					if (zmq_recvmsg(items[1].socket, &sMsg, 0) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					Int32  iRecvMore = 0;
					Size_t iLen = sizeof(iRecvMore);
					if (zmq_getsockopt(items[1].socket, ZMQ_RCVMORE, &iRecvMore, &iLen) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					if (zmq_sendmsg(items[0].socket, &sMsg, iRecvMore? ZMQ_SNDMORE: 0) < 0)
					{
						zmq_msg_close(&sMsg);
						return false;
					}

					if (iRecvMore == 0)
						break;
				}

				zmq_msg_close(&sMsg);
			}

			//单次执行, 退出while循环
			if (bOnce)
				return iEvents > 0;

		}while(!bOnce);
		
		return true;
	}
}
