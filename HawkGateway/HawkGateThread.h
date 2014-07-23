#ifndef HAWK_GATETHREAD_H
#define HAWK_GATETHREAD_H

#include "HawkGateDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ط����߳�(����libevent�ı�Ե�¼�����ģ��)                         */
	/************************************************************************/
	class HawkGateway;
	class GATE_API HawkGateThread : public HawkRefCounter
	{
	public:
		//����
		HawkGateThread(HawkGateway* pGateway = 0);

		//����
		virtual ~HawkGateThread();

		friend class HawkGateway;

	public:
		//�Ự״̬
		enum
		{
			SESSION_STATE_CLOSED = 0,
			SESSION_STATE_ACTIVE,
			SESSION_STATE_CLOSING,
		};

		//�Զ��������
		enum
		{
			SE_NONE = 100,
			SE_BEV_WRITE,
			SE_BEV_LENGTH,
			SE_BEV_REMOVE,			
			SE_BUF_FULL,
			SE_DECIPHER,
			SE_DECODE,
			SE_SHUTDOWN,
			SE_SEL_SERVER,
		};

		//Э��CRC���
		typedef map<ProtoType, UInt32> ProtoCrcMap;

		//�Ự��Ϣ����
		struct Session
		{
			//�ỰID
			SID				Sid;
			//�׽���
			SOCKET			Socket;
			//�󶨷�����ID
			UInt32			SvrId;
			//״̬
			UInt32			State;
			//Э��Ƶ��
			UInt32			ProtoFreq;
			//Ƶ��ͳ�ƿ�ʼʱ��
			UInt32			FreqTime;
			//��ַ��Ϣ
			SocketAddr*		Addr;
			//�Ự���¼�
			void*			Event;
			//�������α��
			Bool			Trust;
			//�����̶߳���
			HawkGateThread* GThread;
			//���뻺����
			OctetsStream*	IBuffer;
			//���ܶ���
			HawkSecurity*	ISecurity;
			//���ܶ���
			HawkSecurity*	OSecurity;
			//״̬Э����
			ProtoCrcMap*	ProtoTypeCrc;
		};		

		//�Ự�б���
		typedef map<SID, Session*>		  SessionMap;
		//�Ϸ�Э���
		typedef map<ProtoType, UInt8>	  ProtoTypeDriveMap;

	public:
		//��ʼ��
		virtual Bool  Init(UInt32 iIdentify, UInt32 iBaseSid, UInt32 iSidStep);

		//����
		virtual Bool  Start();			

		//�ر�
		virtual Bool  Close();

		//��ȡ�߳�ID
		virtual Int32 GetThreadId() const;

	protected:
		//����Ự
		Session*	  AllocSession(SOCKET hSocket, const SocketAddr& sAddr);

		//����Ự
		virtual Bool  FreeSession(Session* pSession);		

		//�ͷŻỰ�б�
		virtual Bool  FreeSessionMap();

		//���ɻỰID
		virtual SID   GenSessionId();

		//�������ӻỰ
		virtual Bool  StartSession(SOCKET hSocket, const SocketAddr& sAddr, Session** ppSession = 0);

		//�رջỰ
		virtual Bool  CloseSession(SID iSid, Int32 iErrCode = 0);

		//��������������Ϣ
		virtual Bool  RecvGateMsg(GateMsgHeader& sHeader, OctetsStream* pOctets);

		//��������������Ϣ(Я������
		virtual Bool  RecvGateExtData(OctetsStream* pOctets);

		//�����ط���������Ϣ
		virtual Bool  SendGateMsg(const ThreadMsgHeader& sHeader, void* pData, Size_t iSize);		

		//�Ự����Э��
		virtual Bool  SendProtocol(Session* pSession, Protocol* pProto);

		//�������뻺����ΪЭ��
		virtual Bool  DecodeIBuffer(Session* pSession);

	public:
		//�߳�ִ�к���, �̺߳�������
		virtual Bool  OnThreadLoop();

		//�Ự�¼�, iErrCode �� HEVENT_EXCEPT �¼�����Ч
		virtual Bool  OnSessionEvent(UInt32 iEvent, Session* pSession, Int32 iErrCode = 0);

	protected:
		//��Ϣ�����¼�
		virtual Bool  OnGatewayEvent();

		//����״̬
		virtual Bool  OnThreadIdle();

		//�Ự���ݿɶ��ص�
		virtual Bool  OnSessionRead(Session* pSession);
		
		//�Ự���ݿ�д�ص�
		virtual Bool  OnSessionWrite(Session* pSession);

		//�Ự��������ص�
		virtual Bool  OnSessionError(Session* pSession, Int32 iErrCode);

	protected:
		//�Ự���뻺�����ɽ����ж�
		virtual Bool  OnSessionDecode(Session* pSession, OctetsStream* pBuffer);

		//�Ự����������ɼ����ж�
		virtual Bool  OnSessionEncode(Session* pSession, OctetsStream* pBuffer);

		//�ڲ�Э�鴦��
		virtual Bool  OnSysProtocol(Session* pSession, ProtoType iProtoType);

	protected:
		//�¼������߳�
		HawkThread*			m_pThread;
		//�¼���������
		void*				m_pBase;
		//��Ϣ����
		HawkZmq*			m_pZmq;
		//��Ϣ����Buffer
		OctetsStream*		m_pOctets;
		//�����ỰID
		UInt32				m_iBaseSid;
		//�ỰID�ݽ�����
		UInt32				m_iSidStep;
		//��ǰ�Ự����
		UInt32				m_iCurSid;
		//��ǰ�Ự�б�
		SessionMap			m_mSession;
		//��������Ӧ��
		HawkGateway*		m_pGateway;
		//����״̬
		Bool				m_bIdle;
		//������״̬
		volatile Bool		m_bRunning;
		//�Ϸ�Э��ID�б�
		ProtoTypeDriveMap	m_mProtoTypeDrive;
	};
}
#endif
