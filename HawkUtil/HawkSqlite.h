#ifndef HAWK_SQLITE_H
#define HAWK_SQLITE_H

#include "HawkDatabase.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//Sqlite3数据库操作封装,默认采用Utf8编码								//
	//////////////////////////////////////////////////////////////////////////
	class UTIL_API HawkSqlite : public HawkDatabase
	{
	public:
		//DB对象构造
		HawkSqlite();

		//DB对象析构
		virtual ~HawkSqlite();

	public:
		//打开数据库
		virtual Bool	Open(const DBConn& sConn);

		//关闭数据库
		virtual Bool	Close();		

	public:		
		//数据库增删改查操作
		virtual Bool	Execute(const UString& sSql);

		//查询操作
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//插入操作
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);
		
		//更新操作
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);
		
		//删除操作
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//开始事物处理
		virtual Bool	BeginTrans();

		//回滚事物
		virtual Bool	RollbackTrans();

		//提交事物
		virtual Bool	CommitTrans();

	public:				
		//安全转义sqlite3字符串
		virtual UString AmendString(const UString& sValue);

		//安全转义sqlite3字符串
		virtual AString AmendString(const AString& sValue);
  
	protected:
		//Mysql查询
		virtual Int32	SqliteExec(const UString& sSql, Char** ppErrMsg);

		//释放查询结果
		virtual Bool	FreeResult(QueryResult* pResult);

		//获取查询结果集
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);

		//生成错误信息
		virtual  void	GenError();
	};
}
#endif
