#ifndef HAWK_MSGPUMP_H
#define HAWK_MSGPUMP_H

#include "HawkManagerBase.h"
#include "HawkMsgListener.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息泵处理器                                                         */
	/************************************************************************/
	class UTIL_API HawkMsgPump : public HawkManagerBase
	{
	public:
		//构造
		HawkMsgPump();

		//析构
		virtual ~HawkMsgPump();

		//管理器单例申明
		HAKW_SINGLETON_DECL(MsgPump);

		//消息监听者列表
		typedef map<HawkMsgListener*, HawkMsgListener*> ListenerMap;

		//消息监听列表
		typedef map<Int32, ListenerMap>	MsgListenerMap;

	public:
		//注册消息监听器
		virtual Bool     AddListener(Int32 iMsg, HawkMsgListener* pListener);		

		//移除消息监听器
		virtual Bool     RemoveListener(Int32 iMsg, HawkMsgListener* pListener);

	public:
		//投递消息(非阻塞)
		virtual Bool	 Post(HawkMsg* pMsg);

		//发送消息(阻塞)
		virtual Bool     Send(HawkMsg* pMsg);

		//分发所有消息
		virtual Bool     Tick();

	protected:
		//消息监听列表
		MsgListenerMap	  m_mListener;		
		//消息队列
		vector<HawkMsg*>  m_vPumpMsg;
	};

	#define P_MsgPump  HawkMsgPump::GetInstance()
}
#endif
