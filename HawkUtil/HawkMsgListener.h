#ifndef HAWK_MSGLISTENER_H
#define HAWK_MSGLISTENER_H

#include "HawkMsg.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息监听器                                                           */
	/************************************************************************/
	class UTIL_API HawkMsgListener
	{
	public:
		//构造
		HawkMsgListener();

		//析构
		virtual ~HawkMsgListener();

		//消息注册表
		typedef map<Int32, Int32> MsgTypeMap;

	public:
		//注册消息监听
		virtual Bool   MonitorMsg(Int32 iMsg);

		//消息处理
		virtual Bool   OnMessage(const HawkMsg& sMsg);

		//移除消息监听(参数为0表示移除所有)
		virtual Bool   AbandonMsg(Int32 iMsg = 0);

	protected:
		//注册的消息映射
		MsgTypeMap  m_mMsgType;
	};	
}
#endif
