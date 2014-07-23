#ifndef HAWK_GATEDEF_H
#define HAWK_GATEDEF_H

#include "HawkUtil.h"

namespace Hawk
{
#pragma pack(push, 1)
	//网关线程发送的数据头(Args主要用来网关处理特殊参数)
	struct ThreadMsgHeader 
	{
		enum
		{
			MSG_NORMAL = 0,
		};

		//对应服务器ID
		UInt32	SvrId;
		//会话ID
		SID		Sid;
		//参数类型(0 表示普通消息, 其他见定义)
		Int8    Type;
		//数据参数(>0 表示协议ID, <=0 表示消息)
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

	//网关发送的数据头
	struct GateMsgHeader
	{
		//会话ID
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

	//服务器发送的数据头
	struct SvrMsgHeader
	{
		//会话ID
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

	//内部通知数据
	struct GateNotify
	{
		//通知类型标记
		enum
		{
			NOTIFY_UNKNOWN = 0,
			NOTIFY_SESSION_CONNECT,		//新建会话连接
			NOTIFY_SESSION_DISCONN,		//会话连接断开
			NOTIFY_SESSION_CLOSE,		//主动关闭会话
			NOTIFY_SERVICE_EXIT,		//退出网关服务
			NOTIFY_SERVICE_ATTACH,		//服务挂载
			NOTIFY_SERVICE_DETACH,		//服务卸载
			NOTIFY_PROTO_REGISTER,		//协议注册
			NOTIFY_PROTO_BROADCAST,		//协议广播
		};

		//默认构造
		GateNotify(UInt8 iType = NOTIFY_UNKNOWN) : Type(iType) {};

		//通知类型
		UInt8  Type;

		//通知参数
		union
		{			
			//接收新连接
			struct
			{
				//连接端套接字
				SOCKET	Handle;
				//会话地址
				Char	Address[IPV_LENGTH];
				//地址长度
				UInt8	AddrLen;
				//绑定服务器ID
				UInt32	SvrId;
			}eConnect;				

			//连接会话断开(被动)
			struct
			{
				//会话ID
				SID		Sid;
				//会话地址
				Char	Address[IPV_LENGTH];
				//地址长度
				UInt8	AddrLen;
			}eDisConn;

			//关闭连接会话(主动)
			struct
			{
				//服务器ID
				UInt32  SvrId;
				//会话ID
				SID		Sid;				
			}eClose;

			//服务挂载
			struct 
			{
				//服务器ID
				UInt32  SvrId;
				//最大连接数
				UInt32  ConnLimit;
			}eAttach;

			//服务卸载
			struct 
			{
				//服务器ID
				UInt32  SvrId;
			}eDetach;
		};
	};

	//网关命令
	struct GateCommand
	{
		//通知类型标记
		enum
		{
			CMD_UNKNOWN = 0,			
			CMD_IPSECURITY,
			CMD_WHITELIST,
			CMD_BLACKLIST,
			CMD_PROF_REQ,
			CMD_PROF_INFO,
		};

		//默认构造
		GateCommand(UInt8 iType = CMD_UNKNOWN) : Type(iType) {};

		//命令类型
		UInt8  Type;

		union
		{
			//协议和连接控制
			struct 
			{
				UInt32 ProtocolFreq;
				UInt32 IpPeerLimit;
			}eIpSecurity;

			//白名单加减控制
			struct 
			{
				Bool    AddIp;
				Char	IpAddr[IPV_LENGTH];
			}eWhiteList;

			//黑名单加减控制
			struct 
			{
				Bool    AddIp;
				Char	IpAddr[IPV_LENGTH];
			}eBlackList;

			//性能数据
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
