#ifndef HAWK_DBMANAGER_H
#define HAWK_DBMANAGER_H

#include "HawkManagerBase.h"
#include "HawkObjManager.h"
#include "HawkDatabase.h"

namespace Hawk
{
	/************************************************************************/
	/* 数据库管理类                                                         */
	/************************************************************************/
	class UTIL_API HawkDBManager : public HawkManagerBase
	{
	protected:
		//构造
		HawkDBManager();

		//析构
		virtual ~HawkDBManager();

		//管理单例申明
		HAKW_SINGLETON_DECL(DBManager);				

	public:
		//数据库管理容器类型
		typedef HawkObjManager<AString, HawkDatabase> DBManager;	

		//安全DB操作封装
		class UTIL_API SafeDB : public DBManager::SafeObj
		{
		public:
			SafeDB(const AString& sDBName = "");

			virtual ~SafeDB();
		};

		//申明友员访问
		friend class SafeDB;

	public:
		//打开数据库
		virtual HawkDatabase* OpenDatabase(const AString& sDBName,const HawkDatabase::DBConn& sDBConn);				

		//关闭数据库
		virtual Bool		  CloseDatabase(const AString& sDBName);

	protected:
		//创建数据库对象
		virtual HawkDatabase* InnerCreate(UInt32 iType);

	protected:
		//数据库管理器
		DBManager*  m_pDBMan;
	};

	//数据库全局管理器对象
	#define P_DBManager  HawkDBManager::GetInstance()
}
#endif
