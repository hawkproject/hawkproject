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
				//��ǩ����
				pElement->SetTag(StringType(pNode->name(), pNode->name_size()));

				//��������
				if (pNode->value_size())
					pElement->SetValue(StringType(pNode->value(), pNode->value_size()));

				//��ȡ����
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

				//�ӽڵ�Ԥ���ٿռ�
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
		//���ݱ����ʽ����
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

		//�����ĵ�
		return ParseDocument(pDoc, pData);
	}

	Bool HawkXmlFile::ParseData(const void* pData, Size_t iSize, PVoid pDoc)
	{
		if (!pData || iSize <=0 || !pDoc)
			return false;

		//�����ĵ�
		return ParseDocument(pDoc, pData);
	}
}
