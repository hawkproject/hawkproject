#ifndef HAWK_GATETHREAD_H
#define HAWK_GATETHREAD_H

#include "HawkGateDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 网关服务线程(基于libevent的边缘事件触发模型)                         */
	/************************************************************************/
	class HawkGateway;
	class GATE_API HawkGateThread : public HawkRefCounter
	{
	public:
		//构造
		HawkGateThread(HawkGateway* pGateway = 0);

		//析构
		virtual ~HawkGateThread();

		friend class HawkGateway;

	public:
		//会话状态
		enum
		{
			SESSION_STATE_CLOSED = 0,
			SESSION_STATE_ACTIVE,
			SESSION_STATE_CLOSING,
		};

		//自定义错误码
		enum
		{
			SE_NONE = 100,
			SE_BEV_WRITE,
			SE_BEV_LENGTH,
			SE_BEV_REMOVE,			
			SE_BUF_FULL,
			SE_DECIPHER,
			SE_DECODE,
			SE_SHUTDOWN,
			SE_SEL_SERVER,
		};

		//协议CRC存根
		typedef map<ProtoType, UInt32> ProtoCrcMap;

		//会话信息定义
		struct Session
		{
			//会话ID
			SID				Sid;
			//套接字
			SOCKET			Socket;
			//绑定服务器ID
			UInt32			SvrId;
			//状态
			UInt32			State;
			//协议频率
			UInt32			ProtoFreq;
			//频率统计开始时间
			UInt32			FreqTime;
			//地址信息
			SocketAddr*		Addr;
			//会话读事件
			void*			Event;
			//网关信任标记
			Bool			Trust;
			//所属线程对象
			HawkGateThread* GThread;
			//输入缓冲区
			OctetsStream*	IBuffer;
			//加密对象
			HawkSecurity*	ISecurity;
			//解密对象
			HawkSecurity*	OSecurity;
			//状态协议存根
			ProtoCrcMap*	ProtoTypeCrc;
		};		

		//会话列表定义
		typedef map<SID, Session*>		  SessionMap;
		//合法协议表
		typedef map<ProtoType, UInt8>	  ProtoTypeDriveMap;

	public:
		//初始化
		virtual Bool  Init(UInt32 iIdentify, UInt32 iBaseSid, UInt32 iSidStep);

		//运行
		virtual Bool  Start();			

		//关闭
		virtual Bool  Close();

		//获取线程ID
		virtual Int32 GetThreadId() const;

	protected:
		//分配会话
		Session*	  AllocSession(SOCKET hSocket, const SocketAddr& sAddr);

		//缓存会话
		virtual Bool  FreeSession(Session* pSession);		

		//释放会话列表
		virtual Bool  FreeSessionMap();

		//生成会话ID
		virtual SID   GenSessionId();

		//开启连接会话
		virtual Bool  StartSession(SOCKET hSocket, const SocketAddr& sAddr, Session** ppSession = 0);

		//关闭会话
		virtual Bool  CloseSession(SID iSid, Int32 iErrCode = 0);

		//接收网关数据消息
		virtual Bool  RecvGateMsg(GateMsgHeader& sHeader, OctetsStream* pOctets);

		//接收网关数据消息(携带数据
		virtual Bool  RecvGateExtData(OctetsStream* pOctets);

		//给网关发送数据消息
		virtual Bool  SendGateMsg(const ThreadMsgHeader& sHeader, void* pData, Size_t iSize);		

		//会话发送协议
		virtual Bool  SendProtocol(Session* pSession, Protocol* pProto);

		//加码输入缓冲区为协议
		virtual Bool  DecodeIBuffer(Session* pSession);

	public:
		//线程执行函数, 线程函数调用
		virtual Bool  OnThreadLoop();

		//会话事件, iErrCode 在 HEVENT_EXCEPT 事件下有效
		virtual Bool  OnSessionEvent(UInt32 iEvent, Session* pSession, Int32 iErrCode = 0);

	protected:
		//消息队列事件
		virtual Bool  OnGatewayEvent();

		//空闲状态
		virtual Bool  OnThreadIdle();

		//会话数据可读回调
		virtual Bool  OnSessionRead(Session* pSession);
		
		//会话数据可写回调
		virtual Bool  OnSessionWrite(Session* pSession);

		//会话发生错误回调
		virtual Bool  OnSessionError(Session* pSession, Int32 iErrCode);

	protected:
		//会话输入缓冲区可解码判断
		virtual Bool  OnSessionDecode(Session* pSession, OctetsStream* pBuffer);

		//会话输出缓冲区可加码判断
		virtual Bool  OnSessionEncode(Session* pSession, OctetsStream* pBuffer);

		//内部协议处理
		virtual Bool  OnSysProtocol(Session* pSession, ProtoType iProtoType);

	protected:
		//事件处理线程
		HawkThread*			m_pThread;
		//事件基础对象
		void*				m_pBase;
		//消息队列
		HawkZmq*			m_pZmq;
		//消息数据Buffer
		OctetsStream*		m_pOctets;
		//基础会话ID
		UInt32				m_iBaseSid;
		//会话ID递进步长
		UInt32				m_iSidStep;
		//当前会话索引
		UInt32				m_iCurSid;
		//当前会话列表
		SessionMap			m_mSession;
		//所属网关应用
		HawkGateway*		m_pGateway;
		//空闲状态
		Bool				m_bIdle;
		//运行中状态
		volatile Bool		m_bRunning;
		//合法协议ID列表
		ProtoTypeDriveMap	m_mProtoTypeDrive;
	};
}
#endif
