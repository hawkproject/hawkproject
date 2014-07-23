#ifndef HAWK_XMLDOCUMENT_H
#define HAWK_XMLDOCUMENT_H

#include "HawkXmlElement.h"

namespace Hawk
{
	/************************************************************************/
	/* XML�ĵ�������                                                        */
	/************************************************************************/
	template<class StringType> class HawkXmlDocument : public HawkRefCounter
	{
	public:
		//XML�ĵ�Ĭ�Ϲ���
		HawkXmlDocument()
		{
		}

		//XML�ĵ�����
		virtual ~HawkXmlDocument()
		{
		}

	protected:
		//XML�ĵ����ڵ�
		HawkXmlElement<StringType>	Root;

	public:		
		//��ȡ�ĵ����ڵ�
		HawkXmlElement<StringType>* GetRoot()
		{
			return &Root;
		}

		//XML��ʽ���ַ���
		StringType ToString() const
		{
			StringType sRet = Root.ToString();
			HawkStringUtil::Append<StringType,AString>(sRet,"\r\n");
			return sRet;
		}

		//�洢���ļ�
		Bool Save(const AString& sFile, HawkFile::CodeType eCode)
		{
			HawkDiskFile xFile;
			if (!xFile.Open(sFile,HawkFile::OPEN_WRITE))
				return false;

			//�ؽ����θ��ӽṹ
			RebuildRelation();

			//���ݱ��벻ͬ�洢
			if (eCode == HawkFile::CODE_ASCII)
			{				
				AString sTmp = (const Char*)ToString().c_str();
				xFile.Write(sTmp.c_str(),sTmp.size()*sizeof(Char));
			}
			else if (eCode == HawkFile::CODE_UTF8)
			{
				static UChar UTF8_TAG[] = {0xEF,0xBB,0xBF};
				xFile.Write(UTF8_TAG,3);
				UString sTmp = (const Utf8*)ToString().c_str();
				xFile.Write(sTmp.c_str(),sTmp.size()*sizeof(Utf8));
			}
			else
			{
				static UChar UNICODE_TAG[] = {0xFF,0xFE};
				xFile.Write(UNICODE_TAG,2);
				WString sTmp = (const WChar*)ToString().c_str();
				xFile.Write(sTmp.c_str(),sTmp.size()*sizeof(WChar));
			}
			xFile.Close();
			return true;
		}

		//XML�ĵ��ڵ�����
		void Clear()
		{
			Root.Clear();
		}

		//�ؽ��ڵ㸸�ӹ�ϵ
		void RebuildRelation()
		{
			Root.Parent = 0;
			Root.Level  = 0;

			vector< HawkXmlElement<StringType>* > ptr_list;
			ptr_list.push_back(&Root);

			UInt32 iIdx = 0;
			//ѭ����ʽ��ܵݹ����
			while (iIdx < ptr_list.size())
			{
				HawkXmlElement<StringType>* pEle = ptr_list[iIdx++];
				for (Size_t i = 0;i<pEle->Children.size();i++)
				{
					pEle->Children[i].Parent = pEle;
					pEle->Children[i].Level  = pEle->Level+1;
					ptr_list.push_back(&pEle->Children[i]);
				}
			}
		}		
	};

	//XML�ĵ���ͬ����Ķ���
	typedef HawkXmlDocument<AString>	AXmlDocument;
	typedef HawkXmlDocument<UString>	UXmlDocument;
	typedef HawkXmlDocument<WString>	WXmlDocument;
}
#endif
