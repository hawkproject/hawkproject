#ifndef HAWK_GW_CONTROLER_H
#define HAWK_GW_CONTROLER_H

#include "HawkGateDef.h"

namespace Hawk
{	
	/************************************************************************/
	/* 网关服务器命令控制器													*/
	/************************************************************************/
	class HawkGateway;
	class GATE_API HawkGwControler : public HawkRefCounter
	{
	public:
		//构造
		HawkGwControler(HawkGateway* pGateway = 0);

		//析构
		virtual ~HawkGwControler();

	public:
		//初始化控制器
		virtual Bool  Init(const AString& sAddr);

		//更新控制器状态
		virtual Bool  UpdateEvent();

		//停止
		virtual Bool  Stop();

	protected:
		//协议处理
		virtual Bool  OnCommand(const GateCommand* pCmd, const OctetsStream* pIdentify);

	protected:
		//操作的网关对象
		HawkGateway*	m_pGateway;
		//服务通信对象
		HawkZmq*		m_pControler;
		//标识缓冲区
		OctetsStream*	m_pIdentify;
		//通用缓冲区
		OctetsStream*	m_pCommand;
	};
}
#endif
