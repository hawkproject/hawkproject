#ifndef HAWK_BASE64_H 
#define HAWK_BASE64_H 

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* Base64±àÂë                                                           */
	/************************************************************************/
	class UTIL_API HawkBase64
	{
	public:
		//±àÂë
		static void	Encode(const void* pData, Int32 iSize, AString& sOut);
			
		//½âÂë
		static void	Decode(const AString& sData, void* pOut, Int32& iOut);
	};
}
#endif
