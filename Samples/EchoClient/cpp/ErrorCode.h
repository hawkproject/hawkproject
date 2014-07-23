#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

#include "HawkUtil.h"

namespace Hawk
{
	//登陆模块错误码[100-150]
	namespace LoginErr
	{
		enum
		{
			//用户名非法
			NAME_INVALID = 100,
			//用户名错误
			NAME_ERROR = 101,
			//密码错误
			PWD_ERROR = 102,
		};
	};

}
#endif
