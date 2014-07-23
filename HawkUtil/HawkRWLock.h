#ifndef HAWK_RWLOCK_H
#define HAWK_RWLOCK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ��д��������װ                                                       */
	/************************************************************************/
	class UTIL_API HawkRWLock : public HawkRefCounter
	{
	public:
		//����
		HawkRWLock();

		//����
		virtual ~HawkRWLock();

		//��������
		enum
		{
			READ  = 0x01,
			WRITE = 0x02,
		};

	public:
		//������
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkRWLock* pRWLock, UInt32 iType, const AString& sFile = "", Int32 iLine = 0) ;

			virtual ~Scope();

		protected:
			HawkRWLock*  m_pRWLock;
		};

	public:
		//����(�����ȴ�)
		virtual Bool  Lock(UInt32 iType, const AString& sFile = "", Int32 iLine = 0);

		//����(��������)
		virtual Bool  TryLock(UInt32 iType, const AString& sFile = "", Int32 iLine = 0);

		//����
		virtual Bool  Unlock();

	protected:
		PVoid   m_pRWLock;
	};

	#define HawkAutoRWLock(name, rwlock, type) HawkRWLock::Scope name(rwlock, type, __FILE__, __LINE__)
}
#endif
