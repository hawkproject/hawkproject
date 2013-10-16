#ifndef HAWK_IPADDR_H
#define HAWK_IPADDR_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* IP地址封装                                                           */
	/************************************************************************/
	class UTIL_API HawkIPAddr : public HawkRefCounter
	{
	public:
		//IP地址类型
		enum Family
		{
			IPv4,
			IPv6
		};

	public:		
		//默认构造
		HawkIPAddr(Family eFamile = IPv4);

		//拷贝构造
		HawkIPAddr(const HawkIPAddr& sAddr);
		
		//IP地址初始化(支持域名)
		HawkIPAddr(const AString& sIpAddr);

		//pAddr表示IPv4的in_addr 或者 IPv6的in6_addr指针
		HawkIPAddr(const void* pAddr, Int32 iLen);

		//析构
		virtual ~HawkIPAddr();
		
		//赋值运算符
		HawkIPAddr& operator = (const HawkIPAddr& sAddr);
		
	public:
		//返回IP地址族类型
		virtual Family  GetFamily() const;
			
		//IP地址转换为字符串
		virtual AString ToString() const;
			
		//获取IP地址长度
		virtual Int32   GetLength() const;
			
		//返回IP地址信息,IPv4: in_addr IPv6: in6_addr
		const void*     GetAddress() const;
		
		//获取IP地址族,IPv4: AF_INET IPv6: AF_INET6
		virtual Int32   GetAF() const;		

	protected:
		//解析字符串到IP信息
		virtual Bool    ParseIpAddr(const AString& sIpAddr);

		//依据主机名初始化
		virtual Bool    InitByHostName(const AString& sHostName);

	private:
		//IP地址族类型
		Family  m_eFamily;

		//IP地址信息
		union
		{
			struct in_addr  m_sIPv4Addr;
			struct in6_addr m_sIPv6Addr;
		};
	};

	//IP地址简要类型定义
	typedef HawkIPAddr IPAddr;
}
#endif
