#include "HawkManagerBase.h"

namespace Hawk
{
	HawkManagerBase::HawkManagerBase() : m_bRunning(false)
	{
	}

	HawkManagerBase::~HawkManagerBase()
	{
	}

	Bool  HawkManagerBase::Start()
	{
		if(!m_bRunning)
		{
			m_bRunning = true;
			return true;
		}
		return false;
	}

	Bool  HawkManagerBase::IsRunning() const
	{
		return m_bRunning;
	}

	Bool  HawkManagerBase::Tick(UInt32 iPeriod)
	{
		return m_bRunning;
	}

	Bool HawkManagerBase::Stop()
	{
		if(m_bRunning)
		{
			m_bRunning = false;
			return true;
		}
		return false;
	}
}
