#ifndef HAWK_MSGMANAGER_H
#define HAWK_MSGMANAGER_H

#include "HawkManagerBase.h"
#include "HawkMsg.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息管理器,在创建和释放消息之前,必须注册作用的消息类型               */
	/************************************************************************/
	class UTIL_API HawkMsgManager : public HawkManagerBase
	{
	protected:
		//构造
		HawkMsgManager();

		//析构
		virtual ~HawkMsgManager();

		//管理器单例申明
		HAKW_SINGLETON_DECL(MsgManager);

		//注册列表
		typedef map<Int32, Int32> MsgRegMap;

	public:
		//更新管理器
		virtual Bool	 Tick(UInt32 iPeriod);

	public:
		//注册消息类型
		virtual Bool	 Register(Int32 iMsg);	

		//获取消息
		virtual HawkMsg* GetMsg(Int32 iMsg);

		//释放消息
		virtual Bool     FreeMsg(HawkMsg*& pMsg);

		//获取注册协议ID列表
		virtual UInt32	 GetRegMsgIds(vector<Int32>& vMsgIds);

	protected:		
		//注册类型列表
		MsgRegMap	m_mRegMsg;
	};

	#define P_MsgManager  HawkMsgManager::GetInstance()
}
#endif
