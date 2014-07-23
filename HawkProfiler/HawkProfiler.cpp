#include "HawkProfiler.h"

namespace Hawk
{
	extern UInt32 get_processor_number();
	extern UInt64 get_phys_mem();
	extern UInt32 get_cpu_usage();
	extern UInt64 get_memory_usage();

	PVoid hawk_ProfilerRoutine(void* pArgs)
	{
		HawkProfiler* pProfiler = static_cast<HawkProfiler*>(pArgs); 
		if (pProfiler)
			pProfiler->RunLoop();

		return 0;
	}

	HawkProfiler::HawkProfiler(const AString& sAddr)
	{
		m_iCpuCount = 0;
		m_iTotalMem	= 0;
		m_iCpuUsage = 0;
		m_iMemUsage = 0;
		m_pThread	= 0;
		m_iConnect	= 0;
		m_iRecvProto= 0;
		m_iRecvSize	= 0;
		m_iSendProto= 0;
		m_iSendSize	= 0;
		m_bRunning  = false;
		m_sAddr		= sAddr;
		m_pRecvBuf	= new OctetsStream(PAGE_SIZE);
	}

	HawkProfiler::~HawkProfiler()
	{
		m_sSocket.Close();

		HAWK_RELEASE(m_pThread);
		HAWK_RELEASE(m_pRecvBuf);
	}

	Bool HawkProfiler::RegRecvProto(ProtoType iType, ProtoSize iSize)
	{
		if (iType)
			HawkAtomic::Inc<UInt64>(&m_iRecvProto);

		if (iSize)
			HawkAtomic::Add<UInt64>(&m_iRecvSize, iSize);

		return true;
	}

	Bool HawkProfiler::RegSendProto(ProtoType iType, ProtoSize iSize)
	{
		if (iType)
			HawkAtomic::Inc<UInt64>(&m_iSendProto);

		if (iSize)
			HawkAtomic::Add<UInt64>(&m_iSendSize, iSize);

		return true;
	}

	Bool HawkProfiler::RegConnect(Bool bConn)
	{
		if (bConn)
			HawkAtomic::Inc<UInt32>(&m_iConnect);
		else
			HawkAtomic::Dec<UInt32>(&m_iConnect);

		return true;
	}

	UInt32 HawkProfiler::GetProcessorNumber()
	{
		if (!m_iCpuCount)
			m_iCpuCount = get_processor_number();

		return m_iCpuCount;
	}

	UInt64 HawkProfiler::GetTotalMem()
	{
		if (!m_iTotalMem)
			m_iTotalMem = get_phys_mem();

		return m_iTotalMem;
	}

	UInt32 HawkProfiler::GetCpuUsage()
	{
		m_iCpuUsage = get_cpu_usage();
		return m_iCpuUsage;
	}

	UInt64 HawkProfiler::GetMemUsage()
	{
		m_iMemUsage = get_memory_usage();
		return m_iMemUsage;
	}

	UInt32 HawkProfiler::GetPeerCount() const
	{
		return m_iConnect;
	}

	UInt64 HawkProfiler::GetRecvProto() const
	{
		return m_iRecvProto;
	}

	UInt64 HawkProfiler::GetRecvSize() const
	{
		return m_iRecvSize;
	}

	UInt64 HawkProfiler::GetSendProto() const
	{
		return m_iSendProto;
	}

	UInt64 HawkProfiler::GetSendSize() const
	{
		return m_iSendSize;
	}

	Bool HawkProfiler::Start(const AString& sAddr)
	{
		if (!m_bRunning)
		{
			if (sAddr.size())
				m_sAddr = sAddr;

			GetProcessorNumber();
			GetTotalMem();
			GetCpuUsage();
			GetMemUsage();

			if(!m_sSocket.Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
			{
				HawkPrint("Profiler Init Socket Error.");
				return false;
			}

			m_sSocket.SetNoDelay(true);
			m_sSocket.SetBlocking(false);

			if(!m_sSocket.Bind(SocketAddr(m_sAddr)))
			{
				HawkPrint("Profiler Bind Socket Error.");
				return false;
			}

			m_bRunning = true;
			m_pThread  = new HawkThread(hawk_ProfilerRoutine);
			m_pThread->Start(this);

			return true;
		}		
		return false;
	}

	Bool HawkProfiler::Stop()
	{
		m_bRunning = false;

		if (m_pThread)
			m_pThread->Close();

		return true;
	}

	Bool HawkProfiler::RunLoop()
	{
		UInt32 iCalcTime = HawkOSOperator::GetTickCount();
		
		while(m_bRunning)
		{
			UInt32 iCurTime = HawkOSOperator::GetTickCount();

			//定期计算CPU和内存使用情况
			if (iCurTime -  iCalcTime >= 1000)
			{
				GetCpuUsage();
				GetMemUsage();
				iCalcTime = iCurTime;
			}

			if (!m_sSocket.UpdateEvent(HEVENT_READ, DEFAULT_SLEEP))
				continue;

			m_pRecvBuf->Clear();
			Size_t iRecv = (Size_t)m_pRecvBuf->Capacity();

			SocketAddr sAddr;
			if(!m_sSocket.ReceiveFrom(m_pRecvBuf->Begin(), iRecv, sAddr))
				continue;

			m_pRecvBuf->Resize((UInt32)iRecv);
			Protocol* pProto = 0;
			try
			{
				pProto = P_ProtocolManager->Decode(*m_pRecvBuf);
				if (!pProto) continue;
			}
			catch (HawkException& rhsExcep)
			{
				HawkFmtPrint("Exception: %s", rhsExcep.GetMsg().c_str());
				continue;
			}

			ProtoType eType = pProto->GetType();
			if(eType == SysProtocol::SYS_PROF_REQ)
			{
				SysProtocol::Sys_ProfInfo sInfo;
				sInfo.m_iTimeStamp	 = HawkOSOperator::GetTickCount();
				sInfo.m_iCpuCount	 = m_iCpuCount;
				sInfo.m_iTotalMem	 = m_iTotalMem;
				sInfo.m_iCpuUsage	 = m_iCpuUsage;
				sInfo.m_iMemUsage	 = m_iMemUsage;
				sInfo.m_iConnect	 = m_iConnect;
				sInfo.m_iRecvProto	 = m_iRecvProto;
				sInfo.m_iRecvSize	 = m_iRecvSize;
				sInfo.m_iSendProto	 = m_iSendProto;
				sInfo.m_iSendSize	 = m_iSendSize;

				m_pRecvBuf->Clear();
				sInfo.Encode(*m_pRecvBuf);
				Size_t iSize = (Size_t)m_pRecvBuf->Size();
				m_sSocket.SendTo(m_pRecvBuf->Begin(), iSize, sAddr);
			}

			P_ProtocolManager->ReleaseProto(pProto);
		}
		return true;
	}
}
