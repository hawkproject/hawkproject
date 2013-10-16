#include "HawkDatabase.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//查询结果集
	HawkDatabase::QueryResult::QueryResult()
	{
		m_iRow	  = 0;
		m_iCol    = 0;
		m_iAffect = 0;
		m_pResHdl = 0;
		m_pDBHdl  = 0;
	}

	HawkDatabase::QueryResult::~QueryResult()
	{
		if (m_pDBHdl && m_pResHdl)
		{
			//清理结果集
			((HawkDatabase*)m_pDBHdl)->FreeResult(this);

			//数据重置
			m_iRow	  = 0;
			m_iCol	  = 0;
			m_iAffect = 0;
		}
	}	

	const Utf8* HawkDatabase::QueryResult::GetResult(UInt64 iRow,UInt64 iCol,UInt32* pResLen) const
	{
		if (m_pDBHdl && m_pResHdl && m_iRow && m_iCol)
		{
			A_Exception(iRow >= 0 && iRow < m_iRow);
			A_Exception(iCol >= 0 && iCol < m_iCol);

			return ((HawkDatabase*)m_pDBHdl)->GetResult(this,iRow,iCol,pResLen);
		}
		return 0;
	}

	const Utf8* HawkDatabase::QueryResult::operator[] (UInt64 iIdx) const
	{		
		if (m_pDBHdl && m_pResHdl && m_iRow && m_iCol)
		{
			UInt64 iRow = iIdx / m_iCol;
			UInt64 iCol = iIdx % m_iCol;
			return GetResult(iRow, iCol);
		}
		return 0;
	}

	const Utf8* HawkDatabase::QueryResult::operator()(UInt64 iRow, UInt64 iCol) const
	{
		return GetResult(iRow,iCol);
	}

	//////////////////////////////////////////////////////////////////////////
	//数据库连接
	HawkDatabase::DBConn::DBConn(UInt32 iType)
	{
		Reset();

		m_iType = iType;
	}

	//析构
	HawkDatabase::DBConn::~DBConn()
	{
		Reset();
	}

	//赋值操作符
	HawkDatabase::DBConn& HawkDatabase::DBConn::operator = (const HawkDatabase::DBConn& sConn)
	{
		if (this != &sConn)
		{
			m_iType   = sConn.m_iType;
			m_iPort   = sConn.m_iPort;
			m_sHost   = sConn.m_sHost;				
			m_sUser   = sConn.m_sUser;
			m_sPwd	  = sConn.m_sPwd;
			m_sDBName = sConn.m_sDBName;
		}
		return *this;
	}

	Bool HawkDatabase::DBConn::IsValid() const
	{
		if ( (m_iType == HDB_SQLITE && m_sDBName.size()) || 
			 (m_iType == HDB_MYSQL && m_sHost.size() && m_iPort > 0 && m_sUser.size()))
			return true;

		return false;
	}

	void HawkDatabase::DBConn::Reset()
	{
		m_iType = 0;
		m_iPort = 0;
		m_sHost.clear();
		m_sUser.clear();
		m_sPwd.clear();
		m_sDBName.clear();
	}

	//////////////////////////////////////////////////////////////////////////

	HawkDatabase::HawkDatabase()
	{
		m_iDBType   = 0;
		m_pDBHandle = 0;		
		m_iErrCode  = 0;		
		m_bTrans    = false;
		m_sDBName.clear();
		m_sErrMsg.clear();
	}

	HawkDatabase::~HawkDatabase()
	{
		Close();
	}

	Bool HawkDatabase::Open(const DBConn& sConn)
	{
		m_sDBConn = sConn;
		return true;
	}

	Bool HawkDatabase::Close()
	{
		if (m_pDBHandle)
		{
			m_pDBHandle = 0;
			return true;
		}

		return false;
	}	

	Bool HawkDatabase::Execute(const UString& sSql)
	{
		if (m_pDBHandle && sSql.size())
			return true;
		
		return false;
	}

	void HawkDatabase::GenError()
	{
	}

	void HawkDatabase::ClearError()
	{
		m_sErrMsg.clear();
		m_iErrCode = 0;
	}	

	Bool HawkDatabase::Select(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			sResult.m_pDBHdl = this;
			return true;
		}

		return false;
	}

	Bool HawkDatabase::Insert(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			sResult.m_pDBHdl = this;
			return true;
		}

		return false;
	}

	Bool HawkDatabase::Update(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			sResult.m_pDBHdl = this;
			return true;
		}

		return false;
	}

	Bool HawkDatabase::Delete(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			sResult.m_pDBHdl = this;
			return true;
		}

		return false;
	}	

	Bool HawkDatabase::BeginTrans()
	{
		if (m_pDBHandle && !m_bTrans)
		{
			m_bTrans = true;
			return true;
		}

		return false;		
	}

	Bool HawkDatabase::RollbackTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			m_bTrans = false;
			return true;
		}

		return false;
	}

	Bool HawkDatabase::CommitTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			m_bTrans = false;
			return true;
		}
		
		return false;
	}

	UString HawkDatabase::AmendString(const UString& sValue) const
	{
		T_Exception("HawkDatabase::AmendString Must Implement.");
		return sValue;
	}

	AString HawkDatabase::AmendString(const AString& sValue) const
	{
		T_Exception("HawkDatabase::AmendString Must Implement.");
		return sValue;
	}

	Utf8* HawkDatabase::GetResult(const QueryResult* pResult,UInt64 iRow,UInt64 iCol,UInt32* pResLen)
	{
		return 0;
	}

	Bool HawkDatabase::FreeResult(QueryResult* pResult)
	{
		return true;
	}
}
