#ifndef HAWK_HMACSHA1SECURITY_H
#define HAWK_HMACSHA1SECURITY_H

#include "HawkSHA1Security.h"

namespace Hawk
{
	/************************************************************************/
	/* HMACSHA1�����㷨��װ                                                 */
	/************************************************************************/
	class HawkHMACSHA1Security : public HawkSHA1Security
	{
	public:
		//����
		HawkHMACSHA1Security();

		//����
		virtual ~HawkHMACSHA1Security();

		enum 
		{
			SHA1_DIGEST_SIZE	= 20,
			SHA1_BLOCK_SIZE		= 64,
		} ;

	public:
		//����
		virtual void  DoFinal(UChar* pData, Int32 iSize, UChar* pKey, Int32 iKeyLen, UChar* pDigest);
	};
}
#endif
