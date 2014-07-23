#ifndef HAWK_APPGATEWAY_H
#define HAWK_APPGATEWAY_H

#include "HawkGateway.h"

namespace Hawk
{
	class HawkAppFrame;
	/************************************************************************/
	/* 框架类使用内嵌网关服务器                                             */
	/************************************************************************/	
	class FRAME_API HawkAppGateway : public HawkGateway
	{
	public:
		//构造
		HawkAppGateway(HawkAppFrame* pApp = 0);

		//析构
		virtual ~HawkAppGateway();

	public:
		//初始化
		virtual Bool    Init();

		//停止
		virtual Bool	Stop();

	protected:
		//App对象
		HawkAppFrame*	m_pApp;
		//网关服务器线程
		HawkThread*		m_pThread;
	};
}
#endif
