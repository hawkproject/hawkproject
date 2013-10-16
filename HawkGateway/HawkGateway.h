#ifndef HAWK_GATEWAY_H
#define HAWK_GATEWAY_H

#include "HawkGateDef.h"
#include "HawkGateThread.h"
#include "HawkProfiler.h"
#include "HawkGateSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* 网关服务器基本封装(基于libevent的边缘事件触发模型)                   */
	/************************************************************************/
	class GATE_API HawkGateway : public HawkRefCounter
	{
	public:
		//构造
		HawkGateway();

		//析构
		virtual ~HawkGateway();

		//服务列表
		struct Service 
		{
			UInt32  SvrId;
			SOCKET  SvrFd;
			UInt32  ConnCap;			
			UInt32  CurCap;

			Service(UInt32 iSvrId = 0, SOCKET iSvrFd = INVALID_SOCKET, UInt32 iConnCap = 0) : SvrId(iSvrId), SvrFd(iSvrFd), ConnCap(iConnCap), CurCap(0) {}
		};
		//当前挂载的服务器列表
		typedef vector<Service>		ServiceVec;
		//会话ID和服务器ID绑定映射
		typedef map<SID, UInt32>	SidBindMap;

		//网关模式
		enum
		{
			GATEMODE_ECHO = 0,					//回显模式
			GATEMODE_SINGLE_SERVICE,			//单个服务器模式
			GATEMODE_MULTI_SERVICE_STOCK,		//多服务器最大负载模式
			GATEMODE_MULTI_SERVICE_BALANCE,		//多服务器负载均衡模式
		};

	public:
		//初始化网关服务
		virtual Bool    Init(const AString& sFrontend, const AString& sBackend, Int32 iThread = 4, Int32 iGateMode = GATEMODE_SINGLE_SERVICE);

		//开启服务
		virtual Bool    Run();

		//关闭服务(通知结束,不阻塞)
		virtual Bool    Stop();		
		
	public:
		//获取线程数
		virtual Int32   GetThreadNum() const;

		//获取线程ID
		virtual Int32   GetThreadId(Int32 iIdx) const;

		//设置主循环阻塞属性
		virtual Bool    SetBlocking(Bool bBlocking = true);

		//设置会话缓冲大小
		virtual void    SetBufSize(Int32 iBufSize);

		//获取会话缓冲大小
		virtual Int32   GetBufSize() const;	

		//设置完全处理直到空闲的任务模式
		virtual Bool    SetWaitIdle(Bool bWaitIdle);

		//是否为处理任务直到无任务的模式
		virtual Bool    IsWaitIdle() const;

		//判断运行状态
		virtual Bool    IsRunning() const;		

		//设置会话超时时间
		virtual void    SetSessionTimeout(Int32 iTimeout);

		//获取会话超时时间
		virtual Int32   GetSessionTimeout() const;

		//开启IO数据流加解密组件
		virtual Bool    TurnGateSecurity();

		//开启性能监视器
		virtual Bool    TurnOnProfiler(const AString& sAddr);
	
		//开启跨域服务(flash 843端口验证)
		virtual Bool    TurnOnCrossDomain();	

	public:
		//创建网关线程
		virtual Bool    CreateGateThread(HawkGateThread*& pThread);

		//创建会话加密组件
		virtual Bool    CreateISecurity(HawkSecurity*& pSecurity);

		//创建会话加密组件
		virtual Bool    CreateOSecurity(HawkSecurity*& pSecurity);

		//获取性能监视器
		HawkProfiler*   GetProfiler();

		//获取线程ZMQ的服务地址
		virtual AString GetThreadZmqAddr() const;

		//获取后端服务监视ZMQ地址
		virtual AString GetServerZmqAddr() const;		

		//会话信任校验(供GateThread检测)
		virtual Bool    TrustSession(void* pSession, OctetsStream* pIBuffer);

		//关闭会话
		virtual Bool    CloseSession(SID iSid, UInt32 iSvrId = 0);

	protected:
		//接收网关线程格式化数据
		virtual Bool    RecvThreadMsg(UInt32& iThread, ThreadMsgHeader& sHeader, OctetsStream* pOctets);		

		//向网关线程发送通知消息
		virtual Bool    SendThreadMsg(UInt32 iThread, const GateMsgHeader& sHeader, void* pData, Size_t iSize, const OctetsStream* pExtData = 0);

		//接收后端服务格式化数据
		virtual Bool    RecvServerMsg(UInt32& iSvrId, SvrMsgHeader& sHeader, OctetsStream* pOctets);

		//接收后端服务格式化数据(携带数据)
		virtual Bool    RecvServerExtData(OctetsStream* pOctets);

		//向后端服务发送通知消息
		virtual Bool    SendServerMsg(UInt32 iSvrId, const GateMsgHeader& sHeader, void* pData, Size_t iSize);		

	protected:
		//接收新连接
		virtual Bool    OnSessionAccept();

		//ThreadZmq可读事件(接收来自网关线程的数据)
		virtual Bool    OnGateThreadEvent();

		//接收客户端协议回调(由OnGateThreadEvent调用, 返回false直接会CloseSession)
		virtual Bool    OnSessionProtocol(SID iSid, const OctetsStream* pOctets);

		//ServerZmq可读事件
		virtual Bool    OnGateServerEvent();

		//MonitorZmq可读事件
		virtual Bool    OnGateMonitorEvent();

		//网关空闲状态处理
		virtual Bool    OnGateIdleEvent();

		//关闭服务(阻塞等待结束)
		virtual Bool    OnGatewayClose();		

	protected:
		//后端服务连接到网关
		virtual Bool    OnServerConnected(SOCKET hSocket);

		//后端服务和网关断开
		virtual Bool    OnServerDisConnect(SOCKET hSocket);

	protected:
		//注册连接数, 返回绑定服务器ID
		virtual Bool    RegConnection(const SocketAddr& sAddr, UInt32& iSvrId, Int32& iErrCode);

		//网关拒绝连接
		virtual Bool    OnGateRefuseConn(Int32 iReason, HawkSocket& sSocket, const SocketAddr* pAddr = 0);

		//会话挂载到服务器
		virtual Bool    OnSessionAttachServer(SID iSid, UInt32 iSvrId);

		//会话从服务器卸载
		virtual Bool    OnSessionDetachServer(SID iSid, UInt32 iSvrId);

	protected:		
		//事件线程
		HawkGateThread** m_ppThread;		
		//网关模式
		Int32			 m_iGateMode;
		//会话ID所绑定的服务ID
		SidBindMap		 m_mSidBind;
		//网关线程数
		Int32			 m_iThread;
		//轮流线程ID
		UInt32			 m_iTurnIdx;
		//基础会话ID
		UInt32			 m_iBaseSid;
		//会话Buffer大小
		Int32			 m_iBufSize;
		//会话超时时间
		Int32			 m_iTimeout;
		//消息数据Buffer
		OctetsStream*	 m_pOctets;
		//服务器套接字
		HawkSocket		 m_sSocket;
		//挂载的服务ID
		ServiceVec		 m_vService;
		//和网关线程通信的ZMQ
		HawkZmq*		 m_pThreadZmq;
		//和后端服务通信的ZMQ
		HawkZmq*		 m_pServerZmq;
		//后端服务ZMQ监视器
		HawkZmq*		 m_pMonitorZmq;
		//性能监视器
		HawkProfiler*	 m_pProfiler;
		//开启加解密模式
		Bool			 m_bSecurity;
		//循环空闲状态
		Bool			 m_bIdle;
		//等待直到空闲(平衡单类型任务处理时间)
		Bool			 m_bWaitIdle;
		//设置网关循环是否阻塞(默认阻塞)
		Bool			 m_bBlocking;
		//运行状态
		volatile Bool    m_bRunning;

	protected:
		//跨域服务线程
		HawkThread*		 m_pCrossDomain;
	};
}
#endif
