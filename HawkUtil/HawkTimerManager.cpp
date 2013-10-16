#include "HawkTimerManager.h"
#include "HawkAtomic.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(TimerManager);

	HawkTimerManager::HawkTimerManager() : m_iTimer(0)
	{
		m_pMutex = new HawkMutex;		
		m_mTimer.clear();
	}

	HawkTimerManager::~HawkTimerManager()
	{		
		HAWK_RELEASE(m_pMutex);

		TimerMap::iterator it = m_mTimer.begin();
		for (;it!=m_mTimer.end();it++)
		{
			HawkFree(it->second);
		}
		m_mTimer.clear();
	}

	Bool  HawkTimerManager::Tick(UInt32 iPeriod)
	{
		if (m_mTimer.size())
		{
			HawkAutoMutex(lock, m_pMutex);

			TimerMap::iterator it = m_mTimer.begin();
			for (;it!=m_mTimer.end();)
			{
				if (!UpdateTimer(it->second, iPeriod))
				{
					HawkFree(it->second);
					m_mTimer.erase(it++);
					continue;
				}
				it ++;
			}
		}

		return HawkManagerBase::Tick(iPeriod);
	}

	Bool HawkTimerManager::UpdateTimer(Timer* pTimer, UInt32 iPeriod)
	{
		if (pTimer)
		{
			pTimer->Elapse += iPeriod;
			if (pTimer->Elapse >= pTimer->Period)
			{
				pTimer->Elapse = 0;
				pTimer->Count ++;

				if(pTimer->Entry)
					(*pTimer->Entry)(pTimer->Args, pTimer->TimerId, pTimer->Count);
			}

			if(!pTimer->Limit || pTimer->Count < pTimer->Limit)
				return true;
		}
		return false;		
	}

	UInt32 HawkTimerManager::AddTimer(UInt32 iPeriod, TimerEntry pEntry, void* pArgs, UInt32 iLimit)
	{		
		if (iPeriod && pEntry)
		{
			UInt32 iTimer = HawkAtomic::Inc<UInt32>(&m_iTimer);
			Timer* pTimer = (Timer*)HawkMalloc(sizeof(Timer));
			pTimer->TimerId	= iTimer;
			pTimer->Period	= iPeriod;
			pTimer->Limit	= iLimit;
			pTimer->Entry	= pEntry;
			pTimer->Args	= pArgs;
			pTimer->Count	= 0;
			pTimer->Elapse	= 0;

			HawkAutoMutex(lock, m_pMutex);
			m_mTimer[iTimer] = pTimer;
			return pTimer->TimerId;
		}
		return 0;
	}

	Bool  HawkTimerManager::KillTimer(UInt32 iTimer)
	{
		HawkAutoMutex(lock, m_pMutex);
		TimerMap::iterator it = m_mTimer.find(iTimer);
		if (it != m_mTimer.end())
		{
			HawkFree(it->second);
			m_mTimer.erase(it);
			return true;
		}
		return false;
	}
}
