#ifndef HAWK_XMLATTRIBUTE_H
#define HAWK_XMLATTRIBUTE_H

#include "HawkStringUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* XML属性KV类型值定义                                                  */
	/************************************************************************/
	template<class StringType> class HawkXmlAttribute : public HawkRefCounter
	{
	public:
		//KV键值默认构造
		HawkXmlAttribute()
		{
			Key.clear();
			Value.clear();
		}

		//键值数据构造
		HawkXmlAttribute(const StringType& sKey,const StringType& sValue)
		{
			Key   = sKey;
			Value = sValue;
		}

		//赋值操作符
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
		//属性值转换为整数
		Int32 IntValue()
		{
			return HawkStringUtil::StringToInt<StringType>(Value);
		}

		//属性值转换为浮点数
		Float FloatValue()
		{
			return HawkStringUtil::StringToFloat<StringType>(Value);
		}

		//属性值转换为字符串
		StringType StringValue()
		{
			return Value;
		}

		//属性值转换为布尔值
		Bool BoolValue()
		{
			return HawkStringUtil::StringToBool<StringType>(Value);
		}

		//XML格式化字符串
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
		//属性键
		StringType Key;
		//属性值
		StringType Value;
	};

	//XML属性不同编码的定义
	typedef HawkXmlAttribute<AString>	AXmlAttribute;
	typedef HawkXmlAttribute<UString>	UXmlAttribute;
	typedef HawkXmlAttribute<WString>	WXmlAttribute;
}
#endif
