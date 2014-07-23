#ifndef HAWK_BEZIER_H
#define HAWK_BEZIER_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ���屴����ƽ�����߼��㷽ʽ,���в���������2D����Ҳ������3D����        */
	/************************************************************************/
	template <class VectorType> class HawkBezier
	{
	public:
		HawkBezier()
		{
			m_vPoints.clear();
		}

		~HawkBezier()
		{
			m_vPoints.clear();
		}

	public:
		void  AddPoint(const VectorType& vPoint)
		{
			m_vPoints.push_back(vPoint);
		}

		void  AddPoints(VectorType* pPoints,Int32 iSize)
		{
			for (Int32 i=0;pPoints && i<iSize;i++)
			{
				AddPoint(pPoints[i]);
			}
		}

	public:
		//�������������
		VectorType  BezierPoint(Float fVal)
		{
			VectorType vPoint;
			memset(&vPoint,0,sizeof(vPoint));
			Int32 iN = m_vPoints.size()-1;
			Int32 iFactorialN = HawkMath::Factorial(iN);

			for (Int32 i=0;i<=iN;i++)
			{
				Float fBernstein = iFactorialN * pow(fVal,i) * pow(1-fVal,iN-i);
				fBernstein /= HawkMath::Factorial(i);
				fBernstein /= HawkMath::Factorial(iN-i);
				vPoint += m_vPoints[i]*fBernstein;
			}
			return vPoint;
		}	

	protected:
		vector<VectorType> m_vPoints;
	};

	typedef HawkBezier<HawkVector2D> HawkBezier2D;
	typedef HawkBezier<HawkVector3D> HawkBezier3D;
}
#endif
