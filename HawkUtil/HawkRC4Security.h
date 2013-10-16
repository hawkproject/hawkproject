#ifndef HAWK_RC4SECURITY_H
#define HAWK_RC4SECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* RC4加密算法                                                          */
	/************************************************************************/
	class UTIL_API HawkRC4Security : public HawkSecurity
	{
	public:
		//RC加密组件构造
		HawkRC4Security();
	
		//RC加密组件构造
		virtual ~HawkRC4Security();

	public:
		//设置加密密钥
		virtual void  SetParameter(const HawkOctets& xParam);

		//加密组件更新字节流
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//加密参数信息
		UChar m_cPerm[256];
		UChar m_iIdx1;
		UChar m_iIdx2;
	};
}
#endif
