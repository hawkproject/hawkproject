#include "HawkXmlFile.h"
#include "HawkLoggerManager.h"
#include "HawkStringUtil.h"
#include "HawkDiskFile.h"
#include "HawkScope.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

namespace Hawk
{
	template <class StringType, class CharType> Bool ParseXmlNode(xml_node<CharType>* pNode, HawkXmlElement<StringType>* pElement)
	{
		if (pNode && pElement)
		{
			if (pNode->type() == rapidxml::node_element && pNode->name_size())
			{
				//标签数据
				pElement->SetTag(StringType(pNode->name(), pNode->name_size()));

				//属性数据
				if (pNode->value_size())
					pElement->SetValue(StringType(pNode->value(), pNode->value_size()));

				//读取属性
				pElement->Attributes.reserve(rapidxml::count_children<CharType>(pNode));
				xml_attribute<CharType>* pNodeAttr = pNode->first_attribute();
				while (pNodeAttr)
				{
					if (pNodeAttr->name_size())
					{
						pElement->AddAttribute(HawkXmlAttribute<StringType>(
							StringType(pNodeAttr->name(),  pNodeAttr->name_size()),
							StringType(pNodeAttr->value(), pNodeAttr->value_size())));
					}
					pNodeAttr = pNodeAttr->next_attribute();
				}

				//子节点预开辟空间
				pElement->Children.reserve(rapidxml::count_children<CharType>(pNode));
				xml_node<CharType>* pChildNode = pNode->first_node();
				while (pChildNode)
				{
					if (pChildNode->type() == rapidxml::node_element && pChildNode->name_size())
					{
						HawkXmlElement<StringType>* pChildElement = pElement->AddChildren(StringType(pChildNode->name(), pChildNode->name_size()));
						if (!ParseXmlNode(pChildNode, pChildElement))
							return false;
					}
					pChildNode = pChildNode->next_sibling();
				}
			}			
			return true;
		}
		return false;
	}

	Bool ParseDocument(void* pDoc, const void* pData)
	{
		//数据编码格式检验
		HawkFile::CodeType eCodeType = HawkFile::GetCodeType(pData);
		A_Exception(eCodeType != HawkFile::CODE_UNICODE && "HawkXmlFile Cannot Parser Unicode.");

		if (eCodeType == HawkFile::CODE_ASCII)
		{
			xml_document<Char> xmlDoc;
			try
			{
				xmlDoc.parse<0>((Char*)pData);
			}
			catch (rapidxml::parse_error& sExp)
			{
				HawkFmtError("rapidxml parse error, what: %s, where: %s", sExp.what(), sExp.where<Char>());
				return false;
			}

			xml_node<Char>* pRoot = xmlDoc.first_node();
			if (pRoot)
			{
				AXmlElement* pXmlNode = ((AXmlDocument*)pDoc)->GetRoot();
				return ParseXmlNode<AString, Char>(pRoot, pXmlNode);
			}
		}
		else if (eCodeType == HawkFile::CODE_UTF8)
		{
			xml_document<Utf8> xmlDoc;
			try
			{
				xmlDoc.parse<0>((Utf8*)pData);
			}
			catch (rapidxml::parse_error& sExp)
			{
				HawkFmtError("rapidxml parse error, what: %s, where: %s", sExp.what(), sExp.where<Char>());
				return false;
			}

			xml_node<Utf8>* pRoot = xmlDoc.first_node();
			if (pRoot)
			{
				UXmlElement* pXmlNode = ((UXmlDocument*)pDoc)->GetRoot();
				return ParseXmlNode<UString, Utf8>(pRoot, pXmlNode);
			}
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	HawkXmlFile::HawkXmlFile()
	{
	}

	HawkXmlFile::~HawkXmlFile()
	{
	}

	Bool HawkXmlFile::ParseFile(const AString& sFile, PVoid pDoc)
	{
		HawkDiskFile xFile;
		if (!pDoc || !xFile.Open(sFile))
			return false;

		Size_t iSize = (Size_t)(xFile.GetFileSize() + 8);
		Char* pData	 = (Char*)HawkMalloc(iSize);
		HawkScope::MallocPtr scope(pData);
		memset(pData, 0, iSize);
		iSize = (Size_t)xFile.Read(pData, xFile.GetFileSize());
		xFile.Close();

		//解析文档
		return ParseDocument(pDoc, pData);
	}

	Bool HawkXmlFile::ParseData(const void* pData, Size_t iSize, PVoid pDoc)
	{
		if (!pData || iSize <=0 || !pDoc)
			return false;

		//解析文档
		return ParseDocument(pDoc, pData);
	}
}
