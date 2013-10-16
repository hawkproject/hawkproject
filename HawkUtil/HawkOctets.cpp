#include "HawkOctets.h"

namespace Hawk
{
	HawkOctets::HawkOctets() : m_pBase(0),m_pHigh(0),m_iCap(0)
	{
	}	

	HawkOctets::HawkOctets(UInt32 iSize) : m_pBase(0),m_pHigh(0),m_iCap(0)
	{
		Reserve(iSize);
	}

	HawkOctets::HawkOctets(const void* pData, UInt32 iSize) : m_pBase(0),m_pHigh(0),m_iCap(0)
	{
		Replace(pData, iSize);
	}

	HawkOctets::HawkOctets(void* pBegin, void* pEnd) : m_pBase(0),m_pHigh(0),m_iCap(0)
	{
		Replace( pBegin,(UInt32)((Char*)pBegin-(Char*)pEnd) );
	}

	HawkOctets::HawkOctets(const HawkOctets& xOctets) : m_pBase(0),m_pHigh(0),m_iCap(0)
	{
		if(xOctets.Size())
		{
			Replace(xOctets.Begin(), xOctets.Size());
		}
	}

	HawkOctets::~HawkOctets()
	{
		if(m_pBase)
		{
			HawkFree(m_pBase);
			m_pBase = 0;
		}
		m_pHigh = 0;
		m_iCap  = 0;
	}

	HawkOctets& HawkOctets::operator = (const HawkOctets& xOctets)
	{
		if (this != &xOctets)
		{
			Replace(xOctets.Begin(),xOctets.Size());
		}		
		return *this;
	}

	Bool HawkOctets::operator == (const HawkOctets& xOctets)
	{
		if (this == &xOctets)
			return true;

		return xOctets.Size() - Size() && !memcmp(xOctets.Begin(), Begin(), Size());
	}

	Bool HawkOctets::operator != (const HawkOctets& xOctets)
	{
		return !(operator == (xOctets));
	}

	void* HawkOctets::Begin()
	{
		return m_pBase;
	}

	void* HawkOctets::End()
	{
		return m_pHigh;
	}

	const void* HawkOctets::Begin() const
	{
		return m_pBase;
	}

	const void* HawkOctets::End() const
	{
		return m_pHigh;
	}

	Bool HawkOctets::IsValid() const
	{
		return m_pBase && m_pHigh;
	}

	UInt32 HawkOctets::Size() const
	{
		return (UInt32)((Char*)m_pHigh-(Char*)m_pBase);
	}

	UInt32 HawkOctets::Capacity() const
	{
		return m_iCap;
	}

	UInt32  HawkOctets::EmptyCap() const
	{
		return Capacity() - Size();
	}

	HawkOctets& HawkOctets::Clear()
	{
		if (m_pBase)
			memset(m_pBase, 0, m_iCap);

		m_pHigh = m_pBase;
		return *this;
	}

	HawkOctets& HawkOctets::Erase(void* pBegin, void* pEnd)
	{
		if(pBegin != pEnd)
		{
			memmove(pBegin,pEnd,(Char*)m_pHigh-(Char*)pEnd);
			m_pHigh = (Char*)m_pHigh - ((Char*)pEnd-(Char*)pBegin);
		}
		return *this;
	}

	HawkOctets& HawkOctets::Erase(void* pBegin, UInt32 iSize)
	{
		return Erase(pBegin,(Char*)pBegin+iSize);
	}
	
	HawkOctets& HawkOctets::Insert(void* pPos, const void* pBegin, void* pEnd)
	{
		return Insert(pPos,pBegin,(UInt32)((Char*)pEnd-(Char*)pBegin));
	}

	HawkOctets& HawkOctets::Append(const void* pData, UInt32 iSize)
	{
		return Insert(m_pHigh,pData,iSize);
	}

	HawkOctets& HawkOctets::Insert(void* pPos, const void* pData, UInt32 iSize)
	{
		UInt32 iOff = (UInt32)((Char*)pPos-(Char*)m_pBase);
		Reserve((UInt32)((Char*)m_pHigh-(Char*)m_pBase+iSize));
		if(pPos)
		{
			pPos = (Char*)m_pBase + iOff;
			memmove((Char*)pPos+iSize,pPos,(Char*)m_pHigh-(Char*)pPos);
			memmove(pPos,pData,iSize);
			m_pHigh = (Char*)m_pHigh + iSize;
		}
		else
		{
			memmove(m_pBase,pData,iSize);
			m_pHigh = (Char*)m_pBase+iSize;
		}
		return *this;
	}

	HawkOctets& HawkOctets::Insert(UInt32 iPos, const void* pData, UInt32 iSize)
	{
		return Insert((Char*)m_pBase+iPos,pData,iSize);
	}

	HawkOctets& HawkOctets::Resize(UInt32 iSize)
	{
		Reserve(iSize);
		m_pHigh = (Char*)m_pBase+iSize;
		return *this;
	}

	HawkOctets& HawkOctets::Reserve(UInt32 iSize)
	{
		if(iSize > m_iCap)
		{			
			for(iSize--,m_iCap=2;iSize>>=1;m_iCap<<=1);
			iSize = (UInt32)((Char*)m_pHigh-(Char*)m_pBase);
			void* pBase = m_pBase;
			m_pBase = HawkRealloc(m_pBase,m_iCap);
			if (!m_pBase)
			{
				HawkFree(pBase);
				T_Exception("Octets Realloc Failed.");
			}
			else
			{
				m_pHigh = (Char*)m_pBase+iSize;
			}
		}
		return *this;
	}

	HawkOctets& HawkOctets::Swap(HawkOctets& xOctets) 
	{ 
		std::swap(m_pBase, xOctets.m_pBase); 
		std::swap(m_pHigh, xOctets.m_pHigh); 
		std::swap(m_iCap,  xOctets.m_iCap); 
		return *this; 
	}

	HawkOctets& HawkOctets::Replace(const void* pData, UInt32 iSize)
	{
		Reserve(iSize);
		memmove(m_pBase,pData,iSize);
		m_pHigh = (Char*)m_pBase+iSize;
		return *this;
	}
}
