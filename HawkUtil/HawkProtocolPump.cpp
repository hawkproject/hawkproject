#include "HawkProtocolPump.h"
#include "HawkProtocolManager.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(ProtocolPump);

	HawkProtocolPump::HawkProtocolPump()
	{
	}

	HawkProtocolPump::~HawkProtocolPump()
	{
		ProtocolListenerMap::iterator it = m_mListener.begin();
		for (;it != m_mListener.end();it++)
		{
			it->second.clear();
		}
		m_mListener.clear();

		for (Size_t i=0;i<m_vProtocol.size();i++)
		{
			P_ProtocolManager->ReleaseProto(m_vProtocol[i]);
		}
		m_vProtocol.clear();
	}

	Bool HawkProtocolPump::AddListener(Int32 iMsg, HawkProtocolListener* pListener)
	{
		ProtocolListenerMap::iterator ml_it = m_mListener.find(iMsg);
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

	Bool HawkProtocolPump::RemoveListener(Int32 iMsg, HawkProtocolListener* pListener)
	{
		ProtocolListenerMap::iterator ml_it = m_mListener.find(iMsg);
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

	Bool HawkProtocolPump::Post(HawkProtocol* pProtocol)
	{
		if (pProtocol)
		{
			m_vProtocol.push_back(pProtocol);
			return true;
		}

		return true;
	}

	Bool HawkProtocolPump::Send(HawkProtocol* pProtocol)
	{
		if (pProtocol)
		{
			ProtocolListenerMap::iterator ml_it = m_mListener.find(pProtocol->GetType());
			if (ml_it != m_mListener.end())
			{
				ListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnProtocol(pProtocol);
				}
			}

			P_ProtocolManager->ReleaseProto(pProtocol);
			return true;
		}
		return false;
	}

	Bool HawkProtocolPump::Tick()
	{
		for (Size_t i=0;i<m_vProtocol.size();i++)
		{
			ProtocolListenerMap::iterator ml_it = m_mListener.find(m_vProtocol[i]->GetType());
			if (ml_it != m_mListener.end())
			{
				ListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnProtocol(m_vProtocol[i]);
				}
			}
			P_ProtocolManager->ReleaseProto(m_vProtocol[i]);
		}
		m_vProtocol.clear();

		return true;
	}	
}
