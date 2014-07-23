#ifndef HAWK_MARSHAL_H
#define HAWK_MARSHAL_H

#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* 数据流的序列化和反序列化基类封装                                     */
	/************************************************************************/
	class UTIL_API HawkMarshal : public HawkRefCounter
	{
	public:
		//数据打包(子类须重载)
		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS);

		//数据解包(子类须重载)
		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS) ;

		//清理数据
		virtual Bool Clear();
	};
}
#endif
