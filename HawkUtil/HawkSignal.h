#ifndef HAWK_SIGNAL_H
#define HAWK_SIGNAL_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 信号操作封装(注: 先Notify后Wait会导致无效,信号丢失,非水平触发模式)   */
	/************************************************************************/
	class UTIL_API HawkSignal : public HawkRefCounter
	{
	public:
		//构造
		HawkSignal();

		//析构
		virtual ~HawkSignal();

	public:
		//iTime表示等待超时时间,-1表示无限等待,单位MS
		//等待Notify的事件并设置事件未激活状态
		virtual Bool  Wait(Int32 iWaitTime = -1);

		//事件通知(是否广播)
		//对此之前wait的信号才生效
		virtual Bool  Notify(Bool bBroadcast = false);

	protected:
		PVoid   m_pMutex;
		PVoid   m_pCond;
	};
}
#endif
