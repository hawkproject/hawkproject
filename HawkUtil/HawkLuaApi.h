#ifndef HAWK_LUAAPI_H
#define HAWK_LUAAPI_H

#include "HawkScriptState.h"

namespace Hawk
{
	/************************************************************************/
	/* Hawk�⹦�ܽű���װ                                                   */
	/************************************************************************/
	class UTIL_API HawkLuaApi
	{
	public:
		//����ע��
		static Bool  Open(lua_State* pState);
	};
}
#endif
