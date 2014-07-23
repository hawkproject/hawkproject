#ifndef HAWK_TASK_H
#define HAWK_TASK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 线程任务对象，每个对象表示一个线程能执行的任务                       */
	/************************************************************************/
	class UTIL_API HawkTask : public HawkRefCounter 
	{
	public:
		//任务函数定义
		typedef PVoid (*PThreadRoutine)(void* pArgs);

	public:
		//构造
		HawkTask(PThreadRoutine pRoutine = 0, PVoid pParam = 0);

		//析构
		virtual ~HawkTask();

	public:
		//获取回调参数
		virtual PVoid  GetParam();

		//执行线程任务
		virtual PVoid  Run(void* pArgs = 0);

		//关闭任务,释放资源
		virtual Int32  Close();

	protected:
		//任务回调入口
		PThreadRoutine  m_pRoutine;
		//任务回调参数
		PVoid           m_pParam;
	};
	//任务队列定义
	typedef deque<HawkTask*> TaskQueue;
}
#endif
