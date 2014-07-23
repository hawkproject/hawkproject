#ifndef HAWK_PROTOCOLPUMP_H
#define HAWK_PROTOCOLPUMP_H

#include "HawkManagerBase.h"
#include "HawkProtocolListener.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息泵处理器                                                         */
	/************************************************************************/
	class UTIL_API HawkProtocolPump : public HawkManagerBase
	{
	public:
		//构造
		HawkProtocolPump();

		//析构
		virtual ~HawkProtocolPump();

		//管理器单例申明
		HAKW_SINGLETON_DECL(ProtocolPump);

		//消息监听者列表
		typedef map<HawkProtocolListener*, HawkProtocolListener*> ListenerMap;

		//消息监听列表
		typedef map<Int32, ListenerMap> ProtocolListenerMap;

	public:
		//注册消息监听器
		virtual Bool     AddListener(Int32 iProtocol, HawkProtocolListener* pListener);

		//移除消息监听器
		virtual Bool     RemoveListener(Int32 iProtocol, HawkProtocolListener* pListener);

	public:
		//投递消息(非阻塞)
		virtual Bool	 Post(HawkProtocol* pProtocol);

		//发送消息(阻塞)
		virtual Bool     Send(HawkProtocol* pProtocol);

		//分发所有消息
		virtual Bool     Tick();		

	protected:
		//清理所有监听器
		virtual Bool     RemoveAllListener();

		//清理所有消息
		virtual Bool     RemoveAllProtocol();

	protected:
		//消息监听列表
		ProtocolListenerMap	  m_mListener;		
		//消息队列
		vector<HawkProtocol*> m_vProtocol;
	};	

	#define P_ProtocolPump  HawkProtocolPump::GetInstance()
}
#endif
