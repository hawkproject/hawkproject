#include "HawkCounter.h"

namespace Hawk
{
	HawkCounter::HawkCounter(UInt32 iPeriod)
	{
		m_iCounter = 0;
		m_bPause   = true;
		m_iPeriod  = iPeriod;
	}

	HawkCounter::~HawkCounter()
	{
	}

	Bool HawkCounter::IsFull() const
	{ 
		return (m_iCounter >= m_iPeriod) ? true : false; 
	}

	void HawkCounter::Reset(bool bFull) 
	{ 
		m_iCounter = bFull ? m_iPeriod : 0;	
	}

	void HawkCounter::SetPause(bool bPause) 
	{ 
		m_bPause = bPause; 
	}
	
	Bool HawkCounter::IncCounter(UInt32 iCounter)
	{ 
		if (!m_bPause)
			m_iCounter += iCounter;

		return (m_iCounter >= m_iPeriod) ? true : false;
	}
	
	void HawkCounter::DecCounter(UInt32 iCounter)
	{ 
		if (m_bPause)
			return;

		if (m_iCounter <= iCounter)
			m_iCounter = 0;
		else
			m_iCounter -= iCounter;
	}
}
