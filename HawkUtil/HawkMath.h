#ifndef HAWK_MATH_H
#define HAWK_MATH_H

#include "HawkStdHeader.h"
#include "HawkRand.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ѧ��,������ѧ�����ת��                                            */
	/************************************************************************/
	class UTIL_API HawkMath
	{
	public:
		//��ֵ��������
		static const  Float	 PI;
		static const  Float	 HALF_PI;
		static const  Float	 TWO_PI;
		static const  Float	 RECIPROCAL_PI;
		static const  Float	 DEG2RAD;
		static const  Float	 RAD2DEG;
		static const  Float	 FLOAT_DIFF;

	public:
		//tVal ��tBegin �� tEnd ֮��
		template <class T>
			static Bool Between(T tBegin,T tEnd,T tVal) 
		{
			if ((tBegin <= tVal && tEnd >= tVal) ||
				(tBegin >= tVal && tEnd <= tVal))
				return true;

			return false;
		}

		//��ֵ���� fVal��Χ��[0,1]
		template <class T>
			static T Lerp(T tBegin,T tEnd,Float fVal) 
		{
			return tEnd*fVal + tBegin*(1.0f - fVal);
		}

		//����ֵ
		template <class T>
			static T Abs(T  tVal)
		{
			return tVal >0 ? tVal : -tVal;
		}

		//��������
		template <class T>
			static Float Reciprocal(T tVal) 
		{
				return 1.0f / (Float)tVal;
		}

		//���ֵ
		template <class T>
			static T Max(T tVal1,T tVal2)
		{
			return tVal1 > tVal2 ? tVal1 : tVal2;
		}

		//���ֵ
		template <class T>
			static T Max(T tVal1,T tVal2,T tVal3)
		{
			return (tVal1 > tVal2 ? tVal1 : tVal2) > tVal3 ? (tVal1 > tVal2 ? tVal1 : tVal2) : tVal3;
		}

		//��Сֵ
		template <class T>
			static T Min(T tVal1,T tVal2)
		{
			return tVal1 < tVal2 ? tVal1 : tVal2;
		}

		//��Сֵ
		template <class T>
			static T Min(T tVal1,T tVal2,T tVal3)
		{
			return (tVal1 < tVal2 ? tVal1 : tVal2) < tVal3 ? (tVal1 < tVal2 ? tVal1 : tVal2) : tVal3;
		}

		//��[tLow,tHigh]֮����tValue��С��
		template <class T>
			static T Clamp (const T& tValue, const T& tLow, const T& tHigh)
		{
			return Min (Max(tValue,tLow), tHigh);
		}

		//����
		template <class T>
			static void Swap(T& tVal1,T& tVal2)
		{
			T tTmp = tVal1;
			tVal1  = tVal2;
			tVal2  = tTmp;
		}

		//��������
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

		//����ж�
		template <class T>
			static Bool Equal(const T& tVal1 , const T& tVal2)
		{
			return tVal1 == tVal2;
		}
				
		//����Ȩ���������
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
		//������Ϊ0�ж�
		inline static Bool	IsZero(Float fVal)
		{
			return fabs(fVal) <= FLOAT_DIFF;
		}

		inline static Bool	FloatEqual(Float fV1, Float fV2)
		{
			return IsZero(fV1 - fV2);
		}

		//���Һ���
		inline static Float Sin(Float fRad)
		{
			return sin(fRad);
		}

		//�����Һ���
		inline static Float Asin(Float fVal)
		{
			return asin(fVal);
		}

		//���Һ���
		inline static Float Cos(Float fRad)
		{
			return cos(fRad);
		}

		//�����Һ���
		inline static Float Acos(Float fVal)
		{
			return acos(fVal);
		}

		//���к���
		inline static Float Tan(Float fRad)
		{
			return tan(fRad);
		}

		//�����к���
		inline static Float Atan(Float fTan)
		{
			return atan(fTan);
		}

		inline static Float Atan2(Float fX,Float fY)
		{
			return atan2(fX,fY);
		}

		//ƽ������
		inline static Float Sqr(Float fVal)
		{
			return fVal * fVal;
		}

		//ƽ��������
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

		//����ȡ��
		inline static Int32 Floor(Float fVal)
		{
			return (Int32)floor(fVal);
		}

		//����ȡ��
		inline static Int32 Ceil(Float fVal)
		{
			return (Int32)ceil(fVal);
		}

		//��׳˺���
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
