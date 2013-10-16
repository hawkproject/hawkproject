#ifndef HAWK_MARSHALDATA_H
#define HAWK_MARSHALDATA_H

#include "HawkMarshal.h"

namespace Hawk
{
	/************************************************************************/
	/* 可序列化数据对象,主要用在Protocol中的数据结构                        */
	/************************************************************************/
	class UTIL_API HawkMarshalData : public HawkMarshal
	{
	public:
		//可序列化对象克隆函数
		virtual HawkMarshalData* Clone() const;

		//可序列化对象赋值函数
		virtual HawkMarshalData& operator = (const HawkMarshalData& rhsMD);
	};

	//简化类型定义
	typedef HawkMarshalData MarshalData;
}
#endif
