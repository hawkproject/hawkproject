#ifndef HAWK_COUNTER_H
#define HAWK_COUNTER_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 计数器,也可以用来计时                                                */
	/************************************************************************/
	class UTIL_API HawkCounter : public HawkRefCounter
	{
	public:
		//构造和析构
		HawkCounter(UInt32 iPeriod = 0);

		//析构
		virtual ~HawkCounter();

	public:
		//计数上限
		DEFINE_PROPERTY(UInt32,	m_iPeriod,  Period);
		//当前计数
		DEFINE_PROPERTY(UInt32,	m_iCounter, Counter);
		//暂停状态
		DEFINE_PROPERTY(Bool,	m_bPause,   IsPause);

	public:
		//计数是否达到满值
		virtual Bool IsFull() const;

		//重置(bFull:把计数重置最大)
		virtual void Reset(bool bFull = false);

		//暂停
		virtual void SetPause(bool bPause);
		
		//增加计数
		virtual Bool IncCounter(UInt32 iCounter = 1);
		
		//减少计数
		virtual void DecCounter(UInt32 iCounter = 1);
	};
}
#endif

