#ifndef HAWK_XMLFILE_H
#define HAWK_XMLFILE_H

#include "HawkDiskFile.h"
#include "HawkXmlAttribute.h"
#include "HawkXmlElement.h"
#include "HawkXmlDocument.h"

namespace Hawk
{
	/************************************************************************/
	/* XML文件读写封装(编码格式: ASCII | UTF8)								*/
	/************************************************************************/
	class UTIL_API HawkXmlFile : public HawkRefCounter
	{
	public:
		//XML文件构造
		HawkXmlFile();

		//XML文件析构
		virtual ~HawkXmlFile();

	public:
		//打开文件
		template<class StringType> Bool  Open(const AString& sFile, HawkXmlDocument<StringType>& sDoc)
		{
			return ParseFile(sFile, &sDoc);
		}

		//从内存加载pData内存数据,iSize为数据字节数,bCodeHeader包含编码字节头字节,eCode编码类型
		template<class StringType> Bool  Open(const void* pData, Int32 iSize, HawkXmlDocument<StringType>& sDoc)
		{
			return ParseData(pData, iSize, &sDoc);
		}

	protected:
		//解析XML文件内容
		virtual Bool  ParseFile(const AString& sFile, PVoid pDoc);

		//解析XML内存内容
		virtual Bool  ParseData(const void* pData, Size_t iSize, PVoid pDoc);
	};
}
#endif
