#ifndef HAWK_MYSQL_H
#define HAWK_MYSQL_H

#include "HawkDatabase.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//Mysql����Ĭ�ϱ���ΪUtf8(����my.ini�����޸�)                           //
	//////////////////////////////////////////////////////////////////////////
	class UTIL_API HawkMysql : public HawkDatabase
	{
	public:
		//Mysql���ݿ������
		HawkMysql();
		
		//Mysql���ݿ��������
		virtual ~HawkMysql();

	public:
		//�����ݿ�
		virtual Bool	Open(const DBConn& sConn);

		//�ر����ݿ�
		virtual Bool	Close();		

	public:		
		//���ݿ���ɾ�Ĳ����
		virtual Bool	Execute(const UString& sSql);

	public:
		//��ѯ����
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//�������
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);

		//���²���
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);

		//ɾ������
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//�ı����ݿ�ʹ���û�
		virtual Bool	ChangeUser(const AString& sUser, const AString& sPwd, const AString& sDBName);	

		//�������ݿ�
		virtual Bool	IsExistDB(const AString& sDBName);

		//�л����ݿ�
		virtual Bool	SelectDB(const AString& sDBName);

		//�������ݿ�
		virtual Bool	CreateDB(const AString& sDBName);

		//�������ݱ�
		virtual Bool	IsExistTable(const AString& sDBName, const AString& sTblName);

		//��ȡAUTO_INCREMENT�����е�����
		virtual UInt64	GetInsertId();

	public:		
		//�Ƿ��ַ�������: 
		//:		\x00(NULL)
		//:		\n(����)
		//:		\r(�س�)
		//:		\t(�Ʊ�)
		//:		\b(�˸�)
		//:		\%(ͨ��)
		//:		\(��б��)
		//:		'(����)
		//:		"(˫��)
		//:		\x1a(CTRL+Z)
		//��ȫת��mysql�ַ���
		virtual UString AmendString(const UString& sValue);

		//��ȫת��mysql�ַ���
		virtual AString AmendString(const AString& sValue);

	protected:
		//Mysql��ѯ
		virtual Int32	MysqlQuery(const UString& sSql);

		//��ȡ��ѯ�����
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);
		
		//�ͷŲ�ѯ���
		virtual Bool	FreeResult(QueryResult* pResult);

		//�������в�ѯ�����
		virtual void	ClearResult();
		
		//���ɴ�����Ϣ
		virtual void    GenError();
	};
}
#endif
