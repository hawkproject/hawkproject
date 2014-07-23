#ifndef HAWK_WEBSOCKET_H
#define HAWK_WEBSOCKET_H

#include "HawkSocket.h"
#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* WebSocket封装                                                        */
	/************************************************************************/
	class UTIL_API HawkWebSocket : public HawkSocket
	{		
	public:		
		//构造
		HawkWebSocket();
		
		//析构
		virtual ~HawkWebSocket();

	public:
		//握手协议生成(依据请求信息生成握手的相应信息)
		static Bool HandShake(const OctetsStream* pHttpReq, OctetsStream* pHttpRep);

	protected:
		//获取Sec-WebSocket-Key("Sec-WebSocket-Key1: " | "Sec-WebSocket-Key2: ")
		static Bool GetSecWebSocketKey(const OctetsStream* pHttpReq, const AString& sKeyType, AString& sSecKey, UChar cFlag = '\r');
		
		//获取8-byte security key
		static Bool Get8ByteSecurityKey(const OctetsStream* pHttpReq, UChar* pSecKey);
	};	
}
#endif
