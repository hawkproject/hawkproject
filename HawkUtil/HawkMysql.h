#ifndef HAWK_MYSQL_H
#define HAWK_MYSQL_H

#include "HawkDatabase.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//Mysql设置默认编码为Utf8(可在my.ini里面修改)                           //
	//////////////////////////////////////////////////////////////////////////
	class UTIL_API HawkMysql : public HawkDatabase
	{
	public:
		//Mysql数据库对象构造
		HawkMysql();
		
		//Mysql数据库对象析构
		virtual ~HawkMysql();

	public:
		//打开数据库
		virtual Bool	Open(const DBConn& sConn);

		//关闭数据库
		virtual Bool	Close();		

	public:		
		//数据库增删改查操作
		virtual Bool	Execute(const UString& sSql);

	public:
		//查询操作
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//插入操作
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);

		//更新操作
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);

		//删除操作
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//改变数据库使用用户
		virtual Bool	ChangeUser(const AString& sUser, const AString& sPwd, const AString& sDBName);	

		//存在数据库
		virtual Bool	IsExistDB(const AString& sDBName);

		//切换数据库
		virtual Bool	SelectDB(const AString& sDBName);

		//创建数据库
		virtual Bool	CreateDB(const AString& sDBName);

		//存在数据表
		virtual Bool	IsExistTable(const AString& sDBName, const AString& sTblName);

		//获取AUTO_INCREMENT插入列的索引
		virtual UInt64	GetInsertId();

	public:		
		//非法字符串处理: 
		//:		\x00(NULL)
		//:		\n(换行)
		//:		\r(回车)
		//:		\t(制表)
		//:		\b(退格)
		//:		\%(通配)
		//:		\(反斜线)
		//:		'(单引)
		//:		"(双引)
		//:		\x1a(CTRL+Z)
		//安全转义mysql字符串
		virtual UString AmendString(const UString& sValue);

		//安全转义mysql字符串
		virtual AString AmendString(const AString& sValue);

	protected:
		//Mysql查询
		virtual Int32	MysqlQuery(const UString& sSql);

		//获取查询结果集
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);
		
		//释放查询结果
		virtual Bool	FreeResult(QueryResult* pResult);

		//清理已有查询结果集
		virtual void	ClearResult();
		
		//生成错误信息
		virtual void    GenError();
	};
}
#endif
