#ifndef HAWK_SCRIPTSTATE_H
#define HAWK_SCRIPTSTATE_H

#include "HawkScriptValue.h"

struct lua_State;

namespace Hawk
{
	/************************************************************************/
	/* LuaState状态封装                                                     */
	/************************************************************************/
	class UTIL_API HawkScriptState : public HawkRefCounter
	{
	public:
		//构造
		HawkScriptState(lua_State* pLS = 0);

		//析构
		virtual ~HawkScriptState();
	
	public:
		typedef Int32 (*PScriptFunc)(lua_State* pLS);

		static HawkScriptState* GetState(lua_State* pLS);

		struct ApiRegistry
		{
			AString     LibName;
			AString     ApiName;
			PScriptFunc ApiPtr;

			ApiRegistry()
			{
				LibName = "";
				ApiName = "";
				ApiPtr  = 0;
			}
		};

		typedef map<AString, ApiRegistry> ApiMap;

	public:		
		//执行文件
		virtual Bool    DoFile(const AString& sFile);

		//执行Buffer
		virtual Bool    DoBuffer(const Char *pBuf, Int32 iSize, const AString& sName);

		//执行字符串明了
		virtual Bool    DoString(const AString& sLuaCmd);	

		//注册函数
		virtual Bool    Register(const AString& sApiName, PScriptFunc pFunc);

		//创建Table
		virtual Bool    CreateTable(const AString& sTblName);

		//释放Table
		virtual Bool    ReleaseTable(const AString& sTblName);

		//注册函数
		virtual Bool    Register(const AString& sTblName,const AString& sApiName,PScriptFunc pFunc);

		//调用脚本函数
		virtual Bool    Call(const AString& sTblFunc, vector<HawkScriptValue>& vArgs, vector<HawkScriptValue>& vResult);

	protected:
		//设置栈数据
		virtual Bool    SetStack(const vector<HawkScriptValue>& vResults);

		//获取栈数据
		virtual Bool    GetStack(vector<HawkScriptValue>& vArgs,Int32 iStartIdx);		
		
		//判断类型匹配
		virtual Bool    MatchType(Int32 iStackIdx, Int32 iSVT);

		//数据压栈
		virtual Bool	Push(const HawkScriptValue& sValue);

		//弹出数据
		virtual Bool    Pop(Int32 iNum);

		//获取堆栈数据
		virtual Bool    Get(Int32 iStackIdx, HawkScriptValue& sValue);

		//获取表数据
		virtual Bool    GetTable(const AString& sTblName);

		//获取指定域数据
		virtual Bool    GetField(const AString& sTblName,const AString& sField);

		//获取堆栈指定索引数据
		virtual Bool	RawGetField(Int32 iStackIdx);

		//获取堆栈指定名称数据
		virtual Bool    RawGetField(const AString& sField);

		//往下继续
		virtual Bool    Next();

		//设置表的指定名称对应的值数据
		virtual Bool    SetField(const AString& sTblName, const AString& sField, const HawkScriptValue& sValue);			

		//函数调用
		virtual Bool    PCall(const AString& sTblName, const AString& sApiName, Int32 iArgNum);			

		//获取lua句柄
		lua_State*		GetLuaState() const;

	protected:
		//lua句柄
		lua_State*   m_pLuaState;
		//内部句柄
		Bool		 m_bLuaHost;
		//注册的函数表
		ApiMap		 m_mApi;	
	};

//脚本函数定义
#define IMPLEMENT_SCRIPT_API(Api)\
	Int32 Hawk_##Api(lua_State* pLS)\
	{\
		HawkScriptState* pState = HawkScriptState::GetState(pLS);\
		if (pState)\
		{\
			vector<HawkScriptValue> vArgs,vResults;\
			pState->GetStack(vArgs, 1);\
			Api(vArgs, vResults);\
			pState->SetStack(vResults);\
			return vResults.size();\
		}\
		return 0 ;\
	}

//注册全局函数
#define REGISTER_SCRIPT_API(State, Api)\
	{\
		State->Register(#Api, Hawk_##Api);\
	}

//注册库函数
#define REGISTER_SCRIPT_LIBAPI(State,Lib, Api)\
	{\
		State->Register(Lib, #Api, Hawk_##Api);\
	}
}
#endif
