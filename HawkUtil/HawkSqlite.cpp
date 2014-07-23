#include "HawkSqlite.h"
#include "HawkScope.h"
#include "HawkOSOperator.h"
#include "HawkDBManager.h"
#include "HawkLoggerManager.h"
#include "sqlite3.h"

namespace Hawk
{
	//非法字符串转换处理: 
	//:		' -> ''
	//:		/ -> //		
	//:		[ -> /[
	//:		] -> /]
	//:		% -> /%
	//:		& -> /&
	//:		_ -> /_
	//:		( -> /(
	//:		) -> /)
	static const Char sqlite3_trans_chars[] = 
	{ 
		'\'','/','[',']','%','&','_','(',')',0 
	};

	Bool  is_trans_char(Char cVal)
	{
		Size_t iTrans = ARRAY_SIZE(sqlite3_trans_chars);
		for (Size_t i=0;i<iTrans;i++)
		{
			if (cVal == sqlite3_trans_chars[i])
				return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	HawkSqlite::HawkSqlite() : HawkDatabase()
	{
		m_iDBType = HawkDatabase::HDB_SQLITE;
	}

	HawkSqlite::~HawkSqlite()
	{
		Close();
	}

	Bool HawkSqlite::Open(const HawkDatabase::DBConn& sConn)
	{
		ClearError();

		if (!sqlite3_threadsafe())
		{
			HawkFmtError("Sqlite3 Cannot Support MultiThread.");
		}

		if (m_pDBHandle)
			sqlite3_close((sqlite3*)m_pDBHandle);

		HawkOSOperator::MakeSureFileName(sConn.GetDBName());

		Long lHdlAddr = (Long)&m_pDBHandle;

		Int32 iRes = sqlite3_open_v2(sConn.GetDBName().c_str(), reinterpret_cast<sqlite3**>(lHdlAddr),
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,0);

		if(iRes != SQLITE_OK)
			return false;

		return true;
	}

	Bool HawkSqlite::Close()
	{		
		if (m_pDBHandle)
		{
			ClearError();

			CommitTrans();

			Int32 iRes = sqlite3_close((sqlite3*)m_pDBHandle);
			if (iRes == SQLITE_OK)
			{
				m_pDBHandle = 0;
				return true;
			}
		}
		return false;
	}

	void HawkSqlite::GenError()
	{
		if (m_pDBHandle)
		{			
			m_iErrCode= sqlite3_errcode((sqlite3*)m_pDBHandle);
		}		
	}

	UString HawkSqlite::AmendString(const UString& sValue)
	{
		AString sRet = AmendString((Char*)sValue.c_str());
		return (Utf8*)sRet.c_str();
	}

	AString HawkSqlite::AmendString(const AString& sValue)
	{
		Int32 iSize = (Int32)sValue.size()*2 + 1;
		Char* pBuf  = (Char*)HawkMalloc(iSize * sizeof(Char));
		memset(pBuf,0,iSize * sizeof(Char));
		HawkScope::MallocPtr scope(pBuf);		
		Char* pPos = pBuf;

		for (Size_t i=0;i<sValue.size();i++)
		{
			if (is_trans_char(sValue[i]))
			{
				if(sValue[i] == '\'')
				{
					*pPos++ = '\'';
					*pPos++ = '\'';

					//跳过后续点
					if (i < sValue.size()-1 && sValue[i+1] == '\'')
						i += 1;					
				}
				else
				{
					if (i > 0 && sValue[i-1] != '/')
						*pPos++ = '/';

					*pPos++ = sValue[i];
				}
			}
			else
			{
				*pPos++ = sValue[i];
			}
		}

		return pBuf;
	}

	Bool HawkSqlite::BeginTrans()
	{
		if (m_pDBHandle && !m_bTrans)
		{
			ClearError();

			Char* pErrMsg = 0;
			Int32 iRes    = sqlite3_exec((sqlite3*)m_pDBHandle,"BEGIN;",0,0,&pErrMsg);			
			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;				
				sqlite3_free(pErrMsg);
				return false;
			}

			m_bTrans = true;
			return true;
		}
		return false;		
	}

	Int32 HawkSqlite::SqliteExec(const UString& sSql, Char** ppErrMsg)
	{
		if (!m_pDBHandle)
			return SQLITE_ERROR;

		return sqlite3_exec((sqlite3*)m_pDBHandle,(const Char*)sSql.c_str(),0,0,ppErrMsg);
	}

	Bool HawkSqlite::Execute(const UString& sSql)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			Char* pErrMsg = 0;
			Int32 iRes    = SqliteExec(sSql,&pErrMsg);
			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			return true;
		}
		return false;
	}

	Bool HawkSqlite::Select(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			sResult.SetDBHdl(this);

			Char*  pErrMsg = 0;
			Char** ppData  = 0;
			Int32 iRow     = 0;
			Int32 iCol     = 0;
			Int32 iRes     = sqlite3_get_table((sqlite3*)m_pDBHandle,(const Char*)sSql.c_str(),&ppData,&iRow,&iCol,&pErrMsg);

			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			sResult.SetRow(iRow);
			sResult.SetCol(iCol);
			sResult.SetResHdl((void*)ppData);
			return true;
		}
		return false;
	}

	Bool HawkSqlite::Insert(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			sResult.SetDBHdl(this);

			Char* pErrMsg = 0;
			Int32 iRes    = SqliteExec(sSql,&pErrMsg);
			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			sResult.SetAffectRows(sqlite3_changes((sqlite3*)m_pDBHandle));
			return true;
		}
		return false;
	}

	Bool HawkSqlite::Update(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			sResult.SetDBHdl(this);

			Char* pErrMsg = 0;
			Int32 iRes    = SqliteExec(sSql,&pErrMsg);
			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			sResult.SetAffectRows(sqlite3_changes((sqlite3*)m_pDBHandle));
			return true;
		}
		return false;
	}

	Bool HawkSqlite::Delete(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			ClearError();

			sResult.SetDBHdl(this);

			Char* pErrMsg = 0;
			Int32 iRes	  = SqliteExec(sSql,&pErrMsg);
			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			sResult.SetAffectRows(sqlite3_changes((sqlite3*)m_pDBHandle));
			return true;
		}
		return false;
	}

	Bool HawkSqlite::FreeResult(QueryResult* pResult)
	{
		if (m_pDBHandle && pResult && pResult->GetResHdl())
		{
			sqlite3_free_table((Char**)pResult->GetResHdl());
			return true;
		}
		return false;
	}

	Bool HawkSqlite::RollbackTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			ClearError();

			Char* pErrMsg = 0;
			Int32 iRes    = SqliteExec((Utf8*)"ROLLBACK;",&pErrMsg);

			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}

			m_bTrans = false;
			return true;
		}
		return false;
	}

	Bool HawkSqlite::CommitTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			ClearError();

			Char* pErrMsg = 0;
			Int32 iRes    = SqliteExec((Utf8*)"COMMIT;",&pErrMsg);

			if (iRes != SQLITE_OK)
			{
				GenError();
				m_sErrMsg = (Utf8*)pErrMsg;
				sqlite3_free(pErrMsg);
				return false;
			}
			m_bTrans = false;
			return true;
		}
		return false;
	}

	Utf8* HawkSqlite::GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol,UInt32* pResLen)
	{
		if (!pResult || !pResult->GetDBHdl() || !pResult->GetResHdl())
			return 0;

		UInt64 iTotalRow = pResult->GetRow();
		UInt64 iTotalCol = pResult->GetCol();
		if (iTotalRow <= 0 || iTotalCol <= 0 )
			return 0;

		//跳过第一行列名
		Utf8** ppResult = (Utf8**)pResult->GetResHdl() + iTotalCol;
		return ppResult[iRow * iTotalCol + iCol];
	}
}
