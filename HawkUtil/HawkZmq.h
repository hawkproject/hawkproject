#ifndef HAWK_ZMP_H
#define HAWK_ZMP_H

#include "HawkObjBase.h"
#include "HawkProtocolManager.h"

namespace Hawk
{
	/************************************************************************/
	/* 0MQ分布式消息队列操作封装                                            */
	/************************************************************************/
	class UTIL_API HawkZmq : public HawkRefCounter
	{
	protected:
		//构造
		HawkZmq();

		//析构
		virtual ~HawkZmq();

		//友员什么便于manager的Init调用
		friend class HawkZmqManager;

	public:
		//ZMQ通讯类型
		enum
		{
			HZMQ_PAIR	= 0,
			HZMQ_PUB	= 1,
			HZMQ_SUB	= 2,
			HZMQ_REQ	= 3,
			HZMQ_REP	= 4,
			HZMQ_DEALER	= 5,
			HZMQ_ROUTER	= 6,
			HZMQ_PULL	= 7,
			HZMQ_PUSH	= 8,
			HZMQ_XPUB	= 9,
			HZMQ_XSUB	= 10,
			HZMQ_STREAM = 11,
		};

		//Send和Recv的Option参数
		enum
		{
			HZMQ_DONTWAIT = 1,
			HZMQ_SNDMORE  = 2,
			HZMQ_RCVMORE  = 13,
		};

		//超时常量设置
		enum
		{
			HZMQ_TIMEOUT = 100,
		};

		//ZMQ监视器事件
		enum
		{
			HZMQ_EVENT_CONNECTED		= 0x0001,
			HZMQ_EVENT_CONNECT_DELAYED	= 0x0002,
			HZMQ_EVENT_CONNECT_RETRIED	= 0x0004,

			HZMQ_EVENT_LISTENING		= 0x0008,
			HZMQ_EVENT_BIND_FAILED		= 0x0010,

			HZMQ_EVENT_ACCEPTED			= 0x0020,
			HZMQ_EVENT_ACCEPT_FAILED	= 0x0040,

			HZMQ_EVENT_CLOSED			= 0x0080,
			HZMQ_EVENT_CLOSE_FAILED		= 0x0100,
			HZMQ_EVENT_DISCONNECTED		= 0x0200,
			HZMQ_EVENT_MONITOR_STOPPED  = 0x0400,

			HZMQ_EVENT_ALL				= (HZMQ_EVENT_CONNECTED | HZMQ_EVENT_CONNECT_DELAYED |
										  HZMQ_EVENT_CONNECT_RETRIED | HZMQ_EVENT_LISTENING |
										  HZMQ_EVENT_BIND_FAILED | HZMQ_EVENT_ACCEPTED |
										  HZMQ_EVENT_ACCEPT_FAILED | HZMQ_EVENT_CLOSED |
										  HZMQ_EVENT_CLOSE_FAILED | HZMQ_EVENT_DISCONNECTED |
										  HZMQ_EVENT_MONITOR_STOPPED),
		};

	public:
		//Svr模式绑定地址
		virtual Bool   Bind(const AString& sAddr);

		//Clt模式连接
		virtual Bool   Connect(const AString& sAddr);

		//发送数据(基于消息发送)
		virtual Bool   Send(void* pBuf, Size_t iSize, Int32 iFlag = 0);

		//接收数据(基于消息接收)
		virtual Bool   Recv(void* pBuf, Size_t& iSize, Int32 iFlag = 0);	

		//发送数据(基于协议发送)
		virtual Bool   SendProtocol(HawkProtocol* pProto, Int32 iFlag = 0);

		//接收数据(基于协议接收)
		virtual Bool   RecvProtocol(HawkProtocol*& pProto, Int32 iFlag = 0);

		//事件检测
		virtual UInt32 PollEvent(UInt32 iEvents = HEVENT_READ, Int32 iTimeout = -1);	
		
		//开启事件监视器
		virtual Bool   StartMonitor(UInt32 iEvents, const AString& sAddr = "");

		//更新监视器事件
		virtual UInt32 UpdateMonitor(OctetsStream* pData = 0);

		//关闭事件监视器
		virtual Bool   StopMonitor();

		//丢弃消息数据
		virtual Bool   DiscardMsg();

	public:
		//获取句柄
		virtual void*  GetHandle();

		//获取类型
		virtual Int32  GetType() const;

		//错误码
		virtual Int32  GetErrCode() const;

		//有更多数据要读取
		virtual Bool   IsWaitRecv() const;		

		//设置IDENTITY属性
		virtual Bool   SetIdentity(const void* pOptVal, Int32 iSize);	

		//设置ZMQ参数
		virtual Bool   SetOption(Int32 iOption, const void* pOptVal, Size_t iSize);

		//获取ZMQ参数
		virtual Bool   GetOption(Int32 iOption, void* pOptVal, Size_t& iSize);

	protected:
		//初始化ZMQ句柄
		virtual Bool   Init(Int32 iType);	

		//填充错误信息
		virtual Bool   FillErr();

		//关闭队列
		virtual Bool   Close();

	public:
		//生成有效的唯一标识(低32位为1, 保证首字节不为0)
		static UInt64  GenValidZid(UInt32 iTid);

	protected:
		void*			m_pHandle;
		void*			m_pMonitor;
		Int32			m_iType;			
		Int32			m_iErrCode;
		AString			m_sErrMsg;
		Int32			m_iRecvMore;
		OctetsStream*	m_pRecvBuf;

	protected:
#ifdef _DEBUG
		Size_t			m_iLastSend;
		Size_t			m_iLastRecv;
#endif
	};
}
#endif
