#ifndef HAWK_APPGATEPROXY_H
#define HAWK_APPGATEPROXY_H

#include "HawkGateProxy.h"

namespace Hawk
{
	class HawkAppFrame;
	/************************************************************************/
	/* 框架类使用网关代理器                                                 */
	/************************************************************************/
	class FRAME_API HawkAppGateProxy : public HawkGateProxy
	{
	public:
		//构造
		HawkAppGateProxy(HawkAppFrame* pApp = 0);

		//析构
		virtual ~HawkAppGateProxy();

	public:
		//初始化
		virtual Bool  Init();

	public:
		//代理器连接
		virtual Bool  OnProxyConnected();

		//代理器断开
		virtual Bool  OnProxyDisconnect();

		//接收到协议
		virtual Bool  OnRecvProtocol(const GateMsgHeader& sHeader, HawkProtocol* pProto);

	public:
		//是否连接状态
		virtual Bool  IsConnect() const;

	protected:
		//App对象
		HawkAppFrame*	m_pApp;
		//连接状态
		Bool			m_bConnect;
	};
}
#endif
