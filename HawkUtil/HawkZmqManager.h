#ifndef HAWK_ZMQMANAGER_H
#define HAWK_ZMQMANAGER_H

#include "HawkManagerBase.h"
#include "HawkZmq.h"

namespace Hawk
{
	/************************************************************************/
	/* ZMQ对象管理器                                                        */
	/************************************************************************/
	class UTIL_API HawkZmqManager : public HawkManagerBase
	{
	protected:
		//构造和析构
		HawkZmqManager();

		//析构
		virtual ~HawkZmqManager();

		//单例申明
		HAKW_SINGLETON_DECL(ZmqManager);

		//对象表类型定义
		typedef map<HawkZmq*, HawkZmq*> ZmqMap;

	public:
		//创建上下文
		virtual Bool	 SetupZmqCtx(Int32 iThreads = 1);

		//获取上下文
		virtual void*	 GetZmqCtx();

		//创建ZMQ
		virtual HawkZmq* CreateZmq(Int32 iType);
		
		//关闭ZMQ
		virtual Bool	 CloseZmq(HawkZmq*& pZmq);
		
		//开启ZMQ数据通道,Bothway: 双向传输, Timeout: 时间超时(-1为无限等待), Once: 传输一次
		virtual Bool     ProxyZmq(HawkZmq* pFrontend, HawkZmq* pBackend, Bool bBothway, Int32 iTimeout, Bool bOnce);

	protected:
		//上下文句柄
		void*   m_pZmqCtx;
		//创建的Zmq列表
		ZmqMap  m_mZmq;
	};

	//简易宏定义
	#define P_ZmqManager  HawkZmqManager::GetInstance()
}
#endif
