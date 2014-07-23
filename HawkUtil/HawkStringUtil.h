#ifndef HAWK_STRINGUTIL_H
#define HAWK_STRINGUTIL_H

#include "HawkMath.h"

namespace Hawk
{
	/************************************************************************/
	/* �ַ�������������,�����ַ����ĸ�������                              */
	/************************************************************************/
	class UTIL_API HawkStringUtil
	{
	public:
		//�ַ����滻
		template <class StringType> static StringType& Replace(StringType& sVal, const StringType& sSrc, const StringType& sDst)
		{
			StringType sTmp;
			typename StringType::size_type  iBegin = 0;
			typename StringType::size_type  iEnd   = sVal.find(sSrc, iBegin);
			while (iEnd >= iBegin && iEnd != StringType::npos)
			{
				if (iEnd > iBegin)
				{
					sTmp += sVal.substr(iBegin, iEnd-iBegin);
				}
				sTmp += sDst;

				iBegin = iEnd + sSrc.size();
				iEnd   = sVal.find(sSrc,iBegin);
			}

			if (sVal.size() >= iBegin + 1)
			{
				sTmp += sVal.substr(iBegin,sVal.size()-iBegin);
			}
			sVal = sTmp;
			return sVal;
		}

		//ȥ���ַ�����ո�&�س�&����
		template <class StringType> static StringType& TrimLeft(StringType& sVal)
		{
			typename StringType::size_type iPos  = 0;
			typename StringType::size_type iSize = sVal.size();
			for (; iPos<iSize; iPos++)
			{
				if (sVal[iPos] != ' ' && sVal[iPos] != '\r' && sVal[iPos] != '\n' && sVal[iPos] != '\t')
				{
					sVal = sVal.substr(iPos,sVal.size()-iPos);
					break;
				}
			}
			return sVal;
		}

		//ȥ���ַ����ҿո�&�س�&����
		template <class StringType> static StringType& TrimRight(StringType& sVal)
		{
			typename StringType::size_type iSize = sVal.size();
			typename StringType::size_type iPos  = iSize-1;
			for (; iPos!=StringType::npos; iPos--)
			{
				if (sVal[iPos] != ' ' && sVal[iPos] != '\r' && sVal[iPos] != '\n' && sVal[iPos] != '\t')
				{
					sVal = sVal.substr(0,iPos+1);
					break;
				}
			}
			return sVal;
		}

		//ȥ���ַ������ҿո�&�س�&����
		template <class StringType> static StringType& Trim(StringType& sVal)
		{
			TrimLeft(sVal);
			TrimRight(sVal);
			return sVal;
		}

		//����Сд�ַ����д
		template <class StringType> static StringType& UpCase(StringType& sVal)
		{
			typename StringType::size_type iSize = sVal.size();
			for (typename StringType::size_type i=0; i<iSize; i++)
			{
				typename StringType::value_type cVal = sVal.at(i);
				if (cVal >= 'a' && cVal <= 'z')
				{
					sVal[i] = cVal - 32;
				}
			}
			return sVal;
		}

		//���д�д�ַ���Сд
		template <class StringType> static StringType& LowerCase(StringType& sVal)
		{
			typename StringType::size_type iSize = sVal.size();
			for (typename StringType::size_type i=0; i<iSize; i++)
			{
				typename StringType::value_type cVal = sVal.at(i);
				if (cVal >= 'A' && cVal <= 'Z')
				{
					sVal[i] = cVal + 32;
				}
			}
			return sVal;
		}

		//�Ƚϲ����ִ�Сд
		template <class StringType> static Bool CompareNoCase(const StringType& sVal1, const StringType& sVal2)
		{
			if (sVal1 == sVal2) 
				return true;

			StringType s1 = sVal1;
			StringType s2 = sVal2;

			return LowerCase(s1) == LowerCase(s2);
		}

		//�ַ����ָ�
		template <class StringType> static Int32 Split(const StringType& sVal, vector<StringType>& vVal, const StringType& sSplit)
		{
			vVal.clear();
			if (sVal.size() && sSplit.size())
			{
				typename StringType::size_type iBegin = 0;
				typename StringType::size_type iEnd   = sVal.find(sSplit,iBegin);
				while (iEnd >= iBegin && iEnd != StringType::npos)
				{
					StringType sTmp = sVal.substr(iBegin,iEnd-iBegin);
					if(sTmp.size())
					{
						vVal.push_back(sTmp);
					}
					iBegin = iEnd + sSplit.size();
					iEnd   = sVal.find(sSplit,iBegin);
				}

				if (iBegin < sVal.size())
				{
					StringType sTmp = sVal.substr(iBegin,sVal.size()-iBegin);
					if(sTmp.size())
					{
						vVal.push_back(sTmp);
					}
				}
			}
			return (Int32)vVal.size();
		}

		//URL�����ָ�
		template <class StringType> static Int32 SplitUrlField(const StringType& sVal, map<StringType, StringType>& mVal)
		{
			vector<StringType> sv;
			Split<StringType>(sVal, sv, StringType(1, '&'));
			for (Size_t i=0;i<sv.size(); i++)
			{
				vector<StringType> kv;
				if (Split<StringType>(sv[i], kv, StringType(1, '=')) == 2 &&
					kv[0].size() && kv[1].size())
				{
					mVal[ kv[0] ] = kv[1];
				}
			}
			return (Int32)mVal.size();
		}

		//�ж��ض�ֵ�Ƿ������ݱ���
		template <class ValueType> static Bool ValueInTable(const ValueType& tVal, const vector<ValueType>& vVal)
		{
			for (Size_t i=0; i<vVal.size(); i++)
			{
				if(tVal == vVal[i])
					return true;
			}
			return false;
		}

		//���ַ�����ͷ
		template <class StringType> static Bool BeginWith(const StringType& sVal,const StringType& sBegin)
		{
			typename StringType::size_type iPos = sVal.find(sBegin);
			if (iPos == 0)
				return true;

			return false;
		}

		//���ַ�����β
		template <class StringType> static Bool EndWith(const StringType& sVal, const StringType& sEnd)
		{
			typename StringType::size_type iPos = sVal.find(sEnd);
			if(iPos + sEnd.size() == sVal.size())
				return true;
		
			return false;
		}

		//�ַ���ת��Ϊ����
		template <class StringType> static Int32 StringToInt(const StringType& sVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			while (iIdx < MINI_BUF_SIZE && iIdx < sVal.size())
			{
				cVal[iIdx] = (Char)sVal[iIdx];
				iIdx++;
			}
			return (Int32)atoi(cVal);
		};

		//����ת��Ϊ�ַ���
		template <class StringType> static StringType IntToString(Int32 iVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			sprintf(cVal,"%d",iVal);
			StringType sVal;
			while (cVal[iIdx])
			{
				sVal.append(1,cVal[iIdx]);
				iIdx++;
			}
			return sVal.c_str();
		};

		//�ַ���ת�����޷�������
		template <class StringType> static UInt32 StringToUInt(const StringType& sVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			while (iIdx < MINI_BUF_SIZE && iIdx < sVal.size())
			{
				cVal[iIdx] = (Char)sVal[iIdx];
				iIdx++;
			}
			return (UInt32)_atoi64(cVal);
		};

		//�޷�������ת�����ַ���
		template <class StringType> static StringType UIntToString(UInt32 iVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			sprintf(cVal,"%u",iVal);
			StringType sVal;
			while (cVal[iIdx])
			{
				sVal.append(1,cVal[iIdx]);
				iIdx++;
			}
			return sVal.c_str();
		};

		//�ַ���ת����64λ����
		template <class StringType> static Int64 StringToInt64(const StringType& sVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			while (iIdx < MINI_BUF_SIZE && iIdx < sVal.size())
			{
				cVal[iIdx] = (Char)sVal[iIdx];
				iIdx++;
			}
			return (Int64)_atoi64(cVal);
		};

		//64λ����ת�����ַ���
		template <class StringType> static StringType Int64ToString(Int64 iVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			sprintf(cVal,"%lld",iVal);
			StringType sVal;
			while (cVal[iIdx])
			{
				sVal.append(1,cVal[iIdx]);
				iIdx++;
			}
			return sVal.c_str();
		};

		//�ַ���ת��Ϊ������
		template <class StringType> static  Float StringToFloat(const StringType& sVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			while (iIdx < MINI_BUF_SIZE && iIdx < sVal.size())
			{
				cVal[iIdx] = (Char)sVal[iIdx];
				iIdx++;
			}
			return (Float)(atof(cVal));
		};

		//������ת��Ϊ�ַ���
		template <class StringType> static StringType FloatToString(Float fVal)
		{
			Char cVal[MINI_BUF_SIZE] = {0};
			sprintf(cVal,"%f",fVal);
			Int32 iIdx = 0;
			StringType sVal;
			while (cVal[iIdx])
			{
				sVal.append(1,cVal[iIdx]);
				iIdx++;
			}
			return sVal.c_str();
		};

		//�ַ���ת��Ϊ˫���ȸ�����
		template <class StringType> static  Double StringToDouble(const StringType& sVal)
		{
			Char cVal[MINI_BUF_SIZE]   = {0};
			typename StringType::size_type iIdx = 0;
			while (iIdx < MINI_BUF_SIZE && iIdx < sVal.size())
			{
				cVal[iIdx] = (Char)sVal[iIdx];
				iIdx++;
			}
			return atof(cVal);
		};

		//˫���ȸ�����ת��Ϊ�ַ���
		template <class StringType> static StringType DoubleToString(Double fVal)
		{
			Char cVal[MINI_BUF_SIZE] = {0};
			sprintf(cVal,"%lf",fVal);
			Int32 iIdx = 0;
			StringType sVal;
			while (cVal[iIdx])
			{
				sVal.append(1,cVal[iIdx]);
				iIdx++;
			}
			return sVal.c_str();
		};

		//�ַ���ת��Ϊ����ֵ
		template <class StringType> static Bool StringToBool(const StringType& sVal)
		{
			StringType sTmp = sVal;
			LowerCase(sTmp);

			if (sTmp.size()>=4)
			{
				if (sTmp[0] == 't' && sTmp[1] == 'r' && sTmp[2] == 'u' && sTmp[3] == 'e')
				{
					return true;
				}
			}
			return false;
		};

		//����ֵת��Ϊ�ַ���
		template <class StringType> static StringType BoolToString(Bool bVal)
		{
			StringType sVal;
			if(bVal)
			{
				sVal.append(1,'t');
				sVal.append(1,'r');
				sVal.append(1,'u');
				sVal.append(1,'e');
			}
			else
			{
				sVal.append(1,'f');
				sVal.append(1,'a');
				sVal.append(1,'l');
				sVal.append(1,'s');
				sVal.append(1,'e');
			}
			return sVal.c_str();
		}
	
		//���ֱ����ʽ�ַ�����ֵ 
		template <class ST1,class ST2> static void Append(ST1& sSrcVal, const ST2& sSubVal)
		{
			for (Size_t i=0; i<sSubVal.size(); i++)
			{
				sSrcVal.append(1,sSubVal.at(i));
			}
		}

		//�ַ���Hash����
		template <class StringType> static UInt32 HashString(const StringType& sVal)
		{
			Size_t iSize = sVal.size() * sizeof(typename StringType::value_type);
			UInt32 iHash = CalcHash((const Char*)sVal.c_str(), iSize);
			return iHash;
		}

		//����ַ���
		template <class StringType> static StringType RandomString(Int32 iSize)
		{
			StringType sVal;
			sVal.reserve(iSize);
			for (Int32 i=0; i<iSize; i++)
			{
				if (HawkRand::RandInt(0,1) == 0)
				{
					sVal.push_back(HawkRand::RandInt('a','z'));
				}
				else
				{
					sVal.push_back(HawkRand::RandInt('A','Z'));
				}				
			}
			return sVal.c_str();
		}

		//�Ƿ�ΪӢ���ַ�
		template <class CharType> static Bool IsChar(CharType cVal)
		{
			return (cVal >= 'a' && cVal <= 'z') || (cVal >= 'A' && cVal <= 'Z');
		}

		//�Ƿ�Ϊ�����ַ�
		template <class CharType> static  Bool  IsNumeric(CharType cVal)
		{
			return cVal >= '0' && cVal <= '9';
		}

	public:
		//�ַ���Hash�㷨
		static UInt32  CalcHash(const Char * pData, Size_t iSize);

		//ת��ΪUTF8����
		static AString CodePointToUTF8(UInt32 iCp);

		//URLת��
		static AString UrlEncode(const AString& sUrl);

		//�ַ���ʮ�������ַ���
		static AString Char2Hex(Char cCh); 

	public:
		//asciiתunicode����
		static UInt32  AsciiToUnicode(const Char* pSrc,UInt32 iSrcLen,WChar* pDst,UInt32 iDstLen);

		//unicodeתascii����
		static UInt32  UnicodeToAscii(const WChar* pSrc,UInt32 iSrcLen,Char* pDst, UInt32 iDstLen);

		//utf8תunicode����
		static UInt32  Utf8ToUnicode(const Utf8* pSrc,UInt32 iSrcLen,WChar* pDst,UInt32 iDstLen);

		//unicodeתutf8����
		static UInt32  UnicodeToUtf8(const WChar* pSrc,UInt32 iSrcLen,Utf8* pDst,UInt32 iDstLen);

		//asciiתutf8����
		static UInt32  AsciiToUtf8(const Char* pSrc,UInt32 iSrcLen,Utf8* pDst,UInt32 iDstLen);

		//utf8תascii����
		static UInt32  Utf8ToAscii(const Utf8* pSrc,UInt32 iSrcLen,Char* pDst,UInt32 iDstLen);		

	public:
		//wstring�ַ���תstring 
		static UString ToUtf8(const WString& sVal);

		//ustring�ַ���תstring 
		static UString ToUtf8(const AString& sVal);

		//wstring�ַ���תstring 
		static AString ToString(const WString& sVal);

		//ustring�ַ���תstring 
		static AString ToString(const UString& sVal);

		//string�ַ���תwstring
		static WString ToWString(const AString& sVal);

		//ustring�ַ���תwstring
		static WString ToWString(const UString& sVal);
	};

	/************************************************************************/
	/* �ַ�����ת��                                                         */
	/************************************************************************/
	//ת��ΪUtf8����
	UTIL_API UString  _Utf8(const Char* sVal);
	UTIL_API UString  _Utf8(const WChar* sVal);
	UTIL_API UString  _Utf8(const AString& sVal);
	UTIL_API UString  _Utf8(const WString& sVal);

	//ת��ΪAscii����
	UTIL_API AString  _Ascii(const UChar* sVal);
	UTIL_API AString  _Ascii(const WChar* sVal);
	UTIL_API AString  _Ascii(const UString& sVal);
	UTIL_API AString  _Ascii(const WString& sVal);	

	//ת��ΪUnicode����
	UTIL_API WString  _Unicode(const Char* sVal);
	UTIL_API WString  _Unicode(const UChar* sVal);
	UTIL_API WString  _Unicode(const AString& sVal);
	UTIL_API WString  _Unicode(const UString& sVal);
}
#endif
