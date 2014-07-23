#include "HawkTriangle3D.h"
#include "HawkVector3D.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"
#include "HawkSegment3D.h"

namespace Hawk
{
	//Ĭ�Ϲ��캯��
	HawkTriangle3D::HawkTriangle3D(){};

	//���½�����������߹���
	HawkTriangle3D::HawkTriangle3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2)
	{
		Point	= oPt;
		Edge[0] = oEdge1;
		Edge[1] = oEdge2;
	}

	//��������
	HawkTriangle3D::HawkTriangle3D(const HawkTriangle3D& oTrig)
	{
		Point	= oTrig.Point;
		Edge[0] = oTrig.Edge[0];
		Edge[1] = oTrig.Edge[1];
	}

	//����������
	Bool HawkTriangle3D::operator == (const HawkTriangle3D& oTrig) const
	{
		return  Point == oTrig.Point	&& 
				Edge[0]==oTrig.Edge[0]	&& 
				Edge[1]==oTrig.Edge[1];
	}

	Bool HawkTriangle3D::operator != (const HawkTriangle3D& oTrig) const
	{
		return !(*this == oTrig);
	}

	HawkTriangle3D HawkTriangle3D::operator + (const HawkVector3D& oVec) const
	{
		return HawkTriangle3D(Point+oVec,Edge[0],Edge[1]);
	}

	HawkTriangle3D& HawkTriangle3D::operator += (const HawkVector3D& oVec)
	{
		Point += oVec;
		return *this;
	}

	HawkTriangle3D HawkTriangle3D::operator - (const HawkVector3D& oVec) const
	{
		return HawkTriangle3D(Point-oVec,Edge[0],Edge[1]);
	}

	HawkTriangle3D HawkTriangle3D::operator -= (const HawkVector3D& oVec)
	{
		Point -= oVec;
		return *this;
	}

	HawkVector3D HawkTriangle3D::operator [](Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=2);

		if (iIdx == 0) 
			return Point;
		else
			return Point + Edge[iIdx - 1];
	}
	
	HawkVector3D HawkTriangle3D::GetPoint(Int32 iIdx) const
	{
		return (*this)[iIdx];
	}

	//�������������
	Float HawkTriangle3D::GetArea() const
	{
		return HawkMath::Abs(Edge[0].DotProduct(Edge[1]) * 0.5f);
	}

	//���ö���
	void HawkTriangle3D::SetOriginPoint(const HawkVector3D& oPt)
	{
		Point = oPt;
	}

	const HawkVector3D& HawkTriangle3D::GetOriginPoint() const
	{
		return Point;
	}

	//���ñ�
	void HawkTriangle3D::SetEdge(Int32 iIdx,const HawkVector3D& oEdge)
	{
		A_Exception(iIdx>=0 && iIdx<=1);

		Edge[iIdx] = oEdge[iIdx];
	}

	//��ñ�
	const HawkVector3D& HawkTriangle3D::GetEdge(Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=1);

		return Edge[iIdx];
	}

	//���������η���(��������ϵ,Ϊ������)
	HawkVector3D HawkTriangle3D::GetNormal() const
	{
		return Edge[0].CrossProduct(Edge[1]).Normalize();
	}

	Bool HawkTriangle3D::IsFrontFacing(const HawkVector3D& vVec3) const
	{
		HawkVector3D vNormal = GetNormal();
		Float fDis = vNormal.DotProduct(vVec3);
		return fDis >= 0.0f;
	}

	//������ֱ��ͬһ��(vP1,vP2��A,Bͬһ��)
	Bool HawkTriangle3D::IsOnSameSide(const HawkVector3D& vP1,const HawkVector3D& vP2,const HawkVector3D& vA, const HawkVector3D& vB)
	{
		HawkVector3D bminusa = vB - vA;
		HawkVector3D cp1 = bminusa.CrossProduct(vP1 - vA);
		HawkVector3D cp2 = bminusa.CrossProduct(vP2 - vA);
		return (cp1.DotProduct(cp2) >= 0.0f);
	}  
	
	//�����������ڲ�
	Bool HawkTriangle3D::IsPointInside(const HawkVector3D& oVec3) const
	{
		return (IsOnSameSide(oVec3, GetPoint(0), GetPoint(1), GetPoint(2)) &&
			IsOnSameSide(oVec3, GetPoint(1), GetPoint(0), GetPoint(2)) &&
			IsOnSameSide(oVec3, GetPoint(2), GetPoint(0), GetPoint(1)));
	}

	//���������ε���������
	void  HawkTriangle3D::SetByPoint(const HawkVector3D& vPt1,const HawkVector3D& vPt2,const HawkVector3D& vPt3)
	{
		Point   = vPt1;
		Edge[0] = vPt2 - vPt1;
		Edge[1] = vPt3 - vPt1;
	}	

	HawkVector3D HawkTriangle3D::GetCenterPoint() const
	{
		return (GetPoint(0) + GetPoint(1) + GetPoint(2)) / 3.0f;
	}

	HawkSegment3D HawkTriangle3D::GetSegment(Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=2);
		if (iIdx == 0)
		{
			return HawkSegment3D(GetPoint(0),GetPoint(1));
		}
		else if (iIdx == 1)
		{
			return HawkSegment3D(GetPoint(1),GetPoint(2));
		}
		return HawkSegment3D(GetPoint(2),GetPoint(0));
	}

	void HawkTriangle3D::GetProjection (const HawkVector3D& vAxis,Float& fMin, Float& fMax) const
	{
		Float dot[3] =
		{
			vAxis.DotProduct(GetPoint(0)),
			vAxis.DotProduct(GetPoint(1)),
			vAxis.DotProduct(GetPoint(2))
		};

		fMin = dot[0];
		fMax = fMin;

		if (dot[1] < fMin)
		{
			fMin = dot[1];
		}
		else if (dot[1] > fMax)
		{
			fMax = dot[1];
		}

		if (dot[2] < fMin)
		{
			fMin = dot[2];
		}
		else if (dot[2] > fMax)
		{
			fMax = dot[2];
		}
	}
}

