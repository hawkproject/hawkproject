#ifndef HAWK_TIMERMANAGER_H
#define HAWK_TIMERMANAGER_H

#include "HawkManagerBase.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* 定时器管理器封装                                                     */
	/************************************************************************/
	class UTIL_API HawkTimerManager : public HawkManagerBase
	{
	protected:
		//构造
		HawkTimerManager();

		//析构
		virtual ~HawkTimerManager();

		//管理器单例申明
		HAKW_SINGLETON_DECL(TimerManager);

	protected:
		//定时器回调函数格式
		typedef void (*TimerEntry)(void* pArgs, UInt32 iTimer, UInt32 iCount);

		//定时器对象
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

		//定时器表
		typedef map<UInt32, Timer*> TimerMap;

	public:
		//更新定时器
		virtual Bool   Tick(UInt32 iPeriod);

		//添加定时器实体
		virtual UInt32 AddTimer(UInt32 iPeriod, TimerEntry pEntry, void* pArgs = 0, UInt32 iLimit = 0);

		//删除定时器实体
		virtual Bool   KillTimer(UInt32 iTimer);

	protected:
		//更新定时器对象
		virtual Bool   UpdateTimer(Timer* sTimer, UInt32 iPeriod);

	protected:
		//定时器表锁
		HawkMutex*	m_pMutex;
		//定时器表
		TimerMap	m_mTimer;
		//当前分配定时器ID
		UInt32		m_iTimer;
	};
	
	#define P_TimerManager  HawkTimerManager::GetInstance()
}
#endif
