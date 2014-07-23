#ifndef HAWK_HMACSHA1SECURITY_H
#define HAWK_HMACSHA1SECURITY_H

#include "HawkSHA1Security.h"

namespace Hawk
{
	/************************************************************************/
	/* HMACSHA1加密算法封装                                                 */
	/************************************************************************/
	class HawkHMACSHA1Security : public HawkSHA1Security
	{
	public:
		//构造
		HawkHMACSHA1Security();

		//析构
		virtual ~HawkHMACSHA1Security();

		enum 
		{
			SHA1_DIGEST_SIZE	= 20,
			SHA1_BLOCK_SIZE		= 64,
		} ;

	public:
		//加密
		virtual void  DoFinal(UChar* pData, Int32 iSize, UChar* pKey, Int32 iKeyLen, UChar* pDigest);
	};
}
#endif
