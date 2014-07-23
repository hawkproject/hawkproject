#ifndef HAWK_MUTEX_H
#define HAWK_MUTEX_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ��������װ����                                                     */
	/************************************************************************/
	class UTIL_API HawkMutex : public HawkRefCounter
	{
	public:
		//����(Recursive��ʾ�߳���Ƕ��ʹ��)
		HawkMutex(Bool bRecursive = false);

		//����
		virtual ~HawkMutex();

	public:
		//������
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			//���������ܱ�ǩ
			enum
			{
				LOCK	= 0x01,
				UNLOCK	= 0x02,
				ALL		= (LOCK | UNLOCK),
			};

		public:
			Scope(HawkMutex* pMutex, UInt32 iFlag = ALL, const AString& sFile = "", Int32 iLine = 0);

			virtual ~Scope();

		protected:
			//�����
			UInt32		m_iFlag;
			//������
			HawkMutex*	m_pMutex;
		};

	public:
		//����(�����ȴ�)
		virtual Bool  Lock(const AString& sFile = "", Int32 iLine = 0);

		//����(��������)
		virtual Bool  TryLock(const AString& sFile = "", Int32 iLine = 0);

		//����
		virtual Bool  Unlock();

	protected:
		PVoid   m_pMutex;

#ifdef _DEBUG
		Bool    m_bLocked;
		AString m_sFile;
		Int32   m_iLine;
		Int32   m_iThread;
#endif
	};

	#define HawkAutoMutex(name, mutex) HawkMutex::Scope name(mutex, 0x03, __FILE__, __LINE__)
}
#endif
