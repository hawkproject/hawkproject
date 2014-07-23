#include "HawkSignal.h"
#include "HawkOSOperator.h"
#include "pthread.h"

namespace Hawk
{
	HawkSignal::HawkSignal() : m_pMutex(0), m_pCond(0)
	{
		m_pMutex = HawkMalloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init((pthread_mutex_t*)m_pMutex, 0) != HAWK_OK)
		{
			HawkFree(m_pMutex);
			m_pMutex = 0;
		}

		m_pCond = HawkMalloc(sizeof(pthread_cond_t));				
		if (pthread_cond_init((pthread_cond_t*)m_pCond, 0) != HAWK_OK)
		{
			HawkFree(m_pCond);
			m_pCond = 0;
		}
	}

	HawkSignal::~HawkSignal()
	{
		if (m_pMutex)
		{
			pthread_mutex_destroy((pthread_mutex_t*)m_pMutex);
			HawkFree(m_pMutex);
			m_pMutex = 0;
		}

		if (m_pCond)
		{
			pthread_cond_destroy((pthread_cond_t*)m_pCond);
			HawkFree(m_pCond);
			m_pCond = 0;
		}
	}

	Bool HawkSignal::Wait(Int32 iWaitTime)
	{
		HawkAssert(m_pMutex && m_pCond);
		if (!m_pMutex || !m_pCond)
			return false;

		pthread_mutex_lock((pthread_mutex_t*)m_pMutex);

		if (iWaitTime > 0)
		{
			struct timespec ts;
			ts.tv_sec  = HawkOSOperator::GetSysTime() + iWaitTime / 1000;
			ts.tv_nsec = (iWaitTime % 1000) * 1000;

			if (pthread_cond_timedwait((pthread_cond_t*)m_pCond, (pthread_mutex_t*)m_pMutex, &ts) != HAWK_OK)
			{
				pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);
				return false;			
			}
		}
		else
		{
			if (pthread_cond_wait((pthread_cond_t*)m_pCond, (pthread_mutex_t*)m_pMutex) != HAWK_OK)
			{
				pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);
				return false;
			}
		}		

		pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);

		return true;
	}

	Bool HawkSignal::Notify(Bool bBroadcast)
	{
		HawkAssert(m_pMutex && m_pCond);
		if (!m_pMutex || !m_pCond)
			return false;

		pthread_mutex_lock((pthread_mutex_t*)m_pMutex);

		if (bBroadcast)
		{
			if (pthread_cond_broadcast((pthread_cond_t*)m_pCond) != HAWK_OK)
			{
				pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);
				return false;
			}
		}
		else
		{
			if (pthread_cond_signal((pthread_cond_t*)m_pCond) != HAWK_OK)
			{
				pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);
				return false;
			}
		}

		pthread_mutex_unlock((pthread_mutex_t*)m_pMutex);

		return true;
	}
}
