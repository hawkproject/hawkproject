#ifndef HAWK_GATEPROXY_H
#define HAWK_GATEPROXY_H

#include "HawkGateDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ������ƥ������Ӷ˷�װ                                               */
	/************************************************************************/
	class GATE_API HawkGateProxy : public HawkRefCounter
	{
	public:
		//����
		HawkGateProxy();

		//����
		virtual ~HawkGateProxy();

		//�߳�˽�о��
		typedef map<UInt32, HawkZmq*>	ThreadProxyMap;

		//ע��Э������
		typedef map<ProtoType, UInt8>   ProtoTypeDriveMap;

	public:
		//��ʼ��
		virtual Bool  Init(const AString& sAddr, UInt32 iSvrId, UInt32 iConnLimit = 0);

		//������Ϣ
		virtual Bool  Update(Int32 iTimeout = 0);

	public:
		//ע��Э������
		virtual Bool  RegisterProtoType(const ProtoTypeDriveMap& mTypeDrive);

		//ע������߳�
		virtual Bool  RegisterThreads(const vector<UInt32>& vThreads);

		//����Э��
		virtual Bool  SendProtocol(SID iSid, HawkProtocol* pProto);

		//����Э��
		virtual Bool  SendProtocol(const SvrMsgHeader& sHeader, HawkProtocol* pProto);		

		//��������
		virtual Bool  SendRawData(SID iSid, const OctetsStream* pData);

		//��������
		virtual Bool  SendRawData(const SvrMsgHeader& sHeader, const OctetsStream* pData);	

		//�㲥Э��
		virtual Bool  BroadcastProto(const vector<SID>& vSid, HawkProtocol* pProto);

		//����Э��(ֻ��������Э��, һ�㲻����ʹ��)
		virtual Bool  RecvProtocol(GateMsgHeader& sHeader, HawkProtocol*& pProto, Int32 iTimeout = -1);

		//�رջỰ
		virtual Bool  CloseSession(SID iSid);

	protected:
		//���ӵ����ػص�
		virtual Bool  OnProxyConnected();

		//����Э��ص�(�߳�ģʽ��,�̵߳���), ����true����ʾЭ�鱻Ӧ�ý���, Ӧ�ø����ͷ�, ���������ͷ�
		virtual Bool  OnRecvProtocol(const GateMsgHeader& sHeader, HawkProtocol* pProto);

		//�����ضϿ����ӻص�
		virtual Bool  OnProxyDisconnect();

	protected:
		//�����������״̬(���߳�ģʽ�����ֶ�����)
		virtual Bool  CheckProxyState();

		//����֪ͨ(SIDΪĿ�����ID, Ϊ0����ʾ��ϵͳ��)
		virtual Bool  SendNotify(const GateNotify& sNotify, const OctetsStream* pExtData = 0);

		//�����ط���Ĭ��ע����Ϣ
		virtual Bool  SendRegsterMsg();

		//��ȡ�߳�˽��ͨ�Ŷ���
		HawkZmq*	  GetThreadProxy();		

	protected:
		//ͨ�û�����
		OctetsStream*		m_pOctets;
		//���ص�ַ
		AString				m_sGateAddr;
		//�����ʶID
		UInt32				m_iSvrId;
		//�������������
		UInt32				m_iConnLimit;
		//��Ϣͨ��
		HawkZmq*			m_pProxyZmq;
		//�Ƿ����������
		UInt32				m_iProxyState;
		//�߳����ݽ�����
		HawkZmq*			m_pCollectZmq;
		//�߳�˽��ͨ�Ŷ���
		ThreadProxyMap		m_mThreadProxy;
		//ע��Э���б�
		ProtoTypeDriveMap	m_mProtoTypeDrive;
	};
}
#endif
