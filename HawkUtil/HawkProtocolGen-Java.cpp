#include "HawkProtocolManager.h"
#include "HawkProtocolGenDef.h"

#define MARSHALDATA_CLASS "\tpublic static class %s extends HawkMarshal\r\n\
\t{\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t};\r\n\r\n"

#define PROTOCOL_CLASS "\tpublic static class %s extends HawkProtocol {\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t\r\n\
\t	%s\r\n\
\t};\r\n\r\n"

#define VECTOR_WRITE_FUNCTION "\t\t\tstream.writeUInt(%s.size());\r\n\
\t\t\tfor(%s %s : %s) {\r\n\
\t\t\t\t%s\r\n\
\t\t\t}\r\n"

#define MAP_WRITE_FUNCTION "\t\t\tstream.writeUInt(%s.size());\r\n\
\t\t\tfor(Map.Entry<%s, %s> entry : %s.entrySet()){\r\n\
\t\t\t\t%s\r\n\
\t\t\t\t%s\r\n\
\t\t\t}\r\n"

#define VECTOR_READ_FUNCTION "\t\t\tint %sSize = rhsOS.readUInt();\r\n\
\t\t\tfor(int i=0 ;i < %sSize;i++){\r\n\
\t\t\t\t%s\r\n\
\t\t\t\t%s.add(%s);\r\n\
\t\t\t}\r\n"

#define MAP_READ_FUNCTION "\t\t\tint %sSize = rhsOS.readUInt();\r\n\
\t\t\tfor(int i=0 ;i < %sSize;i++){\r\n\
\t\t\t\t%s\r\n\
\t\t\t\t%s\r\n\
\t\t\t\t%s.put(%s,%s);\r\n\
\t\t\t}\r\n"

#define REGISTER_PROTO_CLASS "package %s;\r\n\r\n\
import org.hawk.protocol.HawkProtocolManager;\r\n\r\n\
public class %s {\r\n\r\n\
\tpublic static void init() {\r\n\
%s\t}\r\n}\r\n"

#define PROCTOCL_CONSTRUCT_SUPER "\t\t\tsuper(%s.%s, %s, %s);\r\n"

#define CLONE_INSTANCE "public HawkMarshal clone() {\r\n\t\t\treturn new %s();\r\n\t\t}"

#define REGISTER_PROTO_ITEM	"\t\tHawkProtocolManager.getInstance().register(%s.%s,new %s.%s());\r\n"

namespace Hawk
{
	extern ProtocolSchema* g_ProtocolSchema;

	Bool IsJavaValueType(const AString& sType)
	{
		return sType == "Bool" ||
			sType == "Byte" ||
			sType == "Int8" ||
			sType == "UInt8" ||
			sType == "Char" ||
			sType == "UChar" ||
			sType == "Short" ||			
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

	AString GetTypeDefaultValue(const AString& sType)
	{
		if (sType == "Boolean")
		{
			return "false";
		}
		else if (sType == "Byte" || sType == "Short" || sType == "Integer")
		{
			return "0";
		}
		else if (sType == "Long")
		{
			return "0L";
		}
		else if (sType == "Float")
		{
			return "0.0F";
		}
		else if (sType == "Double")
		{
			return "0.0";
		}
		else if (sType == "AString" || sType == "UString")
		{
			return "\"\"";
		}
		else 
		{
			return "new " + sType + "()";
		}
	}

	AString GetJavaTypeByCfg(const AString& sType) 
	{
		if (sType == "Bool")
		{
			return "Boolean";
		}
		else if (sType == "Int8" || sType == "UInt8" || sType == "Byte" || sType == "Char" || sType == "UChar")
		{
			return "Byte";
		}
		else if (sType == "Int16" || sType == "UInt16" || sType == "Short")
		{
			return "Short";
		}
		else if (sType == "Int32" || sType == "UInt32" || sType == "Integer")
		{
			return "Integer";
		}
		else if (sType == "Int64" || sType == "UInt64" || sType == "Long")
		{
			return "Long";
		}
		else if (sType == "Float")
		{
			return "Float";
		}
		else if (sType == "Double")
		{
			return "Double";
		}
		else if(sType == "AString" || sType == "UString")
		{
			return "String";
		}
		else if(sType == "HawkOctets")
		{
			return "HawkOctets";
		}
		else if(sType.find("vector") != AString::npos)
		{
			return "List<" + GetJavaTypeByCfg(ProtocolSchema::GetVectorItemType(sType)) + ">";
		}
		else if(sType.find("map") != AString::npos)
		{
			std::pair<AString, AString> key_val_type = ProtocolSchema::GetMapItemType(sType);
			return "Map<" + GetJavaTypeByCfg(key_val_type.first) + "," + GetJavaTypeByCfg(key_val_type.second) + ">";
		}
		return sType;
	}

	struct java_GenVarData : ProtocolSchema::GenVarData
	{
		AString GetMemberFmt()
		{
			AString sValue = GetJavaTypeByCfg(Type);
			sValue += " ";
			sValue += Name;
			sValue += ";";
			return sValue;
		}

		AString GetConstructFmt()
		{
			AString sValue = "this." + Name + " = ";

			if(Type == "HawkOctets")
			{
				sValue += "new HawkOctets();";
			}
			else if(Type.find("vector") != AString::npos)
			{
				sValue += "new ArrayList<" +  GetJavaTypeByCfg(ProtocolSchema::GetVectorItemType(Type)) + ">();";
			}
			else if(Type.find("map") != AString::npos)
			{
				std::pair<AString, AString> key_val_type = ProtocolSchema::GetMapItemType(Type);
				sValue += "new HashMap<" + GetJavaTypeByCfg(key_val_type.first) + "," + GetJavaTypeByCfg(key_val_type.second) + ">();";
			}
			else 
			{
				sValue += GetTypeDefaultValue(GetJavaTypeByCfg(Type)) + ";";
			}

			return sValue;
		}

		AString GetClearFmt()
		{
			AString sJavaType = GetJavaTypeByCfg(Type);
			AString sValue    = "this." + Name ;

			if (sJavaType == "Boolean")
			{
				sValue += " = false;";
			}
			else if (sJavaType == "Byte" || sJavaType == "Short" || sJavaType == "Integer")
			{
				sValue += "= 0;";
			}
			else if (sJavaType == "Long")
			{
				sValue += " = 0L;";
			}
			else if (sJavaType == "Float")
			{
				sValue += " = 0.0F;";
			}
			else if (sJavaType == "Double")
			{
				sValue += " = 0.0;";
			}
			else if (sJavaType == "String" || sJavaType == "AString" || sJavaType == "UString")
			{
				sValue += " = \"\";";
			}
			else
			{
				sValue += ".clear();";
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
	};

	struct java_GenClassData : ProtocolSchema::GenClassData
	{		
		Bool BuildConstructFunc()
		{
			java.ConstructFunc = "public ";
			java.ConstructFunc += ClassName;
			java.ConstructFunc += "() {\r\n";

			if(ClassType == TYPE_PROTOCOL_CLASS)
			{				
				AString sDriveType = "DriveType.DRIVE_EVENT";
				if (Drive == 1)
					sDriveType = "DriveType.DRIVE_STATE";

				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer, PROCTOCL_CONSTRUCT_SUPER, 
					g_ProtocolSchema->ProtocolIdFile.c_str(), Macro.c_str(), 
					sDriveType.c_str(), States.size() > 0 ? States.c_str() : "\"\"");
				
				java.ConstructFunc += g_ProtocolSchema->Buffer;
			}
		
			for (Size_t i=0;i<VarData.size();i++)
			{
				java_GenVarData* pVar = (java_GenVarData*)&VarData[i];
				AString sConstruct = pVar->GetConstructFmt();
				if(sConstruct.size() > 0){
					java.ConstructFunc += "\t\t\t" + sConstruct + "\r\n";
				}
			}
			java.ConstructFunc += "\t\t};";
			return true;
		}

		AString GenWriteFuncName(const AString& sType, const AString& sName)
		{
			AString sWriteFunc = "";
			if(sType == "Bool" || sType == "Boolean")
			{
				sWriteFunc =  "writeBoolean";
			}	
			else if(sType == "Int8" || sType == "UInt8" || sType == "Byte" || sType == "Char" || sType == "UChar")
			{
				sWriteFunc =  "writeByte";
			}
			else if(sType == "Int16" || sType == "UInt16" || sType == "Short")
			{
				sWriteFunc =  "writeShort";
			}
			else if(sType == "Int32" || sType == "UInt32" || sType == "Integer")
			{
				if (sType == "UInt32")
					sWriteFunc = "writeUInt";
				else
					sWriteFunc = "writeInt";
			}
			else if(sType == "Int64" || sType == "UInt64" || sType == "Long")
			{
				sWriteFunc =  "writeLong";
			}
			else if(sType == "Float")
			{
				sWriteFunc =  "writeFloat";
			}
			else if(sType == "Double")
			{
				sWriteFunc =  "writeDouble";
			}			
			else if(sType == "String" || sType == "UString" || sType == "AString")
			{
				sWriteFunc =  "writeString";
			}			
			else if(sType == "HawkOctets")
			{
				sWriteFunc =  "writeOctets";
			}

			if(sWriteFunc.length() > 0)
				return "os." + sWriteFunc + "(" + sName + ");";

			return sName + ".marshal(os);";
		}

		Bool  BuildClearFunc()
		{
			java.ClearFunc = "public boolean clear() {\n";
			for (Size_t i=0;i<VarData.size();i++)
			{
				java_GenVarData* pVar = (java_GenVarData*)&VarData[i];
				AString sClearFunc = pVar->GetClearFmt();
				if(sClearFunc.size() > 0)
				{
					java.ClearFunc += "\t\t\t" + sClearFunc + "\r\n";
				}
			}
			
			java.ClearFunc += "\t\t\treturn true;\n\t\t};";
			return true;
		}

		AString GenReadFuncName(const AString& sType, const AString& sName)
		{
			AString sReadFunc = "";
			if(sType == "Bool" || sType == "Boolean")
			{
				sReadFunc =  "readBoolean()";
			}
			else if(sType == "Int8" || sType == "UInt8" || sType == "Byte" || sType == "Char" || sType == "UChar")
			{
				sReadFunc =  "readByte()";
			}
			else if(sType == "Int16" || sType == "UInt16" || sType == "Short")
			{
				sReadFunc =  "readShort()";
			}
			else if(sType == "Int32" || sType == "UInt32" || sType == "Integer")
			{
				if (sType == "UInt32")
					sReadFunc = "readUInt()";
				else
					sReadFunc = "readInt()";
			}
			else if(sType == "Int64" || sType == "UInt64" || sType == "Long")
			{
				sReadFunc =  "readLong()";
			}
			else if(sType == "Float")
			{
				sReadFunc =  "readFloat()";
			}
			else if(sType == "Double")
			{
				sReadFunc =  "readDouble()";
			}			
			else if(sType == "String" || sType == "UString" || sType == "AString")
			{
				sReadFunc =  "readString()";
			}
			else if(sType == "HawkOctets")
			{
				sReadFunc =  "readOctets()";
			}

			if(sReadFunc.length() > 0)
			{
				return sName + " = " + "os." + sReadFunc + ";";
			}
			else
			{
				AString sNew = sName + " = new " + sType + "();\r\n";
				return  sNew + "\t\t\t\t" + sName + ".unmarshal(os);";
			}
		}

		Bool  BuildMarshalFunc()
		{
			java.MarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				java.MarshalFunc = "public void marshal(HawkOctetsStream os) {\r\n";
				if (VarData.size())
				{					
					for (Size_t i=0;i<VarData.size();i++)
					{
						if(IsJavaValueType(VarData[i].Type))
						{
							java.MarshalFunc += "\t\t\t" + GenWriteFuncName(VarData[i].Type,VarData[i].Name) + "\r\n";
						}
						else if(VarData[i].Type.find("vector") != AString::npos)
						{
							AString vectorItemType = GetJavaTypeByCfg(ProtocolSchema::GetVectorItemType(VarData[i].Type));
							AString vectorItemName = vectorItemType;
							vectorItemName[0] = vectorItemName[0] + 32;
							
							memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
							sprintf(g_ProtocolSchema->Buffer, VECTOR_WRITE_FUNCTION,
								VarData[i].Name.c_str(), vectorItemType.c_str(),
								vectorItemName.c_str(), VarData[i].Name.c_str(),
								(GenWriteFuncName(vectorItemType,vectorItemName)).c_str());

							java.MarshalFunc += g_ProtocolSchema->Buffer;

							if (i != VarData.size()-1)
								java.MarshalFunc += "\r\n";
						}
						else if(VarData[i].Type.find("map") != AString::npos)
						{
							std::pair<AString, AString>  mapItemType =  ProtocolSchema::GetMapItemType(VarData[i].Type);
							AString mapKeyType = GetJavaTypeByCfg(mapItemType.first);
							AString mapValType = GetJavaTypeByCfg(mapItemType.second);
							
							memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
							sprintf(g_ProtocolSchema->Buffer, MAP_WRITE_FUNCTION,
								VarData[i].Name.c_str(), mapKeyType.c_str(), 
								mapValType.c_str(), VarData[i].Name.c_str(),
								(GenWriteFuncName(mapKeyType,"entry.getKey()")).c_str(),
								(GenWriteFuncName(mapValType,"entry.getValue()")).c_str());

							java.MarshalFunc += g_ProtocolSchema->Buffer;

							if (i != VarData.size()-1)
								java.MarshalFunc += "\r\n";
						}
						else 
						{
							java.MarshalFunc += "\t\t\t" + VarData[i].Name + ".marshal(os);\r\n";
						}
					}	
				}
				java.MarshalFunc += "\t\t}";
			}
			return true;
		}

		Bool  BuildUnmarshalFunc()
		{
			java.UnmarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				java.UnmarshalFunc = "public void unmarshal(HawkOctetsStream os) throws HawkException {\r\n";
				if (VarData.size())
				{
					for (Size_t i=0;i<VarData.size();i++)
					{
						if(IsJavaValueType(VarData[i].Type))
						{
							java.UnmarshalFunc += "\t\t\t" + GenReadFuncName(VarData[i].Type,VarData[i].Name) + "\r\n";
						}
						else if(VarData[i].Type.find("vector") != AString::npos)
						{
							AString vectorItemType = GetJavaTypeByCfg(ProtocolSchema::GetVectorItemType(VarData[i].Type));
							AString vectorItemName =  AString(vectorItemType);
							vectorItemName[0] = vectorItemName[0] + 32;

							memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
							sprintf(g_ProtocolSchema->Buffer,VECTOR_READ_FUNCTION,
								VarData[i].Name.c_str(), VarData[i].Name.c_str(),
								(vectorItemType + " " + (GenReadFuncName(vectorItemType,vectorItemName))).c_str(),
								VarData[i].Name.c_str(), vectorItemName.c_str());

							java.UnmarshalFunc += g_ProtocolSchema->Buffer;

							if (i != VarData.size()-1)
								java.UnmarshalFunc += "\r\n";
						}
						else if(VarData[i].Type.find("map") != AString::npos)
						{
							std::pair<AString, AString>  mapItemType =  ProtocolSchema::GetMapItemType(VarData[i].Type);
							AString mapKeyType = GetJavaTypeByCfg(mapItemType.first);
							AString mapKeyName = AString(mapKeyType);
							mapKeyName[0] = mapKeyName[0] + 32;

							AString mapValType = GetJavaTypeByCfg(mapItemType.second);
							AString mapValName = mapValType;
							mapValName[0] = mapValName[0] + 32;

							memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
							sprintf(g_ProtocolSchema->Buffer, MAP_READ_FUNCTION,
								VarData[i].Name.c_str(), VarData[i].Name.c_str(),
								(mapKeyType + " " + GenReadFuncName(mapKeyType,mapKeyName)).c_str(),
								(mapValType + " " + GenReadFuncName(mapValType,mapValName)).c_str(),
								VarData[i].Name.c_str(), mapKeyName.c_str(), mapValName.c_str());

							java.UnmarshalFunc += g_ProtocolSchema->Buffer;

							if (i != VarData.size()-1)
								java.UnmarshalFunc += "\r\n";
						}
						else 
						{
							java.UnmarshalFunc += "\t\t\t" + VarData[i].Name + ".unmarshal(os);\r\n";
						}
					}
				}
				java.UnmarshalFunc += "\t\t}";
			}
			return true;
		}

		Bool  BuildClassMember()
		{
			java.MemberDefine = "";
			if (VarData.size())
			{				
				for (Size_t i=0;i<VarData.size();i++)
				{
					if (i == 0) 
						java.MemberDefine += "public ";
					else
						java.MemberDefine += "\r\n\t\tpublic ";

					java_GenVarData& xVar = (java_GenVarData&)VarData[i];
					java.MemberDefine += xVar.GetMemberFmt();
				}
			}
			return true;
		}

		Bool BuildCloneFunc()
		{
			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);			
			sprintf(g_ProtocolSchema->Buffer, CLONE_INSTANCE, ClassName.c_str());
			java.CloneFunc += g_ProtocolSchema->Buffer;
			return true;
		}

		Bool BuildFunc()
		{
			BuildConstructFunc();
			BuildCloneFunc();			
			BuildMarshalFunc();
			BuildUnmarshalFunc();
			BuildClassMember();
			BuildClearFunc();
			return true;
		}

		AString FormatString()
		{
			if (!BuildFunc()) return "";

			AString sFmtValue = "";

			//描述信息
			if (ClassDesc.size())
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,"\t//%s\r\n",_Utf8(ClassDesc).c_str());
				sFmtValue += g_ProtocolSchema->Buffer;				
			}
			else
			{
				sFmtValue += "\r\n";	
			}

			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,MARSHALDATA_CLASS,
					ClassName.c_str(),
					java.ConstructFunc.c_str(),					
					java.CloneFunc.c_str(),
					java.MarshalFunc.c_str(),
					java.UnmarshalFunc.c_str(),
					java.ClearFunc.c_str(),
					java.MemberDefine.c_str()
				);

				sFmtValue += g_ProtocolSchema->Buffer;
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,PROTOCOL_CLASS,
					ClassName.c_str(),
					java.ConstructFunc.c_str(),
					java.CloneFunc.c_str(),
					java.MarshalFunc.c_str(),
					java.UnmarshalFunc.c_str(),
					java.ClearFunc.c_str(),
					java.MemberDefine.c_str()
				);

				sFmtValue += g_ProtocolSchema->Buffer;
			}

			return sFmtValue;
		}
	};

	typedef vector<java_GenClassData> ClassVector;

	struct java_GenFileData : ProtocolSchema::GenFileData 
	{

		java_GenFileData()
		{
			Name = "";
			Classes.clear();
		}

		AString FormatString()
		{			
			AString sFmtValue = "";
			for (Size_t i=0;i<Classes.size();i++)
			{
				sFmtValue += ((java_GenClassData*)&Classes[i])->FormatString();
			}

			HawkPrint(_Ascii((UChar*)sFmtValue.c_str()));
			return sFmtValue;
		}
	};

	struct java_MacroDefine : ProtocolSchema::MacroDefine
	{
		AString  FormatString(Bool bIds = false)
		{			
			AString sFmtValue   = "";

			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);

			if (bIds)
			{
				if(MacroDesc.size())
					sprintf(g_ProtocolSchema->Buffer,"\t//%s\r\n\tpublic static final int %s = %s;",_Utf8(MacroDesc).c_str(),MacroName.c_str(),MacroValue.c_str());
				else
					sprintf(g_ProtocolSchema->Buffer,"\tpublic static final int %s = %s;",MacroName.c_str(),MacroValue.c_str());
			}
			else
			{
				if(MacroDesc.size())
					sprintf(g_ProtocolSchema->Buffer,"\t\t//%s\r\n\t\tpublic static final int %s = %s;",_Utf8(MacroDesc).c_str(),MacroName.c_str(),MacroValue.c_str());
				else
					sprintf(g_ProtocolSchema->Buffer,"\t\tpublic static final int %s = %s;",MacroName.c_str(),MacroValue.c_str());
			}			

			sFmtValue = g_ProtocolSchema->Buffer;

			return sFmtValue;
		}
	};
	typedef vector<java_MacroDefine> MacroVector;

	struct java_GenSpaceData : ProtocolSchema::GenSpaceData
	{
		AString  FormatString()
		{			
			AString sFmtValue   = "";

			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
			if (Desc.size())
				sprintf(g_ProtocolSchema->Buffer,"\t//%s\r\n\tpublic static final class %s{\r\n",_Utf8(Desc).c_str(),Space.c_str());
			else
				sprintf(g_ProtocolSchema->Buffer,"\tpublic static final class %s\r\n\t\r\n\t\t{\r\n",Space.c_str());

			sFmtValue += g_ProtocolSchema->Buffer;

			for (Size_t i=0;i<Macros.size();i++)
			{
				if(i != 0) sFmtValue += "\r\n";
				sFmtValue += ((java_MacroDefine*)&Macros[i])->FormatString();
			}

			sFmtValue += "\r\n\t};\r\n";

			HawkPrint(_Ascii((UChar*)sFmtValue.c_str()));
			return sFmtValue;
		}
	};
	typedef vector<java_GenSpaceData> SpaceVector;

	struct java_GenMacroData : ProtocolSchema::GenMacroData
	{
		java_GenMacroData()
		{
			Name = "";
			Spaces.clear();
		}

		AString  FormatString()
		{			
			AString sFmtValue = "";

			for (Size_t i=0;i<Spaces.size();i++)
			{
				if(i != 0) sFmtValue += "\r\n";

				sFmtValue += ((java_GenSpaceData*)&Spaces[i])->FormatString();

				if(i != Spaces.size()-1) sFmtValue += "\r\n";
			}

			return sFmtValue;
		}
	};

	Bool GenProtocols_Java(ProtocolSchema& sSchema)
	{
		AString basePath = g_ProtocolSchema->PackagePath;
		HawkStringUtil::Replace<AString>(basePath, ".", "/");

		HawkOSOperator::CreateDir("java/" + basePath);

		//协议文件
		ProtocolSchema::GenFileMap::iterator file_it = sSchema.FileData.begin();
		for (;file_it != sSchema.FileData.end(); file_it++)
		{
			AString sName = file_it->first;
			AString sFile = basePath + "/" + sName + ".java";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			vector<ProtocolSchema::GenFileData>& vGenFileData = file_it->second;

			HawkDiskFile xFile;
			if (xFile.Open("java/"+sFile, HawkFile::OPEN_WRITE))
			{						
				AString sContent = "";

				//创建package头
				sContent += "package " + g_ProtocolSchema->PackagePath + ";\r\n\r\n";

				//import定义
				sContent += "import java.util.Map;\r\n";
				sContent += "import java.util.List;\r\n";
				sContent += "import java.util.HashMap;\r\n";
				sContent += "import java.util.ArrayList;\r\n";			
				sContent += "import org.hawk.os.HawkException;\r\n";
				sContent += "import org.hawk.octets.HawkOctets;\r\n";
				sContent += "import org.hawk.protocol.HawkMarshal;\r\n";
				sContent += "import org.hawk.protocol.HawkProtocol;\r\n";	
				sContent += "import org.hawk.octets.HawkOctetsStream;\r\n";
				sContent += "\r\n@SuppressWarnings(\"unused\")\r\n";
				xFile.Write(sContent.c_str(),sContent.size());

				//类名
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);		
				sprintf(g_ProtocolSchema->Buffer,"\r\npublic class %s {\r\n", sName.c_str());
				sContent = g_ProtocolSchema->Buffer;
				xFile.Write(sContent.c_str(),sContent.size());

				//类实体格式化
				for (Size_t i=0;i<vGenFileData.size();i++)
				{
					sContent = ((java_GenFileData*)&vGenFileData[i])->FormatString();
					xFile.Write(sContent.c_str(),sContent.size());
				}

				sContent = "}\r\n";
				xFile.Write(sContent.c_str(),sContent.size());
				xFile.Close();
			}
		}

		//宏定义
		ProtocolSchema::GenMacroMap::iterator macro_it = sSchema.MacroData.begin();
		for (;macro_it != sSchema.MacroData.end(); macro_it++)
		{
			AString sName = macro_it->first;
			AString sFile = sName + ".java";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			vector<ProtocolSchema::GenMacroData>& vGenMacroData = macro_it->second;

			HawkDiskFile xFile;
			if (xFile.Open("java/" + basePath + "/" + sFile, HawkFile::OPEN_WRITE))
			{				
				AString sContent = "";

				memset(sSchema.Buffer,0,MAX_BUFFER);
				AString sUpCaseName = AString(sName);
				HawkStringUtil::UpCase<AString>(sUpCaseName);
				sContent += sSchema.Buffer;
				sContent += "package " + g_ProtocolSchema->PackagePath + ";\r\n\r\n";
				sContent += "public class " + sName + " {\r\n";
				xFile.Write(sContent.c_str(), sContent.size());

				for (Size_t i=0;i<vGenMacroData.size();i++)
				{
					if (i != 0) 
						xFile.Write("\r\n", 2);

					sContent = ((java_GenMacroData*)&vGenMacroData[i])->FormatString();
					xFile.Write(sContent.c_str(),sContent.size());
				}

				sContent = "}\r\n";
				xFile.Write(sContent.c_str(),sContent.size());
				xFile.Close();	
			}
		}

		//协议ID定义文件
		if (sSchema.ProtocolIdFile.size())
		{
			AString sFile = basePath + "/" + sSchema.ProtocolIdFile + ".java";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			HawkDiskFile xFile;

			if (xFile.Open("java/"+sFile, HawkFile::OPEN_WRITE))
			{
				memset(sSchema.Buffer,0,MAX_BUFFER);
				AString sContent = sSchema.Buffer;
				xFile.Write(sContent.c_str(),sContent.size());

				AString sFmtValue = "package " + sSchema.PackagePath + ";\r\n\r\n";
				sFmtValue += "public interface " + sSchema.ProtocolIdFile + " {\r\n";
				for (Size_t i=0;i<sSchema.ProtocolMacro.size();i++)
				{
					if(i) sFmtValue += "\r\n";
					sFmtValue += ((java_MacroDefine*)&sSchema.ProtocolMacro[i])->FormatString(true);
				}
				sFmtValue += "\r\n};\r\n";
				xFile.Write(sFmtValue.c_str(),sFmtValue.size());
				xFile.Close();
			}
		}

		//协议注册文件
		if (sSchema.ProtocolRegFile.size())
		{
			AString sRegFilePath = sSchema.ProtocolRegFile;
			if (sRegFilePath.find(".java") == AString::npos)
				sRegFilePath += ".java";

			if (HawkOSOperator::ExistFile(sRegFilePath))
				HawkOSOperator::SetFileWritable(sRegFilePath);

			HawkDiskFile sRegFile;
			if (sRegFile.Open("java/" + basePath + "/" + sRegFilePath, HawkFile::OPEN_WRITE))
			{
				AString sContent = "";

				ProtocolSchema::GenFileMap::iterator file_it = sSchema.FileData.begin();
				for (;file_it != sSchema.FileData.end(); file_it++)
				{
					vector<ProtocolSchema::GenFileData>& vGenFileData = file_it->second;
					for(Size_t i = 0;i < vGenFileData.size();i++)
					{
						ProtocolSchema::ClassVector& classVector = vGenFileData[i].Classes ;
						for (Size_t i = 0;i < classVector.size();i++)
						{
							if(classVector[i].Macro.size() > 0)
							{
								Char sRegInfo[1024] = {0};
								sprintf(sRegInfo, REGISTER_PROTO_ITEM,
									sSchema.ProtocolIdFile.c_str(),
									classVector[i].Macro.c_str(),
									file_it->first.c_str(),
									classVector[i].ClassName.c_str());

								sContent += sRegInfo;
							}
						}
					}
				}
			
				AString sRegClass = sRegFilePath;
				HawkStringUtil::Replace<AString>(sRegClass, AString(".java"), AString(""));
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer, REGISTER_PROTO_CLASS, g_ProtocolSchema->PackagePath.c_str(), sRegClass.c_str(), sContent.c_str());
				sRegFile.Write(g_ProtocolSchema->Buffer, strlen(g_ProtocolSchema->Buffer));
				sRegFile.Close();
			}
		}

		return true;
	}
}
