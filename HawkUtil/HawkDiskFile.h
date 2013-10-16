#ifndef HAWK_DISKFILE_H
#define HAWK_DISKFILE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 文件操作基类,提供文件操作的基本接口                                  */
	/************************************************************************/
	class UTIL_API HawkDiskFile : public HawkFile
	{
	public:
		//默认构造函数
		HawkDiskFile();

		//析构文件资源
		virtual ~HawkDiskFile();

	public:
		//文件名
		DEFINE_PROPERTY(AString, m_sFileName, FileName);	

	public:
		//打开文件,sFile为文件名,eCode只有在做写文件的时候才有效
		virtual Bool   Open(const AString& sFile, OpenType eOpen = OPEN_READ);

		//读取文件内容,pData为存储的空间,iSize为读取字节数
		virtual Int64  Read(void* pData, Int64 iSize);

		//写文件,pData为写入的内容,iSize为写的字节数
		virtual Int64  Write(const void* pData, Int64 iSize, Bool bFlush = false);

		//指针偏移
		virtual Int64  Seek(Int64 iOffset, SeekPos ePos = POS_BEGIN);

		//计算到文件头偏移字节
		virtual Int64  Tell();

		//flush文件缓冲
		virtual Bool   Flush();

		//关闭文件并释放所站用内存
		virtual Bool   Close();

		virtual Bool   Chsize(Int64 iSize);

	public:
		//文件句柄
		virtual FILE*  GetFileHandle();

		//写编码字符
		virtual Bool   WriteCodeType(CodeType eType);

		//读一行
		virtual Int64  ReadLine(void* pData, Int64 iBufSize,UChar cSeparate = '\n');

		//文件指针是否到结尾
		virtual Bool   IsEndPos();

	protected:
		//文件句柄
		FILE*	m_pFile;
	};
}
#endif
