#include "HawkBase64.h"
#include "HawkStringUtil.h"

namespace Hawk
{
	void HawkBase64::Encode(const void* pData,Int32 iSize,AString& sOut)
	{
		A_Exception(pData && iSize >0);
		
		sOut.clear();

		const UChar* pTmpData = (UChar*)pData;

		//编码表
		const static Char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		
		//返回值
		AString strEncode = "";

		UChar Tmp[3] = {0};

		for(Int32 i=0;i<(iSize / 3);i++)
		{
			Tmp[0] = *pTmpData++;
			Tmp[1] = *pTmpData++;
			Tmp[2] = *pTmpData++;

			strEncode += EncodeTable[Tmp[0] >> 2];
			strEncode += EncodeTable[((Tmp[0] << 4) | (Tmp[1] >> 4)) & 0x3F];
			strEncode += EncodeTable[((Tmp[1] << 2) | (Tmp[2] >> 6)) & 0x3F];
			strEncode += EncodeTable[Tmp[2] & 0x3F];
		}

		//对剩余数据进行编码
		Int32 iMod = iSize % 3;
		if(iMod == 1)
		{
			Tmp[0] = *pTmpData++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4)];
			strEncode += "==";
		}
		else if(iMod == 2)
		{
			Tmp[0] = *pTmpData++;
			Tmp[1] = *pTmpData++;
			strEncode += EncodeTable[(Tmp[0] & 0xFC) >> 2];
			strEncode += EncodeTable[((Tmp[0] & 0x03) << 4) | ((Tmp[1] & 0xF0) >> 4)];
			strEncode += EncodeTable[((Tmp[1] & 0x0F) << 2)];
			strEncode += "=";
		}

		sOut = strEncode;
	}

	void HawkBase64::Decode(const AString& sData,void* pOut,Int32& iOut)
	{
		A_Exception(sData.size() && pOut && iOut>0);

		AString sTmp = sData;

		const UChar* pTmpData = (const UChar*)sTmp.c_str();

		//解码表
		const static UChar DecodeTable[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				62, // '+'
				0, 0, 0,
				63, // '/'
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
				0, 0, 0, 0, 0, 0, 0,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
				0, 0, 0, 0, 0, 0,
				26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
				39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
		};

		//返回值
		Char* pTmpOut = (Char*)pOut;
		Int32 iOutByte= 0;
		Int32 iVal    = 0;
		Int32 iIdx       = 0;
		Int32 iSize   = (Int32)sTmp.size();

		while (iIdx < iSize)
		{
			A_Exception(iOut >= iOutByte);

			if (*pTmpData != '\r' && *pTmpData != '\n')
			{
				iVal         =  DecodeTable[*pTmpData++] << 18;
				iVal         += DecodeTable[*pTmpData++] << 12;
				(*pTmpOut++) =  Char((iVal & 0x00FF0000) >> 16);
				iOutByte++;

				if (*pTmpData != '=')
				{
					iVal         += DecodeTable[*pTmpData++] << 6;
					(*pTmpOut++) =  Char((iVal & 0x0000FF00) >> 8);
					iOutByte ++;

					if (*pTmpData != '=')
					{
						iVal         += DecodeTable[*pTmpData++];
						(*pTmpOut++) =  Char(iVal & 0x000000FF);
						iOutByte ++;
					}
				}
				iIdx += 4;
			}
			else// 回车换行,跳过
			{
				pTmpData ++;
				iIdx ++;
			}
		}

		iOut = iOutByte;
	}
}
