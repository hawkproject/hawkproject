#ifndef HAWK_ZIPREADER_H
#define HAWK_ZIPREADER_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* Zip文件读取封装                                                      */
	/************************************************************************/
	class UTIL_API HawkZipReader : public HawkRefCounter
	{
	public:
		//构造
		HawkZipReader(const AString& sFile = "");

		//析构
		virtual ~HawkZipReader();

	public:
		//文件信息列表
		struct ZipInfo
		{
			AString FileName;
			Int32	FileSize;
			Int32   ComSize;
			Bool    IsFile;

			ZipInfo()
			{
				FileName = "";
				FileSize = 0;
				ComSize  = 0;
				IsFile   = false;
			}

			Bool operator == (const ZipInfo& sInfo)
			{
				return FileName == sInfo.FileName;
			}
		};

		//类型定义
		typedef map<AString, ZipInfo>  ZipFiles;
		
		//属性定义
		DEFINE_PROPERTY(ZipFiles, m_mFiles, ZipFiles);

	public:
		//打开Zip文件
		virtual Bool  Open(const AString& sFile);

		//是否存在文件
		virtual Bool  ExistFile(const AString& sZipFile, ZipInfo* pInfo = 0);

		//读文件
		virtual Bool  Read(const AString& sZipFile, void*  pData, Int32 iSize);

		//压缩一个文件到磁盘目录下
		virtual Bool  UnzipFile(const AString& sZipFile, const AString& sDiskFile = "");

		//解压缩到指定目录
		virtual Bool  Unzip(const AString& sPath);

		//关闭文件夹
		virtual Bool  Close();

	protected:
		//文件句柄
		PVoid  m_pZip;
	};
}
#endif
