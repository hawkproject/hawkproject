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
		//变量生成属性
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

		//类生成属性
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

		//文件生成属性
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

		//常量定义
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

		//命名空间生成属性
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

		//宏生成属性
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

		//全局缓冲避免多次分配内存
		Char*				 Buffer;

		//协议ID文件
		AString				 ProtocolIdFile;

		//协议注册文件
		AString				 ProtocolRegFile;

		//协议内部命名空间
		AString				 Namespace;

		//包文件
		AString				 PackagePath;

		//协议名字队列
		AStringVector		 ProtocolVec;

		//协议ID和名字映射, 检测相同协议ID重复
		map<Int32,AString>	 ProtocolMap;

		//宏定义, 检测相同值重复
		map<Int32,Int32>	 MacroMap;

		//自定义序列化对象
		AStringMap			 MarshalMap;

		//协议ID定义
		MacroVector			 ProtocolMacro;

		//文件定义列表
		GenFileMap			 FileData;

		//宏定义列表
		GenMacroMap			 MacroData;		
		
		//////////////////////////////////////////////////////////////////////////

		//获取Vector的实际类型
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

		//获取Map的实际类型
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
