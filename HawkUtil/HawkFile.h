#ifndef HAWK_FILE_H
#define HAWK_FILE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 文件操作基类,提供磁盘文件操作的基本接口                              */
	/************************************************************************/
	class UTIL_API HawkFile : public HawkRefCounter
	{
	public:
		//默认构造函数
		HawkFile();

		//析构文件资源
		virtual ~HawkFile();

	public:
		//编码格式
		enum CodeType
		{
			//ASCII编码格式
			CODE_ASCII,
			//UTF8编码格式
			CODE_UTF8,
			//UNICODE编码格式
			CODE_UNICODE,
		};

		//文件偏移位置
		enum SeekPos
		{
			//开始位置
			POS_BEGIN,
			//当前访问位置
			POS_CURRENT,
			//文件末尾
			POS_END,
		};

		//文件打开类型
		enum OpenType
		{
			//只读
			OPEN_READ = 0,
			//只写
			OPEN_WRITE,
			//可读可写(文件必须存在)
			OPEN_RW,
			//可读可写(文件会被自动创建,如果文件存在那么内容被清除)
			OPEN_WR,
			//后续追加
			OPEN_APPEND,
		};	

	public:
		//文件大小
		DEFINE_PROPERTY(Int64,	  m_iFileSize, FileSize);

		//打开方式
		DEFINE_PROPERTY(OpenType, m_eOpenType, OpenType);

	public:
		//打开文件,sFile为文件名,eCode只有在做写文件的时候才有效
		virtual Bool    Open(const AString& sFile, OpenType eOpen = OPEN_READ);

		//读取文件内容,pData为存储的空间,iSize为读取字节数
		virtual Int64   Read(void* pData, Int64 iSize);

		//读一行
		virtual Int64   ReadLine(void* pData, Int64 iBufSize, UChar cSeparate = '\n');

		//写文件,pData为写入的内容,iSize为写的字节数
		virtual Int64   Write(const void* pData, Int64 iSize, Bool bFlush = false);

		//指针偏移
		virtual Int64   Seek(Int64 iOffset, SeekPos ePos = POS_BEGIN);

		//计算到文件头偏移字节
		virtual Int64   Tell();

		//输出文件缓冲
		virtual Bool    Flush();

		//关闭文件并释放所站用内存
		virtual Bool    Close();

		//改变文件大小(截断或者扩充文件大小)
		virtual Bool    Chsize(Int64 iSize);

		//判断打开方式是否可写
		virtual Bool    IsWriteAble() const;

		//判断打开方式是否可读
		virtual Bool    IsReadAble() const;

	public:
		//获取数据头编码格式
		static CodeType GetCodeType(const void* pData);
	};
}
#endif
