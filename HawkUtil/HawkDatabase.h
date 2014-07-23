#ifndef HAWK_DATABASE_H
#define HAWK_DATABASE_H

#include "HawkRefCounter.h"

namespace Hawk
{		
	/************************************************************************/
	/* 数据库操作基类封装                                                   */
	/************************************************************************/
	class UTIL_API HawkDatabase : public HawkRefCounter
	{
	public:
		//DB查询结果集
		class UTIL_API QueryResult : public HawkRefCounter
		{
		public:
			//构造
			QueryResult();

			//析构
			virtual ~QueryResult();
			
			//友员类申明
			friend class HawkDatabase;

		public:
			//结果行数
			DEFINE_PROPERTY(UInt64, m_iRow,	   Row);
			//结果列数
			DEFINE_PROPERTY(UInt64, m_iCol,	   Col);
			//影响行数
			DEFINE_PROPERTY(UInt64, m_iAffect, AffectRows);
			//结果集句柄
			DEFINE_PROPERTY(PVoid,  m_pResHdl, ResHdl);
			//数据库句柄
			DEFINE_PROPERTY(PVoid,  m_pDBHdl,  DBHdl);

		public:
			//根据行列获取返回结果
			const Utf8* GetResult(UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0) const;			

		public:
			//根据索引获取返回结果
			const Utf8* operator[] (UInt64 iIdx) const;

			//操作符的行列访问
			const Utf8* operator()(UInt64 iRow, UInt64 iCol) const;			
		};

		//DB连接对象
		class UTIL_API DBConn : public HawkRefCounter
		{
		public:
			//默认构造
			DBConn(UInt32 iType = HDB_UNKNOWN);

			//析构
			virtual ~DBConn();

		public:
			//主机地址
			DEFINE_PROPERTY(AString, m_sHost,	Host);
			//主机端口
			DEFINE_PROPERTY(Int32,   m_iPort,	Port);
			//登陆用户
			DEFINE_PROPERTY(AString, m_sUser,	User);
			//登陆密码
			DEFINE_PROPERTY(AString, m_sPwd,	Pwd);
			//连接DB
			DEFINE_PROPERTY(AString, m_sDBName,	DBName);
			//DB类型
			DEFINE_PROPERTY(UInt32,  m_iType,	Type);

		public:
			//赋值操作符
			DBConn& operator = (const DBConn& sConn);

		public:
			//连接有效性判断
			Bool  IsValid() const;

			//连接属性重置
			void  Reset();
		};

		//申明友员访问
		friend class QueryResult;

	public:
		//构造
		HawkDatabase();

		//析构
		virtual ~HawkDatabase();

	public:
		//支持的数据库类型定义
		enum
		{
			HDB_UNKNOWN,
			HDB_SQLITE,
			HDB_MYSQL,
		};

	public:
		//数据库类型
		DEFINE_PROPERTY(UInt32,  m_iDBType,	  DBType);
		//数据库名称
		DEFINE_PROPERTY(AString, m_sDBName,	  DBName);
		//数据库句柄
		DEFINE_PROPERTY(PVoid,   m_pDBHandle, DBHandle);
		//数据库连接
		DEFINE_PROPERTY(DBConn,  m_sDBConn,   DBConn);
		//错误信息
		DEFINE_PROPERTY(UString, m_sErrMsg,   ErrMsg);
		//错误码
		DEFINE_PROPERTY(Int32,   m_iErrCode,  ErrCode);
		//是否在事物中
		DEFINE_PROPERTY(Bool,    m_bTrans,    IsTrans);

	public:
		//打开数据库
		virtual Bool	Open(const DBConn& sConn);

		//关闭数据库
		virtual Bool	Close();
		
	public:		
		//SQL语句执行
		virtual Bool	Execute(const UString& sSql);		

		//数据查询
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//数据插入
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);

		//数据更新
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);

		//数据删除
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//开始事物处理
		virtual Bool	BeginTrans();

		//回滚事物
		virtual Bool	RollbackTrans();

		//提交事物
		virtual Bool	CommitTrans();

	public:
		//数据库字符串修正(非法字符串处理)
		virtual UString AmendString(const UString& sValue) const;

		//数据库字符串修正(非法字符串处理)
		virtual AString AmendString(const AString& sValue) const;

	protected:
		//获取查询结果集
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);

		//释放查询结果
		virtual Bool	FreeResult(QueryResult* pResult);	

		//生成错误信息
		virtual void    GenError();

		//清除当前错误
		virtual void	ClearError();
	};

	typedef HawkDatabase::DBConn HawkDBConn;
}
#endif
