#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"
#include "iconv.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#	define get16bits(d) (*((const UInt16 *) (d)))
#endif

#if !defined (get16bits)
#	define get16bits(d) ((((const UInt8 *)(d))[1] << 8)+((const UInt8 *)(d))[0])
#endif

namespace Hawk
{
	//Paul Hsieh's Hash算法
	UInt32 HawkStringUtil::CalcHash(const Char * pData, Size_t iSize)
	{
		if (!pData) return 0;

		if (!iSize)
			iSize = strlen(pData);

		if (!iSize) return 0;
		
		UInt32 iHash = (UInt32)iSize;
		UInt32 iTmp  = 0;
		Int32  iRem  = 0;

		iRem = iSize & 3;
		iSize >>= 2;

		for (;iSize > 0; iSize--) 
		{
			iHash += get16bits(pData);
			iTmp  = (get16bits(pData+2) << 11) ^ iHash;
			iHash = (iHash << 16) ^ iTmp;
			pData += 2*sizeof (UInt16);
			iHash  += iHash >> 11;
		}

		switch (iRem) 
		{
		case 3: 
			iHash += get16bits(pData);
			iHash ^= iHash << 16;
			iHash ^= pData[sizeof (UInt16)] << 18;
			iHash += iHash >> 11;
			break;
		case 2: 
			iHash += get16bits (pData);
			iHash ^= iHash << 11;
			iHash += iHash >> 17;
			break;
		case 1: 
			iHash += *pData;
			iHash ^= iHash << 10;
			iHash += iHash >> 1;
		}

		iHash ^= iHash << 3;
		iHash += iHash >> 5;
		iHash ^= iHash << 4;
		iHash += iHash >> 17;
		iHash ^= iHash << 25;
		iHash += iHash >> 6;

		return iHash;
	}

	//字符到16进制转换
	AString HawkStringUtil::Char2Hex(Char cCh)
	{
		Char dig1 = (cCh & 0xF0) >> 4;
		Char dig2 = (cCh & 0x0F);

		if (0<= dig1 && dig1<= 9)  dig1 += 48;
		if (10<= dig1 && dig1<=15) dig1 += (65-10);
		if (0<= dig2 && dig2<= 9)  dig2 += 48;
		if (10<= dig2 && dig2<=15) dig2 += (65-10);

		AString sVal;
		sVal.append(1,dig1);
		sVal.append(1,dig2);

		return sVal;
	}

	//格式化Url编码
	AString HawkStringUtil::UrlEncode(const AString& sUrl)
	{
		AString sEscaped = "";
		for(Size_t i=0; i<sUrl.size(); i++)
		{
			if (('0' <= sUrl[i] && sUrl[i] <= '9') ||
				('a' <= sUrl[i] && sUrl[i] <= 'z') ||
				('A' <= sUrl[i] && sUrl[i] <= 'Z') ||
				(sUrl[i] == '~' || sUrl[i] == '-' || sUrl[i] == '_' || sUrl[i] == '.')
				)
			{
				sEscaped.append(1, sUrl[i]);
			}
			else
			{
				sEscaped.append("%");
				sEscaped.append(Char2Hex(sUrl[i]));
			}
		}
		return sEscaped;
	}

	//转换为UTF8编码
	AString HawkStringUtil::CodePointToUTF8(UInt32 iCp)
	{
		AString sResult;

		//Description From: http://en.wikipedia.org/wiki/UTF-8

		if (iCp <= 0x7F) 
		{
			sResult.resize(1);
			sResult[0] = static_cast<Char>(iCp);
		} 
		else if (iCp <= 0x7FF) 
		{
			sResult.resize(2);
			sResult[1] = static_cast<Char>(0x80 | (0x3F & iCp));
			sResult[0] = static_cast<Char>(0xC0 | (0x1F & (iCp >> 6)));
		} 
		else if (iCp <= 0xFFFF) 
		{
			sResult.resize(3);
			sResult[2] = static_cast<Char>(0x80 | (0x3F & iCp));
			sResult[1] = 0x80 | static_cast<Char>((0x3F & (iCp >> 6)));
			sResult[0] = 0xE0 | static_cast<Char>((0xF & (iCp >> 12)));
		}
		else if (iCp <= 0x10FFFF) 
		{
			sResult.resize(4);
			sResult[3] = static_cast<Char>(0x80 | (0x3F & iCp));
			sResult[2] = static_cast<Char>(0x80 | (0x3F & (iCp >> 6)));
			sResult[1] = static_cast<Char>(0x80 | (0x3F & (iCp >> 12)));
			sResult[0] = static_cast<Char>(0xF0 | (0x7 & (iCp >> 18)));
		}

		return sResult;
	}

	//ascii转unicode编码
	UInt32 HawkStringUtil::AsciiToUnicode(const Char* pSrc, UInt32 iSrcLen, WChar* pDst, UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"UTF-16LE", (const char *)"GB18030");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(Char);
		Size_t iOutSize = iDstLen * sizeof(WChar);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(WChar) - iOutSize) / sizeof(WChar);
	}

	//unicode转ascii编码
	UInt32 HawkStringUtil::UnicodeToAscii(const WChar* pSrc,UInt32 iSrcLen,Char* pDst, UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"GB18030", (const char *)"UTF-16LE");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(WChar);
		Size_t iOutSize = iDstLen * sizeof(Char);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(Char) - iOutSize) / sizeof(Char);
	}

	//utf8转unicode编码
	UInt32 HawkStringUtil::Utf8ToUnicode(const Utf8* pSrc,UInt32 iSrcLen,WChar* pDst,UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"UTF-16LE", (const char *)"UTF-8");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(Utf8);
		Size_t iOutSize = iDstLen * sizeof(WChar);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(WChar) - iOutSize) / sizeof(WChar);
	}

	//unicode转utf8编码
	UInt32 HawkStringUtil::UnicodeToUtf8(const WChar* pSrc,UInt32 iSrcLen,Utf8* pDst,UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"UTF-8", (const char *)"UTF-16LE");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(WChar);
		Size_t iOutSize = iDstLen * sizeof(Utf8);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(Utf8) - iOutSize) / sizeof(Utf8);
	}

	//ascii转utf8编码
	UInt32 HawkStringUtil::AsciiToUtf8(const Char* pSrc,UInt32 iSrcLen,Utf8* pDst,UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"UTF-8",(const char *)"GB18030");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(Char);
		Size_t iOutSize = iDstLen * sizeof(Utf8);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(Utf8) - iOutSize) / sizeof(Utf8);
	}

	//utf8转ascii编码
	UInt32 HawkStringUtil::Utf8ToAscii(const Utf8* pSrc, UInt32 iSrcLen, Char* pDst, UInt32 iDstLen)
	{
		if(!pSrc || !iSrcLen || !pDst || !iDstLen)
			return 0;

		iconv_t hConv = iconv_open((const char *)"GB18030",(const char *)"UTF-8");
		if (hConv == (iconv_t)HAWK_ERROR) 
			return 0;

		Char* pInBuf	 = (Char*)pSrc;
		Char* pOutBuf	 = (Char*)pDst;
		Size_t iInSize	 = iSrcLen * sizeof(Utf8);
		Size_t iOutSize = iDstLen * sizeof(Char);

		if (iconv(hConv, &pInBuf, &iInSize, &pOutBuf, &iOutSize) == (Size_t)HAWK_ERROR)
		{
			iconv_close(hConv);
			return 0;
		}

		iconv_close(hConv);
		return (iDstLen * sizeof(Char) - iOutSize) / sizeof(Char);
	}

	//////////////////////////////////////////////////////////////////////////
	//stl字符串编码转换
	WString HawkStringUtil::ToWString(const AString& sVal)
	{
		WString sRet;

		sRet.resize(sVal.size());
		sRet.resize(AsciiToUnicode(sVal.data(),sVal.size(),(WChar*)sRet.data(),sRet.size()));

		return sRet;
	}

	WString	HawkStringUtil::ToWString(const UString& sVal)
	{
		WString sRet;

		sRet.resize(sVal.size());
		sRet.resize(Utf8ToUnicode(sVal.c_str(), sVal.size(), (WChar*)sRet.data(), sRet.size()));
		
		return sRet;
	}

	AString HawkStringUtil::ToString(const WString& sVal)
	{
		AString sRet;

		sRet.resize(sVal.size() * 4);
		sRet.resize(UnicodeToAscii(sVal.c_str(), sVal.size(), (Char*)sRet.data(), sRet.size()));

		return sRet;
	}

	AString	HawkStringUtil::ToString(const UString& sVal)
	{
		AString sRet;

		sRet.resize(sVal.size() * 4);
		sRet.resize(Utf8ToAscii(sVal.c_str(), sVal.size(), (Char*)sRet.data(), sRet.size()));

		return sRet;
	}

	UString HawkStringUtil::ToUtf8(const WString& sVal)
	{
		UString sRet;

		sRet.resize(sVal.size() * 4);
		sRet.resize(UnicodeToUtf8(sVal.c_str(), sVal.size(), (Utf8*)sRet.data(), sRet.size()));

		return sRet;
	}

	UString	HawkStringUtil::ToUtf8(const AString& sVal)
	{
		UString sRet;

		sRet.resize(sVal.size() * 4);
		sRet.resize(AsciiToUtf8(sVal.c_str(), sVal.size(), (Utf8*)sRet.data(), sRet.size()));

		return sRet;
	}

	//////////////////////////////////////////////////////////////////////////
	//简易函数定义
	UString  _Utf8(const Char* sVal)
	{
		if(!sVal || !strlen(sVal))
			return (Utf8*)"";

		return HawkStringUtil::ToUtf8(sVal);
	}

	UString  _Utf8(const WChar*	sVal)
	{
		if(!sVal || !wcslen(sVal))
			return (Utf8*)"";

		return HawkStringUtil::ToUtf8(sVal);
	}

	UString  _Utf8(const AString& sVal)
	{
		if(!sVal.size())
			return (Utf8*)"";

		return HawkStringUtil::ToUtf8(sVal);
	}

	UString  _Utf8(const WString& sVal)
	{
		if(!sVal.size())
			return (Utf8*)"";

		return HawkStringUtil::ToUtf8(sVal);
	}

	AString  _Ascii(const UChar* sVal)
	{
		if(!sVal || !strlen((const Char*)sVal))
			return "";

		return HawkStringUtil::ToString(sVal);
	}

	AString  _Ascii(const WChar* sVal)
	{
		if(!sVal || !wcslen(sVal))
			return "";

		return HawkStringUtil::ToString(sVal);
	}

	AString  _Ascii(const UString& sVal)
	{
		if(!sVal.size())
			return "";

		return HawkStringUtil::ToString(sVal);
	}

	AString  _Ascii(const WString& sVal)
	{
		if(!sVal.size())
			return "";

		return HawkStringUtil::ToString(sVal);
	}	

	WString  _Unicode(const Char* sVal)
	{
		if(!sVal || !strlen(sVal))
			return L"";

		return HawkStringUtil::ToWString(sVal);
	}

	WString  _Unicode(const UChar* sVal)
	{
		if(!sVal || !strlen((const Char*)sVal))
			return L"";

		return HawkStringUtil::ToWString(sVal);
	}

	WString  _Unicode(const AString& sVal)
	{
		if(!sVal.size())
			return L"";

		return HawkStringUtil::ToWString(sVal);
	}

	WString  _Unicode(const UString& sVal)
	{
		if(!sVal.size())
			return L"";

		return HawkStringUtil::ToWString(sVal);
	}
}
