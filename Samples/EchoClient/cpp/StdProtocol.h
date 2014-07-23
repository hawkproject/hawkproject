#ifndef __STDPROTOCOL_H__
#define __STDPROTOCOL_H__

#include "ProtocolId.h"
#include "ErrorCode.h"

namespace Hawk
{
	//错误通知
	class ErrNotify : public HawkProtocol
	{
	public:
		ErrNotify(UInt16 iErrId = 0, Int8 iMode = 0) : HawkProtocol(ProtocolId::ERR_NOTIFY), m_iErrId(iErrId), m_iMode(iMode)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new ErrNotify;
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

		virtual Bool Clear()
		{
			m_iErrId = 0;
			m_iMode = 0;
			return true;
		};

	public:
		UInt16 m_iErrId;
		Int8 m_iMode;
	};

	//消息通知
	class MsgNotify : public HawkProtocol
	{
	public:
		MsgNotify(const UString& sMsg = UString(), Int8 iMode = 0) : HawkProtocol(ProtocolId::MSG_NOTIFY), m_sMsg(sMsg), m_iMode(iMode)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new MsgNotify;
		};

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			rhsOS << m_sMsg << m_iMode;
			return rhsOS;
		};

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			rhsOS >> m_sMsg >> m_iMode;
			return rhsOS;
		};

		virtual Bool Clear()
		{
			m_sMsg.clear();
			m_iMode = 0;
			return true;
		};

	public:
		UString m_sMsg;
		Int8 m_iMode;
	};

	//时间同步
	class TimeSync : public HawkProtocol
	{
	public:
		TimeSync(UInt32 iTime = 0) : HawkProtocol(ProtocolId::TIME_SYNC), m_iTime(iTime)
		{
		}

		virtual HawkProtocol* Clone() const
		{
			return new TimeSync;
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

}
#endif
