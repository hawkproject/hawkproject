#ifndef HAWK_MATH_H
#define HAWK_MATH_H

#include "HawkStdHeader.h"
#include "HawkRand.h"

namespace Hawk
{
	/************************************************************************/
	/* 数学库,包含数学运算和转换                                            */
	/************************************************************************/
	class UTIL_API HawkMath
	{
	public:
		//数值常量定义
		static const  Float	 PI;
		static const  Float	 HALF_PI;
		static const  Float	 TWO_PI;
		static const  Float	 RECIPROCAL_PI;
		static const  Float	 DEG2RAD;
		static const  Float	 RAD2DEG;
		static const  Float	 FLOAT_DIFF;

	public:
		//tVal 在tBegin 和 tEnd 之间
		template <class T>
			static Bool Between(T tBegin,T tEnd,T tVal) 
		{
			if ((tBegin <= tVal && tEnd >= tVal) ||
				(tBegin >= tVal && tEnd <= tVal))
				return true;

			return false;
		}

		//插值运算 fVal范围是[0,1]
		template <class T>
			static T Lerp(T tBegin,T tEnd,Float fVal) 
		{
			return tEnd*fVal + tBegin*(1.0f - fVal);
		}

		//绝对值
		template <class T>
			static T Abs(T  tVal)
		{
			return tVal >0 ? tVal : -tVal;
		}

		//倒数运算
		template <class T>
			static Float Reciprocal(T tVal) 
		{
				return 1.0f / (Float)tVal;
		}

		//最大值
		template <class T>
			static T Max(T tVal1,T tVal2)
		{
			return tVal1 > tVal2 ? tVal1 : tVal2;
		}

		//最大值
		template <class T>
			static T Max(T tVal1,T tVal2,T tVal3)
		{
			return (tVal1 > tVal2 ? tVal1 : tVal2) > tVal3 ? (tVal1 > tVal2 ? tVal1 : tVal2) : tVal3;
		}

		//最小值
		template <class T>
			static T Min(T tVal1,T tVal2)
		{
			return tVal1 < tVal2 ? tVal1 : tVal2;
		}

		//最小值
		template <class T>
			static T Min(T tVal1,T tVal2,T tVal3)
		{
			return (tVal1 < tVal2 ? tVal1 : tVal2) < tVal3 ? (tVal1 < tVal2 ? tVal1 : tVal2) : tVal3;
		}

		//在[tLow,tHigh]之间离tValue最小的
		template <class T>
			static T Clamp (const T& tValue, const T& tLow, const T& tHigh)
		{
			return Min (Max(tValue,tLow), tHigh);
		}

		//交换
		template <class T>
			static void Swap(T& tVal1,T& tVal2)
		{
			T tTmp = tVal1;
			tVal1  = tVal2;
			tVal2  = tTmp;
		}

		//打乱排序
		template <class T>
			static Bool RandomOrder(vector<T>& vVal)
		{
			if (vVal.size())
			{
				for (Size_t i=0;i<vVal.size();i++)
				{
					Int32 iIdx	= HawkRand::RandInt(0,vVal.size()-1);
					Swap<T>(vVal[i],vVal[iIdx]);
				}
				return true;
			}
			return false;
		}

		//相等判断
		template <class T>
			static Bool Equal(const T& tVal1 , const T& tVal2)
		{
			return tVal1 == tVal2;
		}
				
		//队列权重随机索引
		template <class T>
			static Int32 RandomVectorIdx(const vector<T>& vWeight)
		{
			HawkAssert(vWeight.size());
			if (vWeight.size())
			{
				vector<T> accu_weight;
				accu_weight.resize(vWeight.size(), 0);

				accu_weight[0] = vWeight[0];		
				for (Size_t i=1;i<vWeight.size();i++)
					accu_weight[i] = vWeight[i] + accu_weight[i-1];

				if (accu_weight.size() && accu_weight[accu_weight.size()-1] != 0)
				{
					Float fRandVal = HawkRand::RandFloat(0.0f, 1.0f);
					for (Size_t i=0;i<accu_weight.size();i++)
					{
						Float fProp = (Float)accu_weight[i] / (Float)accu_weight[accu_weight.size()-1];
						if (fRandVal <= fProp)
							return i;
					}
				}
			}			
			return HAWK_ERROR;
		}
			 
	public:
		//浮点数为0判断
		inline static Bool	IsZero(Float fVal)
		{
			return fabs(fVal) <= FLOAT_DIFF;
		}

		inline static Bool	FloatEqual(Float fV1, Float fV2)
		{
			return IsZero(fV1 - fV2);
		}

		//正弦函数
		inline static Float Sin(Float fRad)
		{
			return sin(fRad);
		}

		//反正弦函数
		inline static Float Asin(Float fVal)
		{
			return asin(fVal);
		}

		//余弦函数
		inline static Float Cos(Float fRad)
		{
			return cos(fRad);
		}

		//反余弦函数
		inline static Float Acos(Float fVal)
		{
			return acos(fVal);
		}

		//正切函数
		inline static Float Tan(Float fRad)
		{
			return tan(fRad);
		}

		//反正切函数
		inline static Float Atan(Float fTan)
		{
			return atan(fTan);
		}

		inline static Float Atan2(Float fX,Float fY)
		{
			return atan2(fX,fY);
		}

		//平方函数
		inline static Float Sqr(Float fVal)
		{
			return fVal * fVal;
		}

		//平方根函数
		inline static Float Sqrt(Float fVal)
		{
			HawkAssert(fVal >= 0);
			return sqrtf(fVal);
		}

		inline static Float Invsqrt(Float fVal)
		{
			HawkAssert(fVal >= 0);
			return 1.0f / sqrtf(fVal);
		}

		//向下取整
		inline static Int32 Floor(Float fVal)
		{
			return (Int32)floor(fVal);
		}

		//向上取整
		inline static Int32 Ceil(Float fVal)
		{
			return (Int32)ceil(fVal);
		}

		//求阶乘函数
		static Int32 Factorial(Int32 iVal)
		{
			if(iVal <= 0)
				return 0;

			Int32 iRet = 1;
			while(iVal > 0)
			{
				iRet *= iVal;
				iVal --;
			}
			return iRet;
		}
	};
}
#endif
