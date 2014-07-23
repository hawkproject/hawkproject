#ifndef HAWK_XMLFILE_H
#define HAWK_XMLFILE_H

#include "HawkDiskFile.h"
#include "HawkXmlAttribute.h"
#include "HawkXmlElement.h"
#include "HawkXmlDocument.h"

namespace Hawk
{
	/************************************************************************/
	/* XML�ļ���д��װ(�����ʽ: ASCII | UTF8)								*/
	/************************************************************************/
	class UTIL_API HawkXmlFile : public HawkRefCounter
	{
	public:
		//XML�ļ�����
		HawkXmlFile();

		//XML�ļ�����
		virtual ~HawkXmlFile();

	public:
		//���ļ�
		template<class StringType> Bool  Open(const AString& sFile, HawkXmlDocument<StringType>& sDoc)
		{
			return ParseFile(sFile, &sDoc);
		}

		//���ڴ����pData�ڴ�����,iSizeΪ�����ֽ���,bCodeHeader���������ֽ�ͷ�ֽ�,eCode��������
		template<class StringType> Bool  Open(const void* pData, Int32 iSize, HawkXmlDocument<StringType>& sDoc)
		{
			return ParseData(pData, iSize, &sDoc);
		}

	protected:
		//����XML�ļ�����
		virtual Bool  ParseFile(const AString& sFile, PVoid pDoc);

		//����XML�ڴ�����
		virtual Bool  ParseData(const void* pData, Size_t iSize, PVoid pDoc);
	};
}
#endif
