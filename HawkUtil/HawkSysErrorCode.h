#ifndef __HAWKSYSERRORCODE_H__
#define __HAWKSYSERRORCODE_H__

#include "HawkUtil.h"

namespace Hawk
{
	HAWK_START_NAMESPACE(SysProtocol)

	//网关错误
	namespace GateErr
	{
		enum
		{
			//拒绝连接
			ERR_REFUSE_CONN = 1,
			//无可用服务
			ERR_SERVICE_INVALID = 2,
			//黑名单ip列表
			ERR_IP_BLACKLIST = 3,
			//ip连接数限制
			ERR_IP_PEERLIMIT = 4,
		};
	};

	HAWK_CLOSE_NAMESPACE(SysProtocol)
}
#endif
