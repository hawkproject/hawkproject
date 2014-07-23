#include "HawkHMACSHA1Security.h"

namespace Hawk
{
	HawkHMACSHA1Security::HawkHMACSHA1Security() 
	{
	}

	HawkHMACSHA1Security::~HawkHMACSHA1Security()
	{
	}

	void HawkHMACSHA1Security::DoFinal(UChar* pData, Int32 iSize, UChar* pKey, Int32 iKeyLen, UChar* pDigest)
	{
		UInt8 vKey[SHA1_BLOCK_SIZE]  = {0};
		UInt8 vPad[SHA1_BLOCK_SIZE]  = {0};
		UInt8 vSha[SHA1_DIGEST_SIZE] = {0};

		if (iKeyLen > SHA1_BLOCK_SIZE)
		{
			HawkSHA1Security::Reset();
			HawkSHA1Security::Update((UInt8*)pKey, iKeyLen);
			HawkSHA1Security::Final();
			HawkSHA1Security::GetHash(vKey);
			iKeyLen = SHA1_DIGEST_SIZE;
		}
		else
		{
			memcpy(vKey, pKey, iKeyLen);
		}
		
		//////////////////////////////////////////////////////////////////////////

		for (Int32 i = 0;i<iKeyLen;i++) 
			vPad[i] = vKey[i] ^ 0x36;

		for (Int32 i = iKeyLen;i<SHA1_BLOCK_SIZE;i++) 
			vPad[i] = 0x36 ;

		HawkSHA1Security::Reset();
		HawkSHA1Security::Update(vPad, SHA1_BLOCK_SIZE);
		HawkSHA1Security::Update(pData, iSize);
		HawkSHA1Security::Final();
		HawkSHA1Security::GetHash(vSha);

		//////////////////////////////////////////////////////////////////////////

		for (Int32 i = 0;i<iKeyLen;i++) 
			vPad[i] = vKey[i] ^ 0x5C;

		for (Int32 i = iKeyLen;i<SHA1_BLOCK_SIZE;i++) 
			vPad[i] = 0x5C ;

		HawkSHA1Security::Reset();
		HawkSHA1Security::Update(vPad, SHA1_BLOCK_SIZE);
		HawkSHA1Security::Update(vSha, SHA1_DIGEST_SIZE);
		HawkSHA1Security::Final();

		HawkSHA1Security::GetHash((UInt8*)pDigest);
	}
}
