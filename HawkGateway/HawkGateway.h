#ifndef HAWK_GATEWAY_H
#define HAWK_GATEWAY_H

#include "HawkGateDef.h"
#include "HawkGateThread.h"
#include "HawkProfiler.h"
#include "HawkGateSecurity.h"
#include "HawkGwControler.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ط�����������װ(����libevent�ı�Ե�¼�����ģ��)                   */
	/************************************************************************/
	class GATE_API HawkGateway : public HawkRefCounter
	{
	public:
		//����
		HawkGateway();

		//����
		virtual ~HawkGateway();

		//�����б�
		struct Service 
		{
			UInt32  SvrId;
			SOCKET  SvrFd;
			UInt32  ConnCap;			
			UInt32  CurCap;

			Service(UInt32 iSvrId = 0, SOCKET iSvrFd = INVALID_SOCKET, UInt32 iConnCap = 0) : SvrId(iSvrId), SvrFd(iSvrFd), ConnCap(iConnCap), CurCap(0) {}
		};

		//�ỰID����Ϣ
		struct BindInfo
		{
			UInt32  SvrId;
			AString IpAddr;

			BindInfo(UInt32 iSvrId = 0, const AString& sIpAddr = "") : SvrId(iSvrId), IpAddr(sIpAddr) {}
		};

		//��ǰ���صķ������б�
		typedef vector<Service>		ServiceVec;
		//�ỰID�ͷ�����ID��ӳ��
		typedef map<SID, BindInfo>	SidBindMap;
		//ip����������
		typedef map<AString, Int32> IpPeerMap;

		//����ģʽ
		enum
		{
			GATEMODE_ECHO = 0,					//����ģʽ
			GATEMODE_SINGLE_SERVICE,			//����������ģʽ
			GATEMODE_MULTI_SERVICE_MANUAL ,		//��������ֶ�ѡ��ģʽ
			GATEMODE_MULTI_SERVICE_FULLLOAD,	//������������ģʽ
			GATEMODE_MULTI_SERVICE_BALANCE,		//����������ؾ���ģʽ			
		};

		//�ӽ�������
		enum
		{
			GATESECURITY_NONE = 0,
			GATESECURITY_RC4,		//RC4�ԳƼ���
			GATESECURITY_XOR,		//XOR������
			GATESECURITY_ZLIB,		//ZLIBѹ������
		};

	public:
		//��ʼ�����ط���
		virtual Bool    Init(const AString& sFrontend, const AString& sBackend, Int32 iThread = 4, Int32 iGateMode = GATEMODE_SINGLE_SERVICE);

		//��������
		virtual Bool    Run();

		//�رշ���(֪ͨ����,������)
		virtual Bool    Stop();		

	public:
		//���ļ���ʼ��
		virtual Bool    InitCfg(const AString& sCfg, const AString& sSecurity);

	public:
		//��ȡ�߳���
		virtual Int32   GetThreadNum() const;

		//��ȡ�߳�ID
		virtual Int32   GetThreadId(Int32 iIdx) const;

		//������ѭ����������
		virtual Bool    SetBlocking(Bool bBlocking = true);

		//���ûỰ�����С
		virtual void    SetBufSize(Int32 iBufSize);

		//��ȡ�Ự�����С
		virtual Int32   GetBufSize() const;	

		//������ȫ����ֱ�����е�����ģʽ
		virtual Bool    SetWaitIdle(Bool bWaitIdle);

		//�Ƿ�Ϊ��������ֱ���������ģʽ
		virtual Bool    IsWaitIdle() const;

		//�ж�����״̬
		virtual Bool    IsRunning() const;		

		//���ߺ󱣳�����
		virtual void    SetHoldConnection(Bool bHold);

		//�ж��Ƿ���߱�������
		virtual Bool    IsHoldConnection() const;

		//���ûỰ��ʱʱ��
		virtual void    SetSessionTimeout(Int32 iTimeout);

		//���ûỰЭ��Ƶ��
		virtual void    SetSessionProtoFreq(UInt32 iFreq);

		//��ȡ�ỰЭ��Ƶ��
		virtual UInt32  GetSessionProtoFreq() const;

		//����ip����������
		virtual void    SetIpPeerLimit(UInt32 iLimit);

		//��ȡip����������
		virtual UInt32  GetIpPeerLimit() const;

		//��ȡ�Ự��ʱʱ��
		virtual Int32   GetSessionTimeout() const;

		//��ȡ����ģʽ
		virtual Int32	GetGateMode() const;

		//����IO�������ӽ������
		virtual Bool    EnableGateSecurity(Bool bEnable, UInt32 iSecuType = 0, const AString& sSecuKey = "");

		//�������ܼ�����
		virtual Bool    TurnOnProfiler(const AString& sAddr);
	
		//�����������(flash 843�˿���֤)
		virtual Bool    TurnOnCrossDomain();	

		//��ʼ�����ؿ�����
		virtual Bool    InitControler(const AString& sAddr);

	public:
		//���������߳�
		virtual Bool    CreateGateThread(HawkGateThread*& pThread);

		//�����Ự�������
		virtual Bool    CreateISecurity(HawkSecurity*& pSecurity, UInt32 iSecurity = 0, HawkOctets* pParam = 0);

		//�����Ự�������
		virtual Bool    CreateOSecurity(HawkSecurity*& pSecurity, UInt32 iSecurity = 0, HawkOctets* pParam = 0);

		//��ȡ���ܼ�����
		HawkProfiler*   GetProfiler();

		//��ȡ�߳�ZMQ�ķ����ַ
		virtual AString GetThreadZmqAddr() const;

		//��ȡ��˷������ZMQ��ַ
		virtual AString GetServerZmqAddr() const;		

		//�Ự����У��(��GateThread���)
		virtual Bool    TrustSession(void* pSession, OctetsStream* pIBuffer);

		//�رջỰ
		virtual Bool    CloseSession(SID iSid, UInt32 iSvrId = 0);	

	protected:
		//���������̸߳�ʽ������
		virtual Bool    RecvThreadMsg(UInt32& iThread, ThreadMsgHeader& sHeader, OctetsStream* pOctets);		

		//�������̷߳���֪ͨ��Ϣ
		virtual Bool    SendThreadMsg(UInt32 iThread, const GateMsgHeader& sHeader, void* pData, Size_t iSize, const OctetsStream* pExtData = 0);

		//���պ�˷����ʽ������
		virtual Bool    RecvServerMsg(UInt32& iSvrId, SvrMsgHeader& sHeader, OctetsStream* pOctets);

		//���պ�˷����ʽ������(Я������)
		virtual Bool    RecvServerExtData(OctetsStream* pOctets);

		//���˷�����֪ͨ��Ϣ
		virtual Bool    SendServerMsg(UInt32 iSvrId, const GateMsgHeader& sHeader, void* pData, Size_t iSize);		

	protected:
		//����������
		virtual Bool    OnSessionAccept();

		//ThreadZmq�ɶ��¼�(�������������̵߳�����)
		virtual Bool    OnGateThreadEvent();

		//���տͻ���Э��ص�(��OnGateThreadEvent����, ����falseֱ�ӻ�CloseSession)
		virtual Bool    OnSessionProtocol(SID iSid, const OctetsStream* pOctets);

		//����Э������(����true��ʾ������, Э�鲻�����½���)
		virtual Bool    OnProtocolIntercept(SID iSid, const OctetsStream* pOctets);

		//ServerZmq�ɶ��¼�
		virtual Bool    OnGateServerEvent();

		//MonitorZmq�ɶ��¼�
		virtual Bool    OnGateMonitorEvent();

		//���ؿ���״̬����
		virtual Bool    OnGateIdleEvent();

		//�رշ���(�����ȴ�����)
		virtual Bool    OnGatewayClose();		

	protected:
		//��˷������ӵ�����
		virtual Bool    OnServerConnected(SOCKET hSocket);

		//��˷�������ضϿ�
		virtual Bool    OnServerDisConnect(SOCKET hSocket);

	protected:
		//ע��������, ���ذ󶨷�����ID
		virtual Bool    RegConnection(const SocketAddr& sAddr, UInt32& iSvrId, Int32& iErrCode);

		//���ؾܾ�����
		virtual Bool    OnGateRefuseConn(Int32 iReason, HawkSocket& sSocket, const SocketAddr* pAddr = 0);

		//�Ự���ص�������
		virtual Bool    OnSessionAttachServer(SID iSid, UInt32 iSvrId, const AString& sIpAddr);

		//�Ự�ӷ�����ж��
		virtual Bool    OnSessionDetachServer(SID iSid, UInt32 iSvrId);

	public:
		//��Ӱ�����
		virtual Bool    AddWhiteIp(const AString& sIp);

		//ɾ��������
		virtual Bool    DelWhiteIp(const AString& sIp);

		//��Ӻ�����
		virtual Bool    AddBlackIp(const AString& sIp);

		//ɾ��������
		virtual Bool    DelBlackIp(const AString& sIp);

	protected:
		//ip��ȫУ��
		virtual Bool    CheckIpSecurity(const AString& sIpPort, Int32& iErrCode);		

	protected:		
		//�¼��߳�
		HawkGateThread** m_ppThread;		
		//����ģʽ
		Int32			 m_iGateMode;
		//�ỰID���󶨵ķ���ID
		SidBindMap		 m_mSidBind;
		//�����߳���
		Int32			 m_iThread;
		//�����߳�ID
		UInt32			 m_iTurnIdx;
		//�����ỰID
		UInt32			 m_iBaseSid;
		//�ỰBuffer��С
		Int32			 m_iBufSize;
		//�Ự��ʱʱ��
		Int32			 m_iTimeout;
		//�ỰЭ��Ƶ��
		UInt32			 m_iProtoFreq;
		//ip����������
		UInt32			 m_iIpPeerLimit;
		//��ǰip������ӳ��
		IpPeerMap		 m_mIpPeer;
		//ip���Ӱ�����
		AStringMap		 m_mWhiteIp;
		//ip���Ӻ�����
		AStringMap		 m_mBlackIp;
		//��Ϣ����Buffer
		OctetsStream*	 m_pOctets;
		//�������׽���
		HawkSocket		 m_sSocket;
		//���صķ���ID
		ServiceVec		 m_vService;
		//�������߳�ͨ�ŵ�ZMQ
		HawkZmq*		 m_pThreadZmq;
		//�ͺ�˷���ͨ�ŵ�ZMQ
		HawkZmq*		 m_pServerZmq;
		//��˷���ZMQ������
		HawkZmq*		 m_pMonitorZmq;
		//���ܼ�����
		HawkProfiler*	 m_pProfiler;
		//�����ӽ���ģʽ
		Bool			 m_bSecurity;
		//Ĭ�ϼ���ģʽ
		UInt32			 m_iSecuType;
		//Ĭ�ϼ�����Կ
		AString			 m_sSecuKey;
		//ѭ������״̬
		Bool			 m_bIdle;
		//�ȴ�ֱ������(ƽ�ⵥ����������ʱ��)
		Bool			 m_bWaitIdle;
		//��������ѭ���Ƿ�����(Ĭ������)
		Bool			 m_bBlocking;
		//����״̬
		volatile Bool    m_bRunning;
		//������ж�غ󱣳�����
		Bool			 m_bHoldConn;
		//Gateway���������
		HawkGwControler* m_pControler;

	protected:
		//��������߳�
		HawkThread*		 m_pCrossDomain;
	};
}
#endif
