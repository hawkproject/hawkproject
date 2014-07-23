#ifndef HAWK_ZMP_H
#define HAWK_ZMP_H

#include "HawkObjBase.h"
#include "HawkProtocolManager.h"

namespace Hawk
{
	/************************************************************************/
	/* 0MQ�ֲ�ʽ��Ϣ���в�����װ                                            */
	/************************************************************************/
	class UTIL_API HawkZmq : public HawkRefCounter
	{
	protected:
		//����
		HawkZmq();

		//����
		virtual ~HawkZmq();

		//��Աʲô����manager��Init����
		friend class HawkZmqManager;

	public:
		//ZMQͨѶ����
		enum
		{
			HZMQ_PAIR	= 0,
			HZMQ_PUB	= 1,
			HZMQ_SUB	= 2,
			HZMQ_REQ	= 3,
			HZMQ_REP	= 4,
			HZMQ_DEALER	= 5,
			HZMQ_ROUTER	= 6,
			HZMQ_PULL	= 7,
			HZMQ_PUSH	= 8,
			HZMQ_XPUB	= 9,
			HZMQ_XSUB	= 10,
			HZMQ_STREAM = 11,
		};

		//Send��Recv��Option����
		enum
		{
			HZMQ_DONTWAIT = 1,
			HZMQ_SNDMORE  = 2,
			HZMQ_RCVMORE  = 13,
		};

		//��ʱ��������
		enum
		{
			HZMQ_TIMEOUT = 100,
		};

		//ZMQ�������¼�
		enum
		{
			HZMQ_EVENT_CONNECTED		= 0x0001,
			HZMQ_EVENT_CONNECT_DELAYED	= 0x0002,
			HZMQ_EVENT_CONNECT_RETRIED	= 0x0004,

			HZMQ_EVENT_LISTENING		= 0x0008,
			HZMQ_EVENT_BIND_FAILED		= 0x0010,

			HZMQ_EVENT_ACCEPTED			= 0x0020,
			HZMQ_EVENT_ACCEPT_FAILED	= 0x0040,

			HZMQ_EVENT_CLOSED			= 0x0080,
			HZMQ_EVENT_CLOSE_FAILED		= 0x0100,
			HZMQ_EVENT_DISCONNECTED		= 0x0200,
			HZMQ_EVENT_MONITOR_STOPPED  = 0x0400,

			HZMQ_EVENT_ALL				= (HZMQ_EVENT_CONNECTED | HZMQ_EVENT_CONNECT_DELAYED |
										  HZMQ_EVENT_CONNECT_RETRIED | HZMQ_EVENT_LISTENING |
										  HZMQ_EVENT_BIND_FAILED | HZMQ_EVENT_ACCEPTED |
										  HZMQ_EVENT_ACCEPT_FAILED | HZMQ_EVENT_CLOSED |
										  HZMQ_EVENT_CLOSE_FAILED | HZMQ_EVENT_DISCONNECTED |
										  HZMQ_EVENT_MONITOR_STOPPED),
		};

	public:
		//Svrģʽ�󶨵�ַ
		virtual Bool   Bind(const AString& sAddr);

		//Cltģʽ����
		virtual Bool   Connect(const AString& sAddr);

		//��������(������Ϣ����)
		virtual Bool   Send(void* pBuf, Size_t iSize, Int32 iFlag = 0);

		//��������(������Ϣ����)
		virtual Bool   Recv(void* pBuf, Size_t& iSize, Int32 iFlag = 0);	

		//��������(����Э�鷢��)
		virtual Bool   SendProtocol(HawkProtocol* pProto, Int32 iFlag = 0);

		//��������(����Э�����)
		virtual Bool   RecvProtocol(HawkProtocol*& pProto, Int32 iFlag = 0);

		//�¼����
		virtual UInt32 PollEvent(UInt32 iEvents = HEVENT_READ, Int32 iTimeout = -1);	
		
		//�����¼�������
		virtual Bool   StartMonitor(UInt32 iEvents, const AString& sAddr = "");

		//���¼������¼�
		virtual UInt32 UpdateMonitor(OctetsStream* pData = 0);

		//�ر��¼�������
		virtual Bool   StopMonitor();

		//������Ϣ����
		virtual Bool   DiscardMsg();

	public:
		//��ȡ���
		virtual void*  GetHandle();

		//��ȡ����
		virtual Int32  GetType() const;

		//������
		virtual Int32  GetErrCode() const;

		//�и�������Ҫ��ȡ
		virtual Bool   IsWaitRecv() const;		

		//����IDENTITY����
		virtual Bool   SetIdentity(const void* pOptVal, Int32 iSize);	

		//����ZMQ����
		virtual Bool   SetOption(Int32 iOption, const void* pOptVal, Size_t iSize);

		//��ȡZMQ����
		virtual Bool   GetOption(Int32 iOption, void* pOptVal, Size_t& iSize);

	protected:
		//��ʼ��ZMQ���
		virtual Bool   Init(Int32 iType);	

		//��������Ϣ
		virtual Bool   FillErr();

		//�رն���
		virtual Bool   Close();

	public:
		//������Ч��Ψһ��ʶ(��32λΪ1, ��֤���ֽڲ�Ϊ0)
		static UInt64  GenValidZid(UInt32 iTid);

	protected:
		void*			m_pHandle;
		void*			m_pMonitor;
		Int32			m_iType;			
		Int32			m_iErrCode;
		AString			m_sErrMsg;
		Int32			m_iRecvMore;
		OctetsStream*	m_pRecvBuf;

	protected:
#ifdef _DEBUG
		Size_t			m_iLastSend;
		Size_t			m_iLastRecv;
#endif
	};
}
#endif
