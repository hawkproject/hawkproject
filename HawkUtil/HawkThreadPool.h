#ifndef HAWK_THREADPOOL_H
#define HAWK_THREADPOOL_H

#include "HawkThread.h"

namespace Hawk
{
	/************************************************************************/
	/* 线程池操作封装                                                       */
	/************************************************************************/
	class UTIL_API HawkThreadPool : public HawkRefCounter
	{
	public:
		//线程池构造
		HawkThreadPool();

		//线程池析构
		virtual ~HawkThreadPool();

		//线程列表类型定义
		typedef vector<HawkThread*> ThreadList;

	public:
		//初始化(PoolSize表示线程数, PoolQueue表示是否使用池任务队列)
		virtual Bool   InitPool(Int32 iPoolSize = 2, Bool bPoolQueue = true);

		//添加执行任务(iThread可以在非PoolQueue模式下指定线程执行)
		virtual Bool   AddTask(HawkTask* pTask, Int32 iThreadIdx = -1, Bool bFirst = false);

		//开始执行
		virtual Bool   Start();

		//获得所有线程数
		virtual Int32  GetThreadNum();

		//获取线程ID
		virtual Int32  GetThreadId(Int32 iIdx);

		//结束所有线程
		virtual void   Close();

		//查询是否开始运作(调度中)
		virtual Bool   IsRunning() const;

		//获取Push进去的任务数
		virtual UInt64 GetPushTaskCnt() const;

		//获取Pop出来的任务数
		virtual UInt64 GetPopTaskCnt() const;

		//弹出队列的一个任务(返回值表示是否继续运行)
		virtual Bool   PopTask(HawkTask** ppTask);

	protected:
		//线程列表
		ThreadList		m_vThread;
		//线程池的线程数目
		Int32			m_iThreadNum;
		//是否拥有自己的任务队列
		Bool			m_bPoolQueue;
		//线程池运行状态
		volatile Bool   m_bRunning;		
		//当前使用的线程索引
		volatile UInt32	m_iTurnIdx;
		//线程任务队列
		TaskQueue		m_qTask;
		//任务队列互斥锁
		HawkSpinLock*	m_pLock;
		//添加的任务总数
		UInt64			m_iPushTaskCnt;
		//执行的任务总数
		UInt64			m_iPopTaskCnt;
	};
}
#endif
