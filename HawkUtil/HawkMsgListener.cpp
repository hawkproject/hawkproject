#include "HawkMsgListener.h"
#include "HawkMsgPump.h"

namespace Hawk
{
	HawkMsgListener::HawkMsgListener()
	{
	}

	HawkMsgListener::~HawkMsgListener()
	{
		AbandonMsg();
	}

	Bool  HawkMsgListener::MonitorMsg(Int32 iMsg)
	{
		if (m_mMsgType.find(iMsg) == m_mMsgType.end())
		{
			m_mMsgType[iMsg] = iMsg;
			P_MsgPump->AddListener(iMsg, this);
			return true;
		}		
		return false;
	}

	Bool  HawkMsgListener::OnMessage(const HawkMsg& sMsg)
	{
		if (m_mMsgType.find(sMsg.Msg) != m_mMsgType.end())
		{
			return true;
		}
		return false;
	}

	Bool  HawkMsgListener::AbandonMsg(Int32 iMsg)
	{
		if (iMsg)
		{
			MsgTypeMap::iterator it = m_mMsgType.find(iMsg);
			if (it != m_mMsgType.end())
			{
				P_MsgPump->RemoveListener(iMsg, this);
				m_mMsgType.erase(it);
				return true;
			}
		}
		else
		{
			MsgTypeMap::iterator it = m_mMsgType.begin();
			for (;it != m_mMsgType.end();it++)
			{
				P_MsgPump->RemoveListener(it->first, this);
			}
			m_mMsgType.clear();
			return true;
		}
		return false;
	}
}
