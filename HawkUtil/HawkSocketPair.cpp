#include "HawkSocketPair.h"

namespace Hawk
{
#ifdef PLATFORM_WINDOWS
	Int32 win32_socketpair(Int32 iAF, Int32 iType,SOCKET hSocket[2])
	{
		struct sockaddr_in listen_addr;
		struct sockaddr_in connect_addr;

		SOCKET hListener = socket(iAF,iType,0);
		if (hListener == INVALID_SOCKET)
		{
			return HAWK_ERROR;
		}

		memset(&listen_addr, 0, sizeof(listen_addr));
		listen_addr.sin_family		= iAF;
		listen_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		listen_addr.sin_port		= 0;
		if(::bind(hListener,(struct sockaddr*)&listen_addr, sizeof(listen_addr)) != HAWK_OK)
		{
			closesocket(hListener);
			return HAWK_ERROR;
		}

		if(listen(hListener,SOMAXCONN) != HAWK_OK)
		{
			closesocket(hListener);
			return HAWK_ERROR;
		}

		Int32 iSize = sizeof(connect_addr);
		if (getsockname(hListener, (struct sockaddr*) &connect_addr, &iSize) != HAWK_OK || iSize != sizeof(connect_addr))
		{
			closesocket(hListener);
			return HAWK_ERROR;
		}

		SOCKET hClient = socket(iAF,SOCK_STREAM,0);
		if (hClient == INVALID_SOCKET)
		{
			closesocket(hListener);
			return HAWK_ERROR;
		}

		if (connect(hClient,(struct sockaddr*)&connect_addr, sizeof(connect_addr)) != HAWK_OK)
		{
			closesocket(hClient);
			closesocket(hListener);
			return HAWK_ERROR;
		}

		SOCKET hServer = accept(hListener,(struct sockaddr*)&listen_addr, &iSize);
		if (hServer == INVALID_SOCKET || iSize != sizeof(listen_addr))
		{
			closesocket(hClient);
			closesocket(hListener);
			return HAWK_ERROR;
		}

		closesocket(hListener);

		if (getsockname(hClient, (struct sockaddr*)&connect_addr, &iSize) != HAWK_OK)
		{
			closesocket(hServer);
			closesocket(hClient);
			closesocket(hListener);
			return HAWK_ERROR;
		}

		if (iSize != sizeof (connect_addr)
			|| listen_addr.sin_family != connect_addr.sin_family
			|| listen_addr.sin_addr.s_addr != connect_addr.sin_addr.s_addr
			|| listen_addr.sin_port != connect_addr.sin_port)
		{
			closesocket(hServer);
			closesocket(hClient);
			closesocket(hListener);
			return HAWK_ERROR;
		}

		hSocket[0] = hClient, hSocket[1] = hServer;
		return HAWK_OK;
	}
#endif

	HawkSocketPair::HawkSocketPair()
	{
		m_hSocket[0] = INVALID_SOCKET;
		m_hSocket[1] = INVALID_SOCKET;
		m_iSend		 = 0;
		m_iRecv		 = 0;
		m_pMutex	 = 0;
	}

	HawkSocketPair::~HawkSocketPair()
	{
		Close();
	}
	
	Bool HawkSocketPair::Create(Bool bThreadSafe)
	{
#ifdef PLATFORM_WINDOWS
		 if (win32_socketpair(AF_INET,SOCK_STREAM,m_hSocket) != HAWK_OK)
			 return false;
#else
		if (socketpair(AF_UNIX,SOCK_STREAM,0,m_hSocket) != HAWK_OK)
			return false;
		
#endif

		Int32 iBufSize = HawkSocket::MaximizeSndBuf(GetWriteFd());
		A_Exception(iBufSize > 0);

		if (bThreadSafe)
			m_pMutex = new HawkMutex;

		return true;
	}

	Bool HawkSocketPair::Close()
	{
		SOCKET hSocket = GetWriteFd();
		if (hSocket != INVALID_SOCKET)
			closesocket(hSocket);

		hSocket = GetReadFd();
		if (hSocket != INVALID_SOCKET)
			closesocket(hSocket);

		m_iSend = 0;
		m_iRecv = 0;

		m_hSocket[0] = INVALID_SOCKET;
		m_hSocket[1] = INVALID_SOCKET;
		
		HAWK_RELEASE(m_pMutex);
		return true;
	}

	Bool HawkSocketPair::Clear()
	{
		if (IsValid())
		{
			SOCKET hSocket = GetReadFd();
			if (HawkSocket::SetBlocking(hSocket, false))
			{
				Char iFlag  = 0;
				Int32 iSize = sizeof(Char);
				while(recv(hSocket, &iFlag, iSize,0) > 0);

				return HawkSocket::SetBlocking(hSocket, true);
			}
		}
		return false;
	}

	Bool HawkSocketPair::IsValid() const
	{
		return m_hSocket[0] != INVALID_SOCKET && m_hSocket[1] != INVALID_SOCKET;
	}

	SOCKET HawkSocketPair::GetReadFd() const
	{
		return m_hSocket[0];
	}

	SOCKET HawkSocketPair::GetWriteFd() const
	{
		return m_hSocket[1];
	}	

	Bool HawkSocketPair::Send(const void* pData, Size_t& iSize, Int32 iFlags)
	{
		SOCKET hSocket = GetWriteFd();
		if (hSocket != INVALID_SOCKET)
		{
			if (m_pMutex)
				m_pMutex->Lock(__FILE__, __LINE__);

			iSize = send(hSocket, (const Char*)pData, iSize, iFlags);
			
			if (m_pMutex)
				m_pMutex->Unlock();

			if (iSize != Size_t(-1))
			{
				m_iSend += iSize;
				return true;
			}
		}
		return false;		
	}

	Bool HawkSocketPair::Receive(void* pData, Size_t& iSize, Int32 iFlags)
	{
		SOCKET hSocket = GetReadFd();
		if (hSocket != INVALID_SOCKET)
		{
			iSize = recv(hSocket,(Char*)pData, iSize, iFlags);
			if (iSize != Size_t(-1))
			{
				m_iRecv += iSize;
				return true;
			}
		}
		return false;
	}
}
