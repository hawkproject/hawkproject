#include "HawkProtocolManager.h"
#include "HawkLoggerManager.h"
#include "HawkOSOperator.h"
#include "HawkXmlFile.h"
#include "HawkScope.h"

//1M
#ifndef MAX_BUFFER
#	define MAX_BUFFER 1048576
#endif

namespace Hawk
{
	enum
	{
		TYPE_UNKNOWN_CLASS = 0,
		TYPE_MARSHAL_CLASS,
		TYPE_PROTOCOL_CLASS,
	};

	struct ProtocolSchema 
	{
		//������������
		struct GenVarData
		{
			AString  Name;
			AString  Type;
			AString  Default;

			GenVarData()
			{
				Name    = "";
				Type    = "";
				Default = "";
			};

			Bool IsValid() const
			{
				return Name.size() && Type.size();
			}
			
		};
		typedef vector<GenVarData> VarDataVec;

		//����������
		struct GenClassData
		{
			Int32		ClassType;
			AString		ClassName;
			AString		ClassDesc;		
			Int32		Identify;
			AString		Macro;
			VarDataVec  VarData;
			Int32		Drive;
			AString		Usage;
			AString		States;

			struct 
			{
				AString ConstructFunc;
				AString CopyFunc;
				AString CloneFunc;
				AString OperatorSelfFunc;
				AString MarshalFunc;
				AString UnmarshalFunc;	
				AString ClearFunc;
				AString MemberDefine;
			}cpp;

			struct 
			{
				AString ConstructFunc;
				AString CopyFunc;
				AString CloneFunc;
				AString OperatorSelfFunc;
				AString MarshalFunc;
				AString UnmarshalFunc;
				AString ClearFunc;
				AString MemberDefine;
				AString ImportDefine;
			}java;

			struct  
			{
				AString LuaDesc;
				AString LuaMember;
				AString LuaClone;
				AString LuaMarshal;
				AString LuaUnmarshal;
			}lua;

			GenClassData()
			{
				ClassType	= 0;
				ClassName	= "";
				ClassDesc	= "";
				Identify	= 0;
				Macro		= "";
				Drive		= 0;
				Usage		= "";	
				States		= "";
			}
		};
		typedef vector<GenClassData> ClassVector;

		//�ļ���������
		struct GenFileData
		{
			AString		Name;
			AString		Include;
			ClassVector Classes;

			GenFileData()
			{
				Name	= "";
				Classes.clear();
			}
		};

		//��������
		struct MacroDefine 
		{
			AString MacroName;
			AString MacroValue;
			AString MacroDesc;

			MacroDefine()
			{
				MacroName = "";
				MacroValue = "";
				MacroDesc = "";
			}

			Bool IsValid()
			{
				return MacroName.size() && MacroValue.size();
			}
		};
		typedef vector<MacroDefine> MacroVector;

		//�����ռ���������
		struct GenSpaceData
		{
			AString		Space;
			AString		Desc;
			MacroVector	Macros;

			GenSpaceData()
			{
				Space = "";
				Desc  = "";
				Macros.clear();
			}

			Bool  IsValid()
			{
				return Space.size() != 0;
			}
		};
		typedef vector<GenSpaceData> SpaceVector;		

		//����������
		struct GenMacroData
		{
			AString		Name;
			SpaceVector Spaces;

			GenMacroData()
			{
				Name = "";
				Spaces.clear();
			}
		};

		typedef map<AString, vector<GenMacroData> > GenMacroMap;
		typedef map<AString, vector<GenFileData> >  GenFileMap;

		ProtocolSchema() : Buffer(0)
		{
			Buffer = new Char[MAX_BUFFER];
			memset(Buffer, 0 ,MAX_BUFFER);
		}

		~ProtocolSchema()
		{
			HAWK_DELETE_ARRAY(Buffer);
		}

		//ȫ�ֻ�������η����ڴ�
		Char*				 Buffer;

		//Э��ID�ļ�
		AString				 ProtocolIdFile;

		//Э��ע���ļ�
		AString				 ProtocolRegFile;

		//Э���ڲ������ռ�
		AString				 Namespace;

		//���ļ�
		AString				 PackagePath;

		//Э�����ֶ���
		AStringVector		 ProtocolVec;

		//Э��ID������ӳ��, �����ͬЭ��ID�ظ�
		map<Int32,AString>	 ProtocolMap;

		//�궨��, �����ֵͬ�ظ�
		map<Int32,Int32>	 MacroMap;

		//�Զ������л�����
		AStringMap			 MarshalMap;

		//Э��ID����
		MacroVector			 ProtocolMacro;

		//�ļ������б�
		GenFileMap			 FileData;

		//�궨���б�
		GenMacroMap			 MacroData;		
		
		//////////////////////////////////////////////////////////////////////////

		//��ȡVector��ʵ������
		static AString GetVectorItemType(const AString& sType)
		{
			if (sType.find("vector") == 0)
			{
				Size_t iStart = sType.find_first_of("<");
				Size_t iEnd   = sType.find_last_of(">");
				if (iStart != AString::npos && iEnd != AString::npos && iEnd > iStart)
				{
					AString sItemType = sType.substr(iStart + 1, iEnd - iStart-1);
					HawkStringUtil::Trim<AString>(sItemType);
					return sItemType;
				}
			}
			return "";
		}

		//��ȡMap��ʵ������
		static std::pair<AString, AString> GetMapItemType(const AString& sType)
		{
			if (sType.find("map") == 0)
			{
				Size_t iStart = sType.find_first_of("<");
				Size_t iEnd   = sType.find_last_of(">");
				if (iStart != AString::npos && iEnd != AString::npos && iEnd > iStart)
				{
					AString sKV = sType.substr(iStart + 1, iEnd - iStart - 1);
					AStringVector asv;
					HawkStringUtil::Split<AString>(sKV, asv, ",");
					HawkStringUtil::Trim<AString>(asv[0]);
					HawkStringUtil::Trim<AString>(asv[1]);
					if (asv[0].size() && asv[1].size())
						return std::make_pair(asv[0], asv[1]);
				}			
			}
			return std::make_pair("", "");
		}
	};
}
