#ifndef HAWK_SYSPROTOCOL_H
#define HAWK_SYSPROTOCOL_H

#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* 引擎内部协议定义                                                      */
	/************************************************************************/
	namespace SysProtocol
	{
		//协议ID定义
		enum
		{
			//系统协议开始
			SYS_PROTOCOL_START = 0,

			//消息通知
			SYS_MSG_NOTIFY,
			//错误通知
			SYS_ERR_NOTIFY,
			//心跳通知
			SYS_HEART_BEAT,
			//心跳断开
			SYS_HEART_BREAK,
			//开启新会话
			SYS_SESSION_START,
			//客户端会话断开
			SYS_SESSION_BREAK,
			//服务器请求关闭会话
			SYS_CLOSE_SESSION,
			//记录日志
			SYS_LOG_MSG,
			//系统性能查询
			SYS_PROF_REQ,
			//系统性能信息
			SYS_PROF_INFO,
			//拒绝连接
			SYS_REFUSE_CONN,
			//Ping服务器
			SYS_CLT_PING,
			//服务器响应Ping
			SYS_SVR_PONG,
			//格式化数据
			SYS_OCTETS,

			//系统协议结束
			SYS_PROTOCOL_END   = 100,
		};
		
		//错误ID定义
		enum
		{
			//拒绝连接
			ERR_REFUSE_CONN = 1,
			//无可用服务
			ERR_SERVICE_INVALID,
		};

		//////////////////////////////////////////////////////////////////////////
		//系统内部使用协议定义
		//////////////////////////////////////////////////////////////////////////

		class Sys_MsgNotify : public HawkProtocol
		{
		public:
			Sys_MsgNotify(UInt32 iMsgId = 0, UInt32 iMode = 0) : HawkProtocol(SYS_MSG_NOTIFY)
			{
				m_iMsgId = iMsgId;
				m_iMode  = iMode;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_MsgNotify;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iMsgId << m_iMode;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iMsgId >> m_iMode;
				return rhsOS;
			};

		public:
			UInt32 m_iMsgId;
			UInt32 m_iMode;
		};

		class Sys_ErrNotify : public HawkProtocol
		{
		public:
			Sys_ErrNotify(UInt32 iErrId = 0, UInt32 iMode = 0) : HawkProtocol(SYS_ERR_NOTIFY)
			{
				m_iErrId = iErrId;
				m_iMode  = iMode;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_ErrNotify;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iErrId << m_iMode;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iErrId >> m_iMode;
				return rhsOS;
			};

		public:
			UInt32 m_iErrId;
			UInt32 m_iMode;
		};

		class Sys_HeartBeat : public HawkProtocol
		{
		public:
			Sys_HeartBeat(UInt32 iTime = 0) : HawkProtocol(SYS_HEART_BEAT)
			{
				m_iTime = iTime;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_HeartBeat;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iTime;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iTime;
				return rhsOS;
			};

		public:
			UInt32 m_iTime;
		};
		
		class Sys_HeartBreak : public HawkProtocol
		{
		public:
			Sys_HeartBreak(UInt32 iTime = 0) : HawkProtocol(SYS_HEART_BREAK)
			{
				m_iTime = iTime;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_HeartBreak;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iTime;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iTime;
				return rhsOS;
			};

		public:
			UInt32 m_iTime;
		};
		
		class Sys_SessionStart : public HawkProtocol
		{
		public:
			Sys_SessionStart(UInt32 iSid = 0, const AString& sAddr = "") : HawkProtocol(SYS_SESSION_START)
			{
				m_iSid  = iSid;
				m_sAddr = sAddr;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_SessionStart;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iSid << m_sAddr;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iSid >> m_sAddr;
				return rhsOS;
			};

		public:
			UInt32  m_iSid;
			AString m_sAddr;
		};

		class Sys_SessionBreak : public HawkProtocol
		{
		public:
			Sys_SessionBreak(UInt32 iSid = 0, const AString& sAddr = "") : HawkProtocol(SYS_SESSION_BREAK)
			{
				m_iSid  = iSid;
				m_sAddr = sAddr;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_SessionBreak;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iSid << m_sAddr;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iSid >> m_sAddr;
				return rhsOS;
			};

		public:
			UInt32  m_iSid;
			AString m_sAddr;
		};
		
		class Sys_CloseSession : public HawkProtocol
		{
		public:
			Sys_CloseSession(UInt32 iSid = 0) : HawkProtocol(SYS_CLOSE_SESSION)
			{
				m_iSid = iSid;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_CloseSession;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iSid;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iSid;
				return rhsOS;
			};

		public:
			UInt32 m_iSid;
		};
		
		class Sys_LogMsg : public HawkProtocol
		{
		public:
			Sys_LogMsg(Int32 iSvrId = 0, UInt32 iUsrId = 0, const UString& sUsrName = UString(), Int32 iType = 0, const UString& sKey = UString(), const UString& sMsg = UString()) : HawkProtocol(SYS_LOG_MSG)
			{
				m_iSvrId   = iSvrId;
				m_iUsrId   = iUsrId;
				m_sUsrName = sUsrName;
				m_iType	   = iType;
				m_sKey     = sKey;
				m_sMsg	   = sMsg;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_LogMsg;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iSvrId << m_iUsrId << m_sUsrName << m_iType << m_sKey << m_sMsg;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iSvrId >> m_iUsrId >> m_sUsrName >> m_iType >> m_sKey >> m_sMsg;
				return rhsOS;
			};

		public:
			Int32	 m_iSvrId;
			UInt32   m_iUsrId;
			UString	 m_sUsrName;
			Int32	 m_iType;
			UString  m_sKey;
			UString  m_sMsg;
		};

		class Sys_ProfReq : public HawkProtocol
		{
		public:
			Sys_ProfReq() : HawkProtocol(SYS_PROF_REQ)
			{
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_ProfReq;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS;
			};
		};

		class Sys_ProfInfo : public HawkProtocol
		{
		public:
			Sys_ProfInfo(UInt32 iTimeStamp = 0, UInt32 iCpuCount = 0, UInt64 iTotalMem = 0, UInt32 iCpuUsage = 0, UInt32 iMemUsage = 0, UInt32 iConnect = 0, UInt64 iRecvProto = 0, UInt64 iRecvSize = 0, UInt64 iSendProto = 0, UInt64 iSendSize = 0) : HawkProtocol(SYS_PROF_INFO)
			{
				m_iTimeStamp = iTimeStamp;
				m_iCpuCount  = iCpuCount;
				m_iTotalMem	 = iTotalMem;
				m_iCpuUsage	 = iCpuUsage;
				m_iMemUsage	 = iMemUsage;
				m_iConnect	 = iConnect;
				m_iRecvProto = iRecvProto;
				m_iRecvSize	 = iRecvSize;
				m_iSendProto = iSendProto;
				m_iSendSize	 = iSendSize;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_ProfInfo;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				rhsOS << m_iTimeStamp << m_iCpuCount << m_iTotalMem << m_iCpuUsage << m_iMemUsage << m_iConnect << m_iRecvProto << m_iRecvSize << m_iSendProto << m_iSendSize;
				return rhsOS;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				rhsOS >> m_iTimeStamp >> m_iCpuCount >> m_iTotalMem >> m_iCpuUsage >> m_iMemUsage >> m_iConnect >> m_iRecvProto >> m_iRecvSize >> m_iSendProto >> m_iSendSize;
				return rhsOS;
			};

		public:
			UInt32  m_iTimeStamp;
			UInt32	m_iCpuCount;
			UInt64  m_iTotalMem;
			UInt32	m_iCpuUsage;
			UInt64	m_iMemUsage;
			UInt32	m_iConnect;
			UInt64	m_iRecvProto;
			UInt64	m_iRecvSize;
			UInt64	m_iSendProto;
			UInt64	m_iSendSize;
		};

		class Sys_RefuseConn : public HawkProtocol
		{
		public:
			Sys_RefuseConn(Int32 iErrCode = 0) : HawkProtocol(SYS_REFUSE_CONN)
			{
				m_iErrCode = iErrCode;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_RefuseConn;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS << m_iErrCode;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS >> m_iErrCode;
			};

		public:
			Int32  m_iErrCode;
		};

		class Sys_CltPing : public HawkProtocol
		{
		public:
			Sys_CltPing(UInt32 iCltData = 0) : HawkProtocol(SYS_CLT_PING)
			{
				m_iCltData = iCltData;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_CltPing;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS << m_iCltData;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS >> m_iCltData;
			};

		public:
			UInt32  m_iCltData;
		};

		class Sys_SvrPong : public HawkProtocol
		{
		public:
			Sys_SvrPong(UInt32 iTime = 0) : HawkProtocol(SYS_SVR_PONG)
			{
				m_iTime = iTime;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_SvrPong;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS << m_iTime;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS >> m_iTime;
			};

		public:
			UInt32  m_iTime;
		};

		class Sys_Octets : public HawkProtocol
		{
		public:
			Sys_Octets(const HawkOctets& sOctets = HawkOctets()) : HawkProtocol(SYS_OCTETS)
			{
				m_sOctets = sOctets;
			};

			virtual HawkProtocol* Clone() const
			{
				return new Sys_Octets;
			};

			virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS << m_sOctets;
			};

			virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
			{
				return rhsOS >> m_sOctets;
			};

		public:
			HawkOctets  m_sOctets;
		};
	};	
}
#endif
