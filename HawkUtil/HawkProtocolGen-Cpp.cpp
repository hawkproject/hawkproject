#include "HawkProtocolManager.h"
#include "HawkProtocolGenDef.h"

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
\t	%s\r\n\
\r\n\
\t	%s\r\n%s\
\t};\r\n\r\n"

namespace Hawk
{
	extern ProtocolSchema* g_ProtocolSchema;

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

	struct cpp_GenVarData : ProtocolSchema::GenVarData
	{
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

	struct cpp_GenClassData : ProtocolSchema::GenClassData
	{		
		Bool  BuildConstructFunc()
		{
			cpp.ConstructFunc = "";
			cpp.ConstructFunc += ClassName;
			cpp.ConstructFunc += "(";
			for (Size_t i=0;i<VarData.size();i++)
			{
				cpp_GenVarData* pVar = (cpp_GenVarData*)&VarData[i];
				if (i != 0)
					cpp.ConstructFunc += ", ";
				cpp.ConstructFunc += pVar->GetConstructFmt();
			}

			cpp.ConstructFunc += ")";
			if (ClassType != TYPE_MARSHAL_CLASS || VarData.size())
				cpp.ConstructFunc += " : ";

			if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				cpp.ConstructFunc += "HawkProtocol(";
				if (g_ProtocolSchema->Namespace.size())
					cpp.ConstructFunc += g_ProtocolSchema->Namespace;
				else
					cpp.ConstructFunc += g_ProtocolSchema->ProtocolIdFile;
				cpp.ConstructFunc += "::";
				cpp.ConstructFunc += Macro;
				if (Drive > 0)
				{
					cpp.ConstructFunc += ", ";
					cpp.ConstructFunc += HawkStringUtil::IntToString<AString>(Drive);
				}

				if (States.size())
				{
					cpp.ConstructFunc += ", ";
					cpp.ConstructFunc += States;
				}

				cpp.ConstructFunc += ")";
				if(VarData.size())
					cpp.ConstructFunc += ", ";
			}

			for (Size_t i=0;i<VarData.size();i++)
			{
				cpp_GenVarData* pVar = (cpp_GenVarData*)&VarData[i];
				if (i != 0)
					cpp.ConstructFunc += ", ";
				cpp.ConstructFunc += pVar->GetInitFmt();
			}

			cpp.ConstructFunc += "\r\n\t\t{\r\n\t\t}";
			return true;
		}

		Bool  BuildCopyFunc()
		{
			cpp.CopyFunc = "";
			if(ClassType == TYPE_MARSHAL_CLASS)
			{
				cpp.CopyFunc += ClassName;
				cpp.CopyFunc += "(const ";
				cpp.CopyFunc += ClassName;
				cpp.CopyFunc += "& rhs)";
				if (VarData.size())
					cpp.CopyFunc += " : ";

				for (Size_t i=0;i<VarData.size();i++)
				{
					cpp_GenVarData* pVar = (cpp_GenVarData*)&VarData[i];
					if (i != 0)
						cpp.CopyFunc += ", ";
					cpp.CopyFunc += pVar->GetCopyFmt();
				}
				cpp.CopyFunc += "\r\n\t\t{\r\n\t\t};";
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

			cpp.CloneFunc = sFmtValue;
			return cpp.CloneFunc.size() != 0;
		}

		Bool  BuildOperatorSelfFunc()
		{
			cpp.OperatorSelfFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				cpp.OperatorSelfFunc = "virtual ";
				cpp.OperatorSelfFunc += ClassName;
				cpp.OperatorSelfFunc += "& operator = (const ";
				cpp.OperatorSelfFunc += ClassName;
				cpp.OperatorSelfFunc += "& rhs)\r\n\t\t{\r\n\t\t\tif(this != &rhs)\r\n\t\t\t{\r\n";

				for (Size_t i=0;i<VarData.size();i++)
				{
					cpp_GenVarData* pVar = (cpp_GenVarData*)&VarData[i];
					cpp.OperatorSelfFunc += "\t\t\t\t";
					cpp.OperatorSelfFunc += pVar->GetSetValFunc();
					cpp.OperatorSelfFunc += "\r\n";
				}
				
				cpp.OperatorSelfFunc += "\t\t\t}\r\n\t\t\treturn *this;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildMarshalFunc()
		{
			cpp.MarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				cpp.MarshalFunc = "virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)\r\n\t\t{\r\n";
				if (VarData.size())
				{
					cpp.MarshalFunc += "\t\t\trhsOS";
					for (Size_t i=0;i<VarData.size();i++)
					{
						cpp.MarshalFunc += " << ";
						cpp.MarshalFunc += VarData[i].Name;
					}
					cpp.MarshalFunc += ";\r\n";
				}
				cpp.MarshalFunc += "\t\t\treturn rhsOS;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildUnmarshalFunc()
		{
			cpp.UnmarshalFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				cpp.UnmarshalFunc = "virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)\r\n\t\t{\r\n";
				if (VarData.size())
				{
					cpp.UnmarshalFunc += "\t\t\trhsOS";
					for (Size_t i=0;i<VarData.size();i++)
					{
						cpp.UnmarshalFunc += " >> ";
						cpp.UnmarshalFunc += VarData[i].Name;
					}
					cpp.UnmarshalFunc += ";\r\n";
				}
				cpp.UnmarshalFunc += "\t\t\treturn rhsOS;\r\n\t\t};";
			}
			return true;
		}

		Bool BuildClearFunc()
		{
			cpp.ClearFunc = "";
			if (ClassType == TYPE_MARSHAL_CLASS || ClassType == TYPE_PROTOCOL_CLASS)
			{
				cpp.ClearFunc = "virtual Bool Clear()\r\n\t\t{\r\n";
				if (VarData.size())
				{
					for (Size_t i=0;i<VarData.size();i++)
					{
						if (i != 0)
							cpp.ClearFunc += "\r\n\t\t\t";
						else
							cpp.ClearFunc += "\t\t\t";

						

						if (IsValueType(VarData[i].Type))
						{
							cpp.ClearFunc += VarData[i].Name;
							cpp.ClearFunc += " = 0;";
						}
						else if (VarData[i].Type == "AString" || 
								 VarData[i].Type == "UString" || 
								 VarData[i].Type == "WString" ||
								 VarData[i].Type.find("vector") == 0 ||
								 VarData[i].Type.find("list") == 0 ||
								 VarData[i].Type.find("map") == 0)
						{
							cpp.ClearFunc += VarData[i].Name;
							cpp.ClearFunc += ".clear();";
						}
						else
						{
							cpp.ClearFunc += VarData[i].Name;
							cpp.ClearFunc += ".Clear();";
						}
					}
					cpp.ClearFunc += "\r\n";
				}
				cpp.ClearFunc += "\t\t\treturn true;\r\n\t\t};";
			}
			return true;
		}

		Bool  BuildClassMember()
		{
			cpp.MemberDefine = "";
			if (VarData.size())
			{
				cpp.MemberDefine += "\r\n\tpublic:\r\n";
				for (Size_t i=0;i<VarData.size();i++)
				{
					cpp_GenVarData* pVar = (cpp_GenVarData*)&VarData[i];
					cpp.MemberDefine += "\t\t";
					cpp.MemberDefine += pVar->GetMemberFmt();
					cpp.MemberDefine += "\r\n";
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
			BuildClearFunc();
			BuildClassMember();
			return true;
		}

		AString  FormatString()
		{
			if (!BuildFunc()) return "";

			AString sFmtValue = "";
			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
			
			if (ClassDesc.size())
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,"\t//%s\r\n",_Utf8(ClassDesc).c_str());
				sFmtValue += g_ProtocolSchema->Buffer;				
			}

			if (ClassType == TYPE_MARSHAL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,MARSHALDATA_CLASS,ClassName.c_str(),
					cpp.ConstructFunc.c_str(),
					cpp.CopyFunc.c_str(),
					cpp.CloneFunc.c_str(),
					cpp.OperatorSelfFunc.c_str(),
					cpp.MarshalFunc.c_str(),
					cpp.UnmarshalFunc.c_str(),
					cpp.ClearFunc.c_str(),
					cpp.MemberDefine.c_str());

				sFmtValue += g_ProtocolSchema->Buffer;
			}
			else if (ClassType == TYPE_PROTOCOL_CLASS)
			{
				memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
				sprintf(g_ProtocolSchema->Buffer,PROTOCOL_CLASS,ClassName.c_str(),
					cpp.ConstructFunc.c_str(),
					cpp.CloneFunc.c_str(),
					cpp.MarshalFunc.c_str(),
					cpp.UnmarshalFunc.c_str(),
					cpp.ClearFunc.c_str(),
					cpp.MemberDefine.c_str());

				sFmtValue += g_ProtocolSchema->Buffer;
			}
		
			return sFmtValue;
		}
	};

	struct cpp_GenFileData : ProtocolSchema::GenFileData
	{
		AString  FormatString()
		{			
			AString sFmtValue   = "";			
			for (Size_t i=0;i<Classes.size();i++)
			{
				sFmtValue += ((cpp_GenClassData*)&Classes[i])->FormatString();
			}

			HawkPrint(sFmtValue);
			return sFmtValue;
		}
	};

	struct cpp_MacroDefine : ProtocolSchema::MacroDefine
	{		
		AString  FormatString()
		{			
			AString sFmtValue   = "";
			
			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);

			if(MacroDesc.size())
				sprintf(g_ProtocolSchema->Buffer,"\t\t\t//%s\r\n\t\t\t%s = %s,",_Utf8(MacroDesc).c_str(),MacroName.c_str(),MacroValue.c_str());
			else
				sprintf(g_ProtocolSchema->Buffer,"\t\t\t%s = %s,",MacroName.c_str(),MacroValue.c_str());

			sFmtValue = g_ProtocolSchema->Buffer;

			return sFmtValue;
		}
	};

	struct cpp_GenSpaceData : ProtocolSchema::GenSpaceData
	{		
		AString  FormatString()
		{			
			AString sFmtValue   = "";

			memset(g_ProtocolSchema->Buffer,0,MAX_BUFFER);
			if (Desc.size())
				sprintf(g_ProtocolSchema->Buffer,"\t//%s\r\n\tnamespace %s\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n",_Utf8(Desc).c_str(),Space.c_str());
			else
				sprintf(g_ProtocolSchema->Buffer,"\tnamespace %s\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n",Space.c_str());
			
			sFmtValue += g_ProtocolSchema->Buffer;

			for (Size_t i=0;i<Macros.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += ((cpp_MacroDefine*)&Macros[i])->FormatString();
			}

			sFmtValue += "\r\n\t\t};\r\n\t};\r\n\r\n";

			HawkPrint(sFmtValue);
			return sFmtValue;
		}
	};

	struct cpp_GenMacroData : ProtocolSchema::GenMacroData
	{		
		AString  FormatString()
		{			
			AString sFmtValue   = "";

			for (Size_t i=0;i<Spaces.size();i++)
			{
				if(i != 0)
					sFmtValue += "\r\n";
				sFmtValue += ((cpp_GenSpaceData*)&Spaces[i])->FormatString();
			}

			return sFmtValue;
		}
	};

	Bool GenProtocols_Cpp(ProtocolSchema& sSchema)
	{
		HawkOSOperator::CreateDir("cpp/");

		//协议文件
		ProtocolSchema::GenFileMap::iterator file_it = sSchema.FileData.begin();
		for (;file_it != sSchema.FileData.end(); file_it++)
		{
			AString sName = file_it->first;
			AString sFile = sName + ".h";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			vector<ProtocolSchema::GenFileData>& vGenFileData = file_it->second;

			HawkDiskFile xFile;
			if (xFile.Open("cpp/"+sFile, HawkFile::OPEN_WRITE))
			{						
				AString sContent = "";

				//头文件包含支持
				if (vGenFileData[0].Include.size())
				{
					AString sInclude = vGenFileData[0].Include;

					AStringVector vInclude;
					HawkStringUtil::Split<AString>(sInclude,vInclude,",");

					memset(sSchema.Buffer,0,MAX_BUFFER);
					AString sUpCaseName = HawkStringUtil::UpCase<AString>(sName);
					sprintf(sSchema.Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n",sUpCaseName.c_str(),sUpCaseName.c_str());

					sContent += sSchema.Buffer;

					for (Size_t k=0;k<vInclude.size();k++)
					{
						memset(sSchema.Buffer,0,MAX_BUFFER);
						sprintf(sSchema.Buffer,"#include \"%s.h\"\r\n",vInclude[k].c_str());
						sContent += sSchema.Buffer;
					}

					if(vInclude.size())
						sContent += "\r\n";

					sContent += "namespace Hawk\r\n{\r\n";
					xFile.Write(sContent.c_str(),sContent.size());

					//内部命名空间
					if (sSchema.Namespace.size())
					{
						memset(sSchema.Buffer,0,MAX_BUFFER);
						sprintf(sSchema.Buffer,"\tHAWK_START_NAMESPACE(%s)\r\n\r\n",sSchema.Namespace.c_str());
						sContent = sSchema.Buffer;
						xFile.Write(sContent.c_str(),sContent.size());
					}
				}
				else
				{
					memset(sSchema.Buffer,0,MAX_BUFFER);
					AString sUpCaseName = HawkStringUtil::UpCase<AString>(sName);
					sprintf(sSchema.Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n#include \"%s\"\r\n\r\n",sUpCaseName.c_str(),sUpCaseName.c_str(),"HawkUtil.h");
					sContent += sSchema.Buffer;
					sContent += "namespace Hawk\r\n{\r\n";
					xFile.Write(sContent.c_str(),sContent.size());

					//内部命名空间
					if (sSchema.Namespace.size())
					{
						memset(sSchema.Buffer,0,MAX_BUFFER);
						sprintf(sSchema.Buffer,"\tHAWK_START_NAMESPACE(%s)\r\n\r\n",sSchema.Namespace.c_str());
						sContent = sSchema.Buffer;
						xFile.Write(sContent.c_str(),sContent.size());
					}										
				}

				//类实体格式化
				for (Size_t i=0;i<vGenFileData.size();i++)
				{
					sContent = ((cpp_GenFileData*)&vGenFileData[i])->FormatString();
					xFile.Write(sContent.c_str(),sContent.size());
				}
				
				//内部命名空间
				if (sSchema.Namespace.size())
				{
					memset(sSchema.Buffer,0,MAX_BUFFER);
					sprintf(sSchema.Buffer,"\tHAWK_CLOSE_NAMESPACE(%s)\r\n",sSchema.Namespace.c_str());
					sContent = sSchema.Buffer;
					xFile.Write(sContent.c_str(), sContent.size());
				}

				sContent = "}\r\n#endif\r\n";
				xFile.Write(sContent.c_str(),sContent.size());
				xFile.Close();		
			}
		}

		//宏定义
		ProtocolSchema::GenMacroMap::iterator macro_it = sSchema.MacroData.begin();
		for (;macro_it != sSchema.MacroData.end(); macro_it++)
		{
			AString sName = macro_it->first;
			AString sFile = sName + ".h";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			vector<ProtocolSchema::GenMacroData>& vGenMacroData = macro_it->second;

			HawkDiskFile xFile;
			if (xFile.Open("cpp/"+sFile, HawkFile::OPEN_WRITE))
			{				
				AString sContent = "";

				memset(sSchema.Buffer,0,MAX_BUFFER);
				AString sUpCaseName = HawkStringUtil::UpCase<AString>(sName);
				sprintf(sSchema.Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n#include \"%s\"\r\n\r\n",sUpCaseName.c_str(),sUpCaseName.c_str(),"HawkUtil.h");
				sContent += sSchema.Buffer;
				sContent += "namespace Hawk\r\n{\r\n";
				xFile.Write(sContent.c_str(),sContent.size());

				//内部命名空间
				if (sSchema.Namespace.size())
				{
					memset(sSchema.Buffer,0,MAX_BUFFER);
					sprintf(sSchema.Buffer,"\tHAWK_START_NAMESPACE(%s)\r\n\r\n",sSchema.Namespace.c_str());
					sContent = sSchema.Buffer;
					xFile.Write(sContent.c_str(),sContent.size());
				}

				for (Size_t i=0;i<vGenMacroData.size();i++)
				{
					sContent = ((cpp_GenMacroData*)&vGenMacroData[i])->FormatString();
					xFile.Write(sContent.c_str(),sContent.size());	
				}

				//内部命名空间
				if (sSchema.Namespace.size())
				{
					memset(sSchema.Buffer,0,MAX_BUFFER);
					sprintf(sSchema.Buffer,"\tHAWK_CLOSE_NAMESPACE(%s)\r\n",sSchema.Namespace.c_str());
					sContent = sSchema.Buffer;
					xFile.Write(sContent.c_str(),sContent.size());
				}

				sContent = "}\r\n#endif\r\n";
				xFile.Write(sContent.c_str(),sContent.size());
				xFile.Close();	
			}
		}

		//协议ID定义文件
		if (sSchema.ProtocolIdFile.size())
		{
			AString sFile = sSchema.ProtocolIdFile + ".h";

			if (HawkOSOperator::ExistFile(sFile))
				HawkOSOperator::SetFileWritable(sFile);

			HawkDiskFile xFile;

			if (xFile.Open("cpp/"+sFile,HawkFile::OPEN_WRITE))
			{
				AString sUpCaseIdFile = sSchema.ProtocolIdFile;
				HawkStringUtil::UpCase<AString>(sUpCaseIdFile);

				memset(sSchema.Buffer,0,MAX_BUFFER);
				sprintf(sSchema.Buffer,"#ifndef __%s_H__\r\n#define __%s_H__\r\n\r\n#include \"%s\"\r\n\r\n", sUpCaseIdFile.c_str(), sUpCaseIdFile.c_str(), "HawkUtil.h");
				AString sContent = sSchema.Buffer;
				xFile.Write(sContent.c_str(),sContent.size());

				AString sFmtValue = "";
				sFmtValue += "namespace Hawk\r\n{\r\n";
				sFmtValue += "\tnamespace ";

				//协议ID所处命名空间
				if (sSchema.Namespace.size())
					sFmtValue += sSchema.Namespace;
				else
					sFmtValue += sSchema.ProtocolIdFile;

				sFmtValue += "\r\n\t{\r\n\t\tenum\r\n\t\t{\r\n";

				for (Size_t i=0;i<sSchema.ProtocolMacro.size();i++)
				{
					if(i!=0)
						sFmtValue += "\r\n";
					sFmtValue += ((cpp_MacroDefine*)&sSchema.ProtocolMacro[i])->FormatString();
				}
				sFmtValue += "\r\n\t\t};\r\n\t}\r\n}\r\n#endif\r\n";
				xFile.Write(sFmtValue.c_str(),sFmtValue.size());
				xFile.Close();
			}
		}
		
		//协议注册文件
		if (sSchema.ProtocolRegFile.size())
		{
			AString sRegFilePath = sSchema.ProtocolRegFile;
			if (sRegFilePath.find(".inl") == AString::npos)
				sRegFilePath += ".inl";

			if (HawkOSOperator::ExistFile(sRegFilePath))
				HawkOSOperator::SetFileWritable(sRegFilePath);

			HawkDiskFile sRegFile;
			if (sRegFile.Open("cpp/"+sRegFilePath,HawkFile::OPEN_WRITE))
			{
				for (Size_t i=0;i<sSchema.ProtocolVec.size();i++)
				{
					Char sRegInfo[1024] = {0};

					if (sSchema.Namespace.size())
						sprintf(sRegInfo,"REGISTER_PROTO(%s::%s);\r\n",sSchema.Namespace.c_str(), sSchema.ProtocolVec[i].c_str());
					else
						sprintf(sRegInfo,"REGISTER_PROTO(%s);\r\n",sSchema.ProtocolVec[i].c_str());

					sRegFile.Write(sRegInfo,strlen((sRegInfo)));
				}
				sRegFile.Close();
			}
		}

		return true;
	}
}
