#ifndef __PROTOCOLID_H__
#define __PROTOCOLID_H__

#include "HawkUtil.h"

namespace Hawk
{
	namespace ProtocolId
	{
		enum
		{
			//错误通知
			ERR_NOTIFY = 100,
			//消息通知
			MSG_NOTIFY = 101,
			//时间同步
			TIME_SYNC = 102,
			//登入游戏
			LOGIN_GAME = 200,
			//踢出游戏
			KICKOUT_GAME = 201,
			//玩家初始化
			PLAYER_INIT = 202,
			//同步数据完成
			PLAYER_ASSEMBLY = 203,
		};
	}
}
#endif
