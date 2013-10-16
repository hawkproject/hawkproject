#include "HawkScriptState.h"
#include "HawkLoggerManager.h"
#include "HawkStringUtil.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

namespace Hawk
{
	static map<lua_State*, HawkScriptState*> g_LuaStateMap;

	//½Å±¾³ö´íµÄ»Øµ÷º¯Êý
	Int32 OnLuaError(lua_State* pLS)
	{
		if (!lua_isstring(pLS, -1))
			return lua_gettop(pLS);

		const Char* szErr = lua_tostring(pLS, -1);
		if (szErr)
			HawkFmtError("ScriptError: (%s).", szErr);

		lua_Debug sDebug;
		if(lua_getstack(pLS, 1, &sDebug))
		{
			lua_getinfo(pLS, "Snl", &sDebug);			
			lua_pop(pLS, 1);
		}
		return 0;
	}

	HawkScriptState::HawkScriptState(lua_State* pLS) : m_pLuaState(pLS), m_bLuaHost(false)
	{
		if (!m_pLuaState)
		{
			m_bLuaHost  = true;
			m_pLuaState = lua_open();
			luaL_openlibs(m_pLuaState);
			lua_register(m_pLuaState, "OnLuaError", OnLuaError);
		}
	}

	HawkScriptState::~HawkScriptState()
	{
		if (m_pLuaState && m_bLuaHost)
			lua_close(m_pLuaState);
		
		m_pLuaState = 0;
	}

	HawkScriptState* HawkScriptState::GetState(lua_State* pLS)
	{
		map<lua_State*, HawkScriptState*>::iterator it = g_LuaStateMap.find(pLS);
		if (it != g_LuaStateMap.end())
			return it->second;

		return 0;
	}

	lua_State*  HawkScriptState::GetLuaState() const
	{
		return m_pLuaState;
	}

	Bool  HawkScriptState::DoFile(const AString& sFile)
	{
		if (m_pLuaState)
		{
			Int32 iTop = lua_gettop(m_pLuaState);

			lua_getglobal(m_pLuaState, "OnLuaError");

			if (luaL_loadfile(m_pLuaState, sFile.c_str()))
			{
				const Char *szErr = lua_tostring(m_pLuaState, -1);

				if (szErr)
					HawkFmtError("ScriptError: (%s).", szErr);

				lua_settop(m_pLuaState, iTop);
				return false;
			}

			if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, -2))
			{
				lua_settop(m_pLuaState, iTop);
				return false;
			}

			lua_remove(m_pLuaState, iTop + 1);
			return true;
		}
		return false;
	}

	Bool  HawkScriptState::DoBuffer(const Char *pBuf, Int32 iSize, const AString& sName)
	{
		if (m_pLuaState && pBuf && iSize)
		{
			Int32 iTop = lua_gettop(m_pLuaState);

			lua_getglobal(m_pLuaState, "OnLuaError");

			if (luaL_loadbuffer(m_pLuaState, pBuf, iSize, sName.c_str()))
			{
				const Char *szErr = lua_tostring(m_pLuaState, -1);

				if (szErr)
					HawkFmtError("ScriptError: (%s).", szErr);

				lua_settop(m_pLuaState, iTop);
				return false;
			}

			if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, -2))
			{
				lua_settop(m_pLuaState, iTop);
				return false;
			}

			lua_remove(m_pLuaState, iTop+1);
			return true;
		}
		return false;
	}

	Bool HawkScriptState::DoString(const AString& sLuaCmd)
	{
		if (m_pLuaState && sLuaCmd.size())
		{
			Int32 iTop = lua_gettop(m_pLuaState);

			lua_getglobal(m_pLuaState, "OnLuaError");

			if (luaL_loadstring(m_pLuaState, sLuaCmd.c_str()))
			{
				const Char *szErr = lua_tostring(m_pLuaState, -1);

				if (szErr)
					HawkFmtError("ScriptError: (%s).", szErr);

				lua_settop(m_pLuaState, iTop);
				return false;
			}

			if(lua_pcall(m_pLuaState, 0, LUA_MULTRET, -2))
			{
				lua_settop(m_pLuaState, iTop);
				return false;
			}

			lua_remove(m_pLuaState, iTop+1);
			return true;
		}
		return false;
	}

	Bool HawkScriptState::CreateTable(const AString& sTblName)
	{
		if (m_pLuaState)
		{
			lua_createtable(m_pLuaState, 0, 0);
			lua_pushvalue(m_pLuaState, -1);
			lua_setglobal(m_pLuaState, sTblName.c_str());
			return true;
		}		
		return false;
	}

	Bool HawkScriptState::ReleaseTable(const AString& sTblName)
	{
		if(m_pLuaState)
		{
			lua_pushnil(m_pLuaState);
			lua_setglobal(m_pLuaState, sTblName.c_str());
		}
		return false;
	}

	Bool HawkScriptState::MatchType(Int32 iStackIdx, Int32 iSVT)
	{
		Int32 iType = lua_type(m_pLuaState, iStackIdx);

		switch(iSVT)
		{
		case HawkScriptValue::SVT_NUMBER :
			return iType == LUA_TNUMBER;
		case HawkScriptValue::SVT_BOOL :
			return iType == LUA_TBOOLEAN;
		case HawkScriptValue::SVT_STRING :
			return iType == LUA_TSTRING;
		case HawkScriptValue::SVT_USERDATA :
			return iType == LUA_TLIGHTUSERDATA;
		case HawkScriptValue::SVT_TABLE :
			return iType == LUA_TTABLE;
		}
		return false;
	}

	Bool HawkScriptState::Push(const HawkScriptValue& sValue)
	{
		if (sValue.IsBoolean())
		{
			lua_pushboolean(m_pLuaState, sValue.GetBoolean());
			return true;
		}
		else if (sValue.IsNumber())
		{
			lua_pushnumber(m_pLuaState, sValue.GetDouble());
			return true;
		}
		else if (sValue.IsString())
		{
			lua_pushstring(m_pLuaState, sValue.GetString().c_str());
			return true;
		}
		else if (sValue.IsUserData())
		{
			lua_pushlightuserdata(m_pLuaState, sValue.GetUserData());
			return true;
		}
		else if (sValue.IsNil())
		{
			lua_pushnil(m_pLuaState);
			return true;
		}
		else if (sValue.IsTable())
		{
			lua_newtable(m_pLuaState);

			for (Int32 i=0;i<sValue.TblGetItemCount();i++)
			{
				const HawkScriptValue* pKey = sValue.TblGetKey(i);
				const HawkScriptValue* pVal = sValue.TblGetValue(i);

				if (pKey && pVal)
				{
					Push(*pKey);
					Push(*pVal);

					lua_rawset(m_pLuaState, -3);
				}
			}
			return true;
		}
		return false;
	}

	Bool HawkScriptState::Pop(Int32 iNum)
	{
		lua_pop(m_pLuaState,iNum);
		return true;
	}

	Bool HawkScriptState::GetTable(const AString& sTblName)
	{
		lua_getglobal(m_pLuaState, sTblName.c_str());
		if (lua_type(m_pLuaState, -1) != LUA_TTABLE)
		{
			HawkFmtError("ScriptError: GetTable %s Failed.", sTblName.c_str());

			lua_pop(m_pLuaState, 1); 
			return  false;
		}

		return true;
	}

	Bool HawkScriptState::GetField(const AString& sTblName,const AString& sField)
	{
		if (sTblName.size())
		{
			if(!GetTable(sTblName))
				return false;
		}

		lua_pushstring(m_pLuaState, sField.c_str());
		lua_gettable(m_pLuaState, -2);

		return true;
	}

	Bool HawkScriptState::RawGetField(Int32 iStackIdx)
	{
		lua_rawgeti(m_pLuaState, -1, iStackIdx);
		return true;
	}

	Bool HawkScriptState::RawGetField(const AString& sField)
	{
		lua_pushstring(m_pLuaState, sField.c_str());
		lua_rawget(m_pLuaState, -2);
		return true;
	}

	Bool HawkScriptState::Next()
	{
		return (lua_next(m_pLuaState, -2) != 0);
	}

	Bool HawkScriptState::SetField(const AString& sTblName,const AString& sField,const HawkScriptValue& sValue)
	{
		if(!sValue.IsValid() || sField.size() <= 0) 
			return false;

		if (sTblName.size())
		{
			if(!GetTable(sTblName))
				return false;
		}

		lua_pushstring(m_pLuaState, sField.c_str());
		Push(sValue);
		lua_settable(m_pLuaState, -3);

		return true;
	}

	Bool HawkScriptState::SetStack(const vector<HawkScriptValue>& vResults)
	{
		for (Size_t i=0;i<vResults.size();i++)
		{
			Push(vResults[i]);
		}
		return true;
	}

	Bool HawkScriptState::Get(Int32 iStackIdx, HawkScriptValue& sValue)
	{
		Int32 iType = lua_type(m_pLuaState,iStackIdx);

		if (iType == LUA_TBOOLEAN)
		{
			Bool bVal = (lua_toboolean(m_pLuaState,iStackIdx) == 1);
			sValue = HawkScriptValue(bVal);
			return true;
		}
		else if (iType == LUA_TNUMBER)
		{
			sValue = HawkScriptValue((Double)lua_tonumber(m_pLuaState,iStackIdx));
			return true;
		}
		else if (iType == LUA_TSTRING)
		{
			sValue = HawkScriptValue((const Char*)lua_tostring(m_pLuaState,iStackIdx));
			return true;
		}
		else if (iType == LUA_TLIGHTUSERDATA)
		{
			sValue = HawkScriptValue(lua_touserdata(m_pLuaState,iStackIdx));
			return true;
		}
		else if (iType == LUA_TTABLE)
		{
			sValue.SetType(HawkScriptValue::SVT_TABLE);
			lua_pushnil(m_pLuaState);

			while (lua_next(m_pLuaState,iStackIdx) != 0)
			{
				HawkScriptValue sTmpKey, sTmpValue;
				if (Get(lua_gettop(m_pLuaState), sTmpValue))
				{
					if (Get(lua_gettop(m_pLuaState)-1, sTmpKey))
					{
						sValue.TblAddItem(sTmpKey, sTmpValue);
					}
					else
					{
						HawkAssert(false && "Get Table Key Error.");
					}
				}
				else
				{
					HawkAssert(false && "Get Table Value Error.");
				}
				lua_pop(m_pLuaState,1);
			}
			return true;
		}
		else if (iType == LUA_TNIL)
		{
			sValue.SetType(HawkScriptValue::SVT_NIL);
			return true;
		}
		return false;
	}

	Bool HawkScriptState::GetStack(vector<HawkScriptValue>& vArgs,Int32 iStartIdx)
	{
		Int32 iTop = lua_gettop(m_pLuaState);
		for (Int32 i=iStartIdx;i<=iTop;i++)
		{
			HawkScriptValue sValue;
			if (Get(i,sValue))
			{
				vArgs.push_back(sValue);
			}
		}
		return true;
	}

	Bool HawkScriptState::Register(const AString& sApiName, PScriptFunc pFunc)
	{
		if (!m_pLuaState || sApiName.size() <= 0 || !pFunc)
			return false;
	
		if (m_mApi.find(sApiName) != m_mApi.end())
			return false;
		
		m_mApi[sApiName] = ApiRegistry();
		m_mApi[sApiName].ApiPtr  = pFunc;
		m_mApi[sApiName].ApiName = sApiName;

		ApiMap::iterator it = m_mApi.find(sApiName);
		if (it != m_mApi.end())
		{
			HawkAssert("Register Api Duplicate");
			return false;
		}

		lua_register(m_pLuaState, sApiName.c_str(), pFunc);
		return true;
	}

	Bool HawkScriptState::Register(const AString& sTblName,const AString& sApiName, PScriptFunc pFunc)
	{
		if (!m_pLuaState || sTblName.size() <= 0 || sApiName.size() <= 0 || !pFunc)
			return false;

		AString sTblFunc = sTblName;
		sTblFunc += ".";
		sTblFunc += sApiName;

		if (m_mApi.find(sTblFunc) != m_mApi.end())
			return false;

		m_mApi[sTblFunc] = ApiRegistry();
		m_mApi[sTblFunc].ApiPtr  = pFunc;
		m_mApi[sTblFunc].ApiName = sApiName;
		m_mApi[sTblFunc].LibName = sTblName;

		ApiMap::iterator it = m_mApi.find(sTblFunc);

		luaL_reg luapkg[2];
		luapkg[0].name = it->second.ApiName.c_str();
		luapkg[0].func = it->second.ApiPtr;
		luapkg[1].name = 0;
		luapkg[1].func = 0;

		luaL_register(m_pLuaState, sTblName.c_str(), luapkg);
		lua_pop(m_pLuaState, 1);

		return true;
	}

	Bool HawkScriptState::PCall(const AString& sTblName, const AString& sApiName, Int32 iArgNum)
	{
		Int32 iTop = lua_gettop(m_pLuaState);

		bool bIsTableMethod = sTblName.size()>1 ? true : false;

		if (bIsTableMethod) 
		{
			if (!GetField(sTblName, sApiName))
				return false;

			if(lua_type(m_pLuaState,-1) != LUA_TFUNCTION)
			{				
				HawkFmtError("ScriptError: (%s:%s) Is Nil.", sTblName.c_str(), sApiName.c_str());

				lua_pop(m_pLuaState,1);
				return false;
			}
		}
		else 
		{
			lua_getglobal(m_pLuaState, sApiName.c_str());

			if(lua_type(m_pLuaState,-1) != LUA_TFUNCTION)
			{
				HawkFmtError("ScriptError: (%s) Is Nil.", sApiName.c_str());

				lua_pop(m_pLuaState,1);
				return false;
			}
		}


		if (lua_type(m_pLuaState, -1) != LUA_TFUNCTION)
		{
			if (bIsTableMethod)
				HawkFmtError("ScriptError: (%s:%s) Is Not A Valid Function.", sTblName.c_str(), sApiName.c_str());
			else
				HawkFmtError("ScriptError: (%s) Is Not A Valid Function.", sApiName.c_str());

			HawkAssert(false && "Not A Valid Function");
	
			lua_settop(m_pLuaState, iTop);	

			return false;
		}

		//ÐÞ¸Äº¯ÊýÐèÒªÖ´ÐÐµÄ¶ÑÕ»²ÎÊýË³Ðò
		if (bIsTableMethod) 
		{
			//¶ÑÕ»Ë³Ðò :  Top ----------> Bottom
			//            func  tbl  argn ... arg1
			//Ô¤ÆÚË³Ðò:   argn ... arg1  tbl  func
			//½»»»Ë³Ðò:   tbl and func
			lua_insert(m_pLuaState, -2);   
			if (iArgNum > 0)
			{
				lua_insert(m_pLuaState, -2-iArgNum);  
				lua_insert(m_pLuaState, -2-iArgNum);
			}
		}
		else 
		{
			//¶ÑÕ»Ë³Ðò :  Top ----------> Bottom
			//            func  tbl  argn ... arg1
			//Ô¤ÆÚË³Ðò:   argn ... arg1  tbl  func
			if (iArgNum > 0)
			{
				lua_insert(m_pLuaState, -1-iArgNum );
			}
		}


		Int32 iTmpTop = 0, iErrFuncIdx = 0;
		iTmpTop = lua_gettop(m_pLuaState);

		lua_getglobal(m_pLuaState, "OnLuaError");

		if (bIsTableMethod) 
		{
			//ÐÞ¸Ä¶ÑÕ»Ë³Ðò: argn ... arg1 tbl func errfunc
			lua_insert(m_pLuaState, -3-iArgNum);
			iErrFuncIdx = -3 - iArgNum + lua_gettop(m_pLuaState) + 1;

			if (lua_pcall(m_pLuaState, iArgNum+1, LUA_MULTRET, -3-iArgNum))
			{
				lua_settop(m_pLuaState, iErrFuncIdx-1);
				return false;
			}
		}
		else 
		{
			//ÐÞ¸Ä¶ÑÕ»Ë³Ðò: argn ... arg1 tbl func errfunc
			lua_insert(m_pLuaState, -2-iArgNum);			
			iErrFuncIdx = -2 - iArgNum + lua_gettop(m_pLuaState) + 1;

			if (lua_pcall(m_pLuaState, iArgNum, LUA_MULTRET, -2-iArgNum))
			{
				lua_settop(m_pLuaState, iErrFuncIdx-1);
				return false;
			}
		}

		//ÒÆ³ý´íÎóÐÅÏ¢º¯Êý
		lua_remove(m_pLuaState, iErrFuncIdx);

		return true;
	}

	Bool  HawkScriptState::Call(const AString& sTblFunc, vector<HawkScriptValue>& vArgs, vector<HawkScriptValue>& vResult)
	{
		if (sTblFunc.size() <= 0)
			return false;

		Int32 iTop = lua_gettop(m_pLuaState);
		SetStack(vArgs);

		AStringVector vString;
		HawkStringUtil::Split<AString>(sTblFunc,vString,":");
		AString sTblName, sFuncName;
		if (vString.size()>1)
		{
			sTblName = vString[0];
			sFuncName  = vString[1];
		}
		else
		{
			sFuncName = vString[0];
		}

		if (!PCall(sTblName,sFuncName, (Int32)vArgs.size()))
		{
			lua_settop(m_pLuaState,iTop);
			return false;
		}

		GetStack(vResult,iTop+1);
		lua_settop(m_pLuaState,iTop);

		return true;
	}
}
