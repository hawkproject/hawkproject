#include "HawkRWLock.h"
#include "HawkOSOperator.h"
#include "pthread.h"

namespace Hawk
{
	HawkRWLock::Scope::Scope(HawkRWLock* pRWLock, UInt32 iType, const AString& sFile, Int32 iLine) : m_pRWLock(pRWLock)
	{
		if (m_pRWLock)
			m_pRWLock->Lock(iType, sFile, iLine);
	}

	HawkRWLock::Scope::~Scope()
	{
		if (m_pRWLock)
			m_pRWLock->Unlock();
	}

	//////////////////////////////////////////////////////////////////////////

	HawkRWLock::HawkRWLock() : m_pRWLock(0)
	{
		m_pRWLock = HawkMalloc(sizeof(pthread_rwlock_t));
		if (pthread_rwlock_init((pthread_rwlock_t*)m_pRWLock,0) != HAWK_OK)
		{
			HawkFree(m_pRWLock);
			m_pRWLock = 0;
		}
	}

	HawkRWLock::~HawkRWLock()
	{
		if (m_pRWLock)
		{
			pthread_rwlock_destroy((pthread_rwlock_t*)m_pRWLock);
			HawkFree(m_pRWLock);
			m_pRWLock = 0;
		}
	}

	Bool HawkRWLock::Lock(UInt32 iType, const AString& sFile, Int32 iLine)
	{
		HawkAssert(m_pRWLock);

		if (iType == READ)
		{
			if (pthread_rwlock_rdlock((pthread_rwlock_t*)m_pRWLock) == HAWK_OK)
				return true;
		}
		else if (iType == WRITE)
		{
			if (pthread_rwlock_wrlock((pthread_rwlock_t*)m_pRWLock) == HAWK_OK)
				return true;
		}
		return false;
	}

	Bool HawkRWLock::TryLock(UInt32 iType, const AString& sFile,Int32 iLine)
	{
		HawkAssert(m_pRWLock);

		if (iType == READ)
		{
			if (pthread_rwlock_tryrdlock((pthread_rwlock_t*)m_pRWLock) == HAWK_OK)
				return true;
		}
		else if (iType == WRITE)
		{
			if (pthread_rwlock_trywrlock((pthread_rwlock_t*)m_pRWLock) == HAWK_OK)
				return true;
		}
		return false;
	}

	Bool HawkRWLock::Unlock()
	{
		HawkAssert(m_pRWLock);

		if (pthread_rwlock_unlock((pthread_rwlock_t*)m_pRWLock) == HAWK_OK)
			return true;

		return false;
	}
}
