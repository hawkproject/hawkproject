#include "HawkXorSecurity.h"

namespace Hawk
{
	HawkXorSecurity::HawkXorSecurity() : HawkSecurity()
	{
		m_cXorKey = 0;
	}

	HawkXorSecurity::~HawkXorSecurity()
	{
	}

	void HawkXorSecurity::SetParameter(const HawkOctets& xParam)
	{
		if (xParam.Size())
			SetParameter(*((UChar*)xParam.Begin()));
	}

	void HawkXorSecurity::SetParameter(UChar cXorKey)
	{
		m_cXorKey = cXorKey;
	}

	Bool HawkXorSecurity::Update(HawkOctets& xOctets)
	{
		UChar *pData  = (UChar *)xOctets.Begin();
		UInt32 iSize  = xOctets.Size();
		for (UInt32 i = 0; i < iSize; i++)
		{
			pData[i] ^= m_cXorKey;
		}
		return true;
	}
}
