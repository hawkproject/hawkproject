#ifndef HAWK_RWLOCK_H
#define HAWK_RWLOCK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 读写锁操作封装                                                       */
	/************************************************************************/
	class UTIL_API HawkRWLock : public HawkRefCounter
	{
	public:
		//构造
		HawkRWLock();

		//析构
		virtual ~HawkRWLock();

		//锁定类型
		enum
		{
			READ  = 0x01,
			WRITE = 0x02,
		};

	public:
		//区域锁
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkRWLock* pRWLock, UInt32 iType, const AString& sFile = "", Int32 iLine = 0) ;

			virtual ~Scope();

		protected:
			HawkRWLock*  m_pRWLock;
		};

	public:
		//锁定(阻塞等待)
		virtual Bool  Lock(UInt32 iType, const AString& sFile = "", Int32 iLine = 0);

		//锁定(立即返回)
		virtual Bool  TryLock(UInt32 iType, const AString& sFile = "", Int32 iLine = 0);

		//解锁
		virtual Bool  Unlock();

	protected:
		PVoid   m_pRWLock;
	};

	#define HawkAutoRWLock(name, rwlock, type) HawkRWLock::Scope name(rwlock, type, __FILE__, __LINE__)
}
#endif
