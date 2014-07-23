#ifndef HAWK_GATESECURITY_H
#define HAWK_GATESECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* �������ݰ�ȫ���                                                     */
	/************************************************************************/
	class GATE_API HawkGateSecurity : public HawkSecurity
	{
	public:
		//��ѹ����ѹ����
		HawkGateSecurity(UInt8 iType = SECURITY_NONE);

		//��ѹ����ѹ����
		virtual ~HawkGateSecurity();

	public:
		//���ܽ���
		enum
		{
			SECURITY_NONE = 0,
			SECURITY_ENCODE,
			SECURITY_DECODE,
		};

	public:
		//���ܸ����ֽ���
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//��ѹ���ӿ�
		virtual Bool  Encode(HawkOctets& xOctets);

		//����ѹ�ӿ�
		virtual Bool  Decode(HawkOctets& xOctets);

	protected:
		//����
		UInt8		m_iType;		
		//�м�����������
		HawkOctets  m_sOctets;
		//������ָ��
		PVoid		m_pZStream;
	};
}
#endif
