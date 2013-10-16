#ifndef HAWK_RAND_H
#define HAWK_RAND_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 随机数生成器                                                         */
	/************************************************************************/
	class UTIL_API HawkRand
	{
	public:
		//生成随机整数
		static Int32 RandInt();

		//生成带上限的随机整数
		static Int32 RandInt(Int32 iMax);

		//生成带上下限的随机整数
		static Int32 RandInt(Int32 iLow, Int32 iHigh);

		//生成随机浮点数
		static Float RandFloat();

		//生成带上限的随机浮点数
		static Float RandFloat(Float fMax);

		//生成带上下限的随机浮点数
		static Float RandFloat(Float fLow, Float fHigh);
	};
}
#endif

