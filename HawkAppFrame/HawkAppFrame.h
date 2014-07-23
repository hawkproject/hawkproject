#ifndef HAWK_APPFRAME_H
#define HAWK_APPFRAME_H

#include "HawkUtil.h"
#include "HawkLogProxy.h"
#include "HawkAppGateway.h"
#include "HawkAppGateProxy.h"

namespace Hawk
{
	/************************************************************************/
	/* �������������									                    */
	/************************************************************************/
	class FRAME_API HawkAppFrame : public HawkApp
	{
	public:
		//����
		HawkAppFrame(const XID& sXid = XID());

		//����
		virtual ~HawkAppFrame();

		//�߳�ʹ�����ݿ����
		typedef map<UInt32, HawkMysql*>   DBHdlMap;
		
		//��ȡ�Ự�ĵ�ַ����
		typedef HawkLockMap<SID, AString> SidAddrMap;

		//��Ա����
		friend class HawkAppGateway;
		friend class HawkAppGateProxy;

		//App��ʼ���ṹ��
		struct AppCfg
		{
			//������ID
			UInt32		SvrId;
			//�Ƿ�������̨��ӡ
			Bool		Console;
			//�߼��߳���, ����IO�߳���
			UInt32		Threads;			
			//��־��������ַ
			AString		LogAddr;
			//mysql���ݿ����ӵ�ַ
			HawkDBConn	DBConn;

			//���ش���������
			struct  
			{
				//���ش�������ַ, ����: tcp://
				AString	Address;
				//����������
				UInt32	ConnLimit;
			}Gateproxy;
			
			//��Ƕ����ģʽ��������
			struct 
			{
				//��Ƕ���ط�������������
				AString	GwCfg;
				//��Ƕ���ط�������ȫ����
				AString	GwSecurity;
			}Gateway;

			AppCfg()
			{
				SvrId				= 0;
				Console				= false;
				Threads				= 0;				
				LogAddr				= "";

				Gateproxy.Address	= "";
				Gateproxy.ConnLimit = 0;

				Gateway.GwCfg	    = "";
				Gateway.GwSecurity  = "";
			}
		};

	public:
		//��ʼ��
		virtual Bool		Init(const AppCfg& sCfg);

		//����
		virtual Bool		Run();

		//ֹͣ
		virtual Bool		Stop();

	public:
		//��ȡӦ��ID
		virtual UInt32		GetSvrId() const;

		//��ȡ��ǰ����
		virtual UInt32		GetSysTime() const;
		
		//��ȡ����ʱ��
		virtual UInt32		GetTickTime() const;

		//��ȡ֡����
		virtual UInt32      GetElapseTime() const;

		//��ȡ�Ự��IP��ַ
		virtual AString		GetSidAddr(SID iSid);

		//�˳�ѭ��
		virtual Bool		BreakLoop();

		//��ȡ��ǰ�̰߳�ȫ�����ݿ�����
		HawkMysql*			GetSafeDbHdl();

		//��ȡӦ������
		const AppCfg*		GetAppCfg() const;

	public:
		//����Э��
		virtual Bool		SendProtocol(SID iSid, Protocol* pProto);
		
		//�رջỰ
		virtual Bool		CloseSession(SID iSid);
		
		//�㲥��Ϣ
		virtual Bool		BroadcastMsg(ObjMan* pObjMan, HawkMsg* pMsg);
		
		//�㲥Э��
		virtual Bool		BroadcastProto(Protocol* pProto, const vector<SID>& vSid = vector<SID>());	

	public:
		//��־ģ��ӿ�(��Ϣ)
		virtual Bool		FormatMsg(const Char* pKey, const Char* pFmt, ...);

		//��־ģ��ӿ�(����)
		virtual Bool		FormatWarn(const Char* pKey, const Char* pFmt, ...);

		//��־ģ��ӿ�(����)
		virtual Bool		FormatError(const Char* pKey, const Char* pFmt, ...);

		//��־ģ��ӿ�(��Ϣ)
		virtual Bool		FormatMsg(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

		//��־ģ��ӿ�(����)
		virtual Bool		FormatWarn(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

		//��־ģ��ӿ�(����)
		virtual Bool		FormatError(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

	protected:
		//ÿ֡����
		virtual Bool		OnTick(UInt32 iPeriod);

		//�������ӳɹ�(����ģʽ�ص�)
		virtual Bool		OnGateProxyConnected();

		//�Ự����(����ģʽ�ص�)
		virtual Bool		OnSessionStart(SID iSid, const AString& sAddr);

		//�ỰЭ��֪ͨ(����ģʽ�ص�)
		virtual Bool		OnSessionProtocol(SID iSid, Protocol* pProto);

		//�Ự����֪ͨ(����ģʽ�ص�)
		virtual Bool		OnSessionClose(SID iSid, const AString& sAddr);
		
		//�������ӹر�(����ģʽ�ص�)
		virtual Bool		OnGateProxyDisconnect();

	protected:
		//��������(������ģʽ, ����True���������ӣ�����ֱ�ӹر�)
		virtual Bool		OnAcceptConnect(const HawkSocket& sSocket, const SocketAddr& sAddr);

	protected:
		//Ӧ�ò㴴������(Ӧ�ò����ʵ��)
		virtual HawkAppObj*	AppCreateObj(const XID& sXid);

	protected:
		//Ӧ������
		AppCfg				m_sAppCfg;
		//��ǰϵͳʱ����
		UInt32				m_iSysTime;
		//��ǰϵͳʱ�����
		UInt32				m_iTickTime;
		//�߼�֡ʱ��
		UInt32				m_iElapseTime;
		//�Ự��ַ��
		SidAddrMap			m_mSidAddr;
		//���ݿ�������Ϣ
		HawkDBConn			m_sDbConn;
		//���ݿ���ӳ��
		DBHdlMap			m_mDbHdl;
		//֪ͨ�˳����
		volatile Bool		m_bBreakLoop;

	protected:
		//��־����
		HawkLogProxy*		m_pLogProxy;
		//��Ƕ���ط�����
		HawkAppGateway*		m_pGateway;
		//���ض���
		HawkAppGateProxy*	m_pGateProxy;
	};
}
#endif
