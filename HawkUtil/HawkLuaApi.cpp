#include "HawkLuaApi.h"
#include "HawkSession.h"
#include "HawkOSOperator.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"
#include "HawkProtocolManager.h"

#ifndef TOLUA_SUPPORT

namespace Hawk
{
	Bool HawkLuaApi::Open(lua_State* pState) 
	{
		return true;
	}
}

#else

extern "C" {
#include "tolua_fix.h"
}

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

#define OCTETS_STREAM_POP_TRY   try {
#define OCTETS_STREAM_POP_CATCH } catch (HawkException& rhsExcep) { lua_pushnil(tolua_S); HawkFmtError(rhsExcep.GetMsg().c_str()); }

using namespace Hawk;

//////////////////////////////////////////////////////////////////////////
//函数注册
static int tolua_HawkApi_HawkPrint(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* szPrintMsg = tolua_tostring(tolua_S,1,0);
		if (szPrintMsg && strlen(szPrintMsg))
			HawkPrint(_Ascii((Utf8*)szPrintMsg));
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'HawkPrint'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_HawkLog(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* szLogMsg = tolua_tostring(tolua_S,1,0);
		if (szLogMsg && strlen(szLogMsg))
			HawkLog(_Ascii((Utf8*)szLogMsg));
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'HawkLog'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_HawkError(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S,1,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* szErrMsg = tolua_tostring(tolua_S,1,0);
		if (szErrMsg && strlen(szErrMsg))
			HawkError(_Ascii((Utf8*)szErrMsg));
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'HawkError'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_CalcCrc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UInt32 iCrc = 0;
		const char* szVal = tolua_tostring(tolua_S,2,0);
		if (szVal && strlen(szVal))
			iCrc = HawkOSOperator::CalcCrc((const UChar*)szVal, strlen(szVal));			
		
		tolua_pushnumber(tolua_S, (lua_Number)iCrc);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CalcCrc'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_CalcFileCrc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UInt32 iCrc = 0;
		const char* szVal = tolua_tostring(tolua_S,2,0);
		if (szVal && strlen(szVal))
			iCrc = HawkOSOperator::CalcFileCrc(szVal);			

		tolua_pushnumber(tolua_S, (lua_Number)iCrc);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CalcFileCrc'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_CalcHash(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UInt32 iHash = 0;
		const char* szVal = tolua_tostring(tolua_S,2,0);
		if (szVal && strlen(szVal))
			iHash = HawkStringUtil::CalcHash(szVal, strlen(szVal));			

		tolua_pushnumber(tolua_S, (lua_Number)iHash);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CalcHash'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_GetTickCount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		UInt32 iTickCount = HawkOSOperator::GetTickCount();
		tolua_pushnumber(tolua_S, (lua_Number)iTickCount);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'TickCount'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_RandomInt(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{		
		tolua_pushnumber(tolua_S, (lua_Number)HawkRand::RandInt());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'RandomInt'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_RandomFloat(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"HawkApi",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{		
		tolua_pushnumber(tolua_S, (lua_Number)HawkRand::RandFloat());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'RandomFloat'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{		
		OctetsStream* pOS = (OctetsStream*)Mtolua_new((OctetsStream)());
		tolua_pushusertype(tolua_S, (void*)pOS, "OctetsStream");
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_new01(lua_State* tolua_S)
{
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
	{
		Int32 iSize = ((Int32)tolua_tonumber(tolua_S,2,0));
		HawkAssert(iSize > 0);
		OctetsStream* pOS = (OctetsStream*)Mtolua_new((OctetsStream)(iSize));
		tolua_pushusertype(tolua_S, (void*)pOS, "OctetsStream");
	}
	return 1;
tolua_lerror:
	return tolua_HawkApi_OctetsStream_new00(tolua_S);
}

static int tolua_HawkApi_OctetsStream_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{		
		OctetsStream* pOS = (OctetsStream*)Mtolua_new((OctetsStream)());
		tolua_pushusertype(tolua_S, (void*)pOS, "OctetsStream");
		tolua_register_gc(tolua_S, lua_gettop(tolua_S));
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new_local'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_new01_local(lua_State* tolua_S)
{
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
	{
		Int32 iSize = ((Int32)tolua_tonumber(tolua_S,2,0));
		HawkAssert(iSize > 0);
		OctetsStream* pOS = (OctetsStream*)Mtolua_new((OctetsStream)(iSize));
		tolua_pushusertype(tolua_S, (void*)pOS, "OctetsStream");
		tolua_register_gc(tolua_S, lua_gettop(tolua_S));
	}
	return 1;
tolua_lerror:
	return tolua_HawkApi_OctetsStream_new00_local(tolua_S);
}

static int tolua_HawkApi_OctetsStream_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* self = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
		Mtolua_delete(self);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushBool(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isboolean(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Bool bVal = (tolua_toboolean(tolua_S, 2, 0) != 0);
		if (pOS)
			(*pOS) << bVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushBool'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushInt8(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Int8 iVal = (Int8)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushInt8'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushUInt8(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt8 iVal = (UInt8)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushUInt8'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushInt16(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Int16 iVal = (Int16)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushInt16'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushUInt16(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt16 iVal = (UInt16)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushUInt16'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushInt32(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Int32 iVal = (Int32)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushInt32'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushUInt32(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt32 iVal = (UInt32)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushUInt32'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushInt64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Int64 iVal = (Int64)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushInt64'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushUInt64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt64 iVal = (UInt64)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << iVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushUInt64'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushFloat(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Float fVal = (Float)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << fVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushFloat'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushDouble(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Double dbVal = (Double)tolua_tonumber(tolua_S, 2, 0);
		if (pOS)
			(*pOS) << dbVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushDouble'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushAString(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		AString sVal = (AString)tolua_tostring(tolua_S, 2, "");
		if (pOS)
			(*pOS) << sVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushAString'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushUString(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UString sVal = (Utf8*)tolua_tostring(tolua_S, 2, "");
		if (pOS)
			(*pOS) << sVal;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushUString'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PushHawkOctets(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OctetsStream* pOSPush = (OctetsStream*)tolua_tousertype(tolua_S,2,0);
		if (!pOSPush) goto tolua_lerror;

		*pOS << *pOSPush;
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PushHawkOctets'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopBool(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Bool bVal = false;
		if (pOS)
			(*pOS) >> bVal;

		tolua_pushboolean(tolua_S, bVal);
		
		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopBool'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopInt8(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Int8 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopInt8'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopUInt8(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		UInt8 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopUInt8'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopInt16(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Int16 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopInt16'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopUInt16(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		UInt16 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopUInt16'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopInt32(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Int32 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopInt32'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopUInt32(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		UInt32 iVal = 0;
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopUInt32'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopInt64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Int64 iVal = 0;		
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopInt64'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopUInt64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);	
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		UInt64 iVal = 0;		
		if (pOS)
			(*pOS) >> iVal;

		tolua_pushnumber(tolua_S, (lua_Number)iVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopUInt64'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopFloat(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Float fVal = 0;
		if (pOS)
			(*pOS) >> fVal;

		tolua_pushnumber(tolua_S, (lua_Number)fVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopFloat'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopDouble(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		Double dbVal = 0;
		if (pOS)
			(*pOS) >> dbVal;

		tolua_pushnumber(tolua_S, (lua_Number)dbVal);

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopDouble'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopAString(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		AString sVal = "";
		if (pOS)
			(*pOS) >> sVal;

		lua_pushstring(tolua_S, sVal.c_str());

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopAString'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopUString(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		UString sVal = (Utf8*)"";
		if (pOS)
			(*pOS) >> sVal;

		lua_pushstring(tolua_S, (const char*)sVal.c_str());

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopUString'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_PopHawkOctets(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		OCTETS_STREAM_POP_TRY

		OctetsStream* pOSPop = (OctetsStream*)Mtolua_new((OctetsStream)());
		if (pOS) (*pOS) >> *pOSPop;
		tolua_pushusertype(tolua_S, (void*)pOSPop, "OctetsStream");
		tolua_register_gc(tolua_S, lua_gettop(tolua_S));

		OCTETS_STREAM_POP_CATCH
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'PopHawkOctets'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Size(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushnumber(tolua_S, (lua_Number)pOS->Size());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Size'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Capacity(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushnumber(tolua_S, (lua_Number)pOS->Capacity());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Capacity'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_AvailableSize(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushnumber(tolua_S, (lua_Number)pOS->AvailableSize());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AvailableSize'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_EmptyCap(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushnumber(tolua_S, (lua_Number)pOS->EmptyCap());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'EmptyCap'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_GetPos(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushnumber(tolua_S, (lua_Number)pOS->GetPos());
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'GetPos'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Eos(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		lua_pushboolean(tolua_S, pOS->Eos()?true:false);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Eos'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Clear(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		pOS->Clear();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_RemoveBlank(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		pOS->RemoveBlank();
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'RemoveBlank'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_MoveNonius(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		Int32 iMark = (Int32)tolua_tonumber(tolua_S, 2, 0);
		pOS->MoveNonius(iMark);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'MoveNonius'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Resize(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt32 iSize = (UInt32)tolua_tonumber(tolua_S, 2, 0);
		pOS->Resize(iSize);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Resize'.",&tolua_err);
	return 0;
#endif
}

static int tolua_HawkApi_OctetsStream_Reserve(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"OctetsStream",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		OctetsStream* pOS = (OctetsStream*)tolua_tousertype(tolua_S,1,0);
		if (!pOS) goto tolua_lerror;

		UInt32 iSize = (UInt32)tolua_tonumber(tolua_S, 2, 0);
		pOS->Reserve(iSize);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Reserve'.",&tolua_err);
	return 0;
#endif
}

//回收机制定义函数
static int tolua_collect_HawkApi_OctetsStream(lua_State* tolua_S)
{
	HawkOctetsStream* pOS = (HawkOctetsStream*)tolua_tousertype(tolua_S,1,0);
	HAWK_RELEASE(pOS);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

namespace Hawk
{
	void tolua_reg_hawkapi_types(lua_State* pState)
	{
		tolua_usertype(pState, "HawkApi");
		tolua_usertype(pState, "OctetsStream");
	}

	void tolua_reg_hawkapi_functions(lua_State* pState)
	{
		//全局函数注册
		{
			tolua_function(pState, "HawkPrint", tolua_HawkApi_HawkPrint);
			tolua_function(pState, "HawkLog",	 tolua_HawkApi_HawkLog);
			tolua_function(pState, "HawkError",	 tolua_HawkApi_HawkError);
		}

		//HawkLuaApi函数注册
		tolua_cclass(pState, "HawkApi", "HawkApi", "", NULL);
		tolua_beginmodule(pState, "HawkApi");
		{
			tolua_function(pState, "CalcCrc",		tolua_HawkApi_CalcCrc);
			tolua_function(pState, "CalcFileCrc",	tolua_HawkApi_CalcFileCrc);
			tolua_function(pState, "CalcHash",		tolua_HawkApi_CalcHash);
			tolua_function(pState, "GetTickCount",	tolua_HawkApi_GetTickCount);
			tolua_function(pState, "RandomInt",		tolua_HawkApi_RandomInt);
			tolua_function(pState, "RandomFloat",	tolua_HawkApi_RandomFloat);
		}
		tolua_endmodule(pState);

		//OctetsStream类封装注册
		tolua_cclass(pState, "OctetsStream", "OctetsStream", "", tolua_collect_HawkApi_OctetsStream);
		tolua_beginmodule(pState,"OctetsStream");
		{
			tolua_function(pState, "new",			tolua_HawkApi_OctetsStream_new00);
			tolua_function(pState, "new_local",		tolua_HawkApi_OctetsStream_new00_local);
			tolua_function(pState, ".call",			tolua_HawkApi_OctetsStream_new00_local);
			tolua_function(pState, "new",			tolua_HawkApi_OctetsStream_new01);
			tolua_function(pState, "new_local",		tolua_HawkApi_OctetsStream_new01_local);
			tolua_function(pState, ".call",			tolua_HawkApi_OctetsStream_new01_local);
			tolua_function(pState, "delete",		tolua_HawkApi_OctetsStream_delete00);
			tolua_function(pState, "PushBool",		tolua_HawkApi_OctetsStream_PushBool);
			tolua_function(pState, "PushInt8",		tolua_HawkApi_OctetsStream_PushInt8);
			tolua_function(pState, "PushUInt8",		tolua_HawkApi_OctetsStream_PushUInt8);
			tolua_function(pState, "PushInt16",		tolua_HawkApi_OctetsStream_PushInt16);
			tolua_function(pState, "PushUInt16",	tolua_HawkApi_OctetsStream_PushUInt16);
			tolua_function(pState, "PushInt32",		tolua_HawkApi_OctetsStream_PushInt32);
			tolua_function(pState, "PushUInt32",	tolua_HawkApi_OctetsStream_PushUInt32);
			tolua_function(pState, "PushInt64",		tolua_HawkApi_OctetsStream_PushInt64);
			tolua_function(pState, "PushUInt64",	tolua_HawkApi_OctetsStream_PushUInt64);
			tolua_function(pState, "PushFloat",		tolua_HawkApi_OctetsStream_PushFloat);
			tolua_function(pState, "PushDouble",	tolua_HawkApi_OctetsStream_PushDouble);
			tolua_function(pState, "PushAString",	tolua_HawkApi_OctetsStream_PushAString);
			tolua_function(pState, "PushUString",	tolua_HawkApi_OctetsStream_PushUString);
			tolua_function(pState, "PushHawkOctets",tolua_HawkApi_OctetsStream_PushHawkOctets);
			tolua_function(pState, "PopBool",		tolua_HawkApi_OctetsStream_PopBool);
			tolua_function(pState, "PopInt8",		tolua_HawkApi_OctetsStream_PopInt8);
			tolua_function(pState, "PopUInt8",		tolua_HawkApi_OctetsStream_PopUInt8);
			tolua_function(pState, "PopInt16",		tolua_HawkApi_OctetsStream_PopInt16);
			tolua_function(pState, "PopUInt16",		tolua_HawkApi_OctetsStream_PopUInt16);
			tolua_function(pState, "PopInt32",		tolua_HawkApi_OctetsStream_PopInt32);
			tolua_function(pState, "PopUInt32",		tolua_HawkApi_OctetsStream_PopUInt32);
			tolua_function(pState, "PopInt64",		tolua_HawkApi_OctetsStream_PopInt64);
			tolua_function(pState, "PopUInt64",		tolua_HawkApi_OctetsStream_PopUInt64);
			tolua_function(pState, "PopFloat",		tolua_HawkApi_OctetsStream_PopFloat);
			tolua_function(pState, "PopDouble",		tolua_HawkApi_OctetsStream_PopDouble);
			tolua_function(pState, "PopAString",	tolua_HawkApi_OctetsStream_PopAString);
			tolua_function(pState, "PopUString",	tolua_HawkApi_OctetsStream_PopUString);
			tolua_function(pState, "PopHawkOctets",	tolua_HawkApi_OctetsStream_PopHawkOctets);
			tolua_function(pState, "Size",			tolua_HawkApi_OctetsStream_Size);
			tolua_function(pState, "Capacity",		tolua_HawkApi_OctetsStream_Capacity);
			tolua_function(pState, "AvailableSize",	tolua_HawkApi_OctetsStream_AvailableSize);
			tolua_function(pState, "EmptyCap",		tolua_HawkApi_OctetsStream_EmptyCap);
			tolua_function(pState, "GetPos",		tolua_HawkApi_OctetsStream_GetPos);
			tolua_function(pState, "Eos",			tolua_HawkApi_OctetsStream_Eos);
			tolua_function(pState, "Clear",			tolua_HawkApi_OctetsStream_Clear);
			tolua_function(pState, "RemoveBlank",	tolua_HawkApi_OctetsStream_RemoveBlank);
			tolua_function(pState, "MoveNonius",	tolua_HawkApi_OctetsStream_MoveNonius);
			tolua_function(pState, "Resize",		tolua_HawkApi_OctetsStream_Resize);
			tolua_function(pState, "Reserve",		tolua_HawkApi_OctetsStream_Reserve);
		}
		tolua_endmodule(pState);
	}

	Bool HawkLuaApi::Open(lua_State* pState)
	{
		tolua_open(pState);

		//注册数据类型
		tolua_reg_hawkapi_types(pState);
		
		//清空模块
		tolua_module(pState,NULL,0);
		tolua_beginmodule(pState,NULL);

		//注册函数
		tolua_reg_hawkapi_functions(pState);

		tolua_endmodule(pState);
		return true;
	}
}
#endif
