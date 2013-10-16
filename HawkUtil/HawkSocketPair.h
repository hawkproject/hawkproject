#ifndef HAWK_SOCKETPAIR_H
#define HAWK_SOCKETPAIR_H

#include "HawkSocket.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* Socket对操作封装,主要用来线程间信息通知                              */
	/************************************************************************/
	class UTIL_API HawkSocketPair : public HawkRefCounter
	{		
	public:		
		//构造
		HawkSocketPair();

		//析构
		virtual ~HawkSocketPair();

	public:
		//创建套接字对
		virtual Bool	Create(Bool bThreadSafe = false);

		//是否有效
		virtual Bool    IsValid() const;

		//发送消息, 注: (Send堆积过多 (Recv不及时) 会造成阻塞
		virtual Bool	Send(const void* pData, Size_t& iSize, Int32 iFlags = 0);

		//接收消息
		virtual Bool	Receive(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//清理未读事件
		virtual Bool    Clear();

		//关闭
		virtual Bool    Close();

	public:
		//获取写FD
		virtual SOCKET	GetWriteFd() const;

		//获取读FD
		virtual SOCKET  GetReadFd() const;

	protected:
		//Socket句柄
		SOCKET		m_hSocket[2];
		//发送端的线程安全锁
		HawkMutex*  m_pMutex;
		//发送量
		UInt64		m_iSend;
		//接收量
		UInt64		m_iRecv;
	};	

	////协议类型简便定义
	typedef HawkSocketPair SocketPair;
}
#endif
