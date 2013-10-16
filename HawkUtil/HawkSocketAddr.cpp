#include "HawkSocketAddr.h"
#include "HawkStringUtil.h"

namespace Hawk
{
	HawkSocketAddr::HawkSocketAddr(const AString& sIpPort) : m_eFamily(IPAddr::IPv4)
	{
		if (sIpPort.size())
		{
			AStringVector asv;
			HawkStringUtil::Split<AString>(sIpPort,asv,":");
			if (asv.size() == 2)
			{
				AString sIp   = asv[0];
				AString sPort = asv[1];

				Init(IPAddr(sIp),HawkStringUtil::StringToInt<AString>(sPort));
			}
			else
			{
				HawkAssert(false && "Invalid SocketAddr.");
			}
		}
	}

	HawkSocketAddr::HawkSocketAddr(const IPAddr& sAddr, UInt16 iPort) : m_eFamily(IPAddr::IPv4)
	{
		Init(sAddr,iPort);
	}

	HawkSocketAddr::HawkSocketAddr(const HawkSocketAddr& sAddr) : m_eFamily(IPAddr::IPv4)
	{
		if (sAddr.GetFamily() == IPAddr::IPv4)
		{
			m_eFamily = IPAddr::IPv4;
			memcpy(&m_sIPv4Addr,&sAddr.m_sIPv4Addr,sizeof(m_sIPv4Addr));
		}
		else if (sAddr.GetFamily() == IPAddr::IPv6)
		{
			m_eFamily = IPAddr::IPv6;
			memcpy(&m_sIPv6Addr,&sAddr.m_sIPv6Addr,sizeof(m_sIPv6Addr));
		}
		else
		{
			HawkAssert(false && "Invalid SocketAddr.");
		}
	}

	HawkSocketAddr::HawkSocketAddr(const struct sockaddr* pAddr, Int32 iLen) : m_eFamily(IPAddr::IPv4)
	{
		if (iLen == sizeof(struct sockaddr_in))
		{
			m_eFamily = IPAddr::IPv4;
			memcpy(&m_sIPv4Addr, pAddr, sizeof(m_sIPv4Addr));
		}
		else if (iLen == sizeof(struct sockaddr_in6))
		{
			m_eFamily = IPAddr::IPv6;
			memcpy(&m_sIPv6Addr, pAddr, sizeof(m_sIPv6Addr));
		}
		else
		{
			HawkAssert(false && "Invalid SocketAddr.");
		}
	}

	HawkSocketAddr::~HawkSocketAddr()
	{
	}

	HawkSocketAddr& HawkSocketAddr::operator = (const HawkSocketAddr& sAddr)
	{
		if (&sAddr != this)
		{
			if (sAddr.GetFamily() == IPAddr::IPv4)
			{
				m_eFamily = IPAddr::IPv4;
				memcpy(&m_sIPv4Addr,&sAddr.m_sIPv4Addr,sizeof(m_sIPv4Addr));
			}
			else if (sAddr.GetFamily() == IPAddr::IPv6)
			{
				m_eFamily = IPAddr::IPv6;
				memcpy(&m_sIPv6Addr,&sAddr.m_sIPv6Addr,sizeof(m_sIPv6Addr));
			}
		}
		return *this;
	}

	void  HawkSocketAddr::Init(const IPAddr& sHost,UInt16 iPort)
	{
		if (sHost.GetFamily() == IPAddr::IPv4)
		{
			m_eFamily = IPAddr::IPv4;
			memset(&m_sIPv4Addr, 0, sizeof(m_sIPv4Addr));
			m_sIPv4Addr.sin_family = AF_INET;
			memcpy(&m_sIPv4Addr.sin_addr, sHost.GetAddress(), sizeof(m_sIPv4Addr.sin_addr));
			m_sIPv4Addr.sin_port = htons(iPort);
		}
		else if (sHost.GetFamily() == IPAddr::IPv6)
		{
			m_eFamily = IPAddr::IPv6;
			memset(&m_sIPv6Addr, 0, sizeof(m_sIPv6Addr));
			m_sIPv6Addr.sin6_family = AF_INET6;
			memcpy(&m_sIPv6Addr.sin6_addr, sHost.GetAddress(), sizeof(m_sIPv6Addr.sin6_addr));
			m_sIPv6Addr.sin6_port = htons(iPort);
		}
		else
		{
			HawkAssert(false && "Invalid SocketAddr.");
		}
	}

	Bool HawkSocketAddr::operator < (const HawkSocketAddr& sAddr) const
	{
		if(GetFamily() == sAddr.GetFamily())
		{
			if (m_eFamily == IPAddr::IPv4)
			{
				return m_sIPv4Addr.sin_addr.s_addr < sAddr.m_sIPv4Addr.sin_addr.s_addr  ||
					(m_sIPv4Addr.sin_addr.s_addr == sAddr.m_sIPv4Addr.sin_addr.s_addr && 
					 m_sIPv4Addr.sin_port < sAddr.m_sIPv4Addr.sin_port);
			}
			else if (m_eFamily == IPAddr::IPv6)
			{
				return m_sIPv6Addr.sin6_addr.s6_addr < sAddr.m_sIPv6Addr.sin6_addr.s6_addr  ||
					(m_sIPv6Addr.sin6_addr.s6_addr == sAddr.m_sIPv6Addr.sin6_addr.s6_addr && 
					m_sIPv6Addr.sin6_port < sAddr.m_sIPv6Addr.sin6_port);
			}
		}		

		return false;
	}

	Bool  HawkSocketAddr::operator == (const HawkSocketAddr& sAddr) const
	{
		if(GetFamily() == sAddr.GetFamily())
		{
			if (m_eFamily == IPAddr::IPv4)
			{
				return m_sIPv4Addr.sin_addr.s_addr == sAddr.m_sIPv4Addr.sin_addr.s_addr && 
					m_sIPv4Addr.sin_port == sAddr.m_sIPv4Addr.sin_port;
			}
			else if (m_eFamily == IPAddr::IPv6)
			{
				return m_sIPv6Addr.sin6_addr.s6_addr == sAddr.m_sIPv6Addr.sin6_addr.s6_addr && 
					m_sIPv6Addr.sin6_port == sAddr.m_sIPv6Addr.sin6_port;
			}
		}		

		return false;
	}

	Bool HawkSocketAddr::GetIPAddr(IPAddr& sIpAddr) const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			sIpAddr = IPAddr(&m_sIPv4Addr.sin_addr, sizeof(m_sIPv4Addr.sin_addr));
			return true;
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			sIpAddr = IPAddr(&m_sIPv6Addr.sin6_addr, sizeof(m_sIPv6Addr.sin6_addr));
			return true;
		}
		return false;
	}

	UInt16 HawkSocketAddr::GetPort() const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			return ntohs(m_sIPv4Addr.sin_port);
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			return ntohs(m_sIPv6Addr.sin6_port);
		}
		return 0;
	}

	Int32 HawkSocketAddr::GetLength() const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			return sizeof(m_sIPv4Addr);
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			return sizeof(m_sIPv6Addr);
		}
		return 0;
	}

	const sockaddr* HawkSocketAddr::GetAddress() const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			return reinterpret_cast<const struct sockaddr*>(&m_sIPv4Addr);
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			return reinterpret_cast<const struct sockaddr*>(&m_sIPv6Addr);
		}
		return 0;
	}

	UInt8 HawkSocketAddr::CopyAddr(Char* pData) const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			memcpy(pData, &m_sIPv4Addr, sizeof(m_sIPv4Addr));
			return sizeof(m_sIPv4Addr);
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			memcpy(pData, &m_sIPv6Addr, sizeof(m_sIPv6Addr));
			return sizeof(m_sIPv6Addr);
		}
		return 0;
	}

	Int32 HawkSocketAddr::GetAF() const
	{
		if (m_eFamily == IPAddr::IPv4)
		{
			return m_sIPv4Addr.sin_family;
		}
		else if (m_eFamily == IPAddr::IPv6)
		{
			return m_sIPv6Addr.sin6_family;
		}
		return 0;
	}

	AString HawkSocketAddr::ToString() const
	{
		IPAddr sIpAddr;
		if (GetIPAddr(sIpAddr))
		{
			return sIpAddr.ToString() + ":" + HawkStringUtil::IntToString<AString>(GetPort());
		}
		return "";
	}

	Bool HawkSocketAddr::IsValid() const
	{
		return m_eFamily == IPAddr::IPv4 || m_eFamily == IPAddr::IPv6;
	}

	Int32 HawkSocketAddr::GetFamily() const
	{
		return m_eFamily;
	}
}
