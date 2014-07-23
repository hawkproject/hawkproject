#ifndef HAWK_WEBSOCKET_H
#define HAWK_WEBSOCKET_H

#include "HawkSocket.h"
#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* WebSocket��װ                                                        */
	/************************************************************************/
	class UTIL_API HawkWebSocket : public HawkSocket
	{		
	public:		
		//����
		HawkWebSocket();
		
		//����
		virtual ~HawkWebSocket();

	public:
		//����Э������(����������Ϣ�������ֵ���Ӧ��Ϣ)
		static Bool HandShake(const OctetsStream* pHttpReq, OctetsStream* pHttpRep);

	protected:
		//��ȡSec-WebSocket-Key("Sec-WebSocket-Key1: " | "Sec-WebSocket-Key2: ")
		static Bool GetSecWebSocketKey(const OctetsStream* pHttpReq, const AString& sKeyType, AString& sSecKey, UChar cFlag = '\r');
		
		//��ȡ8-byte security key
		static Bool Get8ByteSecurityKey(const OctetsStream* pHttpReq, UChar* pSecKey);
	};	
}
#endif
