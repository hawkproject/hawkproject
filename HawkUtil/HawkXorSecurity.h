#ifndef HAWK_XORSECURITY_H
#define HAWK_XORSECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* XOR�����㷨                                                          */
	/************************************************************************/
	class UTIL_API HawkXorSecurity : public HawkSecurity
	{
	public:
		//xor�����������
		HawkXorSecurity();
	
		//xor�����������
		virtual ~HawkXorSecurity();

	public:
		//���ü�����Կ(���ֽ������Ԫ)
		virtual void  SetParameter(const HawkOctets& xParam);

		//���������Ԫ
		virtual void  SetParameter(UChar cXorKey);

		//������������ֽ���
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//���ܲ�����Ϣ
		UChar m_cXorKey;
	};
}
#endif
