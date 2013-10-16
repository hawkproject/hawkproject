#include "HawkThreadPool.h"
#include "HawkOSOperator.h"
#include "HawkAtomic.h"

namespace Hawk
{
	PVoid hawk_ThreadPoolRoutine(void* pData)
	{
		HawkThread* pThread = (HawkThread*)pData;
		if (!pThread || !pThread->GetOwner())
			return 0;

		HawkThreadPool* pPool = (HawkThreadPool*)pThread->GetOwner();
		while (pPool->IsRunning())
		{
			HawkTask* pTask = 0;
			if (!pPool->PopTask(&pTask))
				return false;

			if (pTask)
			{
				pTask->Run(pTask->GetParam());
				pTask->Close();
				HAWK_RELEASE(pTask);
			}
		}
		return 0;
	}

	HawkThreadPool::HawkThreadPool()
	{
		m_bPoolQueue   = true;
		m_bRunning	   = false;
		m_iTurnIdx	   = 0;
		m_iThreadNum   = 0;
		m_iPushTaskCnt = 0;
		m_iPopTaskCnt  = 0;
		m_pLock		   = new HawkSpinLock;
	}

	HawkThreadPool::~HawkThreadPool()
	{
		Close();

		for (Size_t i=0;i<m_vThread.size();i++)
		{
			HAWK_RELEASE(m_vThread[i]);
		}
		m_vThread.clear();

		HAWK_RELEASE(m_pLock);
	}

	Bool HawkThreadPool::IsRunning() const
	{
		return m_bRunning;
	}

	UInt64 HawkThreadPool::GetPushTaskCnt() const
	{
		return m_iPushTaskCnt;
	}

	UInt64 HawkThreadPool::GetPopTaskCnt() const
	{
		return m_iPopTaskCnt;
	}

	Int32 HawkThreadPool::GetThreadNum()
	{
		return m_iThreadNum;
	}

	Int32 HawkThreadPool::GetThreadId(Int32 iIdx)
	{
		if (iIdx >= 0 && iIdx < m_iThreadNum)
		{
			return m_vThread[iIdx]->GetThreadId();
		}
		return 0;
	}

	Bool HawkThreadPool::InitPool(Int32 iPoolSize, Bool bPoolQueue)
	{
		m_bPoolQueue = bPoolQueue;
		for (Int32 i=0;i<iPoolSize;i++)
		{
			HawkThread* pThread = 0;
			if (m_bPoolQueue)
				pThread = new HawkThread(hawk_ThreadPoolRoutine);
			else
				pThread = new HawkThread(true);
			
			if (pThread)
			{
				pThread->SetOwner(this);
				m_vThread.push_back(pThread);
			}
		}
		m_iThreadNum = (Int32)m_vThread.size();
		return true;
	}

	Bool HawkThreadPool::Start()
	{
		if (!m_bRunning)
		{			
			for (Size_t i=0;i<m_vThread.size();i++)
			{
				m_vThread[i]->Start(m_vThread[i]);
			}
			m_bRunning = true;
			return true;
		}
		return false;
	}

	Bool HawkThreadPool::AddTask(HawkTask* pTask, Int32 iThreadIdx, Bool bFirst)
	{
		if (!m_bRunning || !pTask || !m_iThreadNum)
			return false;

		if (m_bPoolQueue)
		{
			pTask->AddRef();

			HawkAutoSpinLock(lock,m_pLock);
			if(bFirst)
				m_qTask.push_front(pTask);
			else
				m_qTask.push_back(pTask);

			m_iPushTaskCnt ++;
		}
		else
		{		
			if (iThreadIdx < 0)
				iThreadIdx = HawkAtomic::Inc<UInt32>(&m_iTurnIdx);
			else if (iThreadIdx >= m_iThreadNum)
				iThreadIdx = iThreadIdx % m_iThreadNum;

			if (iThreadIdx >= 0 && iThreadIdx < m_iThreadNum)
			{
				return m_vThread[iThreadIdx]->AddTask(pTask, bFirst);
			}
		}
		return true;
	}

	void HawkThreadPool::Close()
	{
		m_bRunning = false;
		for (Size_t i=0;i<m_vThread.size();i++)
		{
			m_vThread[i]->Close();
		}
	}

	Bool  HawkThreadPool::PopTask(HawkTask** ppTask)
	{
		if (m_bRunning && ppTask)
		{
			*ppTask = 0;
			if (m_qTask.size())
			{
				HawkAutoSpinLock(lock,m_pLock);
				if (m_qTask.size())
				{
					*ppTask = m_qTask.front();
					m_qTask.pop_front();
					m_iPopTaskCnt ++;
				}				
			}

			if (*ppTask == 0)
			{
				HawkSleep(DEFAULT_SLEEP);
			}

			return true;
		}
		return false;
	}
}
