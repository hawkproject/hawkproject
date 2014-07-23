#ifndef HAWK_SOCKET_H
#define HAWK_SOCKET_H

#include "HawkSocketAddr.h"

namespace Hawk
{
	/************************************************************************/
	/* Socket封装                                                           */
	/************************************************************************/
	class UTIL_API HawkSocket : public HawkRefCounter
	{		
	public:		
		//构造
		HawkSocket(SOCKET hSocket = INVALID_SOCKET);
		
		//析构
		virtual ~HawkSocket();

		//赋值操作符
		HawkSocket& operator = (const HawkSocket& sSocket);

		//套接字类型
		enum
		{
			TYPE_UNKNOWN = 0,
			TYPE_TCP,
			TYPE_UDP,
		};
	public:
		//创建套接字
		virtual Bool	Create(Int32 iAF = AF_INET, Int32 iType = SOCK_STREAM, Int32 iProtocol = IPPROTO_TCP);

		//连接上套接字
		virtual Bool    Attach(SOCKET hSocket, Int32 iType = SOCK_STREAM, Bool bBlocking = true);

		//连接主机
		virtual Bool	Connect(const SocketAddr& sAddr, Int32 iTimeout = DEFAULT_TIMEOUT);

		//绑定
		virtual Bool	Bind(const SocketAddr& sAddr);
		
		//监听
		virtual Bool	Listen(Int32 iBackLog = SOMAXCONN);

		//接受连接
		virtual Bool	Accept(HawkSocket& sSocket, SocketAddr& sAddr);

		//设置参数
		virtual Bool    SetOption(Int32 iOptName, const void* pOptVal, Int32 iOptLen, Int32 iLevel = SOL_SOCKET);

		//获取参数
		virtual Bool    GetOption(Int32 iOptName, void* pOptVal, Int32& iOptLen, Int32 iLevel = SOL_SOCKET);

		//获得句柄
		virtual SOCKET	Handle() const;

		//检测某个信号,Mode为SelectMode的组合值,返回信号集合
		virtual Int32   UpdateEvent(UInt32 iMode, Int32 iTimeout = 0);

		//设置发送缓冲区大小
		virtual void	SetSendBufferSize(Int32 iSize);

		//获取发送缓冲区大小
		virtual Int32	GetSendBufferSize();

		//设置接收缓冲大小
		virtual void	SetRecvBufferSize(Int32 iSize);

		//获取接收缓冲区大小
		virtual Int32	GetRecvBufferSize();

		//获得连接对等方名称
		virtual Bool	GetPeerName(SocketAddr& sAddr);

		//获得当前套接字名称
		virtual Bool	GetSockName(SocketAddr& sAddr);
		
		//准备关闭套接字,SD_SEND,SD_RECEIVE,SD_BOTH
		virtual Bool	Shutdown(Int32 iHow = SD_BOTH);
		
		//关闭(是否先shutdown)
		virtual Bool	Close(Bool bGraceful = true);
		
		//重置
		virtual Bool	Reset();

		//是否有效
		virtual Bool	IsValid() const;
		
		//SO_LINGER标记
		virtual Bool	SetLinger(Bool bOn, Int32 iSeconds);
		
		//SO_LINGER
		virtual void	GetLinger(Bool& bOn, Int32& iSeconds);
		
		//设置TCP_NODELAY延时标记
		virtual Bool	SetNoDelay(Bool bFlag);
		
		//获取打包延时标记
		virtual Bool	GetNoDelay();
		
		//设置FIONBIO阻塞状态
		virtual Bool	SetBlocking(Bool bBlocking);

		//获取阻塞状态
		virtual Bool	GetBlocking() const;	

		//获取套接字类型
		virtual UInt8   GetSocketType() const;

		//判断UDP类型
		virtual Bool    IsUdpTransport() const;

		//判断TCP
		virtual Bool    IsTcpTransport() const;

	public:
		//发送TCP数据
		virtual Bool	Send(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//接收TCP数据
		virtual Bool	Receive(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//发送UDP数据
		virtual Bool	SendTo(void* pData, Size_t& iSize, const SocketAddr& sAddr, Int32 iFlags = 0);

		//接收UDP数据
		virtual Bool	ReceiveFrom(void* pData, Size_t& iSize, SocketAddr& sAddr, Int32 iFlags = 0);
	
	public:
		//初始化服务器TCP的网络IO
		virtual Bool	InitTcpServer(const SocketAddr& sBindAddr, Bool bNoBlocking = true);

		//初始化客户端TCP网络IO
		virtual Bool	InitTcpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking = true, Int32 iConnTimeout = DEFAULT_TIMEOUT);

		//初始化TCP服务器接收端网络IO
		virtual Bool	InitTcpPeer(Bool bNoBlocking = true);

		//初始化服务器UDP网络IO
		virtual Bool	InitUdpServer(const SocketAddr& sBindAddr, Bool bNoBlocking = true);

		//初始化客户端UDP网络IO
		virtual Bool	InitUdpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking = true, Bool bBindClient = false);

	public:
		//设置非阻塞
		static Bool     SetBlocking(SOCKET hSocket, Bool bBlocking);

		//检测事件
		static Int32    UpdateEvent(SOCKET hSocket, UInt32 iMode, Int32 iTimeout);

		//设置Socket最大发送缓冲区
		static Int32    MaximizeSndBuf(SOCKET hSocket);
		
		//设置Socket最大发送缓冲区
		static Int32    MaximizeRcvBuf(SOCKET hSocket);

		//操作错误的错误码
		static Int32	GetSocketErr();

		//获取本地机器名
		static AString  GetHostName();

		//获取本机IP
		static AString  GetHostIp();

	public:
		//将一个32位数从主机字节顺序转换为网络字节顺序
		static UInt32   Htonl(UInt32 iVal);

		//将一个16位数从主机字节顺序转换为网络字节顺序
		static UInt16   Htons(UInt16 iVal);		

		//将一个32位数从网络字节顺序转换为主机字节顺序
		static UInt32	Ntohl (UInt32 iVal);

		//将一个16位数从网络字节顺序转换为主机字节顺序
		static UInt16	Ntohs (UInt16 iVal);

		//将一个用网际标准点分表示法表示的字符串地址转换成网际地址值
		static UInt32	InetAddr(const AString& sAddr);

		//将一个网际地址值转换成一个用点分十进制表示法表示的字符串地址
		static AString  InetNtoa (const in_addr& sAddr);

	protected:
		//Socket句柄
		SOCKET	m_hSocket;
		//类型
		UInt32	m_iTransport;
		//操作错误码
		Int32	m_iErrCode;
		//阻塞标记
		Bool    m_bBlocking;
	};	
}
#endif
