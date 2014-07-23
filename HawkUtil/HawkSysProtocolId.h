#ifndef __HAWKSYSPROTOCOLID_H__
#define __HAWKSYSPROTOCOLID_H__

#include "HawkUtil.h"

namespace Hawk
{
	namespace SysProtocol
	{
		enum
		{
			//加密方式
			SYS_SECURITY = 1,
			//选服务器
			SYS_SEL_SERVER = 2,
			//会话开始
			SYS_SESSION_START = 3,
			//拒绝连接
			SYS_REFUSE_CONN = 4,
			//会话主动断开
			SYS_SESSION_BREAK = 5,
			//会话被动关闭
			SYS_CLOSE_SESSION = 6,
			//服务器关闭
			SYS_SERVER_SHUTDOWN = 7,
			//Ping操作
			SYS_CLT_PING = 8,
			//Pong操作
			SYS_SVR_PONG = 9,
			//字节数组
			SYS_OCTETS = 10,
			//会话心跳
			SYS_HEART_BEAT = 11,
			//心跳超时
			SYS_HEART_BREAK = 12,
			//消息通知
			SYS_MSG_NOTIFY = 13,
			//错误通知
			SYS_ERR_NOTIFY = 14,
			//日志消息
			SYS_LOG_MSG = 15,
			//性能请求
			SYS_PROF_REQ = 16,
			//性能信息
			SYS_PROF_INFO = 17,
		};
	}
}
#endif
