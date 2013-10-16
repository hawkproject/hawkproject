#ifndef HAWK_THREAD_H
#define HAWK_THREAD_H

#include "HawkTask.h"
#include "HawkSpinLock.h"

namespace Hawk
{
	/************************************************************************/
	/* 线程操作封装                                                         */
	/************************************************************************/
	class UTIL_API HawkThread : public HawkRefCounter
	{
	public:
		//是否等待线程
		HawkThread(Bool bWaitTask = false);

		//线程任务构造
		HawkThread(HawkTask* pTask);

		//线程函数定义
		typedef HawkTask::PThreadRoutine PThreadRoutine;

		//线程构造
		HawkThread(PThreadRoutine pRoutine);

		//析构
		virtual ~HawkThread();

	public:
		//状态定义
		enum
		{
			STATE_NONE = 0,
			STATE_RUNNING,
			STATE_CLOSING,
			STATE_CLOSED,
		};

	public:
		//开启线程
		virtual Bool   Start(void* pArgs = 0, Bool bHighPriority = false);

		//阻塞等待结束线程
		virtual Bool   Close();

		//添加任务对象,只能使用在多任务线程
		virtual Bool   AddTask(HawkTask* pTask, Bool bFirst = false);

		//获取Push进去的任务数
		virtual UInt64 GetPushTaskCnt() const;

		//获取Pop出来的任务数
		virtual UInt64 GetPopTaskCnt() const;

		//获取线程ID
		virtual UInt32 GetThreadId() const;

		//获取线程运行状态
		virtual Bool   IsRunning() const;

		//是否等待关闭
		virtual Bool   IsClosing() const;

		//设置拥有者
		virtual void   SetOwner(PVoid pPool);

		//获取拥有者
		virtual PVoid  GetOwner();

	public:
		//线程函数调用
		virtual PVoid  RunLoop();

	protected:
		//内部初始化
		virtual Bool   InnerInit();

		//清除任务队列
		virtual void   ClearTaskQueue();

		//运行任务队列
		virtual void   RunTaskQueue();

	protected:
		//线程句柄
		PVoid		    m_pThread;
		//线程函数参数
		PVoid			m_pArgs;
		//拥有者(所属线程池)
		PVoid			m_pOwner;
		//线程ID
		UInt32			m_iId;
		//线程状态
		volatile UInt32	m_iState;
		//运行中
		volatile Bool   m_bRunning;
		//线程任务队列
		TaskQueue		m_qTask;
		//线程函数入口地址
		PThreadRoutine	m_pRoutine;
		//任务队列互斥锁
		HawkSpinLock*	m_pLock;
		//添加的任务总数
		UInt64			m_iPushTaskCnt;
		//执行的任务总数
		UInt64			m_iPopTaskCnt;
	};
}
#endif
