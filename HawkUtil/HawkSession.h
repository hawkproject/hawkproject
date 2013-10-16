#ifndef HAWK_SESSION_H
#define HAWK_SESSION_H

#include "HawkSecurity.h"
#include "HawkSocket.h"
#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* 网络会话对象封装                                                     */
	/************************************************************************/
	class UTIL_API HawkSession : public HawkRefCounter
	{
	public:
		//构造
		HawkSession();
		
		//析构
		virtual ~HawkSession();

	public:
		//初始化
		virtual Bool	Init(const SocketAddr& sAddr, Int32 iTimeout = DEFAULT_TIMEOUT);		

		//更新会话,读取网络IO数据(返回false表示网络断开)
		virtual Bool    Tick(UInt32 iPeriod);

		//从输入缓冲区中解析出协议
		virtual Bool    DecodeProtocol(Protocol*& pProto);

		//断线重连(需要重新设置加解密组件)
		virtual Bool    Reconnect();

		//关闭会话
		virtual Bool	Close();

	public:
		//设置读入加密组件
		virtual void	SetISecurity(HawkSecurity* pSecurity);

		//设置写出加密组件
		virtual void	SetOSecurity(HawkSecurity* pSecurity);

		//发送原始数据
		virtual Bool	SendRawData(void* pData, Int32 iSize, Bool bFlush = true);

		//发送协议
		virtual Bool	SendProtocol(Protocol* pProto, Bool bFlush = true);	

		//写出发送缓冲区(帧结束时调用)
		virtual Bool	FlushOutput();

		//获取输入Buffer
		OctetsStream*	GetInputBuf();

		//获取会话Socket
		HawkSocket*		GetSocket();

		//获取对端地址
		SocketAddr*		GetAddr();

	protected:
		//是否有效
		virtual Bool    IsValid() const;

	protected:
		//会话套接字
		HawkSocket		m_sSocket;
		//地址
		SocketAddr		m_sAddress;	
		//加密对象
		HawkSecurity*	m_pISecurity;
		//解密对象
		HawkSecurity*	m_pOSecurity;
		//输入缓冲区
		OctetsStream*	m_pInputBuf;
		//输出缓冲区
		OctetsStream*	m_pOutputBuf;
		//临时缓冲区
		OctetsStream*	m_pCommonBuf;
	};
}
#endif
