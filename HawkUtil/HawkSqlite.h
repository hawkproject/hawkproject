#ifndef HAWK_SQLITE_H
#define HAWK_SQLITE_H

#include "HawkDatabase.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//Sqlite3���ݿ������װ,Ĭ�ϲ���Utf8����								//
	//////////////////////////////////////////////////////////////////////////
	class UTIL_API HawkSqlite : public HawkDatabase
	{
	public:
		//DB������
		HawkSqlite();

		//DB��������
		virtual ~HawkSqlite();

	public:
		//�����ݿ�
		virtual Bool	Open(const DBConn& sConn);

		//�ر����ݿ�
		virtual Bool	Close();		

	public:		
		//���ݿ���ɾ�Ĳ����
		virtual Bool	Execute(const UString& sSql);

		//��ѯ����
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//�������
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);
		
		//���²���
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);
		
		//ɾ������
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//��ʼ���ﴦ��
		virtual Bool	BeginTrans();

		//�ع�����
		virtual Bool	RollbackTrans();

		//�ύ����
		virtual Bool	CommitTrans();

	public:				
		//��ȫת��sqlite3�ַ���
		virtual UString AmendString(const UString& sValue);

		//��ȫת��sqlite3�ַ���
		virtual AString AmendString(const AString& sValue);
  
	protected:
		//Mysql��ѯ
		virtual Int32	SqliteExec(const UString& sSql, Char** ppErrMsg);

		//�ͷŲ�ѯ���
		virtual Bool	FreeResult(QueryResult* pResult);

		//��ȡ��ѯ�����
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);

		//���ɴ�����Ϣ
		virtual  void	GenError();
	};
}
#endif
