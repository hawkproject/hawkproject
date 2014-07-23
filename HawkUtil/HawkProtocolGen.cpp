#include "HawkProtocolManager.h"
#include "HawkProtocolGenDef.h"

namespace Hawk
{	
	ProtocolSchema* g_ProtocolSchema = 0;

	extern Bool GenProtocols_Cpp(ProtocolSchema& sSchema);
	extern Bool GenProtocols_Lua(ProtocolSchema& sSchema);
	extern Bool GenProtocols_Flash(ProtocolSchema& sSchema);
	extern Bool GenProtocols_Java(ProtocolSchema& sSchema);

	Bool ParseElement(ProtocolSchema& sSchema, AXmlElement* pElement, ProtocolSchema::GenFileData& xGenData, ProtocolSchema::MacroVector& vMacro)
	{
		if (pElement && pElement->GetAttribute("Name"))
		{
			ProtocolSchema::GenClassData xClassData;

			if(pElement->GetAttribute("Name"))
				xClassData.ClassName = pElement->GetAttribute("Name")->StringValue();

			if(pElement->GetAttribute("Id"))
				xClassData.Identify = pElement->GetAttribute("Id")->IntValue();			

			if(xClassData.Identify)
			{
				if(sSchema.ProtocolMap.find(xClassData.Identify) == sSchema.ProtocolMap.end())
				{
					sSchema.ProtocolMap[xClassData.Identify] = xClassData.ClassName;
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

			if (pElement->GetAttribute("States"))
				xClassData.States = pElement->GetAttribute("States")->StringValue();

			if (xClassData.Identify && xClassData.Macro.size())
			{
				ProtocolSchema::MacroDefine xMacro;
				xMacro.MacroName = xClassData.Macro;
				xMacro.MacroValue= HawkStringUtil::IntToString<AString>(xClassData.Identify);
				xMacro.MacroDesc = xClassData.ClassDesc;

				if (xMacro.IsValid())
					vMacro.push_back(xMacro);
			}

			if (pElement->GetTag() == "Marshal")
			{
				xClassData.ClassType = TYPE_MARSHAL_CLASS;

				if (sSchema.MarshalMap.find(xClassData.ClassName) != sSchema.MarshalMap.end())
				{
					HawkAssert(false && "Marshal Name Repeat.");
					HawkFmtPrint("Marshal Name Repeat: %s",xClassData.ClassName.c_str());
					HawkOSOperator::OSSleep(10000);
				}

				sSchema.MarshalMap[xClassData.ClassName] = xClassData.ClassName;
			}
			else if (pElement->GetTag() == "Protocol")
			{
				xClassData.ClassType = TYPE_PROTOCOL_CLASS;
				sSchema.ProtocolVec.push_back(xClassData.ClassName);
			}

			Int32 iVarCnt = pElement->GetChildrenNum();
			for (Int32 i=0;i<iVarCnt;i++)
			{
				AXmlElement* pVar = pElement->GetChildren(i);
				if (pVar && pVar->GetTag() == "Var")
				{
					ProtocolSchema::GenVarData xVar;

					if (pVar->GetAttribute("Name"))
						xVar.Name = pVar->GetAttribute("Name")->StringValue();

					if (pVar->GetAttribute("Type"))
						xVar.Type = pVar->GetAttribute("Type")->StringValue();

					if (pVar->GetAttribute("Default"))
						xVar.Default = pVar->GetAttribute("Default")->StringValue();

					if (xVar.IsValid())
						xClassData.VarData.push_back(xVar);
				}				
			}

			xGenData.Classes.push_back(xClassData);
			return true;
		}
		return false;
	}

	Bool ParseIdSpace(ProtocolSchema& sSchema, AXmlElement* pElement, ProtocolSchema::GenMacroData& xMacroData)
	{
		if (pElement && pElement->GetAttribute("Space"))
		{
			ProtocolSchema::GenSpaceData xSpace;

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
					ProtocolSchema::MacroDefine xMacro;

					if (pId->GetAttribute("Macro"))
						xMacro.MacroName = pId->GetAttribute("Macro")->StringValue();

					if (pId->GetAttribute("Value"))
						xMacro.MacroValue = pId->GetAttribute("Value")->StringValue();

					if (pId->GetAttribute("Desc"))
						xMacro.MacroDesc = pId->GetAttribute("Desc")->StringValue();

					if (xMacro.IsValid())
					{
						Int32 iMacro = HawkStringUtil::StringToInt<AString>(xMacro.MacroValue);
						if (sSchema.MacroMap.find(iMacro) != sSchema.MacroMap.end())
						{
							HawkAssert(false && "Macro Id Repeat.");
							HawkFmtPrint("Macro Id Repeat: %d",iMacro);
							HawkOSOperator::OSSleep(10000);
						}
						else
						{
							xSpace.Macros.push_back(xMacro);
							sSchema.MacroMap[iMacro] = iMacro;
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

	Bool HawkProtocolManager::GenProtocols(const AString& sCfgFile, const AString& sArgs)
	{
		HawkDiskFile protoXml;
		if (!protoXml.Open(sCfgFile))
			return false;

		//模式文件
		ProtocolSchema sSchema;
		{
			HawkXmlFile    xml;
			AXmlDocument   doc;
			Int32 iSize = (Int32)protoXml.GetFileSize();
			Utf8* pData = (Utf8*)new Utf8[iSize + 1];
			memset(pData, 0, iSize + 1);
			protoXml.Read(pData, iSize);	
			AString sXmlContent = HawkStringUtil::ToString(pData);
			HawkFree(pData);
			if (iSize <= 0 || !xml.Open<AString>(sXmlContent.c_str(), sXmlContent.size(), doc))
				return false;

			AXmlElement* pRoot = doc.GetRoot();
			if (!pRoot || pRoot->GetTag() != "ProtocolGen")
				return false;

			if (pRoot->GetAttribute("ProtocolIdFile"))
				sSchema.ProtocolIdFile = pRoot->GetAttribute("ProtocolIdFile")->StringValue();

			if (pRoot->GetAttribute("ProtocolRegFile"))
				sSchema.ProtocolRegFile = pRoot->GetAttribute("ProtocolRegFile")->StringValue();

			if (pRoot->GetAttribute("Namespace"))
				sSchema.Namespace = pRoot->GetAttribute("Namespace")->StringValue();

			if (pRoot->GetAttribute("Package"))
				sSchema.PackagePath = pRoot->GetAttribute("Package")->StringValue();

			ProtocolSchema::MacroVector vProtocolMacro;

			Int32 iGenCount = pRoot->GetChildrenNum();
			for (Int32 i=0;i<iGenCount;i++)
			{
				AXmlElement* pGenFile = pRoot->GetChildren(i);
				if (!pGenFile || pGenFile->GetTag() != "GenFile" || !pGenFile->GetAttribute("Name"))
					continue;
				
				ProtocolSchema::GenFileData	 xGenData;
				ProtocolSchema::GenMacroData xMacroData;

				AString sName   = pGenFile->GetAttribute("Name")->StringValue();
				xGenData.Name	 = sName;
				xMacroData.Name = sName;

				AString sContent = "";
				xGenData.Include = "HawkUtil";
				if (pGenFile->GetAttribute("Include"))
					xGenData.Include = pGenFile->GetAttribute("Include")->StringValue();

				Int32 iChildCnt = pGenFile->GetChildrenNum();
				for (Int32 j=0;j<iChildCnt;j++)
				{
					AXmlElement* pElement = pGenFile->GetChildren(j);
					xGenData.Classes.clear();
					xMacroData.Spaces.clear();

					if(pElement->GetTag() == "IdSpace")
					{
						ParseIdSpace(sSchema, pElement, xMacroData);
						
						if (sSchema.MacroData.find(xMacroData.Name) == sSchema.MacroData.end())
							sSchema.MacroData[xMacroData.Name] = vector<ProtocolSchema::GenMacroData>();

						sSchema.MacroData[xMacroData.Name].push_back(xMacroData);
					}
					else
					{
						ParseElement(sSchema, pElement, xGenData, sSchema.ProtocolMacro);

						if (sSchema.FileData.find(xGenData.Name) == sSchema.FileData.end())
							sSchema.FileData[xGenData.Name] = vector<ProtocolSchema::GenFileData>();

						sSchema.FileData[xGenData.Name].push_back(xGenData);
					}
				}
			}
		}		
		
		//////////////////////////////////////////////////////////////////////////
		
		g_ProtocolSchema = &sSchema;

		GenProtocols_Cpp(sSchema);

		if (sArgs.find("lua") != AString::npos)
			GenProtocols_Lua(sSchema);

		if (sArgs.find("java") != AString::npos)
			GenProtocols_Java(sSchema);

		if (sArgs.find("flash") != AString::npos)
			GenProtocols_Flash(sSchema);

		return true;
	}
}
