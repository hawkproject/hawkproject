#ifndef HAWK_SECURITY_H
#define HAWK_SECURITY_H

#include "HawkOctets.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ݼӰ���ܽӿڻ��ඨ��                                             */
	/************************************************************************/
	class UTIL_API HawkSecurity : public HawkRefCounter
	{
	protected:
		//���ܶ�����
		HawkSecurity();

		//���ܶ�������
		virtual ~HawkSecurity() ;

	public:
		//���ü�����Կ
		virtual void  SetParameter(const HawkOctets& xOctets);

		//��ȡ������Կ
		virtual void  GetParameter(HawkOctets& xOctets);	

		//���ܸ����ֽ���
		virtual Bool  Update(HawkOctets& xOctets);
	};
}
#endif
