#ifndef HAWK_RC4SECURITY_H
#define HAWK_RC4SECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* RC4�����㷨                                                          */
	/************************************************************************/
	class UTIL_API HawkRC4Security : public HawkSecurity
	{
	public:
		//RC�����������
		HawkRC4Security();
	
		//RC�����������
		virtual ~HawkRC4Security();

	public:
		//���ü�����Կ
		virtual void  SetParameter(const HawkOctets& xParam);

		//������������ֽ���
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//���ܲ�����Ϣ
		UChar m_cPerm[256];
		UChar m_iIdx1;
		UChar m_iIdx2;
	};
}
#endif
