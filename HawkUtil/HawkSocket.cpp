#include "HawkSocket.h"
#include "HawkStringUtil.h"

namespace Hawk
{
	HawkSocket::HawkSocket(SOCKET hSocket)
	{
		m_hSocket    = hSocket;
		m_bBlocking  = true;
		m_iErrCode   = 0;
		m_iTransport = TYPE_UNKNOWN;
	}

	HawkSocket::~HawkSocket()
	{
	}	

	HawkSocket& HawkSocket::operator = (const HawkSocket& sSocket)
	{
		if (&sSocket != this)
		{
			m_hSocket	 = sSocket.m_hSocket;
			m_iTransport = sSocket.m_iTransport;
			m_bBlocking	 = sSocket.m_bBlocking;
			m_iErrCode	 = sSocket.m_iErrCode;
		}
		return *this;
	}

	Bool HawkSocket::Create(Int32 iAF, Int32 iType, Int32 iProtocol)
	{
		Reset();

		m_hSocket = socket(iAF,iType,iProtocol);

		if (m_hSocket == INVALID_SOCKET)
		{
			m_iErrCode = GetSocketErr();
			return false;
		}

		if (iType == SOCK_DGRAM)
			m_iTransport = TYPE_UDP;
		else
			m_iTransport = TYPE_TCP;

		return true;
	}

	Bool HawkSocket::Attach(SOCKET hSocket, Int32 iType, Bool bBlocking)
	{
		m_hSocket   = hSocket;
		m_bBlocking = bBlocking;

		if (iType == SOCK_DGRAM)
			m_iTransport = TYPE_UDP;
		else
			m_iTransport = TYPE_TCP;

		return m_hSocket != INVALID_SOCKET;
	}

	UInt8 HawkSocket::GetSocketType() const
	{
		if (IsTcpTransport())
			return TYPE_TCP;

		return TYPE_UDP;
	}

	Bool HawkSocket::IsUdpTransport() const
	{
		return m_iTransport == TYPE_UDP;
	}

	Bool HawkSocket::IsTcpTransport() const
	{
		return m_iTransport == TYPE_TCP;
	}

	Bool HawkSocket::Connect(const SocketAddr& sAddr, Int32 iTimeout)
	{
		if (!IsValid() || !sAddr.IsValid())
			return false;

		if (iTimeout <= 0)
		{
			Int32 iRet = connect(m_hSocket, sAddr.GetAddress(), sAddr.GetLength());
			if (iRet != HAWK_OK)
			{
				m_iErrCode = GetSocketErr();
				return false;
			}
		}
		else
		{
			Bool bSetBlocking = false;
			if (GetBlocking())
			{
				SetBlocking(false);
				bSetBlocking = true;
			}

			//连接超时错误处理
			if (connect(m_hSocket,sAddr.GetAddress(),sAddr.GetLength()) != HAWK_OK)
			{
				m_iErrCode = GetSocketErr();

				//发生错误
				if (!CHECK_ERROR_CONNECT_RETRIABLE(m_iErrCode))
				{
					if (bSetBlocking)
						SetBlocking(true);

					closesocket(m_hSocket);
					return false;
				}					
			}

			//连接成功等待套接字可写事件(避免出现ENOTCONN错误)
			//在非阻塞情况下的套接字连接之后,如果在可读事件之前投递Socket相关操作
			//可能引发10057的错误(未连接的套接字)
			if (!UpdateEvent(HEVENT_WRITE, iTimeout))
			{
				if (bSetBlocking)
					SetBlocking(true);

				closesocket(m_hSocket);
				return false;
			}

			if (bSetBlocking)
				SetBlocking(true);			
		}

		return true;
	}

	Bool HawkSocket::Bind(const SocketAddr& sAddr)
	{
		if (!IsValid() || !sAddr.IsValid())
			return false;

		if (bind(m_hSocket,sAddr.GetAddress(),sAddr.GetLength()) != HAWK_OK)
		{
			m_iErrCode = GetSocketErr();
			return false;
		}

		return true;
	}

	Bool HawkSocket::Listen(Int32 iBackLog)
	{
		if (!IsValid())
			return false;

		if (listen(m_hSocket,iBackLog) != HAWK_OK)
		{
			m_iErrCode = GetSocketErr();
			return false;
		}

		return true;
	}
	
	Bool HawkSocket::Accept(HawkSocket& sSocket, SocketAddr& sAddr)
	{
		if (!IsValid() || !sAddr.IsValid())
			return false;

		socklen_t iLen = sAddr.GetLength();
		SOCKET hSocket = accept(m_hSocket, const_cast<sockaddr*>(sAddr.GetAddress()), &iLen);
		if (hSocket == INVALID_SOCKET)
		{
			m_iErrCode = GetSocketErr();
			return false;			
		}

		sSocket.Attach(hSocket,true);
		return true;
	}

	Bool HawkSocket::Send(void* pData, Size_t& iSize, Int32 iFlags)
	{
		if (!IsValid())
			return false;

		if(!pData || !iSize)
			return true;

		iSize = send(m_hSocket, (Char*)pData, iSize, iFlags);
		if (iSize != Size_t(-1))
			return true;
		
		m_iErrCode = GetSocketErr();
		if (CHECK_ERROR_RW_RETRIABLE(m_iErrCode))
		{
			iSize = 0;
			return true;
		}

		return false;
	}

	Bool HawkSocket::Receive(void* pData, Size_t& iSize, Int32 iFlags)
	{
		if (!IsValid())
			return false;

		if(!pData || !iSize)
			return true;

		iSize = recv(m_hSocket,(Char*)pData,iSize,iFlags);
		
		//连接被关闭
		if (iSize == 0)
		{							
			m_iErrCode = GetSocketErr();
			return false;
		}
		//正常数据接收
		else if (iSize != Size_t(-1))
		{
			return true;
		}
		
		//发生错误
		m_iErrCode = GetSocketErr();
		if (CHECK_ERROR_RW_RETRIABLE(m_iErrCode))
		{				
			iSize = 0;
			return true;
		}
		
		return false;
	}

	Bool HawkSocket::SendTo(void* pData, Size_t& iSize, const SocketAddr& sAddr, Int32 iFlags)
	{
		if (!IsValid() || !sAddr.IsValid())
			return false;

		if(!pData || !iSize)
			return true;

		iSize = sendto(m_hSocket, (Char*)pData, iSize, iFlags, sAddr.GetAddress(), sAddr.GetLength());
		if (iSize != Size_t(-1))
			return true;
		
		m_iErrCode = GetSocketErr();
		if (CHECK_ERROR_RW_RETRIABLE(m_iErrCode))
		{
			iSize = 0;
			return true;
		}

		return false;
	}

	Bool HawkSocket::ReceiveFrom(void* pData, Size_t& iSize, SocketAddr& sAddr ,Int32 iFlags)
	{
		if (!IsValid() || !sAddr.IsValid())
			return false;

		if(!pData || !iSize)
			return true;

		socklen_t iAddrLen  = sAddr.GetLength();
		iSize = recvfrom(m_hSocket,(Char*)pData,iSize,iFlags,const_cast<sockaddr*>(sAddr.GetAddress()),&iAddrLen);

		//连接被关闭
		if (iSize == 0)
		{							
			m_iErrCode = GetSocketErr();
			return false;
		}		
		//正常接收数据
		else if (iSize != Size_t(-1))
		{
			return true;
		}	

		//发生错误
		m_iErrCode = GetSocketErr();
		if (CHECK_ERROR_RW_RETRIABLE(m_iErrCode))
		{				
			iSize = 0;
			return true;
		}

		return false;
	}

	Bool HawkSocket::SetOption(Int32 iOptName, const void* pOptVal, Int32 iOptLen, Int32 iLevel)
	{
		if (IsValid() && pOptVal && iOptLen > 0)
		{
			if (setsockopt(m_hSocket,iLevel,iOptName,(const char*)pOptVal,iOptLen) == HAWK_OK)
				return true;
		}
		return false;
	}

	Bool HawkSocket::GetOption(Int32 iOptName, void* pOptVal, Int32& iOptLen, Int32 iLevel)
	{
		if (IsValid() && pOptVal && iOptLen > 0)
		{
			socklen_t iLen = (socklen_t)iOptLen;
			if (getsockopt(m_hSocket,iLevel,iOptName,(char*)pOptVal,&iLen) == HAWK_OK)
			{
				iOptLen = iLen;
				return true;
			}
		}
		return false;
	}

	SOCKET HawkSocket::Handle() const
	{
		return m_hSocket;
	}

	Bool HawkSocket::IsValid() const
	{
		return m_hSocket != INVALID_SOCKET;
	}

	Bool HawkSocket::GetPeerName(SocketAddr& sAddr)
	{
		if (IsValid() && sAddr.IsValid())
		{
			socklen_t iAddrLen = sAddr.GetLength();
			if(getpeername(m_hSocket,const_cast<sockaddr*>(sAddr.GetAddress()),&iAddrLen) == HAWK_OK)
				return true;
		
			m_iErrCode = GetSocketErr();
		}
		return false;
	}

	Bool HawkSocket::GetSockName(SocketAddr& sAddr)
	{
		if (IsValid() && sAddr.IsValid())
		{
			socklen_t iAddrLen = sAddr.GetLength();
			if(getsockname(m_hSocket,const_cast<sockaddr*>(sAddr.GetAddress()),&iAddrLen) == HAWK_OK)
				return true;

			m_iErrCode = GetSocketErr();
		}
		return false;
	}

	Bool HawkSocket::Reset()
	{
		if(IsValid())
			return Close(true);
			
		return false;
	}

	Bool HawkSocket::Shutdown(Int32 iHow)
	{
		if(IsValid())
		{
			if (shutdown(m_hSocket,iHow) == HAWK_OK)
				return true;
		}
		return false;
	}

	Bool HawkSocket::Close(Bool bGraceful)
	{
		if(IsValid())
		{
			if (bGraceful)
				Shutdown(SD_BOTH);
			
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			return true;
		}
		return false;
	}

	Bool HawkSocket::SetLinger(Bool bOn, Int32 iSeconds)
	{
		struct linger xLin;
		xLin.l_onoff  = bOn ? 1 : 0;
		xLin.l_linger = iSeconds;
		return SetOption(SO_LINGER, &xLin, sizeof(xLin), SOL_SOCKET);
	}

	void HawkSocket::GetLinger(Bool& bOn, Int32& iSeconds)
	{
		struct linger xLin;
		Int32 iLen = sizeof(xLin);
		SetOption(SO_LINGER, &xLin, iLen,SOL_SOCKET);
		bOn      = xLin.l_onoff != 0;
		iSeconds = xLin.l_linger;
	}

	Bool HawkSocket::SetNoDelay(Bool bFlag)
	{
		Int32 iVal = bFlag ? 1 : 0;
		return SetOption(TCP_NODELAY, &iVal, sizeof(iVal), IPPROTO_TCP);
	}

	Bool HawkSocket::GetNoDelay()
	{
		Int32 iVal = 0;
		Int32 iLen = sizeof(iVal);
		GetOption(TCP_NODELAY,&iVal,iLen,IPPROTO_TCP);
		return iVal != 0;
	}

	void HawkSocket::SetSendBufferSize(Int32 iSize)
	{
		if(IsValid()) 
			SetOption(SO_SNDBUF,&iSize,sizeof(iSize),SOL_SOCKET);
	}

	Int32 HawkSocket::GetSendBufferSize()
	{
		Int32 iSize = 0;
		Int32 iLen  = sizeof(iSize);
		GetOption(SO_SNDBUF, &iSize, iLen, SOL_SOCKET);
		return iSize;
	}

	void HawkSocket::SetRecvBufferSize(Int32 iSize)
	{
		if(IsValid()) 
			SetOption(SO_RCVBUF,&iSize,sizeof(iSize),SOL_SOCKET);
	}

	Int32 HawkSocket::GetRecvBufferSize()
	{
		Int32 iSize = 0;
		Int32 iLen  = sizeof(iSize);
		GetOption(SO_RCVBUF, &iSize,iLen,SOL_SOCKET);
		return iSize;
	}

	Bool   HawkSocket::GetBlocking() const
	{
		return m_bBlocking;
	}

	Bool HawkSocket::SetBlocking(Bool bBlocking)
	{
#ifdef PLATFORM_WINDOWS
		Int32 iArg = bBlocking ? 0 : 1;
		if (ioctlsocket(m_hSocket, FIONBIO, reinterpret_cast<u_long*>(&iArg)) == HAWK_OK)
		{
			m_bBlocking = bBlocking;
			return true;
		}
#else
		Int32 iFlag = fcntl(m_hSocket, F_GETFL, 0);
		if (bBlocking)
			iFlag &= ~O_NONBLOCK;
		else 
			iFlag |= O_NONBLOCK;

		if (fcntl(m_hSocket, F_SETFL, iFlag) != HAWK_ERROR)
		{
			m_bBlocking = bBlocking;
			return true;
		}
#endif
		return false;
	}

	Int32 HawkSocket::UpdateEvent(UInt32 iMode, Int32 iTimeout)
	{
		if(!IsValid()) 
			return 0;

		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExcept;
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcept);

		if (iMode & HEVENT_READ)
		{
			FD_SET(m_hSocket, &fdRead);
		}

		if (iMode & HEVENT_WRITE)
		{
			FD_SET(m_hSocket, &fdWrite);
		}

		if (iMode & HEVENT_EXCEPT)
		{
			FD_SET(m_hSocket, &fdExcept);
		}

		struct timeval tv;
		tv.tv_sec  = iTimeout/1000;
		tv.tv_usec = (iTimeout % 1000) * 1000;

		Int32 iEventNum = select(m_hSocket + 1, &fdRead, &fdWrite, &fdExcept, iTimeout >= 0 ? &tv : 0);
		if(iEventNum > 0)
		{
			UInt32 iEvent = 0;
			if (FD_ISSET(m_hSocket,&fdRead))
				iEvent |= HEVENT_READ;

			if (FD_ISSET(m_hSocket,&fdWrite))
				iEvent |= HEVENT_WRITE;

			if (FD_ISSET(m_hSocket,&fdExcept))
				iEvent |= HEVENT_EXCEPT;

			return iEvent;
		} 
		return 0;
	}

	Int32 HawkSocket::GetSocketErr()
	{
#ifdef PLATFORM_WINDOWS
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	Bool HawkSocket::SetBlocking(SOCKET hSocket, Bool bBlocking)
	{
#ifdef PLATFORM_WINDOWS
		Int32 iArg = bBlocking ? 0 : 1;
		if (ioctlsocket(hSocket, FIONBIO, reinterpret_cast<u_long*>(&iArg)) == HAWK_OK)
			return true;
#else
		Int32 iFlag = fcntl(hSocket, F_GETFL, 0);
		if (bBlocking)
			iFlag &= ~O_NONBLOCK;
		else 
			iFlag |= O_NONBLOCK;

		if (fcntl(hSocket, F_SETFL, iFlag) != HAWK_ERROR)
			return true;
#endif
		return false;
	}

	Int32 HawkSocket::UpdateEvent(SOCKET hSocket, UInt32 iMode, Int32 iTimeout)
	{
		if(hSocket == INVALID_SOCKET) 
			return 0;

		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExcept;
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcept);

		if (iMode & HEVENT_READ)
		{
			FD_SET(hSocket, &fdRead);
		}

		if (iMode & HEVENT_WRITE)
		{
			FD_SET(hSocket, &fdWrite);
		}

		if (iMode & HEVENT_EXCEPT)
		{
			FD_SET(hSocket, &fdExcept);
		}

		struct timeval tv;
		tv.tv_sec  = iTimeout/1000;
		tv.tv_usec = (iTimeout % 1000) * 1000;

		Int32 iEventNum = select(hSocket + 1, &fdRead, &fdWrite, &fdExcept, iTimeout >= 0 ? &tv : 0);
		if(iEventNum > 0)
		{
			UInt32 iEvent = 0;
			if (FD_ISSET(hSocket,&fdRead))
				iEvent |= HEVENT_READ;

			if (FD_ISSET(hSocket,&fdWrite))
				iEvent |= HEVENT_WRITE;

			if (FD_ISSET(hSocket,&fdExcept))
				iEvent |= HEVENT_EXCEPT;

			return iEvent;
		} 
		return 0;
	}

	AString  HawkSocket::GetHostName()
	{
		Char sHostName[DEFAULT_SIZE] = {0};
		if (gethostname(sHostName, DEFAULT_SIZE) == HAWK_OK)
			return sHostName;

		return "";
	}

	AString  HawkSocket::GetHostIp()
	{
		Char sHostName[DEFAULT_SIZE] = {0};
		if (gethostname(sHostName, DEFAULT_SIZE) != HAWK_OK)
			return "";

		struct hostent* pHost = gethostbyname(sHostName);
		if (!pHost)
			return "";

		for (Int32 i=0; i<pHost->h_length; i++)
		{
			if (pHost->h_addr_list[i] == 0)
				continue;

			struct in_addr sAddr;
			memcpy(&sAddr, pHost->h_addr_list[i], sizeof(sAddr));
			AString sIp = inet_ntoa(sAddr);
			HawkStringUtil::Trim(sIp);
			if (sIp.size() && sIp != "127.0.0.1")
				return sIp;
		}

		return "127.0.0.1";
	}

	Bool  HawkSocket::InitTcpServer(const SocketAddr& sBindAddr, Bool bNoBlocking)
	{
		if(!Create()) 
			return false;

		if(bNoBlocking)
			SetBlocking(false);

		SetNoDelay(true);

		struct linger sLing = {0, 0};
		Int32  iFlags = 1;

		SetOption(SO_REUSEADDR,(const void *)&iFlags, sizeof(iFlags));
		SetOption(SO_KEEPALIVE,(const void *)&iFlags, sizeof(iFlags));
		SetOption(SO_LINGER,   (const void *)&sLing,  sizeof(sLing));

		if(!Bind(sBindAddr))
			return false;

		if(!Listen()) 
			return false;

		return IsValid();
	}

	Bool  HawkSocket::InitTcpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking, Int32 iConnTimeout)
	{
		if (!Create())
			return false;

		SetNoDelay(true);

		if(bNoBlocking)
			SetBlocking(false);

		if(!Connect(sSvrAddr, iConnTimeout))
			return false;

		return IsValid();
	}

	Bool  HawkSocket::InitTcpPeer(Bool bNoBlocking)
	{
		SetNoDelay(true);

		if(bNoBlocking)
			SetBlocking(false);

		return IsValid();
	}

	Bool  HawkSocket::InitUdpServer(const SocketAddr& sBindAddr, Bool bNoBlocking)
	{
		if(!Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
			return false;

		SetNoDelay(true);

		if(bNoBlocking)
			SetBlocking(false);

		Int32  iFlags = 1;
		SetOption(SO_REUSEADDR,(const void *)&iFlags, sizeof(iFlags));

		if(!Bind(sBindAddr))
			return false;

		return IsValid();
	}

	Bool  HawkSocket::InitUdpClient(const SocketAddr& sSvrAddr, Bool bNoBlocking, Bool bBindClient)
	{
		if(!Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
			return false;

		SetNoDelay(true);

		if(bNoBlocking)
			SetBlocking(false);

		if (bBindClient)
		{
			sockaddr_in sAddr;
			sAddr.sin_family = AF_INET;
			sAddr.sin_addr.s_addr = htons(INADDR_ANY);
			sAddr.sin_port = htons(0);

			SocketAddr sClientAddr((sockaddr*)&sAddr,sizeof(sAddr));
			if(!Bind(sClientAddr))
				return false;
		}

		return IsValid();
	}

	Int32 HawkSocket::MaximizeSndBuf(SOCKET hSocket)
	{
		socklen_t iIntSize = sizeof(Int32);
		Int32 iLast = 0, iMin = 0, iMax = 0, iAvg = 0, iOld = 0;

		if (getsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (Char*)&iOld, &iIntSize) != 0) 
			return 0;

		iLast = iOld;
		iMin  = iOld;
		iMax  = MAX_SENDBUF_SIZE;

		while (iMin <= iMax) 
		{
			iAvg = ((UInt32)(iMin + iMax)) / 2;
			if (setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (Char*)&iAvg, iIntSize) == 0) 
			{
				iLast = iAvg;
				iMin  = iAvg + 1;
			} 
			else 
			{
				iMax = iAvg - 1;
			}
		}
		return iLast;
	}

	Int32 HawkSocket::MaximizeRcvBuf(SOCKET hSocket)
	{
		socklen_t iIntSize = sizeof(Int32);
		Int32 iLast = 0, iMin = 0, iMax = 0, iAvg = 0, iOld = 0;

		if (getsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (Char*)&iOld, &iIntSize) != 0) 
			return 0;

		iLast = iOld;
		iMin  = iOld;
		iMax  = MAX_SENDBUF_SIZE;

		while (iMin <= iMax) 
		{
			iAvg = ((UInt32)(iMin + iMax)) / 2;
			if (setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (Char*)&iAvg, iIntSize) == 0) 
			{
				iLast = iAvg;
				iMin = iAvg + 1;
			} 
			else 
			{
				iMax = iAvg - 1;
			}
		}
		return iLast;
	}

	UInt32 HawkSocket::Htonl(UInt32 iVal)
	{
		return htonl(iVal);
	}

	UInt16  HawkSocket::Htons(UInt16 iVal)
	{
		return htons(iVal);
	}

	UInt32 HawkSocket::Ntohl(UInt32 iVal)
	{
		return ntohl(iVal);
	}

	UInt16 HawkSocket::Ntohs(UInt16 iVal)
	{
		return ntohs(iVal);
	}

	UInt32 HawkSocket::InetAddr(const AString& sAddr)
	{
		return inet_addr(sAddr.c_str());
	}

	AString HawkSocket::InetNtoa(const in_addr& sAddr)
	{
		return inet_ntoa(sAddr);
	}
}
