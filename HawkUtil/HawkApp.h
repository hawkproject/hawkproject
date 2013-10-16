#ifndef HAWK_APP_H
#define HAWK_APP_H

#include "HawkAppObj.h"
#include "HawkObjManager.h"
#include "HawkMsgManager.h"
#include "HawkThreadPool.h"

namespace Hawk
{
	/************************************************************************/
	/* 应用层封装                                                           */
	/************************************************************************/
	class UTIL_API HawkApp : public HawkAppObj
	{
	public:
		//构造
		HawkApp(const XID& sXid = XID());

		//析构
		virtual ~HawkApp();

		//内部对象访问
		friend  class  HawkApp_ProtoTask;

		friend  class  HawkApp_MsgTask;

		friend  class  HawkApp_TickTask;

	public:
		//对象定义以及对象管理器定义
		typedef HawkObjBase<XID, HawkAppObj>	 ObjBase;

		//管理器类型定义
		typedef HawkObjManager<XID, HawkAppObj>  ObjMan;

		//管理器映射表
		typedef map<UInt32, ObjMan*>			 ObjManMap;

		//会话ID和对象ID的映射表
		typedef map<SID, XID>					 SidXidMap;

		//安全对象定义
		class SafeObj : public ObjMan::SafeObj
		{
		public:
			//构造
			SafeObj(XID sXid = XID());
			//析构
			virtual ~SafeObj();
		};

	public:
		//初始化应用(开启线程运行)
		virtual Bool		Init(Int32 iThread = 1);
		//启动服务
		virtual Bool		Run();
		//关闭服务
		virtual Bool		Stop();

	public:
		//是否运行状态
		virtual Bool		IsRunning() const;

		//线程池线程数目
		virtual Int32		GetThreadNum() const;	

		//发送命令(应用层实现)
		virtual Bool		SendProtocol(SID iSid, Protocol* pProto);

	public:
		//注册创建对象管理器
		virtual ObjMan*		CreateObjMan(UInt32 iType);

		//获取对象管理器
		virtual ObjMan*		GetObjMan(UInt32 iType);

		//创建对象
		virtual HawkAppObj*	CreateObj(const XID& sXid, SID iSid = 0);

		//销毁对象
		virtual Bool		DeleteObj(const XID& sXid);

	public:
		//接收到协议后投递到应用, 未绑定的会话ID会因找不到对象而投递失败
		virtual Bool		PostProtocol(SID iSid, Protocol* pProto);

		//接收到协议后投递到应用, 对象ID需要有效才能投递成功
		virtual Bool		PostProtocol(const XID& sXid, SID iSid, Protocol* pProto);

		//直接投递消息
		virtual Bool		PostMsg(HawkMsg* pMsg);

		//向特定对象投递消息
		virtual Bool		PostMsg(const XID& sXid, HawkMsg* pMsg);	

		//广播消息
		virtual Bool		PostMsg(const XIDVector& vXID, HawkMsg* pMsg);

		//提交更新
		virtual Bool		PostTick(const XIDVector& vXID);

		//投递应用任务
		virtual Bool		PostAppTask(HawkTask* pTask, Int32 iThreadIdx = -1);
	
	public:
		//根据回去对象ID
		virtual XID			GetXidBySid(SID iSid);

		//绑定会话ID和对象ID
		virtual void		BindSidXid(SID iSid, const XID& sXid);

		//解绑定会话ID和对象ID
		virtual void		UnbindSidXid(SID iSid);

	protected:
		//分发协议
		virtual Bool		DispatchProto(const XID& sXid, SID iSid, Protocol* pProto);

		//分发消息
		virtual Bool		DispatchMsg(const XID& sXid, HawkMsg* pMsg);

		//触发更新
		virtual Bool		DispatchTick(const XID& sXid);

		//应用层创建对象
		virtual HawkAppObj*	AppCreateObj(const XID& sXid);

	protected:
		//是否运行状态
		volatile Bool	m_bRunning;
		//主线程ID
		UInt32			m_iMainTid;
		//对象管理器
		ObjManMap		m_mObjMan;
		//会话映射表
		SidXidMap		m_mSidXid;
		//会话表锁
		HawkSpinLock*	m_pSidXidLock;
		//线程池
		HawkThreadPool*	m_pThreadPool;
	};
}
#endif
