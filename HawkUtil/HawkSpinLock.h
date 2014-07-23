#ifndef HAWK_SPINLOCK_H
#define HAWK_SPINLOCK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ������������װ                                                       */
	/************************************************************************/
	class UTIL_API HawkSpinLock : public HawkRefCounter
	{
	public:
		//����
		HawkSpinLock();

		//����
		virtual ~HawkSpinLock();

	public:
		//������
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkSpinLock* pSpinLock, const AString& sFile = "", Int32 iLine = 0);

			virtual ~Scope();

		protected:
			HawkSpinLock*  m_pSpinLock;
		};

	public:
		//����(�����ȴ�)
		virtual Bool  Lock(const AString& sFile = "", Int32 iLine = 0);

		//����(��������)
		virtual Bool  TryLock(const AString& sFile = "", Int32 iLine = 0);

		//����
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
