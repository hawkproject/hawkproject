#ifndef HAWK_IPADDR_H
#define HAWK_IPADDR_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* IP��ַ��װ                                                           */
	/************************************************************************/
	class UTIL_API HawkIPAddr : public HawkRefCounter
	{
	public:
		//IP��ַ����
		enum Family
		{
			IPv4,
			IPv6
		};

	public:		
		//Ĭ�Ϲ���
		HawkIPAddr(Family eFamile = IPv4);

		//��������
		HawkIPAddr(const HawkIPAddr& sAddr);
		
		//IP��ַ��ʼ��(֧������)
		HawkIPAddr(const AString& sIpAddr);

		//pAddr��ʾIPv4��in_addr ���� IPv6��in6_addrָ��
		HawkIPAddr(const void* pAddr, Int32 iLen);

		//����
		virtual ~HawkIPAddr();
		
		//��ֵ�����
		HawkIPAddr& operator = (const HawkIPAddr& sAddr);
		
	public:
		//����IP��ַ������
		virtual Family  GetFamily() const;
			
		//IP��ַת��Ϊ�ַ���
		virtual AString ToString() const;
			
		//��ȡIP��ַ����
		virtual Int32   GetLength() const;
			
		//����IP��ַ��Ϣ,IPv4: in_addr IPv6: in6_addr
		const void*     GetAddress() const;
		
		//��ȡIP��ַ��,IPv4: AF_INET IPv6: AF_INET6
		virtual Int32   GetAF() const;		

	protected:
		//�����ַ�����IP��Ϣ
		virtual Bool    ParseIpAddr(const AString& sIpAddr);

		//������������ʼ��
		virtual Bool    InitByHostName(const AString& sHostName);

	private:
		//IP��ַ������
		Family  m_eFamily;

		//IP��ַ��Ϣ
		union
		{
			struct in_addr  m_sIPv4Addr;
			struct in6_addr m_sIPv6Addr;
		};
	};

	//IP��ַ��Ҫ���Ͷ���
	typedef HawkIPAddr IPAddr;
}
#endif
