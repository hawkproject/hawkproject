#include "HawkProtocolManager.h"
#include "HawkLoggerManager.h"
#include "HawkOSOperator.h"
#include "HawkXmlFile.h"
#include "HawkScope.h"

/*
<!-- ===========================协议格式定义===========================	-->
<!-- //xml special words:(&, &amp;),(<, &lt;),(>, &gt;),(", &quot;)		-->
<!-- Name: 变量名称, Type: 变量类型, Default: 构造默认值				-->
*/

//////////////////////////////////////////////////////////////////////////
//1M
#define MAX_BUFFER		1048576

#define MARSHALDATA_CLASS	"\tclass %s : public MarshalData\r\n\
\t{\r\n\
\tpublic:\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n%s\
\t};\r\n\r\n"

#define PROTOCOL_CLASS	"\tclass %s : public HawkProtocol\r\n\
\t{\r\n\
\tpublic:\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n\
\r\n\
\t	%s\r\n%s\
\t};\r\n\r\n"

#define LUA_SCHEMA "-----------------------------------------------------------%s\r\n\
%s = \r\n\
{\r\n\
%s\r\n\
}\r\n\r\n\
function %s:clone()\r\n\
	local inst_tbl = CopyTable(self);%s\r\n\
	return inst_tbl;\r\n\
end\r\n\r\n\
function %s:marshal(oct)\r\n\
%s\r\n\
end\r\n\r\n\
function %s:unmarshal(oct)\r\n\
%s\r\n\
end\r\n\r\n"

enum
{
	TYPE_UNKNOWN_CLASS = 0,
	TYPE_MARSHAL_CLASS,
	TYPE_PROTOCOL_CLASS,
};

//////////////////////////////////////////////////////////////////////////

namespace Hawk
{
#ifdef LUA_PROTOCOL
	Bool	g_EnableLua		= true;
#else
	Bool	g_EnableLua		= false;
#endif

	//全局缓冲, 避免多次分配内存
	Char*   g_Buffer = 0;

	//协议ID文件和协议注册文件
	AString g_ProtocolIdFile  = "";
	AString g_ProtocolRegFile = "";
	
	//协议名字队列
	AStringVector		g_ProtocolVec;
	//协议ID和名字映射, 检测相同协议ID重复
	map<Int32,AString>	g_ProtocolMap;
	//宏定义, 检测相同值重复
	map<Int32,Int32>	g_MacroMap;
	//自定义序列化对象
	AStringMap			g_MarshalMap;

	Bool IsValueType(const AString& sType)
	{
		return sType == "Int8" ||
			sType == "UInt8" ||
			sType == "Int16" ||
			sType == "UInt16" ||
			sType == "Int32" ||
			sType == "UInt32" ||
			sType == "Int64" || 
			sType == "UInt64" ||
			sType == "Float" ||
			sType == "Double" ||
			sType == "Bool" ||
			sType == "Char" ||
			sType == "UChar" ||
			sType == "Utf8" ||
			sType == "WChar";
	};

	Bool IsLuaBaseType(const AString& sType)
	{
		return sType == "Int8" ||
			sType == "UInt8" ||
			sType == "Int16" ||
			sType == "UInt16" ||
			sType == "Int32" ||
			sType == "UInt32" ||
			sType == "Int64" || 
			sType == "UInt64" ||
			sType == "Float" ||
			sType == "Double" ||
			sType == "Bool" ||
			sType == "AString" ||
			sType == "UString" ||
			sType == "HawkOctets";
	};

	AString GetVectorItemType(const AString& sType)
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

	std::pair<AString, AString> GetMapItemType(const AString& sType)
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

	AString GetLuaTypeDefault(const AString& sType, const AString& sDefault)
	{
		if (!sDefault.size() && IsLuaBaseType(sType))
		{
			if (sType == "AString" || sType == "UString")
			{
				return AString("\"") + sDefault + AString("\"");
			}
			return sDefault;
		}

		if (sType == "Int8" || sType == "UInt8" ||
			sType == "Int16" || sType == "UInt16" ||
			sType == "Int32" || sType == "UInt32" ||
			sType == "Int64" || sType == "UInt64" ||
			sType == "Float" || sType == "Double")
			return "0";

		if (sType == "Bool")
			return "false";

		if (sType == "AString" || sType == "UString")
			return "\"\"";

		if (sType == "HawkOctets")
			return "nil";

		if (g_MarshalMap.find(sType) != g_MarshalMap.end())
			return sType;

		return "{ }";
	}

	struct GenVarData
	{
		AString Name;
		AString Type;
		AString Default;

		GenVarData()
		{
			Name = "";
			Type = "";
			Default = "";
		};

		Bool IsValid() const
		{
			return Name.size() && Type.size();
		}

		Bool CheckType()
		{
			if (Type.size() == 0)
			{
				if (Name.find("m_i") == 0)
				{
					Type = "Int32";
				}
				else if (Name.find("m_f") == 0)
				{
					Type = "Float";
				}
				else if (Name.find("m_db") == 0)
				{
					Type = "Double";
				}
				else if (Name.find("m_b") == 0)
				{
					Type = "Bool";
				}
				else if (Name.find("m_s") == 0)
				{
					Type = "AString";
				}
			}

			return true;
		}

		AString  GetMemberFmt()
		{
			AString sValue = Type;
			sValue += " ";
			sValue += Name;
			sValue += ";";
			return sValue;
		}

		AString GetConstructFmt()
		{
			AString sConstructName = AString("_") + Name;
			if(Name.find("m_") != AString::npos)
				sConstructName = Name.substr(Name.find("m_")+2, Name.size()-2);

			AString sConstructType = Type;
			if (!IsValueType(Type))
				sConstructType = AString("const ") + Type + AString("&");

			if (Default.size() == 0)
			{
				if (IsValueType(Type))
					Default = "0";
				else
					Default = Type + "()";
			}

			AString sValue = sConstructType;
			sValue += " ";
			sValue += sConstructName;
			if (Default.size())
			{
				sValue += " = ";
				sValue += Default;
			}
			return sValue;
		}

		AString GetInitFmt()
		{
			AString sConstructName = AString("_") + Name;
			if(Name.find("m_") != AString::npos)
				sConstructName = Name.substr(Name.find("m_")+2 ,Name.size()-2);
			
			AString sValue = Name;
			sValue += "(";
			sValue += sConstructName;
			sValue += ")";
			return sValue;
		}

		AString GetCopyFmt()
		{
			AString sValue = Name;
			sValue += "(rhs.";
			sValue += Name;
			sValue += ")";
			return sValue;
		}

		AString GetSetValFunc()
		{
			AString sValue = Name;
			sValue += " = ";
			sValue += "rhs.";
			sValue += Name;
			sValue += ";";
			return sValue;
		}
	};

	struct GenClassData
	{
		Int32   ClassType;
		AString ClassName;
		AString ClassDesc;		
		Int32	Identify;		
		AString	Macro;
		Int32	Drive;
		AString	Usage;

		AString ConstructFunc;
		AString CopyFunc;
		AString CloneFunc;
		AString OperatorSelfFunc;
		AString MarshalFunc;
		AString UnmarshalFunc;		
		AString MemberDefine;
		vector<GenVarData> VarData;

		AString LuaDesc;
		AString LuaMember;
		AString LuaClone;
		AString LuaMarshal;
		AString LuaUnmarshal;

		GenClassData()
		{
			ClassType		    = 0;
			ClassName			= "";
			ClassDesc			= "";
			Identify			= 0;
			Macro				= "";
			Drive				= 0;
			Usage				= "";
			ConstructFunc		= "";
			CopyFunc			= "";
			CloneFunc			= "";
			OperatorSelfFunc	= "";
			MarshalFunc			= "";
			UnmarshalFunc		= "";
			MemberDefine		= "";

			LuaDesc				= "";
			LuaMember			= "";
			LuaClone			= "";
			LuaMarshal			= "";
			LuaUnmarshal		= "";
		}

		Bool  BuildConstructFunc()
		{
			ConstructFunc = "";
			ConstructFunc += ClassName;
			ConstructFunc += "(";
			for (Size_t i=0;i<VarData.size();i++)
			{
				GenVarData& xVar = VarData[i];
				if (i != 0)
					ConstructFunc += ", ";
				ConstructFunc += xVar.GetConstructFmt();
			}

			ConstructFunc += ")";
			if (ClassType != TYPE_MARSHAL_CLASS || VarData.size())
				ConstructFunc += " : ";

			if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				ConstructFunc += "HawkProtocol(";
				ConstructFunc += g_ProtocolIdFile;
				ConstructFunc += "::";
				ConstructFunc += Macro;
				if (Drive > 0)
				{
					ConstructFunc += ", ";
					ConstructFunc += HawkStringUtil::IntToString<AString>(Drive);
				}

				ConstructFunc += ")";
				if(VarData.size())
					ConstructFunc += ", ";
			}

			for (Size_t i=0;i<VarData.size();i++)
			{
				GenVarData& xVar = VarData[i];
				if (i != 0)
					ConstructFunc += ", ";
				ConstructFunc += xVar.GetInitFmt();
			}

			ConstructFunc += "\r\n\t\t{\r\n";
			ConstructFunc += "\t\t};";
			return true;
		}

		Bool  BuildCopyFunc()
		{
			CopyFunc = "";
			if(ClassType == TYPE_MARSHAL_CLASS)
			{
				CopyFunc += ClassName;
				CopyFunc += "(const ";
				CopyFunc += ClassName;
				CopyFunc += "& rhs)";
				if (VarData.size())
					CopyFunc += " : ";

				for (Size_t i=0;i<VarData.size();i++)
				{
					GenVarData& xVar = VarData[i];
					if (i != 0)
						CopyFunc += ", ";
					CopyFunc += xVar.GetCopyFmt();
				}
				CopyFunc += "\r\n\t\t{\r\n\t\t};";
			}			
			return true;
		}

		Bool  BuildCloneFunc()
		{
			AString sFmtValue = "";
			sFmtValue += "virtual ";

			if(ClassType == TYPE_MARSHAL_CLASS)
			{
				sFmtValue += "MarshalData* Clone() const\r\n\t\t{\r\n\t\t\treturn new ";
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				sFmtValue += "HawkProtocol* Clone() const\r\n\t\t{\r\n\t\t\treturn new ";
			}

			sFmtValue += ClassName;
			sFmtValue += ";\r\n\t\t};";

			CloneFunc = sFmtValue;
			return CloneFunc.size() != 0;
		}

		Bool  BuildOperatorSelfFunc()
		{
			OperatorSelfFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				OperatorSelfFunc = "virtual ";
				OperatorSelfFunc += ClassName;
				OperatorSelfFunc += "& operator = (const ";
				OperatorSelfFunc += ClassName;
				OperatorSelfFunc += "& rhs)\r\n\t\t{\r\n\t\t\tif(this != &rhs)\r\n\t\t\t{\r\n";

				for (Size_t i=0;i<VarData.size();i++)
				{
					GenVarData& xVar = VarData[i];
					OperatorSelfFunc += "\t\t\t\t";
					OperatorSelfFunc += xVar.GetSetValFunc();
					OperatorSelfFunc += "\r\n";
				}
				
				OperatorSelfFunc += "\t\t\t}\r\n\t\t\treturn *this;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildMarshalFunc()
		{
			MarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				MarshalFunc = "virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)\r\n\t\t{\r\n";
				if (VarData.size())
				{
					MarshalFunc += "\t\t\trhsOS";
					for (Size_t i=0;i<VarData.size();i++)
					{
						MarshalFunc += " << ";
						MarshalFunc += VarData[i].Name;
					}
					MarshalFunc += ";\r\n";
				}
				MarshalFunc += "\t\t\treturn rhsOS;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildUnmarshalFunc()
		{
			UnmarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				UnmarshalFunc = "virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)\r\n\t\t{\r\n";
				if (VarData.size())
				{
					UnmarshalFunc += "\t\t\trhsOS";
					for (Size_t i=0;i<VarData.size();i++)
					{
						UnmarshalFunc += " >> ";
						UnmarshalFunc += VarData[i].Name;
					}
					UnmarshalFunc += ";\r\n";
				}
				UnmarshalFunc += "\t\t\treturn rhsOS;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildClassMember()
		{
			MemberDefine = "";
			if (VarData.size())
			{
				MemberDefine += "\r\n\tpublic:\r\n";
				for (Size_t i=0;i<VarData.size();i++)
				{
					GenVarData& xVar = VarData[i];
					MemberDefine += "\t\t";
					MemberDefine += xVar.GetMemberFmt();
					MemberDefine += "\r\n";
				}
			}
			return true;
		}

		Bool  BuildFunc()
		{
			BuildConstructFunc();
			BuildCopyFunc();
			BuildCloneFunc();
			BuildOperatorSelfFunc();
			BuildMarshalFunc();
			BuildUnmarshalFunc();
			BuildClassMember();
			return true;
		}

		Bool BulidLua()
		{
			//添加协议类型
			memset(g_Buffer,0,MAX_BUFFER);				
			sprintf(g_Buffer,"\tm_iType = %s.%s,",
				g_ProtocolIdFile.c_str(), Macro.c_str());
			LuaMember = g_Buffer;

			for (Size_t i=0;i<VarData.size();i++)
			{
				GenVarData& xVar = VarData[i];

				LuaMember += "\r\n";
				if (i != 0)
				{					
					LuaMarshal += "\r\n";
					LuaUnmarshal += "\r\n";
				}

				memset(g_Buffer,0,MAX_BUFFER);				
				sprintf(g_Buffer,"\t%s = %s,",
					xVar.Name.c_str(), GetLuaTypeDefault(xVar.Type, xVar.Default).c_str());
				LuaMember += g_Buffer;

				if (IsLuaBaseType(xVar.Type))
				{
					//Marshal基础类型
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\toct:Push%s(self.%s);",
						xVar.Type.c_str(), xVar.Name.c_str());
					LuaMarshal += g_Buffer;

					//Unmarshal基础类型
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\tself.%s = oct:Pop%s();",
						xVar.Name.c_str(),xVar.Type.c_str());
					LuaUnmarshal += g_Buffer;
				}
				else if (xVar.Type.find("vector") == 0)
				{
					AString sItemType = GetVectorItemType(xVar.Type);

					//Marshal vector的元素个数
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\toct:PushUInt32(TableSize(self.%s));\r\n",
						xVar.Name.c_str());

					if (LuaMarshal.size())
						LuaMarshal += "\r\n";
					LuaMarshal += g_Buffer;

					//Marshal vector的实际元素
					memset(g_Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sItemType))
					{
						sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(v);\r\n\tend",
							xVar.Name.c_str(), sItemType.c_str());
						LuaMarshal += g_Buffer;
					}
					else
					{
						sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tv:marshal(oct);\r\n\tend",
							xVar.Name.c_str());
						LuaMarshal += g_Buffer;
					}

					//Unmarshal vector的元素个数
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\tlocal tbl_sz_%s = oct:PopUInt32();\r\n",
						xVar.Name.c_str());

					if (LuaUnmarshal.size())
						LuaUnmarshal += "\r\n";
					LuaUnmarshal += g_Buffer;

					//Unmarshal vector的实际元素
					memset(g_Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sItemType))
					{
						sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tself.%s[i] = oct:Pop%s();\r\n\tend",
							xVar.Name.c_str(), xVar.Name.c_str(), sItemType.c_str());
						LuaUnmarshal += g_Buffer;
					}
					else
					{
						sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tself.%s[i] = %s:clone();\r\n\t\tself.%s[i].unmarshal(oct);\r\n\tend",
							xVar.Name.c_str(), xVar.Name.c_str(), sItemType.c_str(), xVar.Name.c_str());
						LuaUnmarshal += g_Buffer;
					}					
				}
				else if (xVar.Type.find("map") == 0)
				{
					std::pair<AString, AString> sKV = GetMapItemType(xVar.Type);

					//Marshal map的元素个数
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\toct:PushUInt32(TableSize(self.%s));\r\n",
						xVar.Name.c_str());
					
					if (LuaMarshal.size())
						LuaMarshal += "\r\n";
					LuaMarshal += g_Buffer;

					//Marshal map的实际元素
					memset(g_Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sKV.first))
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(k);\r\n\t\toct:Push%s(v);\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str());
							LuaMarshal += g_Buffer;
						}
						else
						{
							sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(k);\r\n\t\tv:marshal(oct);\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str());
							LuaMarshal += g_Buffer;
						}						
					}
					else
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tk:marshal(oct);\r\n\t\toct:Push%s(v);\r\n\tend",
								xVar.Name.c_str(), sKV.second.c_str());
							LuaMarshal += g_Buffer;
						}
						else
						{
							sprintf(g_Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tk:marshal(oct);\r\n\t\tv:marshal(oct);\r\n\tend",
								xVar.Name.c_str());
							LuaMarshal += g_Buffer;
						}						
					}

					//Unmarshal map的元素个数
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\tlocal tbl_sz_%s = oct:PopUInt32();\r\n",
						xVar.Name.c_str());

					if (LuaUnmarshal.size())
						LuaUnmarshal += "\r\n";
					LuaUnmarshal += g_Buffer;

					//Unmarshal map的实际元素
					memset(g_Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sKV.first))
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = oct:Pop%s();\r\n\t\tlocal v = oct:Pop%s();\r\n\t\tself.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(), xVar.Name.c_str());
							LuaUnmarshal += g_Buffer;
						}
						else
						{
							sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = oct:Pop%s();\r\n\t\tlocal v = %s:clone();\r\n\t\tv:unmarshal(oct);\r\n\t\tself.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(), xVar.Name.c_str());
							LuaUnmarshal += g_Buffer;
						}						
					}
					else
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = %s:clone();\r\n\t\tk:unmarshal(oct);\r\n\t\tlocal v = oct:Pop%s();self.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(),xVar.Name.c_str());
							LuaUnmarshal += g_Buffer;
						}
						else
						{
							sprintf(g_Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = %s:clone();\r\n\t\tk:unmarshal(oct);\r\n\t\tlocal v = %s:clone();\r\n\t\tv:unmarshal(oct);self.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(),xVar.Name.c_str());
							LuaUnmarshal += g_Buffer;
						}						
					}
				}
				else if (g_MarshalMap.find(xVar.Type) != g_MarshalMap.end())
				{
					//Marshal基础类型
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\tself.%s:marshal(oct);",xVar.Name.c_str());

					if (LuaMarshal.size())
						LuaMarshal += "\r\n";
					LuaMarshal += g_Buffer;

					//Unmarshal基础类型
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\tself.%s = %s:clone();\r\n\tself.%s:unmarshal(oct);",xVar.Name.c_str(),xVar.Type.c_str(),xVar.Name.c_str());
					
					if (LuaUnmarshal.size())
						LuaUnmarshal += "\r\n";
					LuaUnmarshal += g_Buffer;

					//克隆时重新构造新对象					
					memset(g_Buffer,0,MAX_BUFFER);	
					sprintf(g_Buffer,"\r\n\tinst_tbl.%s = self.%s:clone();",xVar.Name.c_str(),xVar.Name.c_str());
					LuaClone += g_Buffer;
				}
				else
				{
					HawkFmtPrint("未定义的数据类型: %s",xVar.Type.c_str());
					HawkAssert(false && "Unknown VarType.");					
					HawkOSOperator::OSSleep(10000);
				}
			}

			return true;
		}

		AString  FormatString()
		{
			if (!BuildFunc()) return "";

			AString sFmtValue = "";
			memset(g_Buffer,0,MAX_BUFFER);
			
			if (ClassDesc.size())
			{
				memset(g_Buffer,0,MAX_BUFFER);
				sprintf(g_Buffer,"\t//%s\r\n",ClassDesc.c_str());
				sFmtValue += g_Buffer;				
			}

			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				memset(g_Buffer,0,MAX_BUFFER);
				sprintf(g_Buffer,MARSHALDATA_CLASS,ClassName.c_str(),
					ConstructFunc.c_str(),
					CopyFunc.c_str(),
					CloneFunc.c_str(),
					OperatorSelfFunc.c_str(),
					MarshalFunc.c_str(),
					UnmarshalFunc.c_str(),
					MemberDefine.c_str());

				sFmtValue += g_Buffer;
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				memset(g_Buffer,0,MAX_BUFFER);
				sprintf(g_Buffer,PROTOCOL_CLASS,ClassName.c_str(),
					ConstructFunc.c_str(),
					CloneFunc.c_str(),
					MarshalFunc.c_str(),
					UnmarshalFunc.c_str(),
					MemberDefine.c_str());

				sFmtValue += g_Buffer;
			}
		
			return sFmtValue;
		}

		AString  FormatLuaString()
		{
			if (!BulidLua()) return "";

			AString sFmtValue = "";
			memset(g_Buffer,0,MAX_BUFFER);

			if (ClassDesc.size())
			{
				memset(g_Buffer,0,MAX_BUFFER);
				sprintf(g_Buffer,"\r\n--%s",ClassDesc.c_str());
				LuaDesc = g_Buffer;				
			}

			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				memset(g_Buffer,0,MAX_BUFFER);
				
				sprintf(g_Buffer,LUA_SCHEMA,LuaDesc.c_str(),
					ClassName.c_str(),
					LuaMember.c_str(),
					ClassName.c_str(),
					LuaClone.c_str(),
					ClassName.c_str(),
					LuaMarshal.c_str(),
					ClassName.c_str(),
					LuaUnmarshal.c_str());

				sFmtValue += g_Buffer;
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				memset(g_Buffer,0,MAX_BUFFER);
				
				sprintf(g_Buffer,LUA_SCHEMA,LuaDesc.c_str(),
					ClassName.c_str(),
					LuaMember.c_str(),
					ClassName.c_str(),
					LuaClone.c_str(),
					ClassName.c_str(),
					LuaMarshal.c_str(),
					ClassName.c_str(),
					LuaUnmarshal.c_str());

				sFmtValue += g_Buffer;
			}

			return sFmtValue;
		}
	};
	typedef vector<GenClassData> ClassVector;

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

		AString  FormatString()
		{			
			AString sFmtValue   = "";
			if (Include.size())
			{
				memset(g_Buffer,0,MAX_BUFFER);
				sprintf(g_Buffer,"#include \"%s\"\r\n",Include.c_str());
				sFmtValue += g_Buffer;
			}

			for (Size_t i=0;i<Classes.size();i++)
			{
				sFmtValue += Classes[i].FormatString();
			}

			HawkPrint(sFmtValue);
			return sFmtValue;
		}

		AString  FormatLuaString()
		{			
			AString sFmtValue   = "";

			for (Size_t i=0;i<Classes.size();i++)
			{
				if (Classes[i].Usage != "s2s")
					sFmtValue += Classes[i].FormatLuaString();
			}

			HawkPrint(sFmtValue);
			return sFmtValue;
		}
	};

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

		AString  FormatString()
		{			
			AString sFmtValue   = "";
			
			memset(g_Buffer,0,MAX_BUFFER);

			if(MacroDesc.size())
				sprintf(g_Buffer,"\t\t\t//%s\r\n\t\t\t%s = %s,",MacroDesc.c_str(),MacroName.c_str(),MacroValue.c_str());
			else
				sprintf(g_Buffer,"\t\t\t%s = %s,",MacroName.c_str(),MacroValue.c_str());

			sFmtValue = g_Buffer;

			return sFmtValue;
		}

		AString  FormatLuaString()
		{			
			AString sFmtValue   = "";

			memset(g_Buffer,0,MAX_BUFFER);

			if(MacroDesc.size())
				sprintf(g_Buffer,"\t--%s\r\n\t%s = %s,",MacroDesc.c_str(),MacroName.c_str(),MacroValue.c_str());
			else
				sprintf(g_Buffer,"\t%s = %s,",MacroName.c_str(),MacroValue.c_str());

			sFmtValue = g_Buffer;

			return sFmtValue;
		}
	};
	typedef vector<MacroDefine> MacroVector;

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

		AString  FormatString()
		{			
			AString sFmtValue   = "";

			memset(g_Buffer,0,MAX_BUFFER);
			if (Desc.size())
				sprintf(g_Buffer,"\t//%s\r\n\tnamespace %s\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n",Desc.c_str(),Space.c_str());
			else
				sprintf(g_Buffer,"\tnamespace %s\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n",Space.c_str());
			
			sFmtValue += g_Buffer;

			for (Size_t i=0;i<Macros.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += Macros[i].FormatString();
			}

			sFmtValue += "\r\n\t\t};\r\n\t};\r\n\r\n";

			HawkPrint(sFmtValue);
			return sFmtValue;
		}

		AString  FormatLuaString()
		{			
			AString sFmtValue   = "";

			memset(g_Buffer,0,MAX_BUFFER);
			if (Desc.size())
				sprintf(g_Buffer,"\r\n--%s\r\n%s = \r\n{\r\n",Desc.c_str(),Space.c_str());
			else
				sprintf(g_Buffer,"\r\n%s = \r\n{\r\n",Space.c_str());

			sFmtValue += g_Buffer;

			for (Size_t i=0;i<Macros.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += Macros[i].FormatLuaString();
			}

			sFmtValue += "\r\n}\r\n";

			HawkPrint(sFmtValue);
			return sFmtValue;
		}
	};
	typedef vector<GenSpaceData> SpaceVector;

	struct GenMacroData
	{
		AString		Name;
		SpaceVector Spaces;

		GenMacroData()
		{
			Name = "";
			Spaces.clear();
		}

		AString  FormatString()
		{			
			AString sFmtValue   = "";

			for (Size_t i=0;i<Spaces.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += Spaces[i].FormatString();
			}

			return sFmtValue;
		}

		AString  FormatLuaString()
		{
			AString sFmtValue   = "";

			for (Size_t i=0;i<Spaces.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += Spaces[i].FormatLuaString();
			}

			return sFmtValue;
		}
	};

	Bool ParseElement(AXmlElement* pElement,GenFileData& xGenData,MacroVector& vMacro)
	{
		if (pElement && pElement->GetAttribute("Name"))
		{
			GenClassData xClassData;

			if(pElement->GetAttribute("Name"))
				xClassData.ClassName = pElement->GetAttribute("Name")->StringValue();

			if(pElement->GetAttribute("Id"))
				xClassData.Identify = pElement->GetAttribute("Id")->IntValue();			

			if(xClassData.Identify)
			{
				if(g_ProtocolMap.find(xClassData.Identify) == g_ProtocolMap.end())
				{
					g_ProtocolMap[xClassData.Identify] = xClassData.ClassName;
				}
				else
				{
					HawkFmtPrint("协议ID重复: %d",xClassData.Identify);
					HawkAssert(false && "Protocol Id Repeat.");					
					HawkOSOperator::OSSleep(10000);
				}
			}			

			if(pElement->GetAttribute("Macro"))
				xClassData.Macro = pElement->GetAttribute("Macro")->StringValue();	

			if (pElement->GetAttribute("Desc"))
				xClassData.ClassDesc = pElement->GetAttribute("Desc")->StringValue();

			if (pElement->GetAttribute("Drive"))
				xClassData.Drive = pElement->GetAttribute("Desc")->IntValue();

			if (pElement->GetAttribute("Usage"))
				xClassData.Usage = pElement->GetAttribute("Usage")->StringValue();

			if (xClassData.Identify && xClassData.Macro.size())
			{
				MacroDefine xMacro;
				xMacro.MacroName = xClassData.Macro;
				xMacro.MacroValue= HawkStringUtil::IntToString<AString>(xClassData.Identify);
				xMacro.MacroDesc = xClassData.ClassDesc;

				if (xMacro.IsValid())
					vMacro.push_back(xMacro);
			}

			if (pElement->GetTag() == "Marshal")
			{
				xClassData.ClassType = TYPE_MARSHAL_CLASS;

				if (g_MarshalMap.find(xClassData.ClassName) != g_MarshalMap.end())
				{
					HawkAssert(false && "Marshal Name Repeat.");
					HawkFmtPrint("自定义类型重复: %s",xClassData.ClassName.c_str());
					HawkOSOperator::OSSleep(10000);
				}

				g_MarshalMap[xClassData.ClassName] = xClassData.ClassName;
			}
			else if (pElement->GetTag() == "Protocol")
			{
				xClassData.ClassType = TYPE_PROTOCOL_CLASS;
				g_ProtocolVec.push_back(xClassData.ClassName);
			}

			Int32 iVarCnt = pElement->GetChildrenNum();
			for (Int32 i=0;i<iVarCnt;i++)
			{
				AXmlElement* pVar = pElement->GetChildren(i);
				if (pVar && pVar->GetTag() == "Var")
				{
					GenVarData xVar;

					if (pVar->GetAttribute("Name"))
						xVar.Name = pVar->GetAttribute("Name")->StringValue();

					if (pVar->GetAttribute("Type"))
						xVar.Type = pVar->GetAttribute("Type")->StringValue();

					if (pVar->GetAttribute("Default"))
						xVar.Default = pVar->GetAttribute("Default")->StringValue();

					if (xVar.CheckType() && xVar.IsValid())
						xClassData.VarData.push_back(xVar);
				}				
			}

			xGenData.Classes.push_back(xClassData);
			return true;
		}
		return false;
	}

	Bool ParseIdSpace(AXmlElement* pElement,GenMacroData& xMacroData)
	{
		if (pElement && pElement->GetAttribute("Space"))
		{
			GenSpaceData xSpace;

			if(pElement->GetAttribute("Space"))
				xSpace.Space = pElement->GetAttribute("Space")->StringValue();

			if(pElement->GetAttribute("Desc"))
				xSpace.Desc = pElement->GetAttribute("Desc")->StringValue();

			Int32 iIdCnt = pElement->GetChildrenNum();
			for (Int32 i=0;i<iIdCnt;i++)
			{
				AXmlElement* pId = pElement->GetChildren(i);
				if (pId && pId->GetTag() == "Id")
				{
					MacroDefine xMacro;

					if (pId->GetAttribute("Macro"))
						xMacro.MacroName = pId->GetAttribute("Macro")->StringValue();

					if (pId->GetAttribute("Value"))
						xMacro.MacroValue = pId->GetAttribute("Value")->StringValue();

					if (pId->GetAttribute("Desc"))
						xMacro.MacroDesc = pId->GetAttribute("Desc")->StringValue();

					if (xMacro.IsValid())
					{
						Int32 iMacro = HawkStringUtil::StringToInt<AString>(xMacro.MacroValue);
						if (g_MacroMap.find(iMacro) != g_MacroMap.end())
						{
							HawkAssert(false && "Macro Id Repeat.");
							HawkFmtPrint("宏定义ID重复: %d",iMacro);
							HawkOSOperator::OSSleep(10000);
						}
						else
						{
							xSpace.Macros.push_back(xMacro);
							g_MacroMap[iMacro] = iMacro;
						}	
					}
				}
			}

			if (xSpace.IsValid())
				xMacroData.Spaces.push_back(xSpace);

			return true;
		}
		return false;
	}

	Bool HawkProtocolManager::GenProtocols(const AString& sCfgFile, const AString& sHawkHead)
	{
		g_Buffer = (Char*)HawkMalloc(MAX_BUFFER);
		HawkScope::MallocPtr scope(g_Buffer);

		AString sXmlContent = "";

		HawkDiskFile protoXml;
		if (protoXml.Open(sCfgFile))
		{
			Int32 iSize = (Int32)protoXml.GetFileSize();
			Utf8* pData = (Utf8*)HawkMalloc(iSize + 1);
			memset(pData,0,iSize+1);
			protoXml.Read(pData,iSize);
			sXmlContent = HawkStringUtil::ToString(pData);
			HawkFree(pData);
		}
		else
		{
			return false;
		}

		//对Octets类型的支持
		if (g_EnableLua)
			g_MarshalMap["HawkOctets"] = "HawkOctets";

		HawkXmlFile  xml;
		AXmlDocument doc;
		if (sXmlContent.size() && xml.Open<AString>(sXmlContent.c_str(),sXmlContent.size(),doc))
		{
			AXmlElement* pRoot = doc.GetRoot();
			if (pRoot && pRoot->GetTag() == "ProtocolGen")
			{				
				if (pRoot->GetAttribute("ProtocolIdFile"))
					g_ProtocolIdFile = pRoot->GetAttribute("ProtocolIdFile")->StringValue();
				
				if (pRoot->GetAttribute("ProtocolRegFile"))
					g_ProtocolRegFile = pRoot->GetAttribute("ProtocolRegFile")->StringValue();

				MacroVector vProtocolMacro;

				Int32 iGenCount = pRoot->GetChildrenNum();
				for (Int32 i=0;i<iGenCount;i++)
				{
					AXmlElement* pGenFile = pRoot->GetChildren(i);
					if (pGenFile && pGenFile->GetTag() == "GenFile" && pGenFile->GetAttribute("Name"))
					{
						GenFileData		xGenData;
						GenMacroData	xMacroData;
						
						AString sName = pGenFile->GetAttribute("Name")->StringValue();
						xGenData.Name	= sName;
						xMacroData.Name = sName;
						AString sFile = sName + ".h";
						AString sLuaFile = sName + ".lua";
						AString sLuaContent;

						if (HawkOSOperator::ExistFile(sFile))
							HawkOSOperator::SetFileWritable(sFile);

						if (HawkOSOperator::ExistFile(sLuaFile))
							HawkOSOperator::SetFileWritable(sLuaFile);

						HawkDiskFile xFile;
						HawkDiskFile xLua;

						//Lua导出
						if (g_EnableLua && !xLua.Open(sLuaFile, HawkFile::OPEN_WRITE))
							HawkFmtError("Open LuaFile: %s Error.", sLuaFile.c_str());							

						//C++导出
						if (xFile.Open(sFile, HawkFile::OPEN_WRITE))
						{						
							AString sContent = "";

							if (pGenFile->GetAttribute("Include"))
							{
								AString sInclude = pGenFile->GetAttribute("Include")->StringValue();
								
								AStringVector vInclude;
								HawkStringUtil::Split<AString>(sInclude,vInclude,",");

								memset(g_Buffer,0,MAX_BUFFER);
								AString sUpCaseName = HawkStringUtil::UpCase<AString>(sName);
								sprintf(g_Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n",sUpCaseName.c_str(),sUpCaseName.c_str());

								sContent += g_Buffer;

								for (Size_t k=0;k<vInclude.size();k++)
								{
									memset(g_Buffer,0,MAX_BUFFER);
									sprintf(g_Buffer,"#include \"%s.h\"\r\n",vInclude[k].c_str());
									sContent += g_Buffer;
								}

								if(vInclude.size())
									sContent += "\r\n";

								sContent += "namespace Hawk\r\n{\r\n";

								xFile.Write(sContent.c_str(),sContent.size());
							}
							else
							{
								memset(g_Buffer,0,MAX_BUFFER);
								AString sUpCaseName = HawkStringUtil::UpCase<AString>(sName);
								sprintf(g_Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n#include \"%s\"\r\n\r\n",sUpCaseName.c_str(),sUpCaseName.c_str(),sHawkHead.c_str());
								sContent += g_Buffer;

								sContent += "namespace Hawk\r\n{\r\n";

								xFile.Write(sContent.c_str(),sContent.size());
							}

							Int32 iChildCnt = pGenFile->GetChildrenNum();
							for (Int32 j=0;j<iChildCnt;j++)
							{
								AXmlElement* pElement = pGenFile->GetChildren(j);
								xGenData.Classes.clear();
								xMacroData.Spaces.clear();

								if(pElement->GetTag() == "IdSpace")
								{
									ParseIdSpace(pElement,xMacroData);
									
									//C++导出
									sContent = xMacroData.FormatString();
									xFile.Write(sContent.c_str(),sContent.size());

									//Lua导出
									if (g_EnableLua)
									{
										sContent = xMacroData.FormatLuaString();
										xLua.Write(sContent.c_str(),sContent.size());
									}
								}
								else
								{
									ParseElement(pElement,xGenData,vProtocolMacro);

									//C++导出
									sContent = xGenData.FormatString();
									xFile.Write(sContent.c_str(),sContent.size());

									//Lua导出
									if (g_EnableLua)
									{
										sContent = xGenData.FormatLuaString();
										xLua.Write(sContent.c_str(),sContent.size());
									}
								}								
							}

							//C++导出
							sContent = "}\r\n#endif\r\n";
							xFile.Write(sContent.c_str(),sContent.size());
							xFile.Close();

							//Lua导出
							if (g_EnableLua)
							{
								xLua.Close();
							}							
						}
					}
				}

				if (g_ProtocolIdFile.size()/* && vProtocolMacro.size()*/)
				{
					AString sFile = g_ProtocolIdFile + ".h";
					AString sLuaFile = g_ProtocolIdFile + ".lua";

					if (HawkOSOperator::ExistFile(sFile))
						HawkOSOperator::SetFileWritable(sFile);

					if (HawkOSOperator::ExistFile(sLuaFile))
						HawkOSOperator::SetFileWritable(sLuaFile);

					HawkDiskFile xFile;
					HawkDiskFile xLua;

					//C++导出
					if (xFile.Open(sFile,HawkFile::OPEN_WRITE))
					{
						memset(g_Buffer,0,MAX_BUFFER);
						sprintf(g_Buffer,"#ifndef __PROTOCOLID_H__\r\n#define __PROTOCOLID_H__\r\n\r\n#include \"%s\"\r\n\r\n",sHawkHead.c_str());
						AString sContent = g_Buffer;
						xFile.Write(sContent.c_str(),sContent.size());

						AString sFmtValue = "";
						sFmtValue += "namespace Hawk\r\n{\r\n";
						sFmtValue += "\tnamespace ";
						sFmtValue += g_ProtocolIdFile;
						sFmtValue += "\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n";

						for (Size_t i=0;i<vProtocolMacro.size();i++)
						{
							if(i!=0)
								sFmtValue += "\r\n";
							sFmtValue += vProtocolMacro[i].FormatString();
						}
						sFmtValue += "\r\n\t\t};\r\n\t}\r\n}\r\n#endif\r\n";
						xFile.Write(sFmtValue.c_str(),sFmtValue.size());
						xFile.Close();
					}

					//Lua导出
					if (g_EnableLua)
					{						
						if (xLua.Open(sLuaFile,HawkFile::OPEN_WRITE))
						{
							memset(g_Buffer,0,MAX_BUFFER);
							sprintf(g_Buffer,"%s = \r\n{\r\n",g_ProtocolIdFile.c_str());
							AString sContent = g_Buffer;
							xLua.Write(sContent.c_str(),sContent.size());

							for (Size_t i=0;i<vProtocolMacro.size();i++)
							{						
								if (vProtocolMacro[i].MacroDesc.size())
									sprintf(g_Buffer,"\t--%s\r\n\t%s = %s,\r\n", vProtocolMacro[i].MacroDesc.c_str(), vProtocolMacro[i].MacroName.c_str(), vProtocolMacro[i].MacroValue.c_str());
								else
									sprintf(g_Buffer,"\t%s = %s,\r\n", vProtocolMacro[i].MacroName.c_str(), vProtocolMacro[i].MacroValue.c_str());

								sContent = g_Buffer;
								xLua.Write(sContent.c_str(),sContent.size());
							}
							
							sContent = "}\r\n";
							xLua.Write(sContent.c_str(),sContent.size());

							//记录存根
							memset(g_Buffer,0,MAX_BUFFER);
							sprintf(g_Buffer,"\r\nProtocolStub = \r\n{\r\n");
							sContent = g_Buffer;
							xLua.Write(sContent.c_str(),sContent.size());

							for (Size_t i=0;i<vProtocolMacro.size();i++)
							{						
								sprintf(g_Buffer,"\t[%s] = %s,\r\n", vProtocolMacro[i].MacroValue.c_str(), g_ProtocolVec[i].c_str());
								sContent = g_Buffer;
								xLua.Write(sContent.c_str(),sContent.size());
							}

							sContent = "}\r\n";
							xLua.Write(sContent.c_str(),sContent.size());

							xLua.Close();
						}
					}
				}
			}			

			if (g_ProtocolRegFile.size()/* && g_ProtocolVec.size()*/)
			{
				if (HawkOSOperator::ExistFile(g_ProtocolRegFile))
					HawkOSOperator::SetFileWritable(g_ProtocolRegFile);

				HawkDiskFile sRegFile;
				if (sRegFile.Open(g_ProtocolRegFile,HawkFile::OPEN_WRITE))
				{
					for (Size_t i=0;i<g_ProtocolVec.size();i++)
					{
						Char sRegInfo[1024] = {0};
						sprintf(sRegInfo,"REGISTER_PROTO(%s);\r\n",g_ProtocolVec[i].c_str());
						sRegFile.Write(sRegInfo,strlen((sRegInfo)));
					}
					sRegFile.Close();
				}
			}

			return true;
		}
		return false;
	}
}
