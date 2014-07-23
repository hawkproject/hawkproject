#include "HawkDBManager.h"
#include "HawkSqlite.h"
#include "HawkMysql.h"

namespace Hawk
{
	//���ݿ�������������
	HAKW_SINGLETON_IMPL(DBManager);	

	//////////////////////////////////////////////////////////////////////////
	//��ȫ���������ʵ��
	HawkDBManager::SafeDB::SafeDB(const AString& sDBName) : DBManager::SafeObj(P_DBManager->m_pDBMan,sDBName)
	{
	}
	
	HawkDBManager::SafeDB::~SafeDB()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	//���ݿ������
	HawkDBManager::HawkDBManager()
	{
		m_pDBMan = new DBManager;
	}

	HawkDBManager::~HawkDBManager()
	{		
		HAWK_RELEASE(m_pDBMan);
	}

	HawkDatabase*  HawkDBManager::OpenDatabase(const AString& sDBName, const HawkDatabase::DBConn& sDBConn)
	{
		if (!m_pDBMan->QueryObject(sDBName))
		{
			HawkDatabase* pDB = InnerCreate(sDBConn.GetType());
			if (pDB)
			{
				if(pDB->Open(sDBConn))
				{
					if (m_pDBMan->AllocObject(sDBName,pDB))
					{
						pDB->SetDBName(sDBName);
						return pDB;
					}	
				}
				HAWK_RELEASE(pDB);
			}
		}		
		return 0;
	}

	Bool HawkDBManager::CloseDatabase(const AString& sDBName)
	{
		if (m_pDBMan)
		{
			return m_pDBMan->FreeObject(sDBName);
		}
		return false;
	}

	HawkDatabase* HawkDBManager::InnerCreate(UInt32 iType)
	{
		HawkDatabase* pDB = 0;

		if (iType == HawkDatabase::HDB_SQLITE)
			pDB = new HawkSqlite();

#ifdef HAWK_MYSQL
		if (iType == HawkDatabase::HDB_MYSQL)
			pDB = new HawkMysql;
#endif

		return pDB;
	}
}
