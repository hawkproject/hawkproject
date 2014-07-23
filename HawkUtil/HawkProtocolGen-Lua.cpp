#include "HawkProtocolManager.h"
#include "HawkProtocolGenDef.h"

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

namespace Hawk
{
	extern ProtocolSchema* g_ProtocolSchema;

	Bool IsLuaBaseType(const AString& sType)
	{
		return sType == "Bool" ||
			sType == "Int8" ||
			sType == "UInt8" ||
			sType == "Int16" ||
			sType == "UInt16" ||
			sType == "Int32" ||
			sType == "UInt32" ||
			sType == "Int64" || 
			sType == "UInt64" ||
			sType == "Float" ||
			sType == "Double" ||			
			sType == "AString" ||
			sType == "UString" ||
			sType == "HawkOctets";
	};

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

		if (g_ProtocolSchema->MarshalMap.find(sType) != g_ProtocolSchema->MarshalMap.end())
			return sType;

		return "{ }";
	}

	struct lua_GenVarData : ProtocolSchema::GenVarData
	{
	};

	struct lua_GenClassData : ProtocolSchema::GenClassData
	{
		Bool BulidLua()
		{
			//添加协议类型
			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
			if (Macro.size())
			{				
				sprintf(g_ProtocolSchema->Buffer,"\tm_iType = %s.%s,",
					g_ProtocolSchema->ProtocolIdFile.c_str(), Macro.c_str());
				lua.LuaMember = g_ProtocolSchema->Buffer;
			}
			
			for (Size_t i=0;i<VarData.size();i++)
			{
				ProtocolSchema::GenVarData& xVar = VarData[i];

				if (lua.LuaMember.size())
					lua.LuaMember += "\r\n";

				if (i != 0)
				{					
					lua.LuaMarshal   += "\r\n";
					lua.LuaUnmarshal += "\r\n";
				}

				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);				
				sprintf(g_ProtocolSchema->Buffer,"\t%s = %s,",
					xVar.Name.c_str(), GetLuaTypeDefault(xVar.Type, xVar.Default).c_str());
				lua.LuaMember += g_ProtocolSchema->Buffer;

				if (IsLuaBaseType(xVar.Type))
				{
					//Marshal基础类型
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\toct:Push%s(self.%s);",
						xVar.Type.c_str(), xVar.Name.c_str());
					lua.LuaMarshal += g_ProtocolSchema->Buffer;

					//Unmarshal基础类型
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\tself.%s = oct:Pop%s();",
						xVar.Name.c_str(),xVar.Type.c_str());
					lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
				}
				else if (xVar.Type.find("vector") == 0)
				{
					AString sItemType = ProtocolSchema::GetVectorItemType(xVar.Type);

					//Marshal vector的元素个数
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\toct:PushUInt32(TableSize(self.%s));\r\n",
						xVar.Name.c_str());

					if (lua.LuaMarshal.size())
						lua.LuaMarshal += "\r\n";
					lua.LuaMarshal += g_ProtocolSchema->Buffer;

					//Marshal vector的实际元素
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sItemType))
					{
						sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(v);\r\n\tend",
							xVar.Name.c_str(), sItemType.c_str());
						lua.LuaMarshal += g_ProtocolSchema->Buffer;
					}
					else
					{
						sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tv:marshal(oct);\r\n\tend",
							xVar.Name.c_str());
						lua.LuaMarshal += g_ProtocolSchema->Buffer;
					}

					//Unmarshal vector的元素个数
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\tlocal tbl_sz_%s = oct:PopUInt32();\r\n",
						xVar.Name.c_str());

					if (lua.LuaUnmarshal.size())
						lua.LuaUnmarshal += "\r\n";
					lua.LuaUnmarshal += g_ProtocolSchema->Buffer;

					//Unmarshal vector的实际元素
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sItemType))
					{
						sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tself.%s[i] = oct:Pop%s();\r\n\tend",
							xVar.Name.c_str(), xVar.Name.c_str(), sItemType.c_str());
						lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
					}
					else
					{
						sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tself.%s[i] = %s:clone();\r\n\t\tself.%s[i].unmarshal(oct);\r\n\tend",
							xVar.Name.c_str(), xVar.Name.c_str(), sItemType.c_str(), xVar.Name.c_str());
						lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
					}					
				}
				else if (xVar.Type.find("map") == 0)
				{
					std::pair<AString, AString> sKV = ProtocolSchema::GetMapItemType(xVar.Type);

					//Marshal map的元素个数
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\toct:PushUInt32(TableSize(self.%s));\r\n",
						xVar.Name.c_str());
					
					if (lua.LuaMarshal.size())
						lua.LuaMarshal += "\r\n";
					lua.LuaMarshal += g_ProtocolSchema->Buffer;

					//Marshal map的实际元素
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sKV.first))
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(k);\r\n\t\toct:Push%s(v);\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str());
							lua.LuaMarshal += g_ProtocolSchema->Buffer;
						}
						else
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\toct:Push%s(k);\r\n\t\tv:marshal(oct);\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str());
							lua.LuaMarshal += g_ProtocolSchema->Buffer;
						}						
					}
					else
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tk:marshal(oct);\r\n\t\toct:Push%s(v);\r\n\tend",
								xVar.Name.c_str(), sKV.second.c_str());
							lua.LuaMarshal += g_ProtocolSchema->Buffer;
						}
						else
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor k, v in pairs(self.%s) do\r\n\t\tk:marshal(oct);\r\n\t\tv:marshal(oct);\r\n\tend",
								xVar.Name.c_str());
							lua.LuaMarshal += g_ProtocolSchema->Buffer;
						}						
					}

					//Unmarshal map的元素个数
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\tlocal tbl_sz_%s = oct:PopUInt32();\r\n",
						xVar.Name.c_str());

					if (lua.LuaUnmarshal.size())
						lua.LuaUnmarshal += "\r\n";
					lua.LuaUnmarshal += g_ProtocolSchema->Buffer;

					//Unmarshal map的实际元素
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					if (IsLuaBaseType(sKV.first))
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = oct:Pop%s();\r\n\t\tlocal v = oct:Pop%s();\r\n\t\tself.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(), xVar.Name.c_str());
							lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
						}
						else
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = oct:Pop%s();\r\n\t\tlocal v = %s:clone();\r\n\t\tv:unmarshal(oct);\r\n\t\tself.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(), xVar.Name.c_str());
							lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
						}						
					}
					else
					{
						if (IsLuaBaseType(sKV.second))
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = %s:clone();\r\n\t\tk:unmarshal(oct);\r\n\t\tlocal v = oct:Pop%s();self.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(),xVar.Name.c_str());
							lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
						}
						else
						{
							sprintf(g_ProtocolSchema->Buffer,"\tfor i=1, tbl_sz_%s do\r\n\t\tlocal k = %s:clone();\r\n\t\tk:unmarshal(oct);\r\n\t\tlocal v = %s:clone();\r\n\t\tv:unmarshal(oct);self.%s[k] = v;\r\n\tend",
								xVar.Name.c_str(), sKV.first.c_str(), sKV.second.c_str(),xVar.Name.c_str());
							lua.LuaUnmarshal += g_ProtocolSchema->Buffer;
						}						
					}
				}
				else if (g_ProtocolSchema->MarshalMap.find(xVar.Type) != g_ProtocolSchema->MarshalMap.end())
				{
					//Marshal基础类型
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\tself.%s:marshal(oct);",xVar.Name.c_str());

					if (lua.LuaMarshal.size())
						lua.LuaMarshal += "\r\n";
					lua.LuaMarshal += g_ProtocolSchema->Buffer;

					//Unmarshal基础类型
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\tself.%s = %s:clone();\r\n\tself.%s:unmarshal(oct);",xVar.Name.c_str(),xVar.Type.c_str(),xVar.Name.c_str());
					
					if (lua.LuaUnmarshal.size())
						lua.LuaUnmarshal += "\r\n";
					lua.LuaUnmarshal += g_ProtocolSchema->Buffer;

					//克隆时重新构造新对象					
					memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);	
					sprintf(g_ProtocolSchema->Buffer,"\r\n\tinst_tbl.%s = self.%s:clone();",xVar.Name.c_str(),xVar.Name.c_str());
					lua.LuaClone += g_ProtocolSchema->Buffer;
				}
				else
				{
					HawkFmtPrint("Unknown VarType: %s",xVar.Type.c_str());
					HawkAssert(false && "Unknown VarType.");					
					HawkOSOperator::OSSleep(10000);
				}
			}

			return true;
		}

		AString  FormatLuaString()
		{
			if (!BulidLua()) return "";

			AString sFmtValue = "";
			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);

			if (ClassDesc.size())
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,"\r\n--%s",_Utf8(ClassDesc).c_str());
				lua.LuaDesc = g_ProtocolSchema->Buffer;				
			}

			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				
				sprintf(g_ProtocolSchema->Buffer,LUA_SCHEMA,
					lua.LuaDesc.c_str(),
					ClassName.c_str(),
					lua.LuaMember.c_str(),
					ClassName.c_str(),
					lua.LuaClone.c_str(),
					ClassName.c_str(),
					lua.LuaMarshal.c_str(),
					ClassName.c_str(),
					lua.LuaUnmarshal.c_str());

				sFmtValue += g_ProtocolSchema->Buffer;
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				
				sprintf(g_ProtocolSchema->Buffer,LUA_SCHEMA,
					lua.LuaDesc.c_str(),
					ClassName.c_str(),
					lua.LuaMember.c_str(),
					ClassName.c_str(),
					lua.LuaClone.c_str(),
					ClassName.c_str(),
					lua.LuaMarshal.c_str(),
					ClassName.c_str(),
					lua.LuaUnmarshal.c_str());

				sFmtValue += g_ProtocolSchema->Buffer;
			}

			return sFmtValue;
		}
	};
	struct lua_GenFileData : ProtocolSchema::GenFileData
	{		
		AString  FormatLuaString()
		{			
			AString sFmtValue   = "";

			for (Size_t i=0;i<Classes.size();i++)
			{
				if (Classes[i].Usage != "s2s")
					sFmtValue += ((lua_GenClassData*)&Classes[i])->FormatLuaString();
			}

			HawkPrint(_Ascii((UChar*)sFmtValue.c_str()));
			return sFmtValue;
		}
	};

	struct lua_MacroDefine : ProtocolSchema::MacroDefine
	{		
		AString  FormatLuaString()
		{			
			AString sFmtValue = "";

			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);

			if(MacroDesc.size())
				sprintf(g_ProtocolSchema->Buffer,"\t--%s\r\n\t%s = %s,",_Utf8(MacroDesc).c_str(),MacroName.c_str(),MacroValue.c_str());
			else
				sprintf(g_ProtocolSchema->Buffer,"\t%s = %s,",MacroName.c_str(),MacroValue.c_str());

			sFmtValue = g_ProtocolSchema->Buffer;

			return sFmtValue;
		}
	};

	struct lua_GenSpaceData : ProtocolSchema::GenSpaceData
	{		
		AString  FormatLuaString()
		{			
			AString sFmtValue   = "";

			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
			if (Desc.size())
				sprintf(g_ProtocolSchema->Buffer,"--%s\r\n%s = \r\n{\r\n",_Utf8(Desc).c_str(),Space.c_str());
			else
				sprintf(g_ProtocolSchema->Buffer,"%s = \r\n{\r\n",Space.c_str());

			sFmtValue += g_ProtocolSchema->Buffer;

			for (Size_t i=0;i<Macros.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += ((lua_MacroDefine*)&Macros[i])->FormatLuaString();
			}

			sFmtValue += "\r\n}\r\n\r\n";

			HawkPrint(_Ascii((UChar*)sFmtValue.c_str()));
			return sFmtValue;
		}
	};

	struct lua_GenMacroData : ProtocolSchema::GenMacroData
	{		
		AString  FormatLuaString()
		{
			AString sFmtValue   = "";

			for (Size_t i=0;i<Spaces.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += ((lua_GenSpaceData*)&Spaces[i])->FormatLuaString();
			}

			return sFmtValue;
		}
	};

	Bool GenProtocols_Lua(ProtocolSchema& sSchema)
	{
		Bool bProtoSingleLua = true;

		HawkOSOperator::CreateDir("lua/");
		
		sSchema.MarshalMap["HawkOctets"] = "HawkOctets";

		//协议文件
		ProtocolSchema::GenFileMap::iterator file_it = sSchema.FileData.begin();
		for (;file_it != sSchema.FileData.end(); file_it++)
		{
			AString sName    = file_it->first;
			AString sLuaFile = sName + ".lua";
			AString sLuaContent;

			if (HawkOSOperator::ExistFile(sLuaFile))
				HawkOSOperator::SetFileWritable(sLuaFile);

			vector<ProtocolSchema::GenFileData>& vGenFileData = file_it->second;

			//生成合并文件
			HawkDiskFile xLua;
			if (xLua.Open("lua/"+sLuaFile, HawkFile::OPEN_WRITE))
			{
				for (Size_t i=0;i<vGenFileData.size();i++)
				{
					AString sContent = ((lua_GenFileData*)&vGenFileData[i])->FormatLuaString();
					xLua.Write(sContent.c_str(),sContent.size());
				}

				xLua.Close();
			}

			//当个协议生成一个文件
			if (bProtoSingleLua)
			{
				HawkOSOperator::CreateDir("lua/"+sName+"/");

				for (Size_t i=0;i<vGenFileData.size();i++)
				{
					if (vGenFileData[i].Classes.size())
					{
						HawkDiskFile xLua;
						if (xLua.Open("lua/"+sName+"/"+vGenFileData[i].Classes[0].ClassName+".lua", HawkFile::OPEN_WRITE))
						{
							AString sContent = ((lua_GenFileData*)&vGenFileData[i])->FormatLuaString();
							xLua.Write(sContent.c_str(),sContent.size());
						}
						xLua.Close();
					}
				}
			}				
		}

		//宏定义
		ProtocolSchema::GenMacroMap::iterator macro_it = sSchema.MacroData.begin();
		for (;macro_it != sSchema.MacroData.end(); macro_it++)
		{
			AString sName    = macro_it->first;
			AString sLuaFile = sName + ".lua";
			AString sLuaContent;

			if (HawkOSOperator::ExistFile(sLuaFile))
				HawkOSOperator::SetFileWritable(sLuaFile);

			vector<ProtocolSchema::GenMacroData>& vGenMacroData = macro_it->second;

			HawkDiskFile xLua;
			if (xLua.Open("lua/"+sLuaFile, HawkFile::OPEN_WRITE))
			{
				for (Size_t i=0;i<vGenMacroData.size();i++)
				{
					AString sContent = ((lua_GenMacroData*)&vGenMacroData[i])->FormatLuaString();
					xLua.Write(sContent.c_str(),sContent.size());
				}
				
				xLua.Close();
			}
		}

		//协议ID定义文件
		if (sSchema.ProtocolIdFile.size())
		{
			AString sLuaFile = sSchema.ProtocolIdFile + ".lua";
					
			if (HawkOSOperator::ExistFile(sLuaFile))
				HawkOSOperator::SetFileWritable(sLuaFile);

			HawkDiskFile xLua;

			if (xLua.Open("lua/"+sLuaFile,HawkFile::OPEN_WRITE))
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,"%s = \r\n{\r\n",sSchema.ProtocolIdFile.c_str());
				AString sContent = g_ProtocolSchema->Buffer;
				xLua.Write(sContent.c_str(),sContent.size());

				for (Size_t i=0;i<sSchema.ProtocolMacro.size();i++)
				{						
					if (sSchema.ProtocolMacro[i].MacroDesc.size())
						sprintf(g_ProtocolSchema->Buffer,"\t--%s\r\n\t%s = %s,\r\n", _Utf8(sSchema.ProtocolMacro[i].MacroDesc).c_str(), sSchema.ProtocolMacro[i].MacroName.c_str(), sSchema.ProtocolMacro[i].MacroValue.c_str());
					else
						sprintf(g_ProtocolSchema->Buffer,"\t%s = %s,\r\n", sSchema.ProtocolMacro[i].MacroName.c_str(), sSchema.ProtocolMacro[i].MacroValue.c_str());

					sContent = g_ProtocolSchema->Buffer;
					xLua.Write(sContent.c_str(),sContent.size());
				}

				sContent = "}\r\n";
				xLua.Write(sContent.c_str(),sContent.size());

				//记录存根
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,"\r\nProtocolStub = \r\n{\r\n");
				sContent = g_ProtocolSchema->Buffer;
				xLua.Write(sContent.c_str(),sContent.size());

				for (Size_t i=0;i<sSchema.ProtocolMacro.size();i++)
				{						
					sprintf(g_ProtocolSchema->Buffer,"\t[%s] = %s,\r\n", sSchema.ProtocolMacro[i].MacroValue.c_str(), sSchema.ProtocolVec[i].c_str());
					sContent = g_ProtocolSchema->Buffer;
					xLua.Write(sContent.c_str(),sContent.size());
				}

				sContent = "}\r\n";
				xLua.Write(sContent.c_str(),sContent.size());

				xLua.Close();
			}
		}
		return true;
	}
}
