#ifndef HAWK_DATABASE_H
#define HAWK_DATABASE_H

#include "HawkRefCounter.h"

namespace Hawk
{		
	/************************************************************************/
	/* ���ݿ���������װ                                                   */
	/************************************************************************/
	class UTIL_API HawkDatabase : public HawkRefCounter
	{
	public:
		//DB��ѯ�����
		class UTIL_API QueryResult : public HawkRefCounter
		{
		public:
			//����
			QueryResult();

			//����
			virtual ~QueryResult();
			
			//��Ա������
			friend class HawkDatabase;

		public:
			//�������
			DEFINE_PROPERTY(UInt64, m_iRow,	   Row);
			//�������
			DEFINE_PROPERTY(UInt64, m_iCol,	   Col);
			//Ӱ������
			DEFINE_PROPERTY(UInt64, m_iAffect, AffectRows);
			//��������
			DEFINE_PROPERTY(PVoid,  m_pResHdl, ResHdl);
			//���ݿ���
			DEFINE_PROPERTY(PVoid,  m_pDBHdl,  DBHdl);

		public:
			//�������л�ȡ���ؽ��
			const Utf8* GetResult(UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0) const;			

		public:
			//����������ȡ���ؽ��
			const Utf8* operator[] (UInt64 iIdx) const;

			//�����������з���
			const Utf8* operator()(UInt64 iRow, UInt64 iCol) const;			
		};

		//DB���Ӷ���
		class UTIL_API DBConn : public HawkRefCounter
		{
		public:
			//Ĭ�Ϲ���
			DBConn(UInt32 iType = HDB_UNKNOWN);

			//����
			virtual ~DBConn();

		public:
			//������ַ
			DEFINE_PROPERTY(AString, m_sHost,	Host);
			//�����˿�
			DEFINE_PROPERTY(Int32,   m_iPort,	Port);
			//��½�û�
			DEFINE_PROPERTY(AString, m_sUser,	User);
			//��½����
			DEFINE_PROPERTY(AString, m_sPwd,	Pwd);
			//����DB
			DEFINE_PROPERTY(AString, m_sDBName,	DBName);
			//DB����
			DEFINE_PROPERTY(UInt32,  m_iType,	Type);

		public:
			//��ֵ������
			DBConn& operator = (const DBConn& sConn);

		public:
			//������Ч���ж�
			Bool  IsValid() const;

			//������������
			void  Reset();
		};

		//������Ա����
		friend class QueryResult;

	public:
		//����
		HawkDatabase();

		//����
		virtual ~HawkDatabase();

	public:
		//֧�ֵ����ݿ����Ͷ���
		enum
		{
			HDB_UNKNOWN,
			HDB_SQLITE,
			HDB_MYSQL,
		};

	public:
		//���ݿ�����
		DEFINE_PROPERTY(UInt32,  m_iDBType,	  DBType);
		//���ݿ�����
		DEFINE_PROPERTY(AString, m_sDBName,	  DBName);
		//���ݿ���
		DEFINE_PROPERTY(PVoid,   m_pDBHandle, DBHandle);
		//���ݿ�����
		DEFINE_PROPERTY(DBConn,  m_sDBConn,   DBConn);
		//������Ϣ
		DEFINE_PROPERTY(UString, m_sErrMsg,   ErrMsg);
		//������
		DEFINE_PROPERTY(Int32,   m_iErrCode,  ErrCode);
		//�Ƿ���������
		DEFINE_PROPERTY(Bool,    m_bTrans,    IsTrans);

	public:
		//�����ݿ�
		virtual Bool	Open(const DBConn& sConn);

		//�ر����ݿ�
		virtual Bool	Close();
		
	public:		
		//SQL���ִ��
		virtual Bool	Execute(const UString& sSql);		

		//���ݲ�ѯ
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//���ݲ���
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);

		//���ݸ���
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);

		//����ɾ��
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

	public:
		//��ʼ���ﴦ��
		virtual Bool	BeginTrans();

		//�ع�����
		virtual Bool	RollbackTrans();

		//�ύ����
		virtual Bool	CommitTrans();

	public:
		//���ݿ��ַ�������(�Ƿ��ַ�������)
		virtual UString AmendString(const UString& sValue) const;

		//���ݿ��ַ�������(�Ƿ��ַ�������)
		virtual AString AmendString(const AString& sValue) const;

	protected:
		//��ȡ��ѯ�����
		virtual Utf8*   GetResult(const QueryResult* pResult, UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0);

		//�ͷŲ�ѯ���
		virtual Bool	FreeResult(QueryResult* pResult);	

		//���ɴ�����Ϣ
		virtual void    GenError();

		//�����ǰ����
		virtual void	ClearError();
	};

	typedef HawkDatabase::DBConn HawkDBConn;
}
#endif
