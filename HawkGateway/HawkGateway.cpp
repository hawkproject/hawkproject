#include "HawkGateway.h"
#include "HawkGateSecurity.h"
#include "event.h"
#include "zmq.h"

namespace Hawk
{
	HawkGateway::HawkGateway()
	{
		m_iThread		= 0;
		m_iTurnIdx		= 0;
		m_iBaseSid		= 0;
		m_ppThread		= 0;
		m_pCrossDomain	= 0;
		m_pProfiler		= 0;
		m_pThreadZmq	= 0;
		m_pServerZmq	= 0;
		m_pMonitorZmq	= 0;
		m_pOctets		= 0;
		m_iTimeout		= 0;	
		m_bIdle			= true;
		m_bBlocking		= true;
		m_bWaitIdle		= false;
		m_bRunning		= false;	
		m_bSecurity		= false;
		m_iBufSize		= PAGE_SIZE * 2;
		m_iGateMode		= GATEMODE_SINGLE_SERVICE;
	}

	HawkGateway::~HawkGateway()
	{
		//释放线程
		for (Int32 i=0;m_ppThread && i<m_iThread;i++)
		{
			HAWK_RELEASE(m_ppThread[i]);
		}
		HAWK_DELETE_ARRAY(m_ppThread);

		//关闭监听套接字
		m_sSocket.Close();

		//释放消息缓冲
		HAWK_RELEASE(m_pOctets);
		
		//释放性能监视器
		HAWK_RELEASE(m_pProfiler);

		//关闭消息队列
		P_ZmqManager->CloseZmq(m_pMonitorZmq);
		P_ZmqManager->CloseZmq(m_pServerZmq);
		P_ZmqManager->CloseZmq(m_pThreadZmq);

		//释放跨域线程
		HAWK_RELEASE(m_pCrossDomain);
	}

	void HawkGateway::SetBufSize(Int32 iBufSize)
	{
		m_iBufSize = iBufSize;
	}

	Int32 HawkGateway::GetBufSize() const
	{
		return m_iBufSize;
	}

	Bool HawkGateway::SetWaitIdle(Bool bWaitIdle)
	{
		m_bWaitIdle = bWaitIdle;
		return true;
	}

	Bool HawkGateway::IsWaitIdle() const
	{
		return m_bWaitIdle;
	}

	Bool HawkGateway::IsRunning() const
	{
		return m_bRunning;
	}

	void HawkGateway::SetSessionTimeout(Int32 iTimeout)
	{
		m_iTimeout = iTimeout;
	}

	Int32 HawkGateway::GetSessionTimeout() const
	{
		return m_iTimeout;
	}

	Int32  HawkGateway::GetThreadNum() const
	{
		return m_iThread;
	}

	Int32  HawkGateway::GetThreadId(Int32 iIdx) const
	{
		if (iIdx >= 0 && iIdx < m_iThread && m_ppThread && m_ppThread[iIdx])
			return m_ppThread[iIdx]->GetThreadId();
		
		return 0;
	}

	Bool HawkGateway::SetBlocking(Bool bBlocking)
	{
		m_bBlocking = bBlocking;
		return true;
	}

	Bool HawkGateway::CreateGateThread(HawkGateThread*& pThread)
	{
		pThread = new HawkGateThread(this);
		return pThread != 0;
	}

	Bool HawkGateway::CreateISecurity(HawkSecurity*& pSecurity)
	{
		pSecurity = 0;
		if (m_bSecurity)
			pSecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_DECODE);

		return true;
	}

	Bool HawkGateway::CreateOSecurity(HawkSecurity*& pSecurity)
	{
		pSecurity = 0;
		if (m_bSecurity)
			pSecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_ENCODE);

		return true;
	}

	Bool HawkGateway::Init(const AString& sFrontend, const AString& sBackend, Int32 iThread, Int32 iGateMode)
	{
		HawkAssert(sFrontend.size() && sBackend.size() && iThread > 0);

		if (!sFrontend.size() || !sBackend.size() || iThread <= 0)
			return false;

		//参数赋值
		m_iThread   = iThread;
		m_iGateMode = iGateMode;

		//创建通用缓冲
		if (!m_pOctets)
			m_pOctets = new OctetsStream(m_iBufSize);

		//创建服务器套接字
		if (!m_sSocket.IsValid())
		{
			if (!m_sSocket.InitTcpServer(sFrontend))
			{
				HawkFmtPrint("Init TcpServer Error, Addr: %s, ErrCode: %d", sFrontend.c_str(), m_sSocket.GetSocketErr());
				return false;
			}
		}

		//创建网关线程通信队列
		if (!m_pThreadZmq)
		{
			m_pThreadZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_ROUTER);
			if (!m_pThreadZmq->Bind(GetThreadZmqAddr()))
			{
				HawkFmtPrint("ThreadZmq Bind Error, Addr: %s", GetThreadZmqAddr().c_str());
				return false;
			}
		}

		//创建后端服务通信队列
		if (!m_pServerZmq)
		{
			AString sAddr = sBackend;
			if (sAddr.find("tcp://") == AString::npos)
				sAddr = "tcp://" + sBackend;

			m_pServerZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_ROUTER);

			//创建指定事件监听器
			m_pServerZmq->StartMonitor(HawkZmq::HZMQ_EVENT_ACCEPTED | HawkZmq::HZMQ_EVENT_DISCONNECTED, GetServerZmqAddr());
			//绑定到地址
			if (!m_pServerZmq->Bind(sAddr))
			{
				HawkFmtPrint("ServerZmq Bind Error, Addr: %s", sAddr.c_str());
				return false;
			}			
		}
		
		//创建服务监视器
		if (!m_pMonitorZmq && m_pServerZmq)
		{
			m_pMonitorZmq = P_ZmqManager->CreateZmq(HawkZmq::HZMQ_PAIR);
			//连接到监视器
			if (!m_pMonitorZmq->Connect(GetServerZmqAddr()))
			{
				HawkFmtPrint("MonitorZmq Connect Error, Addr: %s", GetServerZmqAddr().c_str());
				return false;
			}
		}

		//创建网关线程
		m_ppThread = new HawkGateThread*[m_iThread];
		memset(m_ppThread, 0, sizeof(HawkGateThread*) * m_iThread);

		tm xTM = HawkOSOperator::GetSysClock();
		//强制设定高8位, 避免重启导致会话ID重复而引起协议导向错误
		m_iBaseSid = (xTM.tm_sec << 24) & 0xFF000000;

		for (UInt32 i=0;i<(UInt32)m_iThread;i++)
		{
			//创建网关线程
			if (!CreateGateThread(m_ppThread[i]))
			{
				HawkPrint("Create GateThread Failed.");
				return false;
			}

			//初始化网关线程
			if (!m_ppThread[i]->Init(i+1, m_iBaseSid, (UInt32)m_iThread))
			{
				HawkPrint("Init GateThread Failed.");
				return false;
			}

			//开启网关线程
			if (!m_ppThread[i]->Start())
			{
				HawkPrint("Start GateThread Failed.");
				return false;
			}
		}		
		return true;
	}

	Bool HawkGateway::Stop()
	{
		//通知退出循环
		m_bRunning = false;

		//等待网关结束
		if (!m_bBlocking)
			OnGatewayClose();

		return true;
	}

	AString HawkGateway::GetThreadZmqAddr() const
	{
		return "inproc://hawk-gateway-thread";
	}

	AString HawkGateway::GetServerZmqAddr() const
	{
		return "inproc://hawk-server-monitor";
	}

	HawkProfiler* HawkGateway::GetProfiler()
	{
		return m_pProfiler;
	}

	Bool HawkGateway::TurnGateSecurity()
	{
		m_bSecurity = true;
		return true;
	}

	Bool HawkGateway::TurnOnProfiler(const AString& sAddr)
	{
		if (!m_pProfiler)
		{
			m_pProfiler = new HawkProfiler(sAddr);			
		}
		return true;
	}

	Bool HawkGateway::Run()
	{
		if (!m_bRunning)
		{
			m_bRunning = true;
			
			if (m_pCrossDomain)
				m_pCrossDomain->Start(this);

			if (m_pProfiler)
				m_pProfiler->Start();
		}
		
		while (m_bRunning)
		{
			//设置默认空闲状态
			m_bIdle = true;

			//接收新网络连接
			OnSessionAccept();

			//检测监视器事件
			OnGateMonitorEvent();

			//检测网关线程事件
			OnGateThreadEvent();

			//检测后端服务事件
			OnGateServerEvent();

			//单次更新事件
			if (!m_bBlocking)
				return true;

			//网关空闲处理
			OnGateIdleEvent();
		}

		//阻塞等待结束
		OnGatewayClose();

		return true;
	}		

	Bool HawkGateway::RecvThreadMsg(UInt32& iThread, ThreadMsgHeader& sHeader, OctetsStream* pOctets)
	{
		if (m_pThreadZmq && pOctets)
		{
			//提取线程ID标识信息
			pOctets->Clear();			
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;

			UInt64 iThreadZmqId = 0;			
			iThreadZmqId	= *((UInt64*)pOctets->Begin());
			iThread			= HIGH_U32(iThreadZmqId);
			Bool bRecvMore = m_pThreadZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(iThreadZmqId) && bRecvMore);
			if (iSize != sizeof(iThreadZmqId) || !bRecvMore)
				return false;
		
			//提取消息头信息
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;
			
			sHeader	   = *((ThreadMsgHeader*)pOctets->Begin());
			bRecvMore = m_pThreadZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
			if (iSize != sizeof(sHeader) || !bRecvMore)
				return false;

			//提取消息数据体
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pThreadZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::SendThreadMsg(UInt32 iThread, const GateMsgHeader& sHeader, void* pData, Size_t iSize, const OctetsStream* pExtData)
	{
		if (m_pThreadZmq && iThread)
		{
			//线程目标
			UInt64 iThreadZmqId = MAKE_UINT64(iThread, 1);
			if (!m_pThreadZmq->Send(&iThreadZmqId, sizeof(iThreadZmqId), HawkZmq::HZMQ_SNDMORE))
				return false;

			//消息头信息
			if (!m_pThreadZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			//主体数据
			if (!m_pThreadZmq->Send(pData, iSize, pExtData ? HawkZmq::HZMQ_SNDMORE : 0))
				return false;

			//附带数据
			if (pExtData && !m_pThreadZmq->Send((void*)pExtData->Begin(), pExtData->Size()))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkGateway::RecvServerMsg(UInt32& iSvrId, SvrMsgHeader& sHeader, OctetsStream* pOctets)
	{
		if (m_pServerZmq && pOctets)
		{
			//服务器ID标识信息
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			UInt64 iSvrZmqId	= *((UInt64*)pOctets->Begin());
			iSvrId				= HIGH_U32(iSvrZmqId);
			Bool bRecvMore		= m_pServerZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(iSvrZmqId) && bRecvMore);			
			if (iSize != sizeof(iSvrZmqId) || !bRecvMore)
				return false;

			//提取消息头信息
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			sHeader	   = *((SvrMsgHeader*)pOctets->Begin());
			bRecvMore = m_pServerZmq->IsWaitRecv();
			HawkAssert(iSize == sizeof(sHeader) && bRecvMore);
			if (iSize != sizeof(sHeader) || !bRecvMore)
				return false;

			//消息数据体
			pOctets->Clear();
			iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::RecvServerExtData(OctetsStream* pOctets)
	{
		if (m_pServerZmq && pOctets)
		{
			//消息状态校验
			Bool bRecvMore = m_pServerZmq->IsWaitRecv();
			HawkAssert(bRecvMore);
			if (!bRecvMore)
				return false;

			//消息数据体
			pOctets->Clear();
			Size_t iSize = (Size_t)pOctets->Capacity();
			if (!m_pServerZmq->Recv(pOctets->Begin(), iSize))
				return false;

			pOctets->Resize((UInt32)iSize);
			return true;
		}
		return false;
	}

	Bool HawkGateway::SendServerMsg(UInt32 iSvrId, const GateMsgHeader& sHeader, void* pData, Size_t iSize)
	{
		if (m_pServerZmq && iSvrId)
		{
			//目标
			UInt64 iSvrZmqId = MAKE_UINT64(iSvrId, 1);
			if (!m_pServerZmq->Send(&iSvrZmqId, sizeof(iSvrZmqId), HawkZmq::HZMQ_SNDMORE))
				return false;

			//消息头
			if (!m_pServerZmq->Send((void*)&sHeader, sizeof(sHeader), HawkZmq::HZMQ_SNDMORE))
				return false;

			//数据
			if (!m_pServerZmq->Send(pData, iSize))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkGateway::TrustSession(void* pSession, OctetsStream* pIBuffer)
	{
		return true;
	}

	Bool HawkGateway::CloseSession(SID iSid, UInt32 iSvrId)
	{
		if (!iSvrId)
		{
			SidBindMap::const_iterator it = m_mSidBind.find(iSid);
			if (it != m_mSidBind.end())
				iSvrId = it->second;
		}

		if (iSvrId && iSid > m_iBaseSid)
		{
			//消减会话对应服务连接容量
			OnSessionDetachServer(iSid, iSvrId);

			//通知网关线程关闭会话
			GateNotify sNotify(GateNotify::NOTIFY_SESSION_CLOSE);
			sNotify.eClose.Sid   = iSid;
			sNotify.eClose.SvrId = iSvrId;
			UInt32 iIdx = (iSid-m_iBaseSid-1) % m_iThread + 1;
			SendThreadMsg(iIdx, GateMsgHeader(iSid), &sNotify, sizeof(sNotify));

			return true;
		}
		return false;
	}

	Bool HawkGateway::OnSessionAccept()
	{
		if (!m_sSocket.UpdateEvent(HEVENT_READ))
			return true;

		//接收新连接
		HawkSocket sSocket;
		SocketAddr sAddr;
		while (m_sSocket.Accept(sSocket, sAddr))
		{
			//设置非空闲
			m_bIdle = false;

			//注册新连接信息
			UInt32 iSvrId	 = 0;
			Int32  iErrCode = 0;
			if (!RegConnection(sAddr, iSvrId, iErrCode))
				return OnGateRefuseConn(iErrCode, sSocket, &sAddr);

			//投递连接通知
			GateNotify sNotify(GateNotify::NOTIFY_SESSION_CONNECT);
			sNotify.eConnect.SvrId		= iSvrId;
			sNotify.eConnect.AddrLen	= sAddr.CopyAddr(sNotify.eConnect.Address);
			sNotify.eConnect.Handle		= sSocket.Handle();
			UInt32 iIdx = m_iTurnIdx++ % m_iThread + 1;
			SendThreadMsg(iIdx, GateMsgHeader(0), &sNotify, sizeof(sNotify));

			//单次处理平衡任务时间
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::RegConnection(const SocketAddr& sAddr, UInt32& iSvrId, Int32& iErrCode)
	{
		//回显模式
		if (m_iGateMode == GATEMODE_ECHO)
		{
			iSvrId = 0;
			return true;
		}
		
		//当前无挂载服务器
		if (!m_vService.size())
		{
			iErrCode = SysProtocol::ERR_SERVICE_INVALID;
			return false;
		}

		//单一服务器模式, 第一优先满容
		if (m_iGateMode == GATEMODE_SINGLE_SERVICE)
		{			
			if (m_vService[0].ConnCap && m_vService[0].CurCap >= m_vService[0].ConnCap)
			{
				iErrCode = SysProtocol::ERR_REFUSE_CONN;
				return false;
			}

			iSvrId = m_vService[0].SvrId;
			return true;
		}
		//多服务器模式, 顺序满容原则
		else if (m_iGateMode == GATEMODE_MULTI_SERVICE_STOCK)
		{
			for (Size_t i=0;i<m_vService.size();i++)
			{
				if (!m_vService[i].ConnCap || m_vService[i].CurCap < m_vService[i].ConnCap)
				{
					iSvrId = m_vService[i].SvrId;
					return true;
				}
			}

			iErrCode = SysProtocol::ERR_REFUSE_CONN;
			return false;
		}
		//多服务器模式, 负载均衡原则
		else if (m_iGateMode == GATEMODE_MULTI_SERVICE_BALANCE)
		{
			Int32 iSvrIdx = -1;
			Int32 iSvrCap = -1;
			for (Size_t i=0;i<m_vService.size();i++)
			{
				if (!m_vService[i].ConnCap || m_vService[i].CurCap < m_vService[i].ConnCap)
				{
					if (iSvrCap == -1 || iSvrCap > (Int32)m_vService[i].CurCap)
					{
						iSvrIdx = i;
						iSvrCap = m_vService[i].CurCap;
					}
				}
			}

			//存在有效服务器
			if (iSvrIdx >= 0)
			{
				iSvrId = m_vService[iSvrIdx].SvrId;
				return true;
			}

			iErrCode = SysProtocol::ERR_REFUSE_CONN;
			return false;
		}

		return false;
	}

	Bool HawkGateway::OnGateRefuseConn(Int32 iReason, HawkSocket& sSocket, const SocketAddr* pAddr)
	{
		//先发送拒绝连接协议再关闭		
		SysProtocol::Sys_RefuseConn sProto(iReason);		

		m_pOctets->Clear();
		sProto.Encode(*m_pOctets);
		Size_t iSize = (Size_t)m_pOctets->Size();
		sSocket.Send(m_pOctets->Begin(), iSize);
		sSocket.Close();

		return true;
	}

	Bool HawkGateway::OnSessionAttachServer(SID iSid, UInt32 iSvrId)
	{
		//SID暂时未使用, 故此处传进来的SID不一定正确
		for (Size_t i=0;i<m_vService.size();i++)
		{
			if (m_vService[i].SvrId == iSvrId)
			{
				m_vService[i].CurCap ++;				
				m_mSidBind[iSid] = iSvrId;
				return true;
			}
		}
		return false;
	}

	Bool HawkGateway::OnSessionDetachServer(SID iSid, UInt32 iSvrId)
	{
		//SID暂时未使用, 故此处传进来的SID不一定正确
		for (Size_t i=0;i<m_vService.size();i++)
		{
			if (m_vService[i].SvrId == iSvrId)
			{
				if (m_vService[i].CurCap)
					m_vService[i].CurCap --;

				SidBindMap::iterator it = m_mSidBind.find(iSid);
				if (it != m_mSidBind.end())
					m_mSidBind.erase(it);

				return true;
			}
		}
		return false;
	}

	Bool HawkGateway::OnGateThreadEvent()
	{
		while (m_pThreadZmq && m_pThreadZmq->PollEvent(HEVENT_READ, 0))
		{
			//设置非空闲
			m_bIdle = false;

			//接收网关线程信息
			UInt32	iThread = 0;
			ThreadMsgHeader sHeader;
			if (!RecvThreadMsg(iThread, sHeader, m_pOctets))
				return false;
			
			//数据包完整, 无额外附加数据
			if (m_pThreadZmq->IsWaitRecv())
				return false;

			//网关做用户协议数据校验
			if (sHeader.Sid)
			{
				if (!OnSessionProtocol(sHeader.Sid, m_pOctets))
				{
					CloseSession(sHeader.SvrId, sHeader.Sid);
					return true;
				}
			}
			
			//Echo模式直接回复
			if (m_iGateMode == GATEMODE_ECHO)
			{
				//用户协议直接发送回去
				if (sHeader.Args > 0 && sHeader.Sid)
					SendThreadMsg(iThread, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}
			//发送给后端服务
			else
			{
				//会话挂载和卸载判断
				if (sHeader.Args == SysProtocol::SYS_SESSION_START)
				{
					//增加会话对应服务连接容量
					OnSessionAttachServer(sHeader.Sid, sHeader.SvrId);

					//重置消息头携带的会话ID, 传给后端服务器用作服务间协议(非用户协议)
					sHeader.Sid = 0;
				}
				else if (sHeader.Args == SysProtocol::SYS_SESSION_BREAK)
				{
					//消减会话对应服务连接容量
					OnSessionDetachServer(sHeader.Sid, sHeader.SvrId);

					//重置消息头携带的会话ID, 传给后端服务器用作服务间协议(非用户协议)
					sHeader.Sid = 0;
				}

				//转发给后端服务器
				if (sHeader.SvrId)
					SendServerMsg(sHeader.SvrId, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}

			//单次处理平衡任务时间
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnSessionProtocol(SID iSid, const OctetsStream* pOctets)
	{
		if (iSid && pOctets)			
			return true;

		return false;
	}

	Bool HawkGateway::OnGateServerEvent()
	{
		while (m_pServerZmq && m_pServerZmq->PollEvent(HEVENT_READ, 0))
		{
			//设置非空闲
			m_bIdle = false;
			
			//接收后端服务信息
			UInt32 iSvrId = 0;
			SvrMsgHeader sHeader;
			if (!RecvServerMsg(iSvrId, sHeader, m_pOctets))
				return false;

			//会话协议转发
			if (sHeader.Sid)
			{
				//数据包完整, 无额外附加数据
				if (m_pServerZmq->IsWaitRecv())
					return false;

				UInt32 iIdx = (sHeader.Sid-m_iBaseSid-1) % m_iThread + 1;
				SendThreadMsg(iIdx, GateMsgHeader(sHeader.Sid), m_pOctets->Begin(), m_pOctets->Size());
			}
			//系统消息通知
			else
			{
				HawkAssert(m_pOctets->Size() == sizeof(GateNotify));
				GateNotify* pNotify = (GateNotify*)m_pOctets->Begin();
				if (m_pOctets->Size() != sizeof(GateNotify))
					return false;
				
				//通知网关线程关闭会话连接
				if (pNotify->Type == GateNotify::NOTIFY_SESSION_CLOSE)
				{
					//数据包完整, 无额外附加数据
					if (m_pServerZmq->IsWaitRecv())
						return false;

					//解除会话绑定
					OnSessionDetachServer(pNotify->eClose.Sid, pNotify->eClose.SvrId);

					//转发给网关线程处理
					UInt32 iIdx = (pNotify->eClose.Sid-m_iBaseSid-1) % m_iThread + 1;
					SendThreadMsg(iIdx, GateMsgHeader(0), pNotify, sizeof(*pNotify));
				}
				//协议注册
				else if (pNotify->Type == GateNotify::NOTIFY_PROTO_REGISTER)
				{
					//通知消息携带参数
					if (RecvServerExtData(m_pOctets))
					{
						GateNotify sNotify(GateNotify::NOTIFY_PROTO_REGISTER);
						for (Int32 i=0; i<m_iThread; i++)
						{
							UInt32 iIdx = i + 1;
							SendThreadMsg(iIdx, GateMsgHeader(0), &sNotify, sizeof(sNotify), m_pOctets);
						}
					}
				}
				//协议广播
				else if (pNotify->Type == GateNotify::NOTIFY_PROTO_BROADCAST)
				{
					//通知消息携带参数
					if (RecvServerExtData(m_pOctets))
					{
						vector<SID> vSid;
						*m_pOctets >> vSid;

						for (Size_t i=0; i<vSid.size(); i++)
						{
							UInt32 iIdx = (vSid[i]-m_iBaseSid-1) % m_iThread + 1;
							SendThreadMsg(iIdx, GateMsgHeader(vSid[i]), m_pOctets->AvailableData(), m_pOctets->AvailableSize());						
						}
					}
				}
			}

			//单次处理平衡任务时间
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnGateMonitorEvent()
	{
		while (m_pMonitorZmq && m_pMonitorZmq->PollEvent(HEVENT_READ, 0))
		{
			//设置非空闲
			m_bIdle = false;

			//接收头信息
			m_pOctets->Clear();
			Size_t iSize = (Size_t)m_pOctets->Capacity();
			if (!m_pMonitorZmq->Recv(m_pOctets->Begin(), iSize))
				return false;

			//数据校验
			HawkAssert(iSize == sizeof(zmq_event_t));
			zmq_event_t sZmqEvent = *((zmq_event_t*)m_pOctets->Begin());
			if (iSize != sizeof(zmq_event_t))
				return false;

			//设置服务ID已经更新列表
			if (sZmqEvent.event & HawkZmq::HZMQ_EVENT_ACCEPTED)
			{
				HawkFmtPrint("Service Connect Successful, Socket: %d", sZmqEvent.data.accepted.fd);

				if (!OnServerConnected(sZmqEvent.data.accepted.fd))
					return false;
			}
			else if (sZmqEvent.event & HawkZmq::HZMQ_EVENT_DISCONNECTED)
			{
				HawkFmtPrint("Service Disconnect Exception, Socket: %d", sZmqEvent.data.accepted.fd);

				if (!OnServerDisConnect(sZmqEvent.data.disconnected.fd))
					return false;				
			}
			else
			{
				HawkFmtPrint("Gateway Unhandle Event, Event: %d", sZmqEvent.event);
			}

			//单次处理平衡任务时间
			if (!m_bWaitIdle) break;
		}
		return true;
	}

	Bool HawkGateway::OnGateIdleEvent()
	{
		//空闲处理
		if (m_bIdle)
		{
			HawkSleep(DEFAULT_SLEEP);
		}		
		return true;
	}

	Bool HawkGateway::OnGatewayClose()
	{
		//通知网关线程退出
		GateNotify sNotify(GateNotify::NOTIFY_SERVICE_EXIT);
		for (UInt32 j=0;m_pThreadZmq && j<(UInt32)m_iThread;j++)
		{
			SendThreadMsg(j+1, GateMsgHeader(0), &sNotify, sizeof(sNotify));
		}

		//等待线程结束
		for (UInt32 i=0;m_ppThread && i<(UInt32)m_iThread;i++)
		{
			if (m_ppThread[i])
				m_ppThread[i]->Close();
		}

		//关闭性能监视器
		if (m_pProfiler)
			m_pProfiler->Stop();

		//设置运行状态
		m_bRunning = false;

		return true;
	}

	Bool HawkGateway::OnServerConnected(SOCKET hSocket)
	{
		if (hSocket != INVALID_SOCKET)
		{
			if (m_pServerZmq->PollEvent(HEVENT_READ, DEFAULT_TIMEOUT))
			{				
				//接收连接通知
				UInt32 iSvrId = 0;
				SvrMsgHeader sHeader;
				if (!RecvServerMsg(iSvrId, sHeader, m_pOctets))
					return false;

				//数据包完整, 无额外附加数据
				if (m_pServerZmq->IsWaitRecv())
					return false;

				//进行数据校验(传送挂载服务的数据一致性)
				HawkAssert(sHeader.Sid == 0 && iSvrId && m_pOctets->Size() == sizeof(GateNotify));
				GateNotify* pNotify = (GateNotify*)m_pOctets->Begin();
				if (sHeader.Sid || !iSvrId || iSvrId != pNotify->eAttach.SvrId || m_pOctets->Size() != sizeof(GateNotify))
					return false;

				//服务挂载通知
				if (pNotify->Type == GateNotify::NOTIFY_SERVICE_ATTACH)
				{
					//判断服务ID重复
					Size_t i = 0;
					for (;i<m_vService.size();i++)
					{
						if (iSvrId == m_vService[i].SvrId)
							break;
					}

					//添加到列表
					if (i == m_vService.size())
					{						
						m_vService.push_back(Service(iSvrId, hSocket, pNotify->eAttach.ConnLimit));
						HawkFmtPrint("Service Attach, SvrId: %u", iSvrId);
					}

					return true;
				}
			}		
		}
		return false;
	}

	Bool HawkGateway::OnServerDisConnect(SOCKET hSocket)
	{
		if (hSocket != INVALID_SOCKET)
		{
			ServiceVec::iterator it = m_vService.begin();
			for (;it!=m_vService.end();it++)
			{				
				if (it->SvrFd == hSocket)
				{	
					//通知网关线程服务卸载
					GateNotify sNotify(GateNotify::NOTIFY_SERVICE_DETACH);
					sNotify.eDetach.SvrId = it->SvrId;

					for (UInt32 j=0;m_pThreadZmq && j<(UInt32)m_iThread;j++)
					{
						SendThreadMsg(j+1, GateMsgHeader(0), &sNotify, sizeof(sNotify));
					}

					//删除绑定上的会话ID
					SidBindMap::iterator bit = m_mSidBind.begin();
					for (;bit != m_mSidBind.end();)
					{
						if (bit->second == it->SvrId)
						{
							m_mSidBind.erase(bit++);
							continue;
						}
						bit ++;
					}

					//从服务列表删除
					HawkFmtPrint("Service Detach, SvrId: %u", it->SvrId);
					m_vService.erase(it);

					break;
				}
			}
		}
		return false;
	}
}
