#include "HawkMsgPump.h"
#include "HawkMsgManager.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(MsgPump);

	HawkMsgPump::HawkMsgPump()
	{
	}

	HawkMsgPump::~HawkMsgPump()
	{
		MsgListenerMap::iterator it = m_mListener.begin();
		for (;it != m_mListener.end();it++)
		{
			it->second.clear();
		}
		m_mListener.clear();

		for (Size_t i=0;i<m_vPumpMsg.size();i++)
		{
			P_MsgManager->FreeMsg(m_vPumpMsg[i]);
		}
		m_vPumpMsg.clear();
	}

	Bool HawkMsgPump::AddListener(Int32 iMsg, HawkMsgListener* pListener)
	{
		MsgListenerMap::iterator ml_it = m_mListener.find(iMsg);
		if (ml_it == m_mListener.end())
		{
			m_mListener[iMsg] = ListenerMap();
			ml_it = m_mListener.find(iMsg);
		}

		if (ml_it != m_mListener.end())
		{
			ListenerMap& mListener = ml_it->second;
			if (mListener.find(pListener) == mListener.end())
			{
				mListener[pListener] = pListener;
				return true;
			}
		}
		
		return false;
	}

	Bool HawkMsgPump::RemoveListener(Int32 iMsg, HawkMsgListener* pListener)
	{
		MsgListenerMap::iterator ml_it = m_mListener.find(iMsg);
		if (ml_it != m_mListener.end())
		{
			ListenerMap::iterator rl_it = ml_it->second.find(pListener);
			if (rl_it != ml_it->second.end())
			{
				ml_it->second.erase(rl_it);
				return true;
			}
		}

		return false;
	}

	Bool HawkMsgPump::Post(HawkMsg* pMsg)
	{
		if (pMsg)
		{
			m_vPumpMsg.push_back(pMsg);
			return true;
		}

		return true;
	}

	Bool HawkMsgPump::Send(HawkMsg* pMsg)
	{
		if (pMsg)
		{
			MsgListenerMap::iterator ml_it = m_mListener.find(pMsg->Msg);
			if (ml_it != m_mListener.end())
			{
				ListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnMessage(*pMsg);
				}
			}

			P_MsgManager->FreeMsg(pMsg);
			return true;
		}
		return false;
	}

	Bool HawkMsgPump::Tick()
	{
		for (Size_t i=0;i<m_vPumpMsg.size();i++)
		{
			MsgListenerMap::iterator ml_it = m_mListener.find(m_vPumpMsg[i]->Msg);
			if (ml_it != m_mListener.end())
			{
				ListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnMessage(*m_vPumpMsg[i]);
				}
			}

			P_MsgManager->FreeMsg(m_vPumpMsg[i]);
		}

		m_vPumpMsg.clear();
		return true;
	}		
}
