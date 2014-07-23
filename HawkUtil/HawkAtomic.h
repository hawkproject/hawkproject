#ifndef HAWK_ATOMIC_H
#define HAWK_ATOMIC_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 值类型原子操作类封装(支持Int32 & UInt32)	    	                    */
	/************************************************************************/
	class UTIL_API HawkAtomic
	{
	public:
#ifdef PLATFORM_WINDOWS
		//加1操作,返回加1后的值
		template<class IntType> static IntType Inc(IntType volatile* pVal)
		{
			return InterlockedIncrement(pVal);
		}

		//减1操作,返回减1后的值
		template<class IntType> static IntType Dec(IntType volatile* pVal)
		{
			return InterlockedDecrement(pVal);
		}

		//加数操作,返回操作前的值
		template<class IntType> static IntType Add(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchangeAdd(pVal, iVal);
		}

		//减数操作,返回操作前的值
		template<class IntType> static IntType Sub(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchangeAdd(pVal, -iVal);
		}

		//位与操作,返回操作前的值
		template<class IntType> static IntType BitAnd(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedAnd(pVal, iVal);
		}

		//位或操作,返回操作前的值
		template<class IntType> static IntType BitOr(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedOr(pVal, iVal);
		}

		//位异或操作,返回操作前的值
		template<class IntType> static IntType BitXor(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedXor(pVal, iVal);
		}

		//设置操作,返回操作前的值
		template<class IntType> static IntType Exchange(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchange(pVal, iVal);
		}

		//比较相等并设置操作,返回操作前的值
		template<class IntType> static IntType CompareExchange(IntType volatile * pVal, IntType iVal, IntType iCmp)
		{
			return InterlockedCompareExchange(pVal, iVal, iCmp);
		}
#else
		//加1操作,返回加1后的值
		template<class IntType> static IntType Inc(IntType volatile* pVal)
		{
			return __sync_add_and_fetch(pVal, 1);
		}

		//减1操作,返回减1后的值
		template<class IntType> static IntType Dec(IntType volatile* pVal)
		{
			return __sync_sub_and_fetch(pVal, 1);
		}

		//加数操作,返回加完之后的值
		template<class IntType> static IntType Add(IntType volatile* pVal, IntType iVal)
		{
			return __sync_add_and_fetch(pVal, iVal);
		}

		//减数操作,返回减完之后的值
		template<class IntType> static IntType Sub(IntType volatile* pVal, IntType iVal)
		{
			return __sync_sub_and_fetch(pVal, iVal);
		}

		//位与操作,返回操作前的值
		template<class IntType> static IntType BitAnd(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_and(pVal, iVal);
		}

		//位或操作,返回操作前的值
		template<class IntType> static IntType BitOr(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_or(pVal, iVal);
		}

		//位异或操作,返回操作前的值
		template<class IntType> static IntType BitXor(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_xor(pVal, iVal);
		}

		//设置操作,返回操作前的值
		template<class IntType> static IntType Exchange(IntType volatile* pVal, IntType iVal)
		{
			return __sync_lock_test_and_set(pVal, iVal);
		}

		//比较相等并设置操作,返回操作前的值
		template<class IntType> static IntType CompareExchange(IntType volatile * pVal, IntType iVal, IntType iCmp)
		{
			return __sync_val_compare_and_swap(pVal, iCmp, iVal);
		}
#endif
	};
}
#endif
