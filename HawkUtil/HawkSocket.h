#ifndef HAWK_SOCKET_H
#define HAWK_SOCKET_H

#include "HawkSocketAddr.h"

namespace Hawk
{
	/************************************************************************/
	/* Socket��װ                                                           */
	/************************************************************************/
	class UTIL_API HawkSocket : public HawkRefCounter
	{		
	public:		
		//����
		HawkSocket(SOCKET hSocket = INVALID_SOCKET);
		
		//����
		virtual ~HawkSocket();

		//��ֵ������
		HawkSocket& operator = (const HawkSocket& sSocket);

		//�׽�������
		enum
		{
			TYPE_UNKNOWN = 0,
			TYPE_TCP,
			TYPE_UDP,
		};
	public:
		//�����׽���
		virtual Bool	Create(Int32 iAF = AF_INET, Int32 iType = SOCK_STREAM, Int32 iProtocol = IPPROTO_TCP);

		//�������׽���
		virtual Bool    Attach(SOCKET hSocket, Int32 iType = SOCK_STREAM, Bool bBlocking = true);

		//��������
		virtual Bool	Connect(const SocketAddr& sAddr, Int32 iTimeout = DEFAULT_TIMEOUT);

		//��
		virtual Bool	Bind(const SocketAddr& sAddr);
		
		//����
		virtual Bool	Listen(Int32 iBackLog = SOMAXCONN);

		//��������
		virtual Bool	Accept(HawkSocket& sSocket, SocketAddr& sAddr);

		//���ò���
		virtual Bool    SetOption(Int32 iOptName, const void* pOptVal, Int32 iOptLen, Int32 iLevel = SOL_SOCKET);

		//��ȡ����
		virtual Bool    GetOption(Int32 iOptName, void* pOptVal, Int32& iOptLen, Int32 iLevel = SOL_SOCKET);

		//��þ��
		virtual SOCKET	Handle() const;

		//���ĳ���ź�,ModeΪSelectMode�����ֵ,�����źż���
		virtual Int32   UpdateEvent(UInt32 iMode, Int32 iTimeout = 0);

		//���÷��ͻ�������С
		virtual void	SetSendBufferSize(Int32 iSize);

		//��ȡ���ͻ�������С
		virtual Int32	GetSendBufferSize();

		//���ý��ջ����С
		virtual void	SetRecvBufferSize(Int32 iSize);

		//��ȡ���ջ�������С
		virtual Int32	GetRecvBufferSize();

		//������ӶԵȷ�����
		virtual Bool	GetPeerName(SocketAddr& sAddr);

		//��õ�ǰ�׽�������
		virtual Bool	GetSockName(SocketAddr& sAddr);
		
		//׼���ر��׽���,SD_SEND,SD_RECEIVE,SD_BOTH
		virtual Bool	Shutdown(Int32 iHow = SD_BOTH);
		
		//�ر�(�Ƿ���shutdown)
		virtual Bool	Close(Bool bGraceful = true);
		
		//����
		virtual Bool	Reset();

		//�Ƿ���Ч
		virtual Bool	IsValid() const;
		
		//SO_LINGER���
		virtual Bool	SetLinger(Bool bOn, Int32 iSeconds);
		
		//SO_LINGER
		virtual void	GetLinger(Bool& bOn, Int32& iSeconds);
		
		//����TCP_NODELAY��ʱ���
		virtual Bool	SetNoDelay(Bool bFlag);
		
		//��ȡ�����ʱ���
		virtual Bool	GetNoDelay();
		
		//����FIONBIO����״̬
		virtual Bool	SetBlocking(Bool bBlocking);

		//��ȡ����״̬
		virtual Bool	GetBlocking() const;	

		//��ȡ�׽�������
		virtual UInt8   GetSocketType() const;

		//�ж�UDP����
		virtual Bool    IsUdpTransport() const;

		//�ж�TCP
		virtual Bool    IsTcpTransport() const;

	public:
		//����TCP����
		virtual Bool	Send(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//����TCP����
		virtual Bool	Receive(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//����UDP����
		virtual Bool	SendTo(void* pData, Size_t& iSize, const SocketAddr& sAddr, Int32 iFlags = 0);

		//����UDP����
		virtual Bool	ReceiveFrom(void* pData, Size_t& iSize, SocketAddr& sAddr, Int32 iFlags = 0);
	
	public:
		//��ʼ��������TCP������IO
		virtual Bool	InitTcpServer(const SocketAddr& sBindAddr, Bool bNoBlocking = true);

		//��ʼ���ͻ���TCP����IO
		virtual Bool	InitTcpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking = true, Int32 iConnTimeout = DEFAULT_TIMEOUT);

		//��ʼ��TCP���������ն�����IO
		virtual Bool	InitTcpPeer(Bool bNoBlocking = true);

		//��ʼ��������UDP����IO
		virtual Bool	InitUdpServer(const SocketAddr& sBindAddr, Bool bNoBlocking = true);

		//��ʼ���ͻ���UDP����IO
		virtual Bool	InitUdpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking = true, Bool bBindClient = false);

	public:
		//���÷�����
		static Bool     SetBlocking(SOCKET hSocket, Bool bBlocking);

		//����¼�
		static Int32    UpdateEvent(SOCKET hSocket, UInt32 iMode, Int32 iTimeout);

		//����Socket����ͻ�����
		static Int32    MaximizeSndBuf(SOCKET hSocket);
		
		//����Socket����ͻ�����
		static Int32    MaximizeRcvBuf(SOCKET hSocket);

		//��������Ĵ�����
		static Int32	GetSocketErr();

		//��ȡ���ػ�����
		static AString  GetHostName();

		//��ȡ����IP
		static AString  GetHostIp();

	public:
		//��һ��32λ���������ֽ�˳��ת��Ϊ�����ֽ�˳��
		static UInt32   Htonl(UInt32 iVal);

		//��һ��16λ���������ֽ�˳��ת��Ϊ�����ֽ�˳��
		static UInt16   Htons(UInt16 iVal);		

		//��һ��32λ���������ֽ�˳��ת��Ϊ�����ֽ�˳��
		static UInt32	Ntohl (UInt32 iVal);

		//��һ��16λ���������ֽ�˳��ת��Ϊ�����ֽ�˳��
		static UInt16	Ntohs (UInt16 iVal);

		//��һ�������ʱ�׼��ֱ�ʾ����ʾ���ַ�����ַת�������ʵ�ֵַ
		static UInt32	InetAddr(const AString& sAddr);

		//��һ�����ʵ�ֵַת����һ���õ��ʮ���Ʊ�ʾ����ʾ���ַ�����ַ
		static AString  InetNtoa (const in_addr& sAddr);

	protected:
		//Socket���
		SOCKET	m_hSocket;
		//����
		UInt32	m_iTransport;
		//����������
		Int32	m_iErrCode;
		//�������
		Bool    m_bBlocking;
	};	
}
#endif
