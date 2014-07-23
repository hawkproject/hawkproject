#ifndef HAWK_MD5_H
#define HAWK_MD5_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* Md5�����ʽ,����16�����ַ���32���ֽ�                                 */
	/************************************************************************/
	class UTIL_API HawkMd5
	{
	public:
		//����32λMD5�ַ�����16λMD5��,pMd5DataΪChar[16],����16���ֽ�
		static AString MakeMD5(const void* pData, Int32 iSize, UChar* pMd5Data = 0);

		//����32λMD5�ַ�����16λMD5��,pMd5DataΪChar[16],����16���ֽ�
		static AString MakeMD5(HawkFile* pFile, UChar* pMd5Data = 0);
	};
}
#endif
