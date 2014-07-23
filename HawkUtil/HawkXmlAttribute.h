#ifndef HAWK_XMLATTRIBUTE_H
#define HAWK_XMLATTRIBUTE_H

#include "HawkStringUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* XML����KV����ֵ����                                                  */
	/************************************************************************/
	template<class StringType> class HawkXmlAttribute : public HawkRefCounter
	{
	public:
		//KV��ֵĬ�Ϲ���
		HawkXmlAttribute()
		{
			Key.clear();
			Value.clear();
		}

		//��ֵ���ݹ���
		HawkXmlAttribute(const StringType& sKey,const StringType& sValue)
		{
			Key   = sKey;
			Value = sValue;
		}

		//��ֵ������
		HawkXmlAttribute<StringType>& operator = (const HawkXmlAttribute<StringType>& sRhs)
		{
			if (this != &sRhs)
			{
				Key   = sRhs.Key;
				Value = sRhs.Value;
			}

			return *this;
		}

	public:
		//����ֵת��Ϊ����
		Int32 IntValue()
		{
			return HawkStringUtil::StringToInt<StringType>(Value);
		}

		//����ֵת��Ϊ������
		Float FloatValue()
		{
			return HawkStringUtil::StringToFloat<StringType>(Value);
		}

		//����ֵת��Ϊ�ַ���
		StringType StringValue()
		{
			return Value;
		}

		//����ֵת��Ϊ����ֵ
		Bool BoolValue()
		{
			return HawkStringUtil::StringToBool<StringType>(Value);
		}

		//XML��ʽ���ַ���
		StringType ToString()
		{
			StringType sTmp;
			sTmp = Key;
			HawkStringUtil::Append<StringType,AString>(sTmp,"=\"");
			sTmp = sTmp + Value;
			HawkStringUtil::Append<StringType,AString>(sTmp,"\"");
			return sTmp;
		}

	public:
		//���Լ�
		StringType Key;
		//����ֵ
		StringType Value;
	};

	//XML���Բ�ͬ����Ķ���
	typedef HawkXmlAttribute<AString>	AXmlAttribute;
	typedef HawkXmlAttribute<UString>	UXmlAttribute;
	typedef HawkXmlAttribute<WString>	WXmlAttribute;
}
#endif
