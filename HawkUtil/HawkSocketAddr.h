#ifndef HAWK_SOCKETADDR_H
#define HAWK_SOCKETADDR_H

#include "HawkIPAddr.h"

namespace Hawk
{
	/************************************************************************/
	/* Socket��ַ��װ                                                       */
	/************************************************************************/
	class UTIL_API HawkSocketAddr : public HawkRefCounter
	{
	public:
		//IP�˿��ַ�������(֧������)
		HawkSocketAddr(const AString& sIpPort = "");

		//IP��ַ�Ͷ˿ڹ���
		HawkSocketAddr(const IPAddr& sHost, UInt16 iPort);
		
		//��ַ����
		HawkSocketAddr(const struct sockaddr* pAddr, Int32 iLen);
		
		//��������
		HawkSocketAddr(const HawkSocketAddr& sAddr);
		
		//����
		virtual ~HawkSocketAddr();
		
		//��ֵ����
		HawkSocketAddr& operator = (const HawkSocketAddr& sAddr);

	public:
		//��ȡIP��ַ
		virtual Bool	GetIPAddr(IPAddr& sAddr) const;

		//��ȡ�˿�
		virtual UInt16  GetPort() const;

		//��ȡ��ַ�ֽڳ���
		virtual Int32   GetLength() const;

		//��ȡsockaddrָ��
		const sockaddr* GetAddress() const;

		//��ȡ��ַ��AF
		virtual Int32   GetAF() const;

		//ת��Ϊ�ַ�����ʽ
		virtual AString ToString() const;

		//��ȡ��ַ����
		virtual Int32   GetFamily() const;

		//���Ƶ�ַ��Ϣ
		virtual UInt8   CopyAddr(Char* pData) const;

		//��ַ�Ƿ���Ч
		virtual Bool    IsValid() const;

	public:
		//С�ڲ���������
		Bool  operator <  (const HawkSocketAddr& sAddr) const;

		//��Ȳ���������
		Bool  operator == (const HawkSocketAddr& sAddr) const;

	protected:
		//�ڲ���ʼ��
		void  Init(const IPAddr& sHost, UInt16 iPort);

	private:
		//��ַ��
		IPAddr::Family m_eFamily;

		//socket��ַ��Ϣ
		union
		{
			struct sockaddr_in  m_sIPv4Addr;
			struct sockaddr_in6 m_sIPv6Addr;
		};
	};

	//socket��ַ���ͼ�Ҫ����
	typedef HawkSocketAddr SocketAddr;
}
#endif
