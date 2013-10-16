#ifndef HAWK_MD5_H
#define HAWK_MD5_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* Md5编码格式,返回16进制字符串32个字节                                 */
	/************************************************************************/
	class UTIL_API HawkMd5
	{
	public:
		//返回32位MD5字符串和16位MD5码,pMd5Data为Char[16],包含16个字节
		static AString MakeMD5(const void* pData, Int32 iSize, UChar* pMd5Data = 0);

		//返回32位MD5字符串和16位MD5码,pMd5Data为Char[16],包含16个字节
		static AString MakeMD5(HawkFile* pFile, UChar* pMd5Data = 0);
	};
}
#endif
