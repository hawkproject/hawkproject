#include "HawkProtocolListener.h"
#include "HawkProtocolPump.h"

namespace Hawk
{
	HawkProtocolListener::HawkProtocolListener()
	{
	}

	HawkProtocolListener::~HawkProtocolListener()
	{
		AbandonProto();
	}

	Bool  HawkProtocolListener::MonitorProto(ProtoType iType)
	{
		if (m_mProtoType.find(iType) == m_mProtoType.end())
		{
			m_mProtoType[iType] = iType;
			P_ProtocolPump->AddListener(iType, this);
			return true;
		}		
		return false;
	}

	Bool  HawkProtocolListener::OnProtocol(HawkProtocol* pProto)
	{
		if (m_mProtoType.find(pProto->GetType()) != m_mProtoType.end())
		{
			return true;
		}
		return false;
	}

	Bool  HawkProtocolListener::AbandonProto(ProtoType iType)
	{
		if (iType)
		{
			ProtoTypeMap::iterator it = m_mProtoType.find(iType);
			if (it != m_mProtoType.end())
			{
				P_ProtocolPump->RemoveListener(iType, this);
				m_mProtoType.erase(it);
				return true;
			}
		}
		else
		{
			ProtoTypeMap::iterator it = m_mProtoType.begin();
			for (;it != m_mProtoType.end();it++)
			{
				P_ProtocolPump->RemoveListener(it->first, this);
			}
			m_mProtoType.clear();
			return true;
		}
		return false;
	}
}
