#ifndef HAWK_ATOMIC_H
#define HAWK_ATOMIC_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ֵ����ԭ�Ӳ������װ(֧��Int32 & UInt32)	    	                    */
	/************************************************************************/
	class UTIL_API HawkAtomic
	{
	public:
#ifdef PLATFORM_WINDOWS
		//��1����,���ؼ�1���ֵ
		template<class IntType> static IntType Inc(IntType volatile* pVal)
		{
			return InterlockedIncrement(pVal);
		}

		//��1����,���ؼ�1���ֵ
		template<class IntType> static IntType Dec(IntType volatile* pVal)
		{
			return InterlockedDecrement(pVal);
		}

		//��������,���ز���ǰ��ֵ
		template<class IntType> static IntType Add(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchangeAdd(pVal, iVal);
		}

		//��������,���ز���ǰ��ֵ
		template<class IntType> static IntType Sub(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchangeAdd(pVal, -iVal);
		}

		//λ�����,���ز���ǰ��ֵ
		template<class IntType> static IntType BitAnd(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedAnd(pVal, iVal);
		}

		//λ�����,���ز���ǰ��ֵ
		template<class IntType> static IntType BitOr(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedOr(pVal, iVal);
		}

		//λ������,���ز���ǰ��ֵ
		template<class IntType> static IntType BitXor(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedXor(pVal, iVal);
		}

		//���ò���,���ز���ǰ��ֵ
		template<class IntType> static IntType Exchange(IntType volatile* pVal, IntType iVal)
		{
			return InterlockedExchange(pVal, iVal);
		}

		//�Ƚ���Ȳ����ò���,���ز���ǰ��ֵ
		template<class IntType> static IntType CompareExchange(IntType volatile * pVal, IntType iVal, IntType iCmp)
		{
			return InterlockedCompareExchange(pVal, iVal, iCmp);
		}
#else
		//��1����,���ؼ�1���ֵ
		template<class IntType> static IntType Inc(IntType volatile* pVal)
		{
			return __sync_add_and_fetch(pVal, 1);
		}

		//��1����,���ؼ�1���ֵ
		template<class IntType> static IntType Dec(IntType volatile* pVal)
		{
			return __sync_sub_and_fetch(pVal, 1);
		}

		//��������,���ؼ���֮���ֵ
		template<class IntType> static IntType Add(IntType volatile* pVal, IntType iVal)
		{
			return __sync_add_and_fetch(pVal, iVal);
		}

		//��������,���ؼ���֮���ֵ
		template<class IntType> static IntType Sub(IntType volatile* pVal, IntType iVal)
		{
			return __sync_sub_and_fetch(pVal, iVal);
		}

		//λ�����,���ز���ǰ��ֵ
		template<class IntType> static IntType BitAnd(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_and(pVal, iVal);
		}

		//λ�����,���ز���ǰ��ֵ
		template<class IntType> static IntType BitOr(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_or(pVal, iVal);
		}

		//λ������,���ز���ǰ��ֵ
		template<class IntType> static IntType BitXor(IntType volatile* pVal, IntType iVal)
		{
			return __sync_fetch_and_xor(pVal, iVal);
		}

		//���ò���,���ز���ǰ��ֵ
		template<class IntType> static IntType Exchange(IntType volatile* pVal, IntType iVal)
		{
			return __sync_lock_test_and_set(pVal, iVal);
		}

		//�Ƚ���Ȳ����ò���,���ز���ǰ��ֵ
		template<class IntType> static IntType CompareExchange(IntType volatile * pVal, IntType iVal, IntType iCmp)
		{
			return __sync_val_compare_and_swap(pVal, iCmp, iVal);
		}
#endif
	};
}
#endif
