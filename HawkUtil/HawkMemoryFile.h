#ifndef HAWK_MEMORYFILE_H
#define HAWK_MEMORYFILE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 内存文件操作                                                         */
	/************************************************************************/
	class UTIL_API HawkMemoryFile : public HawkFile
	{	
	public:
		//内存文件构造
		HawkMemoryFile();
		
		//内存文件析构
		virtual ~HawkMemoryFile();

	public:
		//文件位置
		DEFINE_PROPERTY(Int64, m_iFilePos, FilePos);

		//文件数据
		DEFINE_PROPERTY(PVoid, m_pData,	   DataPtr);

		//外带数据
		DEFINE_PROPERTY(Bool,  m_bExtra,   IsExtra);

	public:
		//打开内存文件
		virtual Bool     Open(Int64 iSize, OpenType eOpen = OPEN_WRITE);

		virtual Bool     Open(void* pData, Int64 iSize, Bool bExtra, OpenType eOpen = OPEN_READ);

		//打开文件读入内存
		virtual Bool     Open(const AString& sFile);

		//读取文件内容,pData为存储的空间,iSize为读取字节数
		virtual Int64	 Read(void* pData, Int64 iSize);

		//写文件,pData为写入的内容,iSize为写的字节数
		virtual Int64	 Write(const void* pData, Int64 iSize, Bool bFlush = false);

		//文件偏移操作,iSize为文件偏移字节数目,ePos为文件偏移的起始位置
		virtual Int64	 Seek(Int64 iOffset, SeekPos ePos = POS_BEGIN);
		
		//获得文件偏移
		virtual Int64    Tell();

		//清空文件内容(只移动位置, 不释放)
		virtual Bool     Clear();
		
		//计算文件CRC校验码
		virtual UInt32   CalcCrc() const;

		//写结尾符'0'
		virtual Int64    WriteZeroTerminater();

		//关闭文件并释放所站用内存
		virtual Bool     Close();

		//改变文件大小(截断或者扩充文件大小)
		virtual Bool     Chsize(Int64 iSize);

		//保存到磁盘文件
		virtual Bool     SaveToDisk(const AString& sFile);
	};
}
#endif
