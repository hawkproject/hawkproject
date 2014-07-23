#ifndef HAWK_PROTOCOLLISTENER_H
#define HAWK_PROTOCOLLISTENER_H

#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息监听器                                                           */
	/************************************************************************/
	class UTIL_API HawkProtocolListener
	{
	public:
		//构造
		HawkProtocolListener();

		//析构
		virtual ~HawkProtocolListener();

		//消息注册表
		typedef map<ProtoType, ProtoType> ProtoTypeMap;

	public:
		//注册消息监听
		virtual Bool   MonitorProto(ProtoType iType);

		//消息处理
		virtual Bool   OnProtocol(HawkProtocol* pProto);

		//移除消息监听(参数为0表示移除所有)
		virtual Bool   AbandonProto(ProtoType iType = 0);

	protected:
		//注册的消息映射
		ProtoTypeMap  m_mProtoType;
	};	
}
#endif
