#ifndef HAWK_PACKAGE_H
#define HAWK_PACKAGE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 文件数据包,其中数据包的路径分隔符为"/"                               */
	/************************************************************************/
	class UTIL_API HawkPackage : public HawkRefCounter
	{
	public:
		//打开文件包
		HawkPackage(const AString& sPackFile = "",Bool bOnlyRead = true);

		//打开内存文件包
		HawkPackage(void* pData,Int32 iSize);

		//包文件析构
		virtual ~HawkPackage();

	public:
		#pragma pack(push,1)
		//数据包文件头信息
		struct PkgHeader
		{
			UInt32 PackMagic;
			UInt8  PackVersion;
			UInt32 FileCount[2];

			PkgHeader() { memset(this, 0, sizeof(*this)); }
		};

		//数据包文件信息
		struct PkgFile
		{
			UInt32 DataCrc;
			UInt64 DataOffset;
			UInt32 DataSize;
			UInt32 OrigSize;
			UInt16 NameSize;
			Bool   Valid;
			
			PkgFile()
			{
				memset(this, 0, sizeof(*this));
			}
		};
		#pragma pack(pop)

		//文件信息表
		typedef map<AString, PkgFile> PkgFileMap;

		//版本号定义
		enum { PACKAGE_VERSION = 1};

	public:
		//打开文件包(bOnlyRead为true时文件必须存在,可写的时候,如果文件不存在就创建
		virtual Bool     OpenPackage(const AString& sDiskFile, Bool bOnlyRead = true);

		virtual Bool     OpenPackage(const void* pData, UInt32 iSize);

		//获得数据包头信息
		const PkgHeader* GetPkgHeader() const;

		//获得文件信息
		const PkgFile*   GetFileInfo(const AString& sPackFile);

		//判断是否存在文件
		virtual Bool     ExistFile(const AString& sPackFile);

		//判断目录是否存在
		virtual Bool     ExistFolder(const AString& sPackFolder);

		//读文件,返回数据字节大小按照iSize为准
		virtual Bool     Read(void*  pData, UInt32& iSize, const AString& sPackFile);

		//写文件
		virtual Bool     Write(void* pData, UInt32 iSize, const AString& sPackFile);

		//写文件
		virtual Bool     Write(const AString& sDiskFile, const AString& sPackFile = "");

		//移除一个文件
		virtual Bool     RemoveFile(const AString& sPackFile);

		//移除一个目录
		virtual Bool     RemoveFolder(const AString& sPackFolder);

		//关闭文件包
		virtual Bool     Close();

		//压缩一个文件到磁盘目录下
		virtual Bool     UnpackFile(const AString& sPackFile, const AString& sDiskFile = "");
		
		//解压缩到指定目录, 默认空字符串表示包内所有文件解压到当前目录("" ,"")
		virtual Bool     Unpack(const AString& sPackFolder = "", const AString& sDiskFolder = "");
	
		//整理数据包,删除无效文件,返回删除的文件数目
		virtual Bool     Marshal();

		//打印出所有文件信息
		virtual void     PrintAll();

	protected:
		//分析文件包数据
		virtual Bool     Analyze();

	protected:
		HawkFile*	m_pPackFile;
		PkgHeader   m_sPkgHeader;
		PkgFileMap  m_mFileInfo;
		Bool        m_bReadOnly;
	};
}

#endif
