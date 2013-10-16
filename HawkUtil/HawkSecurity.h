#ifndef HAWK_SECURITY_H
#define HAWK_SECURITY_H

#include "HawkOctets.h"

namespace Hawk
{
	/************************************************************************/
	/* 数据加班加密接口基类定义                                             */
	/************************************************************************/
	class UTIL_API HawkSecurity : public HawkRefCounter
	{
	protected:
		//加密对象构造
		HawkSecurity();

		//加密对象析构
		virtual ~HawkSecurity() ;

	public:
		//设置加密密钥
		virtual void  SetParameter(const HawkOctets& xOctets);

		//获取加密密钥
		virtual void  GetParameter(HawkOctets& xOctets);	

		//加密更新字节流
		virtual Bool  Update(HawkOctets& xOctets);
	};
}
#endif
