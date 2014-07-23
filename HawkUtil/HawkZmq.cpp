#include "HawkZmq.h"
#include "HawkZmqManager.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"
#include "zmq.h"

namespace Hawk
{
	HawkZmq::HawkZmq()
	{		
		m_pHandle	= 0;
		m_pMonitor	= 0;
		m_iErrCode  = 0;
		m_iType		= 0;
		m_iRecvMore = 0;		 
		m_pRecvBuf  = 0;
		m_sErrMsg   = "";

#ifdef _DEBUG
		m_iLastSend = 0;
		m_iLastRecv = 0;
#endif
	}

	HawkZmq::~HawkZmq()
	{
		Close();

		HAWK_RELEASE(m_pRecvBuf);
	}

	UInt64  HawkZmq::GenValidZid(UInt32 iTid)
	{
		return MAKE_UINT64(iTid, 1);
	}

	Int32 HawkZmq::GetErrCode() const
	{
		return m_iErrCode;
	}

	Bool HawkZmq::Init(Int32 iType)
	{
		A_Exception(m_pHandle == 0);

		m_iType   = iType;
		m_pHandle = zmq_socket(P_ZmqManager->GetZmqCtx(), m_iType);
		if (m_pHandle)
		{
			//linger选项
			Int32 iLinger = 0;
			if (!SetOption(ZMQ_LINGER,  &iLinger, sizeof(iLinger)))
				return false;

			//接收缓冲区设置
			Int32 iRecvSize = RCVBUF_SIZE;
			if (!SetOption(ZMQ_RCVBUF, &iRecvSize, sizeof(iRecvSize)))
				return false;

			//发送缓冲区设置
			Int32 iSendSize = SNDBUF_SIZE;
			if (!SetOption(ZMQ_SNDBUF, &iSendSize, sizeof(iSendSize)))
				return false;

			//收发超时设置
			Int32 iTimeout = HZMQ_TIMEOUT;
			if (!SetOption(ZMQ_RCVTIMEO, &iTimeout, sizeof(iTimeout)) ||
				!SetOption(ZMQ_SNDTIMEO, &iTimeout, sizeof(iTimeout)) )
				return false;			
		}

		return m_pHandle != 0;
	}

	void* HawkZmq::GetHandle()
	{
		return m_pHandle;
	}

	Int32 HawkZmq::GetType() const
	{
		return m_iType;
	}

	Bool HawkZmq::FillErr()
	{
		m_iErrCode = zmq_errno();

#ifdef _DEBUG
		m_sErrMsg  = zmq_strerror(m_iErrCode);

		HawkFmtPrint("ZmqError, Errno: %d, Errmsg: %s", m_iErrCode, m_sErrMsg.c_str());
#endif

		return true;
	}

	Bool HawkZmq::Close()
	{
		m_iErrCode = HAWK_OK;
		if (m_pHandle)
		{
			Int32 iRet = zmq_close(m_pHandle);
			m_pHandle = 0;

			if(iRet == HAWK_ERROR)
				FillErr();
		}

		if (m_pMonitor)
		{
			Int32 iRet = zmq_close(m_pMonitor);
			m_pMonitor = 0;

			if(iRet == HAWK_ERROR)
				FillErr();
		}

		return m_iErrCode == HAWK_OK;
	}

	Bool HawkZmq::SetIdentity(const void* pOptVal, Int32 iSize)
	{
		HawkAssert(iSize && iSize <= 255);
		HawkAssert(*((const UChar*)pOptVal) != 0);

		if(pOptVal && iSize > 0)
		{
			return SetOption(ZMQ_IDENTITY, pOptVal, iSize);
		}

		return false;
	}

	Bool HawkZmq::SetOption(Int32 iOption, const void* pOptVal, Size_t iSize)
	{
		if (m_pHandle)
		{
			Int32 iRet = zmq_setsockopt(m_pHandle, iOption, pOptVal, iSize);
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return false;
			}
			return true;
		}
		return false;
	}

	Bool HawkZmq::GetOption(Int32 iOption,void* pOptVal,Size_t& iSize)
	{
		if (m_pHandle)
		{
			Int32 iRet = zmq_getsockopt(m_pHandle, iOption, pOptVal, &iSize);
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return false;
			}
			return true;
		}
		return false;
	}

	Bool HawkZmq::Bind(const AString& sAddr)
	{
		if (m_pHandle)
		{
			Int32 iRet = zmq_bind(m_pHandle, sAddr.c_str());
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return false;
			}
			return true;
		}
		return false;
	}

	Bool HawkZmq::Connect(const AString& sAddr)
	{
		if (m_pHandle)
		{
			Int32 iReconnIvl = 1000;
			if (!SetOption(ZMQ_RECONNECT_IVL, &iReconnIvl, sizeof(iReconnIvl)))
				return false;

			Int32 iRet = zmq_connect(m_pHandle,sAddr.c_str());
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return false;
			}
			return true;
		}
		return false;
	}		

	Bool HawkZmq::IsWaitRecv() const
	{
		return m_iRecvMore > 0;
	}

	UInt32 HawkZmq::PollEvent(UInt32 iEvents, Int32 iTimeout)
	{
		if (m_pHandle)
		{
			zmq_pollitem_t items[] = 
			{
				{ m_pHandle, 0, iEvents, 0 },
			};

			Int32 iRet = zmq_poll(items, 1, iTimeout);
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return 0;
			}
			return items[0].revents;
		}
		return 0;
	}

	Bool HawkZmq::StartMonitor(UInt32 iEvents, const AString& sAddr)
	{
		if (m_pHandle)
		{
			Long lMonitorId = (Long)m_pHandle;
			Char sMonitorAddr[DEFAULT_SIZE] = {0};
			
			if (sAddr.size())
				sprintf(sMonitorAddr, "%s", sAddr.c_str());
			else
				sprintf(sMonitorAddr, "inproc://hawk-monitor-%ld", lMonitorId);
			
			if (zmq_socket_monitor(m_pHandle, sMonitorAddr, (Int32)iEvents) == HAWK_ERROR)
				return false;

			if (!sAddr.size())
			{
				if (m_pMonitor)
				{
					zmq_close(m_pMonitor);
					m_pMonitor = 0;
				}

				m_pMonitor = zmq_socket(P_ZmqManager->GetZmqCtx(), HZMQ_PAIR);
				if (!m_pMonitor)
				{
					FillErr();
					return false;
				}

				//linger选项
				Int32 iLinger = 0;
				if (zmq_setsockopt(m_pMonitor, ZMQ_LINGER, &iLinger, sizeof(iLinger)) == HAWK_ERROR)
				{
					FillErr();
					return false;
				}

				//接收缓冲区设置
				Int32 iRecvSize = RCVBUF_SIZE;
				if (!zmq_setsockopt(m_pMonitor, ZMQ_RCVBUF, &iRecvSize, sizeof(iRecvSize)) == HAWK_ERROR)
				{
					FillErr();
					return false;
				}

				//发送缓冲区设置
				Int32 iSendSize = SNDBUF_SIZE;
				if (!zmq_setsockopt(m_pMonitor, ZMQ_SNDBUF, &iSendSize, sizeof(iSendSize)) == HAWK_ERROR)
				{
					FillErr();
					return false;
				}

				//收发超时设置
				Int32 iTimeout = HZMQ_TIMEOUT;
				if (zmq_setsockopt (m_pMonitor, ZMQ_RCVTIMEO, &iTimeout, sizeof(iTimeout)) == HAWK_ERROR ||
					zmq_setsockopt (m_pMonitor, ZMQ_SNDTIMEO, &iTimeout, sizeof(iTimeout)))
				{
					FillErr();
					return false;
				}

				//连接
				if (zmq_connect(m_pMonitor, sMonitorAddr) == HAWK_ERROR)
				{
					FillErr();		
					return false;
				}
			}
			return true;
		}

		return false;
	}

	UInt32 HawkZmq::UpdateMonitor(OctetsStream* pData)
	{
		if (m_pMonitor)
		{
			zmq_pollitem_t items[] = 
			{
				{ m_pMonitor, 0, HEVENT_READ, 0 },
			};

			Int32 iRet = zmq_poll(items, 1, 0);
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return 0;
			}

			if (items[0].revents & HEVENT_READ)
			{
				zmq_event_t sEvent;
				Int32 iRecv = zmq_recv(m_pMonitor, &sEvent, sizeof(sEvent), 0);
				if(iRecv == HAWK_ERROR)
				{
					FillErr();
					return 0;
				}
				else if (iRecv == sizeof(sEvent))
				{
					if (pData)
						pData->Replace(&sEvent, sizeof(sEvent));

					return sEvent.event;
				}
			}
		}
		return 0;
	}

	Bool HawkZmq::StopMonitor()
	{
		if (m_pHandle)
		{
			if (m_pMonitor)
			{
				zmq_close(m_pMonitor);
				m_pMonitor = 0;
			}

			if (zmq_socket_monitor(m_pHandle, 0, 0) == HAWK_ERROR)
				return false;

			return true;
		}
		return false;
	}

	Bool HawkZmq::Send(void* pBuf, Size_t iSize, Int32 iFlag)
	{
		if (m_pHandle && pBuf)
		{
			Int32 iSend = zmq_send(m_pHandle, pBuf, iSize, iFlag);
			if(iSend < 0)
			{
				FillErr();
				return false;
			}

#ifdef _DEBUG
			HawkAssert((Size_t)iSend == iSize);
			m_iLastSend = iSend;
#endif

			return true;
		}
		return false;
	}

	Bool HawkZmq::Recv(void* pBuf, Size_t& iSize, Int32 iFlag)
	{
		if (m_pHandle && pBuf && iSize)
		{
			m_iRecvMore = 0;
			Int32 iRecv = zmq_recv(m_pHandle, pBuf, iSize, iFlag);
			if (iRecv == HAWK_ERROR) 
			{
				FillErr();
				return false;
			}

			iSize = iRecv;

			Size_t iLen = sizeof(m_iRecvMore);
			Int32 iRet  = zmq_getsockopt(m_pHandle, ZMQ_RCVMORE, &m_iRecvMore, &iLen);
			if(iRet == HAWK_ERROR)
			{
				FillErr();
				return false;
			}

#ifdef _DEBUG
			m_iLastRecv = iRecv;
#endif

			return true;
		}
		return false;
	}

	Bool HawkZmq::DiscardMsg()
	{
		if(!m_pRecvBuf)
			m_pRecvBuf = new OctetsStream(RCVBUF_SIZE);

		while (PollEvent(HEVENT_READ, 0))
		{
			m_pRecvBuf->Clear();
			Size_t iSize = (Size_t)m_pRecvBuf->Capacity();
			if(!Recv(m_pRecvBuf->Begin(), iSize, 0))
				return false;
			
			while (IsWaitRecv())
			{
				m_pRecvBuf->Clear();
				Size_t iSize = (Size_t)m_pRecvBuf->Capacity();
				if (!Recv(m_pRecvBuf->Begin(), iSize, 0))
					return false;
			}
		}

		return true;
	}

	Bool HawkZmq::SendProtocol(HawkProtocol* pProto, Int32 iFlag)
	{
		if (pProto)
		{
			if(!m_pRecvBuf)
				m_pRecvBuf = new OctetsStream(RCVBUF_SIZE);

			if (m_pRecvBuf)
			{
				m_pRecvBuf->Clear();
				if(pProto->Encode(*m_pRecvBuf))
				{
					return Send(m_pRecvBuf->Begin(), (Size_t)m_pRecvBuf->Size(), iFlag);
				}
			}
		}
		return false;
	}

	Bool HawkZmq::RecvProtocol(HawkProtocol*& pProto, Int32 iFlag)
	{
		if(!m_pRecvBuf)
			m_pRecvBuf = new OctetsStream(RCVBUF_SIZE);

		if (m_pRecvBuf)
		{
			m_pRecvBuf->Clear();
			Size_t iSize = (Size_t)m_pRecvBuf->Capacity();
			if (Recv(m_pRecvBuf->Begin(), iSize, iFlag) && iSize)
			{
				m_pRecvBuf->Resize((UInt32)iSize);
				pProto = P_ProtocolManager->Decode(*m_pRecvBuf);
				if (pProto)
					return true;
			}
		}		
		return false;
	}
}
