#ifndef HAWK_SCRIPTSTATE_H
#define HAWK_SCRIPTSTATE_H

#include "HawkScriptValue.h"

struct lua_State;

namespace Hawk
{
	/************************************************************************/
	/* LuaState״̬��װ                                                     */
	/************************************************************************/
	class UTIL_API HawkScriptState : public HawkRefCounter
	{
	public:
		//����
		HawkScriptState(lua_State* pLS = 0);

		//����
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
		//ִ���ļ�
		virtual Bool    DoFile(const AString& sFile);

		//ִ��Buffer
		virtual Bool    DoBuffer(const Char *pBuf, Int32 iSize, const AString& sName);

		//ִ���ַ�������
		virtual Bool    DoString(const AString& sLuaCmd);	

		//ע�ắ��
		virtual Bool    Register(const AString& sApiName, PScriptFunc pFunc);

		//����Table
		virtual Bool    CreateTable(const AString& sTblName);

		//�ͷ�Table
		virtual Bool    ReleaseTable(const AString& sTblName);

		//ע�ắ��
		virtual Bool    Register(const AString& sTblName,const AString& sApiName,PScriptFunc pFunc);

		//���ýű�����
		virtual Bool    Call(const AString& sTblFunc, vector<HawkScriptValue>& vArgs, vector<HawkScriptValue>& vResult);

	protected:
		//����ջ����
		virtual Bool    SetStack(const vector<HawkScriptValue>& vResults);

		//��ȡջ����
		virtual Bool    GetStack(vector<HawkScriptValue>& vArgs,Int32 iStartIdx);		
		
		//�ж�����ƥ��
		virtual Bool    MatchType(Int32 iStackIdx, Int32 iSVT);

		//����ѹջ
		virtual Bool	Push(const HawkScriptValue& sValue);

		//��������
		virtual Bool    Pop(Int32 iNum);

		//��ȡ��ջ����
		virtual Bool    Get(Int32 iStackIdx, HawkScriptValue& sValue);

		//��ȡ������
		virtual Bool    GetTable(const AString& sTblName);

		//��ȡָ��������
		virtual Bool    GetField(const AString& sTblName,const AString& sField);

		//��ȡ��ջָ����������
		virtual Bool	RawGetField(Int32 iStackIdx);

		//��ȡ��ջָ����������
		virtual Bool    RawGetField(const AString& sField);

		//���¼���
		virtual Bool    Next();

		//���ñ��ָ�����ƶ�Ӧ��ֵ����
		virtual Bool    SetField(const AString& sTblName, const AString& sField, const HawkScriptValue& sValue);			

		//��������
		virtual Bool    PCall(const AString& sTblName, const AString& sApiName, Int32 iArgNum);			

		//��ȡlua���
		lua_State*		GetLuaState() const;

	protected:
		//lua���
		lua_State*   m_pLuaState;
		//�ڲ����
		Bool		 m_bLuaHost;
		//ע��ĺ�����
		ApiMap		 m_mApi;	
	};

//�ű���������
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

//ע��ȫ�ֺ���
#define REGISTER_SCRIPT_API(State, Api)\
	{\
		State->Register(#Api, Hawk_##Api);\
	}

//ע��⺯��
#define REGISTER_SCRIPT_LIBAPI(State,Lib, Api)\
	{\
		State->Register(Lib, #Api, Hawk_##Api);\
	}
}
#endif
