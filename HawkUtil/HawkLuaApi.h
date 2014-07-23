#ifndef HAWK_LUAAPI_H
#define HAWK_LUAAPI_H

#include "HawkScriptState.h"

namespace Hawk
{
	/************************************************************************/
	/* Hawk库功能脚本封装                                                   */
	/************************************************************************/
	class UTIL_API HawkLuaApi
	{
	public:
		//开启注册
		static Bool  Open(lua_State* pState);
	};
}
#endif
