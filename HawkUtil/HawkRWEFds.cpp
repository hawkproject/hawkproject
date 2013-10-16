#include "HawkRWEFds.h"

namespace Hawk
{
	HawkRWEfds::HawkRWEfds()
	{
		Reset();
	}

	HawkRWEfds::~HawkRWEfds()
	{
		Reset();
	}

	void HawkRWEfds::EnableEvent(SOCKET hSocket, UInt32 iEvent)
	{
		if(hSocket != INVALID_SOCKET)
		{
			if(iEvent & HEVENT_READ)
				FD_SET(hSocket, &m_sRfds);

			if(iEvent & HEVENT_WRITE)
				FD_SET(hSocket, &m_sWfds);

			if(iEvent & HEVENT_EXCEPT)
				FD_SET(hSocket, &m_sEfds);

			if(iEvent) 
				m_iFdNum++;

			if (hSocket >= m_iMaxFd)
				m_iMaxFd = hSocket + 1;
		}
	}

	Int32  HawkRWEfds::SelectEvent(Int32 iTimeout)
	{
		Int32 iEventNum = 0;

		if (m_iFdNum > 0)
		{
			if(iTimeout < 0)
			{
				iEventNum = select(m_iMaxFd, &m_sRfds, &m_sWfds, &m_sEfds, 0);
			}
			else
			{
				struct timeval tv;
				tv.tv_sec  = iTimeout / 1000;
				tv.tv_usec = (iTimeout % 1000)*1000;
				iEventNum = select(m_iMaxFd, &m_sRfds, &m_sWfds, &m_sEfds, &tv);
			}
		}		

		return iEventNum;
	}

	Bool HawkRWEfds::IsSetEvent(SOCKET hSocket, UInt32 iEvent)
	{
		if(hSocket != INVALID_SOCKET)
		{
			if(iEvent & HEVENT_READ)
			{
				if(FD_ISSET(hSocket, &m_sRfds))
					return true;
			}

			if(iEvent & HEVENT_WRITE)
			{
				if(FD_ISSET(hSocket, &m_sWfds))
					return true;
			}

			if(iEvent & HEVENT_EXCEPT)
			{
				if(FD_ISSET(hSocket, &m_sEfds))
					return true;
			}
		}

		return false;
	}

	Int32 HawkRWEfds::GetFdNum()
	{
		return m_iFdNum;
	}

	void HawkRWEfds::Reset()
	{
		FD_ZERO(&m_sRfds);
		FD_ZERO(&m_sWfds);
		FD_ZERO(&m_sEfds);
		m_iFdNum = 0;
		m_iMaxFd = 0;
	}
}
