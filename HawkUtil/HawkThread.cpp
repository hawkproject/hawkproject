#include "HawkThread.h"
#include "HawkOSOperator.h"
#include "pthread.h"

namespace Hawk
{
	PVoid hawk_ThreadRoutine(void* pArgs)
	{
		HawkThread* pThread = static_cast<HawkThread*>(pArgs); 
		if (pThread)
			return pThread->RunLoop();

		return 0;
	}

	HawkThread::HawkThread(Bool bWaitTask) : m_pRoutine(0)
	{
		InnerInit();
	}

	HawkThread::HawkThread(HawkTask* pTask) : m_pRoutine(0)
	{
		InnerInit();

		AddTask(pTask);		
	}

	HawkThread::HawkThread(PThreadRoutine pRoutine) : m_pRoutine(pRoutine)
	{
		InnerInit();
	}	

	HawkThread::~HawkThread()
	{
		Close();
		
		if (m_pThread)
		{
			HawkFree(m_pThread);
			m_pThread = 0;
		}
		
		HAWK_RELEASE(m_pLock);
	}

	Bool HawkThread::InnerInit()
	{
		m_pOwner		= 0;
		m_iId			= 0;
		m_pArgs			= 0;
		m_iPushTaskCnt	= 0;
		m_iPopTaskCnt	= 0;
		m_pThread		= 0;
		m_bRunning		= false;
		m_iState		= STATE_NONE;
		m_pLock  		= new HawkSpinLock;

		return true;
	}

	UInt64 HawkThread::GetPushTaskCnt() const
	{
		return m_iPushTaskCnt;
	}

	UInt64 HawkThread::GetPopTaskCnt() const
	{
		return m_iPopTaskCnt;
	}

	UInt32 HawkThread::GetThreadId() const
	{
		return m_iId;
	}

	Bool HawkThread::IsRunning() const
	{
		return m_bRunning;
	}

	Bool HawkThread::IsClosing() const
	{
		return m_iState == STATE_CLOSING;
	}

	void HawkThread::SetOwner(PVoid pPool)
	{
		m_pOwner = pPool;
	}

	PVoid HawkThread::GetOwner()
	{
		return m_pOwner;
	}

	Bool  HawkThread::AddTask(HawkTask* pTask, Bool bFirst)
	{
		if(pTask)
		{		
			pTask->AddRef();
			HawkAutoSpinLock(lock, m_pLock);

			if(bFirst)
				m_qTask.push_front(pTask);
			else
				m_qTask.push_back(pTask);

			m_iPushTaskCnt ++;
			return true;
		}
		return false;
	}

	Bool  HawkThread::Start(void* pArgs, Bool bHighPriority)
	{
		if(!m_bRunning && !m_pThread)
		{			
			m_pArgs    = pArgs;
			m_bRunning = true;
			m_pThread  = HawkMalloc(sizeof(pthread_t));
			
			pthread_attr_t* pAttr = 0;

			pthread_attr_t sAttr;
			pthread_attr_init(&sAttr);
			pthread_attr_setschedpolicy(&sAttr,SCHED_RR);

			if (bHighPriority)
			{
				sched_param sParam; 
				memset(&sParam, 0, sizeof(sParam));
				sParam.sched_priority = sched_get_priority_max(SCHED_RR);
				pthread_attr_setschedparam(&sAttr, &sParam);
			}
			
			if (pthread_create((pthread_t*)m_pThread, pAttr, hawk_ThreadRoutine, this) != HAWK_OK)
			{
				m_bRunning = false;
				m_iState   = STATE_CLOSED;
				pthread_attr_destroy(&sAttr);
				return false;
			}
			pthread_attr_destroy(&sAttr);

			while (m_iState != STATE_RUNNING)
			{
				HawkSleep(DEFAULT_SLEEP);
			}

			return true;
		}
		return false;
	}

	Bool  HawkThread::Close()
	{
		if (m_pThread && m_bRunning) 
		{
			m_bRunning = false;
			m_iState   = STATE_CLOSING;
			
			//设置退出信号
			pthread_cancel(*((pthread_t*)m_pThread));
			//等待线程结束
			pthread_join(*((pthread_t*)m_pThread), 0);

			HawkFree(m_pThread);
			m_pThread = 0;
			m_iId     = 0;
			m_iState  = STATE_CLOSED;
		}

		ClearTaskQueue();
		return true;
	}

	void HawkThread::ClearTaskQueue()
	{
		HawkAutoSpinLock(lock, m_pLock);
		TaskQueue::iterator it = m_qTask.begin();
		for (;it!=m_qTask.end();it++)
		{
			HawkTask* pTask = *it;
			if(pTask)
				pTask->Close();
			HAWK_RELEASE(pTask);
		}
		m_qTask.clear();
	}

	void HawkThread::RunTaskQueue()
	{
		while (m_bRunning)
		{
			HawkTask* pTask = 0;
			if (m_qTask.size())
			{
				HawkAutoSpinLock(lock, m_pLock);
				if (m_qTask.size())
				{
					pTask = m_qTask.front();
					m_qTask.pop_front();
					m_iPopTaskCnt ++;
				}
			}

			if (pTask)
			{
				pTask->Run(pTask->GetParam());
				pTask->Close();
				HAWK_RELEASE(pTask);
			}
			else
			{
				break;
			}
		}
	}

	PVoid HawkThread::RunLoop()
	{
		m_iId	 = HawkOSOperator::GetThreadId();
		m_iState = STATE_RUNNING;

		if (m_pRoutine)
		{
			return (*m_pRoutine)(m_pArgs);
		}

		while (m_bRunning)
		{
			HawkTask* pTask = 0;
			if (m_qTask.size())
			{
				HawkAutoSpinLock(lock, m_pLock);
				if (m_qTask.size())
				{
					pTask = m_qTask.front();
					m_qTask.pop_front();
					m_iPopTaskCnt ++;
				}
			}

			if (pTask)
			{
				pTask->Run(pTask->GetParam());
				pTask->Close();
				HAWK_RELEASE(pTask);
			}
			else
			{
				HawkSleep(DEFAULT_SLEEP);
			}
		}

		return 0;
	}
}
