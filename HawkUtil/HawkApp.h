#ifndef HAWK_APP_H
#define HAWK_APP_H

#include "HawkAppObj.h"
#include "HawkObjManager.h"
#include "HawkMsgManager.h"
#include "HawkThreadPool.h"

namespace Hawk
{
	/************************************************************************/
	/* Ӧ�ò��װ                                                           */
	/************************************************************************/
	class UTIL_API HawkApp : public HawkAppObj
	{
	public:
		//����
		HawkApp(const XID& sXid = XID());

		//����
		virtual ~HawkApp();

		//�ڲ��������
		friend  class  HawkApp_ProtoTask;

		friend  class  HawkApp_MsgTask;

		friend  class  HawkApp_TickTask;

		friend  class  HawkApp_ExceptionTask;

	public:
		//�������Լ��������������
		typedef HawkObjBase<XID, HawkAppObj>	 ObjBase;

		//���������Ͷ���
		typedef HawkObjManager<XID, HawkAppObj>  ObjMan;

		//������ӳ���
		typedef map<UInt32, ObjMan*>			 ObjManMap;

		//�ỰID�Ͷ���ID��ӳ���
		typedef map<SID, XID>					 SidXidMap;

		//��ȫ������
		class SafeObj : public ObjMan::SafeObj
		{
		public:
			//����
			SafeObj(XID sXid = XID());
			//����
			virtual ~SafeObj();
		};

	public:
		//��ʼ��Ӧ��(�����߳�����)
		virtual Bool		Init(Int32 iThread = 1);
		//��������
		virtual Bool		Run();
		//�رշ���
		virtual Bool		Stop();

	public:
		//�Ƿ�����״̬
		virtual Bool		IsRunning() const;

		//�̳߳��߳���Ŀ
		virtual Int32		GetThreadNum() const;	

		//��������(Ӧ�ò�ʵ��)
		virtual Bool		SendProtocol(SID iSid, Protocol* pProto);

	public:
		//ע�ᴴ�����������
		virtual ObjMan*		CreateObjMan(UInt32 iType);

		//��ȡ���������
		virtual ObjMan*		GetObjMan(UInt32 iType);

		//��������
		virtual HawkAppObj*	CreateObj(const XID& sXid, SID iSid = 0);

		//���ٶ���
		virtual Bool		DeleteObj(const XID& sXid);

	public:
		//���յ�Э���Ͷ�ݵ�Ӧ��, δ�󶨵ĻỰID�����Ҳ��������Ͷ��ʧ��
		virtual Bool		PostProtocol(SID iSid, Protocol* pProto);

		//���յ�Э���Ͷ�ݵ�Ӧ��, ����ID��Ҫ��Ч����Ͷ�ݳɹ�
		virtual Bool		PostProtocol(const XID& sXid, SID iSid, Protocol* pProto);

		//ֱ��Ͷ����Ϣ
		virtual Bool		PostMsg(HawkMsg* pMsg);

		//���ض�����Ͷ����Ϣ
		virtual Bool		PostMsg(const XID& sXid, HawkMsg* pMsg);	

		//�㲥��Ϣ
		virtual Bool		PostMsg(const XIDVector& vXID, HawkMsg* pMsg);

		//�ύ����
		virtual Bool		PostTick(const XIDVector& vXID);

		//�ύ�쳣
		virtual Bool		PostException(const XIDVector& vXID, Int32 iCode);

		//Ͷ��Ӧ������
		virtual Bool		PostAppTask(HawkTask* pTask, Int32 iThreadIdx = -1);
	
	public:
		//���ݻ�ȥ����ID
		virtual XID			GetXidBySid(SID iSid);

		//�󶨻ỰID�Ͷ���ID
		virtual void		BindSidXid(SID iSid, const XID& sXid);

		//��󶨻ỰID�Ͷ���ID
		virtual void		UnbindSidXid(SID iSid);

	protected:
		//�ַ�Э��
		virtual Bool		DispatchProto(const XID& sXid, SID iSid, Protocol* pProto);

		//�ַ���Ϣ
		virtual Bool		DispatchMsg(const XID& sXid, HawkMsg* pMsg);

		//�ַ�����
		virtual Bool		DispatchTick(const XID& sXid);

		//�ַ��쳣
		virtual Bool		DispatchException(const XID& sXid, Int32 iCode);

		//Ӧ�ò㴴������
		virtual HawkAppObj*	AppCreateObj(const XID& sXid);

	protected:
		//�Ƿ�����״̬
		volatile Bool	m_bRunning;
		//���߳�ID
		UInt32			m_iMainTid;
		//���������
		ObjManMap		m_mObjMan;
		//�Ựӳ���
		SidXidMap		m_mSidXid;
		//�Ự����
		HawkSpinLock*	m_pSidXidLock;
		//�̳߳�
		HawkThreadPool*	m_pThreadPool;
	};
}
#endif
