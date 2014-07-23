#ifndef HAWK_KVFILE_H
#define HAWK_KVFILE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 键值对配置文件操作类                                                 */
	/************************************************************************/
	class UTIL_API HawkKVFile : public HawkRefCounter
	{
	public:
		//默认构造函数
		HawkKVFile();

		//析构文件资源
		virtual ~HawkKVFile();

	public:		
		//打开磁盘文件(eType支持OPEN_READ & OPEN_WRITE)
		virtual Bool Open(const AString& sFile, HawkFile::OpenType eType = HawkFile::OPEN_READ);		

		//从文件对象打开
		virtual Bool Open(HawkFile* pFile);

		//保存到文件
		virtual Bool Save(const AString& sFile);

	public:
		//查询指定Key是否存在
		virtual Bool IsKeyExist(const AString& sKey);

		//获取指定Key所对应的值
		virtual Bool QueryValue(const AString& sKey, AString& sVal);

		//添加kv数值
		virtual Bool SetKV(const AString& sKey, const AString& sVal);

		//删除指定的key值数据
		virtual Bool DelKey(const AString& sKey);

		//获取整体kv表数据
		const AStringMap& GetKVTbl() const;

	protected:
		AStringMap m_mKVTbl;
	};
}
#endif
