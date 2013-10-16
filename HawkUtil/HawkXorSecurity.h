#ifndef HAWK_XORSECURITY_H
#define HAWK_XORSECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* XOR加密算法                                                          */
	/************************************************************************/
	class UTIL_API HawkXorSecurity : public HawkSecurity
	{
	public:
		//xor加密组件构造
		HawkXorSecurity();
	
		//xor加密组件构造
		virtual ~HawkXorSecurity();

	public:
		//设置加密密钥(单字节异或子元)
		virtual void  SetParameter(const HawkOctets& xParam);

		//设置异或子元
		virtual void  SetParameter(UChar cXorKey);

		//加密组件更新字节流
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//加密参数信息
		UChar m_cXorKey;
	};
}
#endif
