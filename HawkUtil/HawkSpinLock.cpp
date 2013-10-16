#include "HawkSpinLock.h"
#include "HawkOSOperator.h"
#include "pthread.h"

namespace Hawk
{
	HawkSpinLock::Scope::Scope(HawkSpinLock* pSpinLock, const AString& sFile, Int32 iLine) : m_pSpinLock(pSpinLock)
	{
		if (m_pSpinLock) 
			m_pSpinLock->Lock(sFile,iLine);
	}

	HawkSpinLock::Scope::~Scope()
	{
		if (m_pSpinLock)
			m_pSpinLock->Unlock();
	}

	//////////////////////////////////////////////////////////////////////////

	HawkSpinLock::HawkSpinLock() : m_pSpinLock(0)
	{
		m_pSpinLock = HawkMalloc(sizeof(pthread_spinlock_t));	
		if (pthread_spin_init((pthread_spinlock_t*)m_pSpinLock, 0) != HAWK_OK)
		{
			HawkFree(m_pSpinLock);
			m_pSpinLock = 0;
		}

#ifdef _DEBUG
		m_bLocked = false;
		m_sFile   = "";
		m_iLine   = 0;
		m_iThread = 0;
#endif
	}

	HawkSpinLock::~HawkSpinLock()
	{
		if (m_pSpinLock)
		{
			pthread_spin_destroy((pthread_spinlock_t*)m_pSpinLock);
			HawkFree(m_pSpinLock);
			m_pSpinLock = 0;
		}
	}

	Bool HawkSpinLock::Lock(const AString& sFile,Int32 iLine)
	{
		HawkAssert(m_pSpinLock);

		if (pthread_spin_lock((pthread_spinlock_t*)m_pSpinLock) == HAWK_OK)
		{
#ifdef _DEBUG
			m_bLocked = true;
			m_sFile   = sFile;
			m_iLine   = iLine;
			m_iThread = HawkOSOperator::GetThreadId();
#endif
			return true;
		}
		return false;
	}

	Bool HawkSpinLock::TryLock(const AString& sFile,Int32 iLine)
	{
		HawkAssert(m_pSpinLock);

		if (pthread_spin_trylock((pthread_spinlock_t*)m_pSpinLock) == HAWK_OK)
		{
#ifdef _DEBUG
			m_bLocked = true;
			m_sFile   = sFile;
			m_iLine   = iLine;
			m_iThread = HawkOSOperator::GetThreadId();
#endif
			return true;
		}
		return false;
	}

	Bool HawkSpinLock::Unlock()
	{
		HawkAssert(m_pSpinLock);

#ifdef _DEBUG
		m_bLocked = false;
		m_sFile   = "";
		m_iLine   = 0;
		m_iThread = 0;
#endif
		if (pthread_spin_unlock((pthread_spinlock_t*)m_pSpinLock) == HAWK_OK)
			return true;

		return false;
	}
}
