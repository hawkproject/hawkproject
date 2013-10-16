#include "HawkSHA1Security.h"

#ifndef ROL32
#	define ROL32(_val, _bits)	(((_val)<<(_bits)) | ((_val)>>(32-(_bits))))
#endif

#ifdef HAWK_BIG_ENDIAN
#	define SHABLK0(i)			(m_pBlock->l[i])
#else
#	define SHABLK0(i)			(m_pBlock->l[i] = (ROL32(m_pBlock->l[i],24) & 0xFF00FF00) | (ROL32(m_pBlock->l[i],8) & 0x00FF00FF))
#endif

#define SHABLK(i)				(m_pBlock->l[i&15] = ROL32(m_pBlock->l[(i+13)&15] ^ m_pBlock->l[(i+8)&15] ^ m_pBlock->l[(i+2)&15] ^ m_pBlock->l[i&15],1))

#define _R0(v, w, x, y, z, i)	{ z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30);		}
#define _R1(v, w, x, y, z, i)	{ z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30);		}
#define _R2(v, w, x, y, z, i)	{ z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30);			}
#define _R3(v, w, x, y, z, i)	{ z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30);	}
#define _R4(v, w, x, y, z, i)	{ z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30);			}

namespace Hawk
{
	HawkSHA1Security::HawkSHA1Security()
	{
		m_pBlock = (SHA1_BLOCK*)m_vWorkspace;

		Reset();
	}

	HawkSHA1Security::~HawkSHA1Security()
	{
		Reset();
	}

	void HawkSHA1Security::Reset()
	{
		m_vState[0] = 0x67452301;
		m_vState[1] = 0xEFCDAB89;
		m_vState[2] = 0x98BADCFE;
		m_vState[3] = 0x10325476;
		m_vState[4] = 0xC3D2E1F0;

		m_vCount[0] = 0;
		m_vCount[1] = 0;
	}

	void HawkSHA1Security::Transform(UInt32* pState, UInt8* pBuffer)
	{
		memcpy(m_pBlock, pBuffer, 64);
		UInt32 a = pState[0], b = pState[1], c = pState[2], d = pState[3], e = pState[4];		

		_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
		_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
		_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
		_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
		_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
		_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
		_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
		_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
		_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
		_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
		_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
		_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
		_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
		_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
		_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
		_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
		_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
		_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
		_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
		_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

		pState[0] += a;
		pState[1] += b;
		pState[2] += c;
		pState[3] += d;
		pState[4] += e;
	}

	void HawkSHA1Security::Update(UInt8* pData, UInt32 iSize)
	{
		UInt32 i = 0;
		UInt32 j = m_vCount[0];

		if((m_vCount[0] += iSize << 3) < j) 
			m_vCount[1]++;

		j = (j >> 3) & 63;
		m_vCount[1] += (iSize >> 29);

		if(j + iSize > 63)
		{
			i = 64 - j;
			memcpy(&m_vBuffer[j], pData, i);
			Transform(m_vState, m_vBuffer);

			for(; i + 63 < iSize; i += 64) 
				Transform(m_vState, &pData[i]);

			j = 0;
		}
		else 
		{
			i = 0;
		}

		memcpy(&m_vBuffer[j], &pData[i], iSize - i);
	}

	void HawkSHA1Security::Final()
	{
		UInt8 vFinal[8] = {0};

		for(UInt32 i = 0; i < 8; i++)
		{
			vFinal[i] = (UInt8)((m_vCount[((i >= 4) ? 0 : 1)] >> ((3 - (i & 3)) * 8) ) & 255);
		}

		Update((UInt8 *)"\200", 1);

		while ((m_vCount[0] & 504) != 448)
			Update((UInt8 *)"\0", 1);

		Update(vFinal, 8);

		for(UInt32 i = 0; i < 20; i++)
		{
			m_vDigest[i] = (UInt8)((m_vState[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
		}

		memset(m_vBuffer,0, 64);
		memset(m_vState, 0, 20);
		memset(m_vCount, 0, 8);
	}

	Bool HawkSHA1Security::ReportHash(Char* pReport, Int8 iType)
	{
		char szTemp[16] = {0};
		if(pReport)
		{
			if(iType == REPORT_HEX)
			{
				for(Int32 i = 0; i < 20; i++)
				{
					if (i == 0)
						sprintf(szTemp, "%02X", m_vDigest[i]);
					else
						sprintf(szTemp, " %02X", m_vDigest[i]);

					strcat(pReport, szTemp);
				}
				return true;
			}
			else if(iType == REPORT_DIGIT)
			{
				for(Int32 i = 0; i < 20; i++)
				{
					if (i == 0)
						sprintf(szTemp, "%u", m_vDigest[i]);
					else
						sprintf(szTemp, " %u", m_vDigest[i]);

					strcat(pReport, szTemp);
				}
				return true;
			}
		}
		return false;
	}

	void HawkSHA1Security::GetHash(UInt8 *pHash)
	{
		memcpy(pHash, m_vDigest, 20);
	}
}
