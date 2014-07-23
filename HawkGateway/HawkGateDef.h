#ifndef HAWK_GATEDEF_H
#define HAWK_GATEDEF_H

#include "HawkUtil.h"

namespace Hawk
{
#pragma pack(push, 1)
	//�����̷߳��͵�����ͷ(Args��Ҫ�������ش����������)
	struct ThreadMsgHeader 
	{
		enum
		{
			MSG_NORMAL = 0,
		};

		//��Ӧ������ID
		UInt32	SvrId;
		//�ỰID
		SID		Sid;
		//��������(0 ��ʾ��ͨ��Ϣ, ����������)
		Int8    Type;
		//���ݲ���(>0 ��ʾЭ��ID, <=0 ��ʾ��Ϣ)
		Int32	Args;

		ThreadMsgHeader(UInt32 iSvrId = 0, SID iSid = 0, Int8 iType = 0, Int32 iArgs = 0) : SvrId(iSvrId), Sid(iSid), Type(iType), Args(iArgs) {};

		ThreadMsgHeader& operator = (const ThreadMsgHeader& sHeader)
		{
			if (this != &sHeader)
			{
				SvrId = sHeader.SvrId;
				Sid   = sHeader.Sid;
				Type  = sHeader.Type;
				Args  = sHeader.Args;
			}
			return *this;
		}
	};

	//���ط��͵�����ͷ
	struct GateMsgHeader
	{
		//�ỰID
		SID	 Sid;

		GateMsgHeader(SID iSid = 0) : Sid(iSid) {};

		GateMsgHeader& operator = (const GateMsgHeader& sHeader)
		{
			if (this != &sHeader)
			{
				Sid   = sHeader.Sid;
			}
			return *this;
		}
	};

	//���������͵�����ͷ
	struct SvrMsgHeader
	{
		//�ỰID
		SID	 Sid;

		SvrMsgHeader(SID iSid = 0) : Sid(iSid) {};

		SvrMsgHeader& operator = (const SvrMsgHeader& sHeader)
		{
			if (this != &sHeader)
			{
				Sid   = sHeader.Sid;
			}
			return *this;
		}
	};

	//�ڲ�֪ͨ����
	struct GateNotify
	{
		//֪ͨ���ͱ��
		enum
		{
			NOTIFY_UNKNOWN = 0,
			NOTIFY_SESSION_CONNECT,		//�½��Ự����
			NOTIFY_SESSION_DISCONN,		//�Ự���ӶϿ�
			NOTIFY_SESSION_CLOSE,		//�����رջỰ
			NOTIFY_SERVICE_EXIT,		//�˳����ط���
			NOTIFY_SERVICE_ATTACH,		//�������
			NOTIFY_SERVICE_DETACH,		//����ж��
			NOTIFY_PROTO_REGISTER,		//Э��ע��
			NOTIFY_PROTO_BROADCAST,		//Э��㲥
		};

		//Ĭ�Ϲ���
		GateNotify(UInt8 iType = NOTIFY_UNKNOWN) : Type(iType) {};

		//֪ͨ����
		UInt8  Type;

		//֪ͨ����
		union
		{			
			//����������
			struct
			{
				//���Ӷ��׽���
				SOCKET	Handle;
				//�Ự��ַ
				Char	Address[IPV_LENGTH];
				//��ַ����
				UInt8	AddrLen;
				//�󶨷�����ID
				UInt32	SvrId;
			}eConnect;				

			//���ӻỰ�Ͽ�(����)
			struct
			{
				//�ỰID
				SID		Sid;
				//�Ự��ַ
				Char	Address[IPV_LENGTH];
				//��ַ����
				UInt8	AddrLen;
			}eDisConn;

			//�ر����ӻỰ(����)
			struct
			{
				//������ID
				UInt32  SvrId;
				//�ỰID
				SID		Sid;				
			}eClose;

			//�������
			struct 
			{
				//������ID
				UInt32  SvrId;
				//���������
				UInt32  ConnLimit;
			}eAttach;

			//����ж��
			struct 
			{
				//������ID
				UInt32  SvrId;
			}eDetach;
		};
	};

	//��������
	struct GateCommand
	{
		//֪ͨ���ͱ��
		enum
		{
			CMD_UNKNOWN = 0,			
			CMD_IPSECURITY,
			CMD_WHITELIST,
			CMD_BLACKLIST,
			CMD_PROF_REQ,
			CMD_PROF_INFO,
		};

		//Ĭ�Ϲ���
		GateCommand(UInt8 iType = CMD_UNKNOWN) : Type(iType) {};

		//��������
		UInt8  Type;

		union
		{
			//Э������ӿ���
			struct 
			{
				UInt32 ProtocolFreq;
				UInt32 IpPeerLimit;
			}eIpSecurity;

			//�������Ӽ�����
			struct 
			{
				Bool    AddIp;
				Char	IpAddr[IPV_LENGTH];
			}eWhiteList;

			//�������Ӽ�����
			struct 
			{
				Bool    AddIp;
				Char	IpAddr[IPV_LENGTH];
			}eBlackList;

			//��������
			struct  
			{
				UInt32  TimeStamp;
				UInt32  CpuCount;
				UInt64  TotalMem;
				UInt32  CpuUsage;
				UInt64  MemUsage;
				UInt32  Connect;
				UInt64  RecvProto;
				UInt64  RecvSize;
				UInt64  SendProto;
				UInt64  SendSize;
			}eProfInfo;
		};
	};
#pragma pack(pop)
}
#endif
