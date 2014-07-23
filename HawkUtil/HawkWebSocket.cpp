#include "HawkWebSocket.h"
#include "HawkStringUtil.h"
#include "HawkByteOrder.h"
#include "HawkMd5.h"

namespace Hawk
{
	HawkWebSocket::HawkWebSocket()
	{
		UChar szHttpReq[512] = "GET /game HTTP/1.1\r\n\
							   Upgrade: WebSocket\r\n\
							   Connection: Upgrade\r\n\
							   Host: localhost:5132\r\n\
							   Origin: null\r\n\
							   Sec-WebSocket-Protocol: hawk-protocol\r\n\
							   Sec-WebSocket-Key1: x EO2 59186 4  28\\dY 0+\r\n\
							   Sec-WebSocket-Key2: 1  9  3  57695W    0\r\n\r\n";

		Size_t iSize = strlen((const Char*)szHttpReq);
		szHttpReq[iSize]   = 0x1f;
		szHttpReq[iSize+1] = 0xf6;
		szHttpReq[iSize+2] = 0xf3;
		szHttpReq[iSize+3] = 0x3f;
		szHttpReq[iSize+4] = 0xc7;
		szHttpReq[iSize+5] = 0x17;
		szHttpReq[iSize+6] = 0x20;
		szHttpReq[iSize+7] = 0x88;
	}

	HawkWebSocket::~HawkWebSocket()
	{
	}	

	Bool HawkWebSocket::HandShake(const OctetsStream* pHttpReq, OctetsStream* pHttpRep)
	{
		AString sSecWebSocketKey1, sSecWebSocketKey2;
		UChar   szSecurityKey[8] = {0};

		if (GetSecWebSocketKey(pHttpReq, "Sec-WebSocket-Key1: ", sSecWebSocketKey1) &&
			GetSecWebSocketKey(pHttpReq, "Sec-WebSocket-Key2: ", sSecWebSocketKey2) &&
			Get8ByteSecurityKey(pHttpReq, szSecurityKey))
		{
			AString sFetch, sHost, sOrigin, sProtocol;
			GetSecWebSocketKey(pHttpReq, "GET ", sFetch, ' ');
			GetSecWebSocketKey(pHttpReq, "Host: ", sHost);
			GetSecWebSocketKey(pHttpReq, "Origin: ", sOrigin);
			GetSecWebSocketKey(pHttpReq, "Sec-WebSocket-Protocol: ", sProtocol);

			AString sSecWebSocketKey1_Digit = "", sSecWebSocketKey2_Digit = "";
			UInt32  iSecWebSocketKey1_Space = 0,  iSecWebSocketKey2_Space = 0;

			//统计sec1的数字和空格
			for (Size_t i=0;i<sSecWebSocketKey1.size();i++)
			{
				if (sSecWebSocketKey1[i] == ' ')
					iSecWebSocketKey1_Space ++;
				else if (sSecWebSocketKey1[i] >= '0' && sSecWebSocketKey1[i] <= '9')
					sSecWebSocketKey1_Digit += sSecWebSocketKey1[i];
			}
			sSecWebSocketKey1_Digit += '\0';

			//统计sec2的数字和空格
			for (Size_t i=0;i<sSecWebSocketKey2.size();i++)
			{
				if (sSecWebSocketKey2[i] == ' ')
					iSecWebSocketKey2_Space ++;
				else if (sSecWebSocketKey2[i] >= '0' && sSecWebSocketKey1[i] <= '9')
					sSecWebSocketKey2_Digit += sSecWebSocketKey2[i];
			}
			sSecWebSocketKey2_Digit += '\0';

			//计算整数key
			UInt32 iIntKey1 = HawkByteOrder::ByteOrder32(HawkStringUtil::StringToUInt(sSecWebSocketKey1_Digit) / iSecWebSocketKey1_Space);
			UInt32 iIntKey2 = HawkByteOrder::ByteOrder32(HawkStringUtil::StringToUInt(sSecWebSocketKey2_Digit) / iSecWebSocketKey2_Space);

			//组合整数为字节流进行md5hash计算
			UChar szAllKeys[16] = {0};
			memcpy(szAllKeys + 0, &iIntKey1, 4);
			memcpy(szAllKeys + 4, &iIntKey2, 4);
			memcpy(szAllKeys + 8, szSecurityKey, 8);

			//md5计算
			UChar szMd5Hash[16] = {0};
			AString sMd5 = HawkMd5::MakeMD5(szAllKeys, 16, szMd5Hash);

			//构造http响应信息
			Char szRespData[DEFAULT_SIZE] = {0};
			char* szFormat = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"
				"Upgrade: WebSocket\r\n"
				"Connection: Upgrade\r\n"
				"Sec-WebSocket-Origin: %s\r\n" 
				"Sec-WebSocket-Location: ws://%s%s\r\n"
				"Sec-WebSocket-Protocol: %s\r\n\r\n";

			//写入htttp相应头字节流中
			sprintf(szRespData, szFormat, sOrigin.c_str(), sHost.c_str(), sFetch.c_str(), sProtocol.c_str());
			pHttpRep->Append(szRespData, strlen(szRespData));
			pHttpRep->Append(szMd5Hash, 16);
			return true;
		}

		return false;
	}

	//获取Sec-WebSocket-Key
	Bool HawkWebSocket::GetSecWebSocketKey(const OctetsStream* pHttpReq, const AString& sKeyType, AString& sSecKey, UChar cFlag)
	{
		Char* pData  = (Char*)pHttpReq->Begin();
		Size_t iSize = (Size_t)pHttpReq->Size();
		if (iSize <= sKeyType.size())		
			return false;

		for (Size_t i=0;i<iSize-sKeyType.size();i++)
		{
			AString sVal(pData+i, sKeyType.size());
			if (sVal == sKeyType)
			{
				UInt32 iPos = i + sKeyType.size();	

				sSecKey.clear();
				while (pData[iPos] != cFlag)
				{
					sSecKey.push_back(pData[iPos++]);
				}
				return true;
			}
		}
		return false;
	}

	//获取8-byte security key
	Bool HawkWebSocket::Get8ByteSecurityKey(const OctetsStream* pHttpReq, UChar* pSecKey)
	{
		Int32 iTokenSize = 4;
		Int32 iKeySize   = 8;

		Char* pData  = (Char*)pHttpReq->Begin();
		Size_t iSize = (Size_t)pHttpReq->Size();		
		for (Size_t i=0;i<iSize-iTokenSize-iKeySize;i++)
		{
			AString sVal(pData+i, iTokenSize);
			if (sVal == "\r\n\r\n")
			{
				UInt32 iPos = i + iTokenSize;
				for (Int32 i=0;i<iKeySize;i++)
				{
					pSecKey[i] = pData[iPos++];
				}				
				return true;
			}
		}
		return false;
	}
}
