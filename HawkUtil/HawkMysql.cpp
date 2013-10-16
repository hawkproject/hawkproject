#include "HawkStdHeader.h"

#ifdef HAWK_MYSQL
#	include "mysql.h"
#endif

#include "HawkMysql.h"
#include "HawkScope.h"
#include "HawkDBManager.h"
#include "HawkStringUtil.h"

namespace Hawk
{
#ifdef HAWK_MYSQL
	//////////////////////////////////////////////////////////////////////////
	//mysql连接在 wait-timeout (s) 内没有任何操作执行，服务器主动断开连接   //
	//可以在my.ini里面的mysqld 下设置wai-timeout值来控制					//
	//////////////////////////////////////////////////////////////////////////
	/*my.ini配置
	[mysql]
	default-character-set=utf8

	[mysqld]
	character-set-server=utf8
	default-storage-engine=MYISAM
	wait_timeout=604800
	interactive_timeout=604800
	max_connections=1024
	*/

	HawkMysql::HawkMysql() : HawkDatabase()
	{
		m_iDBType = HawkDatabase::HDB_MYSQL;
	}

	HawkMysql::~HawkMysql()
	{
		Close();
	}

	Bool HawkMysql::Open(const HawkDatabase::DBConn& sConn)
	{
		ClearError();

		MYSQL* pSql = mysql_init((MYSQL*)m_pDBHandle);
		if(!pSql) 
			return false;
		
		m_pDBHandle = pSql;

		Int32 iTimeout = 60;
		Int32 iRes	   = 0;
		
		iRes = mysql_options(pSql, MYSQL_OPT_CONNECT_TIMEOUT, (Char *)&iTimeout);
		if(iRes) 
		{
			GenError();
			return false;
		}

		UInt32 iCltFlag = CLIENT_FOUND_ROWS;

		AString sHost = "localhost";
		if (sConn.GetHost().size())
			sHost = sConn.GetHost();

		Int32 iPort = 3306;
		if (sConn.GetPort())
			iPort = sConn.GetPort();

		AString sUser = "root";
		if (sConn.GetUser().size())
			sUser = sConn.GetUser();

		AString sPwd = "";
		if (sConn.GetPwd().size())
			sPwd = sConn.GetPwd();

		pSql = mysql_real_connect(pSql,sHost.c_str(),sUser.c_str(),sPwd.c_str(),(const Char*)sConn.GetDBName().c_str(),iPort,0,iCltFlag);
		if(!pSql) 
		{
			GenError();
			return false;
		}

		iRes = mysql_set_server_option(pSql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
		if(iRes) 
		{
			GenError();
			return false;
		}

		if (sConn.GetDBName().size() && !SelectDB(sConn.GetDBName()))
		{
			GenError();
			return false;
		}

		return true;
	}

	Bool HawkMysql::Close()
	{
		if (m_pDBHandle)
		{
			mysql_close((MYSQL *)m_pDBHandle);
			m_pDBHandle = 0;
			return true;
		}
		return false;
	}

	void HawkMysql::GenError()
	{
		if (m_pDBHandle)
		{
			m_iErrCode = mysql_errno((MYSQL *)m_pDBHandle);
			m_sErrMsg  = (Utf8*)mysql_error((MYSQL *)m_pDBHandle);
		}			
	}

	UString HawkMysql::AmendString(const UString& sValue)
	{
		Int32 iSize = sValue.size()*2 + 1;
		Utf8* pBuf  = (Utf8*)HawkMalloc(iSize*sizeof(Utf8));
		memset(pBuf,0,iSize * sizeof(Utf8));
		HawkScope::MallocPtr scope(pBuf);

		mysql_real_escape_string((MYSQL *)m_pDBHandle,(Char*)pBuf,(const Char*)sValue.c_str(),sValue.size());
		return pBuf;
	}

	AString HawkMysql::AmendString(const AString& sValue)
	{
		Int32 iSize = sValue.size()*2 + 1;
		Char* pBuf  = (Char*)HawkMalloc(iSize * sizeof(Char));
		memset(pBuf,0,iSize * sizeof(Char));
		HawkScope::MallocPtr scope(pBuf);

		mysql_real_escape_string((MYSQL *)m_pDBHandle,pBuf,sValue.c_str(),sValue.size());
		return pBuf;
	}

	Bool HawkMysql::ChangeUser(const AString& sUser, const AString& sPwd, const AString& sDBName)
	{
		if(!m_pDBHandle)
			return false;

		ClearError();

		if(mysql_change_user((MYSQL *)m_pDBHandle,sUser.c_str(),sPwd.c_str(),sDBName.c_str()))
			return true;

		GenError();
		return false;
	}

	Bool HawkMysql::IsExistDB(const AString& sDBName)
	{
		if(!m_pDBHandle)
			return false;

		ClearError();

		Char sCmd[SQL_DEFAULT_SIZE] = {0};
		sprintf(sCmd,"SELECT * FROM information_schema.SCHEMATA WHERE SCHEMA_NAME = '%s';",sDBName.c_str());

		Int32 iRes = mysql_query((MYSQL *)m_pDBHandle,(const Char*)sCmd);
		if(iRes) 
		{
			GenError();
			return false;
		}

		MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
		if (pRes)
		{
			UInt64 iRow = mysql_num_rows(pRes);
			UInt64 iCol = mysql_num_fields(pRes);
			mysql_free_result(pRes);
			return iRow > 0 && iCol > 0;
		}

		return false;
	}

	Bool HawkMysql::CreateDB(const AString& sDBName)
	{
		if(!m_pDBHandle)
			return false;

		ClearError();

		Char sCmd[SQL_DEFAULT_SIZE] = {0};
		sprintf(sCmd,"CREATE DATABASE '%s';",sDBName.c_str());

		Int32 iRes = mysql_query((MYSQL *)m_pDBHandle,(const Char*)sCmd);
		if(iRes) 
		{
			GenError();
			return false;
		}

		ClearResult();

		return true;
	}

	Bool HawkMysql::SelectDB(const AString& sDBName)
	{
		if(!m_pDBHandle)
			return false;

		ClearError();

		Int32 iRes = mysql_select_db((MYSQL *)m_pDBHandle, sDBName.c_str());
		if(iRes)
		{
			GenError();
			return false;
		}

		ClearResult();

		return true;
	}

	Bool HawkMysql::IsExistTable(const AString& sDBName,const AString& sTblName)
	{
		if(!m_pDBHandle)
			return false;

		ClearError();

		if (sDBName.size() && sTblName.size())
		{
			Char sCmd[SQL_DEFAULT_SIZE] = {0};
			sprintf(sCmd,"SELECT * FROM information_schema.TABLES WHERE TABLE_SCHEMA = '%s' and TABLE_NAME = '%s';",sDBName.c_str(),sTblName.c_str());

			Int32 iRes = mysql_query((MYSQL *)m_pDBHandle,(const Char*)sCmd);
			if(iRes) 
			{
				GenError();
				return false;
			}

			MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if (pRes)
			{
				UInt64 iRow = mysql_num_rows(pRes);
				UInt64 iCol = mysql_num_fields(pRes);
				mysql_free_result(pRes);
				return iRow > 0 && iCol > 0;
			}			
		}

		return false;
	}

	UInt64 HawkMysql::GetInsertId()
	{
		return (UInt64)mysql_insert_id((MYSQL*)m_pDBHandle);
	}

	void HawkMysql::ClearResult()
	{
		//取出结果并清空
		MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
		if(pRes) 
			mysql_free_result(pRes);

		while (!mysql_next_result((MYSQL *)m_pDBHandle))
		{
			pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if(pRes) 
				mysql_free_result(pRes);
		}
	}

	Int32 HawkMysql::MysqlQuery(const UString& sSql)
	{
		if (m_pDBHandle == 0)
			return HAWK_ERROR;

		return mysql_query((MYSQL *)m_pDBHandle,(const Char*)sSql.c_str());
	}

	Bool HawkMysql::Execute(const UString& sSql)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			Int32 iRes = MysqlQuery(sSql);
			if(iRes) 
			{
				GenError();
				return false;
			}
			
			ClearResult();

			return true;
		}		

		return false;
	}

	Bool HawkMysql::Select(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{			
			ClearError();

			sResult.SetDBHdl(this);

			Int32 iRes = MysqlQuery(sSql);
			if(iRes) 
			{
				GenError();
				return false;
			}

			MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if (pRes)
			{
				sResult.SetResHdl((void*)pRes);
				sResult.SetRow(mysql_num_rows(pRes));
				sResult.SetCol(mysql_num_fields(pRes));
			}

			ClearResult();
			return true;
		}
		return false;
	}

	Bool HawkMysql::Insert(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{			
			ClearError();

			sResult.SetDBHdl(this);

			Int32 iRes = MysqlQuery(sSql);
			if(iRes) 
			{
				GenError();
				return false;
			}			

			sResult.SetAffectRows(mysql_affected_rows((MYSQL *)m_pDBHandle));

			MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if (pRes)
			{
				sResult.SetResHdl((void*)pRes);
				sResult.SetRow(mysql_num_rows(pRes));
				sResult.SetCol(mysql_num_fields(pRes));		
			}

			ClearResult();
			return true;
		}
		return false;
	}

	Bool HawkMysql::Update(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{			
			ClearError();

			sResult.SetDBHdl(this);

			Int32 iRes = MysqlQuery(sSql);
			if(iRes) 
			{
				GenError();
				return false;
			}

			sResult.SetAffectRows(mysql_affected_rows((MYSQL *)m_pDBHandle));

			MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if (pRes)
			{
				sResult.SetResHdl((void*)pRes);
				sResult.SetRow(mysql_num_rows(pRes));
				sResult.SetCol(mysql_num_fields(pRes));			
			}

			ClearResult();
			return true;
		}
		return false;
	}

	Bool HawkMysql::Delete(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{			
			ClearError();

			sResult.SetDBHdl(this);

			Int32 iRes = MysqlQuery(sSql);
			if(iRes) 
			{
				GenError();
				return false;
			}

			sResult.SetAffectRows(mysql_affected_rows((MYSQL *)m_pDBHandle));

			MYSQL_RES* pRes = mysql_store_result((MYSQL *)m_pDBHandle);
			if (pRes)
			{
				sResult.SetResHdl((void*)pRes);
				sResult.SetRow(mysql_num_rows(pRes));
				sResult.SetCol(mysql_num_fields(pRes));
			}

			ClearResult();

			return true;
		}
		return false;
	}

	Bool HawkMysql::FreeResult(QueryResult* pResult)
	{
		if (m_pDBHandle && pResult)
		{
			MYSQL_RES* pRes = (MYSQL_RES*)pResult->GetResHdl();
			if (pRes)
				mysql_free_result(pRes);
			return true;
		}
		return false;
	}

	Utf8* HawkMysql::GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen)
	{		
		if (!pResult || !pResult->GetDBHdl())
			return 0;

		MYSQL_RES* pRes = (MYSQL_RES*)pResult->GetResHdl();
		if (!pRes)
			return 0;

		UInt64 iTotalRow = pResult->GetRow();
		UInt64 iTotalCol = pResult->GetCol();
		if (iTotalRow <= 0 || iTotalCol <= 0 )
			return 0;

		A_Exception(iRow >= 0 && iRow < iTotalRow);
		A_Exception(iCol >= 0 && iCol < iTotalCol);
		
		mysql_data_seek(pRes,iRow);

		MYSQL_ROW ppRow = mysql_fetch_row(pRes);
		UInt32* ppLen   = (UInt32*)mysql_fetch_lengths(pRes);
		if (!ppRow || !ppLen)
			return 0;

		if (pResLen)
			*pResLen = (Int32)ppLen[iCol];

		return (Utf8*)ppRow[iCol];
	}
#endif
}
