#ifndef HAWK_TIMERMANAGER_H
#define HAWK_TIMERMANAGER_H

#include "HawkManagerBase.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ʱ����������װ                                                     */
	/************************************************************************/
	class UTIL_API HawkTimerManager : public HawkManagerBase
	{
	protected:
		//����
		HawkTimerManager();

		//����
		virtual ~HawkTimerManager();

		//��������������
		HAKW_SINGLETON_DECL(TimerManager);

	protected:
		//��ʱ���ص�������ʽ
		typedef void (*TimerEntry)(void* pArgs, UInt32 iTimer, UInt32 iCount);

		//��ʱ������
		struct Timer
		{
			UInt32	   TimerId;
			UInt32	   Period;
			UInt32	   Elapse;
			UInt32	   Limit;
			UInt32	   Count;
			void*	   Args;
			TimerEntry Entry;
		};

		//��ʱ����
		typedef map<UInt32, Timer*> TimerMap;

	public:
		//���¶�ʱ��
		virtual Bool   Tick(UInt32 iPeriod);

		//��Ӷ�ʱ��ʵ��
		virtual UInt32 AddTimer(UInt32 iPeriod, TimerEntry pEntry, void* pArgs = 0, UInt32 iLimit = 0);

		//ɾ����ʱ��ʵ��
		virtual Bool   KillTimer(UInt32 iTimer);

	protected:
		//���¶�ʱ������
		virtual Bool   UpdateTimer(Timer* sTimer, UInt32 iPeriod);

	protected:
		//��ʱ������
		HawkMutex*	m_pMutex;
		//��ʱ����
		TimerMap	m_mTimer;
		//��ǰ���䶨ʱ��ID
		UInt32		m_iTimer;
	};
	
	#define P_TimerManager  HawkTimerManager::GetInstance()
}
#endif
