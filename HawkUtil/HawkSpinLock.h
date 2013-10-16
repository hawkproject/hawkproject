#ifndef HAWK_SPINLOCK_H
#define HAWK_SPINLOCK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 自旋锁操作封装                                                       */
	/************************************************************************/
	class UTIL_API HawkSpinLock : public HawkRefCounter
	{
	public:
		//构造
		HawkSpinLock();

		//析构
		virtual ~HawkSpinLock();

	public:
		//区域锁
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkSpinLock* pSpinLock, const AString& sFile = "", Int32 iLine = 0);

			virtual ~Scope();

		protected:
			HawkSpinLock*  m_pSpinLock;
		};

	public:
		//锁定(阻塞等待)
		virtual Bool  Lock(const AString& sFile = "", Int32 iLine = 0);

		//锁定(立即返回)
		virtual Bool  TryLock(const AString& sFile = "", Int32 iLine = 0);

		//解锁
		virtual Bool  Unlock();

	protected:
		PVoid   m_pSpinLock;

#ifdef _DEBUG
		Bool    m_bLocked;
		AString m_sFile;
		Int32   m_iLine;
		Int32   m_iThread;
#endif
	};

	#define HawkAutoSpinLock(name, spinlock) HawkSpinLock::Scope name(spinlock, __FILE__, __LINE__)
}
#endif
