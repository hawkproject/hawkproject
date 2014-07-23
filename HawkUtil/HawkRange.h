#ifndef HAWK_RANGE_H
#define HAWK_RANGE_H

#include "HawkMath.h"

namespace Hawk
{
	/************************************************************************/
	/* 区间表示类						                                    */
	/************************************************************************/
	template <class T> class HawkRange
	{
		//属性定义
		DEFINE_PROPERTY(T, Min, Min);
		DEFINE_PROPERTY(T, Max, Max);

		//开闭区间定义
		DEFINE_PROPERTY(Bool, MinClose, IsMinClose);
		DEFINE_PROPERTY(Bool, MaxClose, IsMaxClose);

	public:
		inline HawkRange(T tMin = 0, T tMax = 0, Bool bMinClose = true, Bool bMaxClose = true) : Min(tMin), Max(tMax), MinClose(bMinClose), MaxClose(bMaxClose) 
		{
		};

		inline void SetRange(T tMin = 0,T tMax = 0)
		{
			Min	= tMin;
			Max	= tMax;
		}

		inline HawkRange<T>& operator = (const HawkRange& oRange)
		{
			if (this != &oRange)
			{
				Min	= oRange.Min;
				Max	= oRange.Max;

				MinClose = oRange.MinClose;
				MaxClose = oRange.MaxClose;
			}
			return *this;
		}

		inline Bool operator == (const HawkRange& oRange) const
		{
			return (Min == oRange.Min && Max == oRange.Max &&
				     MinClose == oRange.MinClose && MaxClose == oRange.MaxClose);
		}

		inline Bool operator != ( const HawkRange& oRange ) const
		{
			return (Min != oRange.Min || Max != oRange.Max ||
				MinClose != oRange.MinClose || MaxClose != oRange.MaxClose);
		}

		inline Bool IsInRange(T tVal) const
		{
			if(tVal > Min && tVal < Max)
				return true;

			if (MinClose && tVal == Min)
				return true;

			if (MaxClose && tVal == Max)
				return true;

			return false;
		}
	};

	//简易类型定义
	typedef	 HawkRange<Int32>  IntRange;
	typedef	 HawkRange<Float>  FloatRange;
}
#endif

