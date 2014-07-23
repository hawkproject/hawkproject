#ifndef HAWK_DBMANAGER_H
#define HAWK_DBMANAGER_H

#include "HawkManagerBase.h"
#include "HawkObjManager.h"
#include "HawkDatabase.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ݿ������                                                         */
	/************************************************************************/
	class UTIL_API HawkDBManager : public HawkManagerBase
	{
	protected:
		//����
		HawkDBManager();

		//����
		virtual ~HawkDBManager();

		//����������
		HAKW_SINGLETON_DECL(DBManager);				

	public:
		//���ݿ������������
		typedef HawkObjManager<AString, HawkDatabase> DBManager;	

		//��ȫDB������װ
		class UTIL_API SafeDB : public DBManager::SafeObj
		{
		public:
			SafeDB(const AString& sDBName = "");

			virtual ~SafeDB();
		};

		//������Ա����
		friend class SafeDB;

	public:
		//�����ݿ�
		virtual HawkDatabase* OpenDatabase(const AString& sDBName,const HawkDatabase::DBConn& sDBConn);				

		//�ر����ݿ�
		virtual Bool		  CloseDatabase(const AString& sDBName);

	protected:
		//�������ݿ����
		virtual HawkDatabase* InnerCreate(UInt32 iType);

	protected:
		//���ݿ������
		DBManager*  m_pDBMan;
	};

	//���ݿ�ȫ�ֹ���������
	#define P_DBManager  HawkDBManager::GetInstance()
}
#endif
