#ifndef __HAWKSYSPROTOCOL_H__
#define __HAWKSYSPROTOCOL_H__

#include "HawkSysProtocolId.h"

namespace Hawk
{
	HAWK_START_NAMESPACE(SysProtocol)

	//加密方式
	class Sys_Security : public HawkProtocol
	{
	public:
		Sys_Security(UInt32 iSecurity = 0, const HawkOctets& sKey = HawkOctets()) : HawkProtocol(SysProtocol::SYS_SECURITY), m_iSecurity(iSecurity), m_sKey(sKey)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_Security;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_iSecurity << m_sKey;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_iSecurity >> m_sKey;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_iSecurity = 0;
			m_sKey.Clear();
			return true;
		};

	public:
		UInt32 m_iSecurity;
		HawkOctets m_sKey;
	};

	//选服务器
	class Sys_SelServer : public HawkProtocol
	{
	public:
		Sys_SelServer(UInt32 iSvrId = 0) : HawkProtocol(SysProtocol::SYS_SEL_SERVER), m_iSvrId(iSvrId)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_SelServer;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_iSvrId;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_iSvrId;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_iSvrId = 0;
			return true;
		};

	public:
		UInt32 m_iSvrId;
	};

	//会话开始
	class Sys_SessionStart : public HawkProtocol
	{
	public:
		Sys_SessionStart(UInt32 iSid = 0, const AString& sAddr = AString()) : HawkProtocol(SysProtocol::SYS_SESSION_START), m_iSid(iSid), m_sAddr(sAddr)
		{
		}

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

		virtual Bool Clear()
		{
			m_iSid = 0;
			m_sAddr.clear();
			return true;
		};

	public:
		UInt32 m_iSid;
		AString m_sAddr;
	};

	//拒绝连接
	class Sys_RefuseConn : public HawkProtocol
	{
	public:
		Sys_RefuseConn(Int32 iErrCode = 0) : HawkProtocol(SysProtocol::SYS_REFUSE_CONN), m_iErrCode(iErrCode)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_RefuseConn;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_iErrCode;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_iErrCode;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_iErrCode = 0;
			return true;
		};

	public:
		Int32 m_iErrCode;
	};

	//会话主动断开
	class Sys_SessionBreak : public HawkProtocol
	{
	public:
		Sys_SessionBreak(UInt32 iSid = 0, const AString& sAddr = AString()) : HawkProtocol(SysProtocol::SYS_SESSION_BREAK), m_iSid(iSid), m_sAddr(sAddr)
		{
		}

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

		virtual Bool Clear()
		{
			m_iSid = 0;
			m_sAddr.clear();
			return true;
		};

	public:
		UInt32 m_iSid;
		AString m_sAddr;
	};

	//会话被动关闭
	class Sys_CloseSession : public HawkProtocol
	{
	public:
		Sys_CloseSession(UInt32 iSid = 0) : HawkProtocol(SysProtocol::SYS_CLOSE_SESSION), m_iSid(iSid)
		{
		}

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

		virtual Bool Clear()
		{
			m_iSid = 0;
			return true;
		};

	public:
		UInt32 m_iSid;
	};

	//服务器关闭
	class Sys_ServerShutdown : public HawkProtocol
	{
	public:
		Sys_ServerShutdown(UInt32 iSvrId = 0, UInt32 iTime = 0) : HawkProtocol(SysProtocol::SYS_SERVER_SHUTDOWN), m_iSvrId(iSvrId), m_iTime(iTime)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_ServerShutdown;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_iSvrId << m_iTime;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_iSvrId >> m_iTime;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_iSvrId = 0;
			m_iTime = 0;
			return true;
		};

	public:
		UInt32 m_iSvrId;
		UInt32 m_iTime;
	};

	//Ping操作
	class Sys_CltPing : public HawkProtocol
	{
	public:
		Sys_CltPing(UInt32 iCltData = 0) : HawkProtocol(SysProtocol::SYS_CLT_PING), m_iCltData(iCltData)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_CltPing;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_iCltData;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_iCltData;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_iCltData = 0;
			return true;
		};

	public:
		UInt32 m_iCltData;
	};

	//Pong操作
	class Sys_SvrPong : public HawkProtocol
	{
	public:
		Sys_SvrPong(UInt32 iTime = 0) : HawkProtocol(SysProtocol::SYS_SVR_PONG), m_iTime(iTime)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_SvrPong;
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

		virtual Bool Clear()
		{
			m_iTime = 0;
			return true;
		};

	public:
		UInt32 m_iTime;
	};

	//字节数组
	class Sys_Octets : public HawkProtocol
	{
	public:
		Sys_Octets(const HawkOctets& sOctets = HawkOctets()) : HawkProtocol(SysProtocol::SYS_OCTETS), m_sOctets(sOctets)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_Octets;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_sOctets;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_sOctets;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_sOctets.Clear();
			return true;
		};

	public:
		HawkOctets m_sOctets;
	};

	//会话心跳
	class Sys_HeartBeat : public HawkProtocol
	{
	public:
		Sys_HeartBeat(UInt32 iTime = 0) : HawkProtocol(SysProtocol::SYS_HEART_BEAT), m_iTime(iTime)
		{
		}

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

		virtual Bool Clear()
		{
			m_iTime = 0;
			return true;
		};

	public:
		UInt32 m_iTime;
	};

	//心跳超时
	class Sys_HeartBreak : public HawkProtocol
	{
	public:
		Sys_HeartBreak(UInt32 iTime = 0) : HawkProtocol(SysProtocol::SYS_HEART_BREAK), m_iTime(iTime)
		{
		}

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

		virtual Bool Clear()
		{
			m_iTime = 0;
			return true;
		};

	public:
		UInt32 m_iTime;
	};

	//消息通知
	class Sys_MsgNotify : public HawkProtocol
	{
	public:
		Sys_MsgNotify(UInt32 iMsgId = 0, UInt32 iMode = 0) : HawkProtocol(SysProtocol::SYS_MSG_NOTIFY), m_iMsgId(iMsgId), m_iMode(iMode)
		{
		}

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

		virtual Bool Clear()
		{
			m_iMsgId = 0;
			m_iMode = 0;
			return true;
		};

	public:
		UInt32 m_iMsgId;
		UInt32 m_iMode;
	};

	//错误通知
	class Sys_ErrNotify : public HawkProtocol
	{
	public:
		Sys_ErrNotify(UInt32 iMsgId = 0, UInt32 iMode = 0) : HawkProtocol(SysProtocol::SYS_ERR_NOTIFY), m_iMsgId(iMsgId), m_iMode(iMode)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new Sys_ErrNotify;
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

		virtual Bool Clear()
		{
			m_iMsgId = 0;
			m_iMode = 0;
			return true;
		};

	public:
		UInt32 m_iMsgId;
		UInt32 m_iMode;
	};

	//日志消息
	class Sys_LogMsg : public HawkProtocol
	{
	public:
		Sys_LogMsg(Int32 iSvrId = 0, UInt32 iUsrId = 0, const UString& sUsrName = UString(), Int32 iType = 0, const UString& sKey = UString(), const UString& sMsg = UString()) : HawkProtocol(SysProtocol::SYS_LOG_MSG), m_iSvrId(iSvrId), m_iUsrId(iUsrId), m_sUsrName(sUsrName), m_iType(iType), m_sKey(sKey), m_sMsg(sMsg)
		{
		}

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

		virtual Bool Clear()
		{
			m_iSvrId = 0;
			m_iUsrId = 0;
			m_sUsrName.clear();
			m_iType = 0;
			m_sKey.clear();
			m_sMsg.clear();
			return true;
		};

	public:
		Int32 m_iSvrId;
		UInt32 m_iUsrId;
		UString m_sUsrName;
		Int32 m_iType;
		UString m_sKey;
		UString m_sMsg;
	};

	//性能请求
	class Sys_ProfReq : public HawkProtocol
	{
	public:
		Sys_ProfReq() : HawkProtocol(SysProtocol::SYS_PROF_REQ)
		{
		}

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

		virtual Bool Clear()
		{
			return true;
		};
	};

	//性能信息
	class Sys_ProfInfo : public HawkProtocol
	{
	public:
		Sys_ProfInfo(UInt32 iTimeStamp = 0, UInt32 iCpuCount = 0, UInt64 iTotalMem = 0, UInt32 iCpuUsage = 0, UInt64 iMemUsage = 0, UInt32 iConnect = 0, UInt64 iRecvProto = 0, UInt64 iRecvSize = 0, UInt64 iSendProto = 0, UInt64 iSendSize = 0) : HawkProtocol(SysProtocol::SYS_PROF_INFO), m_iTimeStamp(iTimeStamp), m_iCpuCount(iCpuCount), m_iTotalMem(iTotalMem), m_iCpuUsage(iCpuUsage), m_iMemUsage(iMemUsage), m_iConnect(iConnect), m_iRecvProto(iRecvProto), m_iRecvSize(iRecvSize), m_iSendProto(iSendProto), m_iSendSize(iSendSize)
		{
		}

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

		virtual Bool Clear()
		{
			m_iTimeStamp = 0;
			m_iCpuCount = 0;
			m_iTotalMem = 0;
			m_iCpuUsage = 0;
			m_iMemUsage = 0;
			m_iConnect = 0;
			m_iRecvProto = 0;
			m_iRecvSize = 0;
			m_iSendProto = 0;
			m_iSendSize = 0;
			return true;
		};

	public:
		UInt32 m_iTimeStamp;
		UInt32 m_iCpuCount;
		UInt64 m_iTotalMem;
		UInt32 m_iCpuUsage;
		UInt64 m_iMemUsage;
		UInt32 m_iConnect;
		UInt64 m_iRecvProto;
		UInt64 m_iRecvSize;
		UInt64 m_iSendProto;
		UInt64 m_iSendSize;
	};

	HAWK_CLOSE_NAMESPACE(SysProtocol)
}
#endif
