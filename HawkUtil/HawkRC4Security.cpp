#include "HawkRC4Security.h"

#define RC4_SBOX_LEN 256

namespace Hawk
{
	HawkRC4Security::HawkRC4Security() : HawkSecurity()
	{
		memset(m_cPerm, 0, 256);
		m_iIdx1 = m_iIdx2 = 0;
	}

	HawkRC4Security::~HawkRC4Security()
	{
	}

	void HawkRC4Security::SetParameter(const HawkOctets& xParam)
	{
		UChar* pIV  = (UChar *)xParam.Begin();
		Int32 iSize = xParam.Size();
		UInt32 i    = 0;
		UChar  j    = 0;

		for (i = 0; i < RC4_SBOX_LEN; i++)
			m_cPerm[i] = (UChar) i;

		for (j = i = 0; i < RC4_SBOX_LEN; i++)
		{
			j += m_cPerm[i] + pIV[i % iSize];
			std::swap(m_cPerm[i], m_cPerm[j]);
		}

		m_iIdx1 = m_iIdx2 = 0;
	}

	Bool HawkRC4Security::Update(HawkOctets& xOctets)
	{
		UChar *pData  = (UChar *)xOctets.Begin();
		UInt32 iSize  = xOctets.Size();
		for (UInt32 i = 0; i < iSize; i++)
		{
			m_iIdx2 += m_cPerm[++m_iIdx1];
			std::swap(m_cPerm[m_iIdx1], m_cPerm[m_iIdx2]);
			UChar j = m_cPerm[m_iIdx1] + m_cPerm[m_iIdx2];
			pData[i] ^= m_cPerm[j];
		}
		return true;
	}
}
