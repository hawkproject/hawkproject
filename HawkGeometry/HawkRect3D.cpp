#include "HawkRect3D.h"
#include "HawkVector3D.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"
#include "HawkSegment3D.h"
#include "HawkTriangle3D.h"

namespace Hawk
{
	//Ĭ�Ϲ��캯��
	HawkRect3D::HawkRect3D(){};

	//���½�����������߹���
	HawkRect3D::HawkRect3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2)
	{
		Point	= oPt;
		Edge[0] = oEdge1;
		Edge[1] = oEdge2;
	}


	//��������
	HawkRect3D::HawkRect3D(const HawkRect3D& oRc)
	{
		Point	= oRc.Point;
		Edge[0] = oRc.Edge[0];
		Edge[1] = oRc.Edge[1];
	}

	//����������
	Bool HawkRect3D::operator == (const HawkRect3D& oRc) const
	{
		return  Point == oRc.Point	&& 
			Edge[0]==oRc.Edge[0]	&& 
			Edge[1]==oRc.Edge[1];
	}

	Bool HawkRect3D::operator != (const HawkRect3D& oRc) const
	{
		return !(*this == oRc);
	}

	HawkRect3D HawkRect3D::operator + (const HawkVector3D& oVec) const
	{
		return HawkRect3D(Point+oVec,Edge[0],Edge[1]);
	}

	HawkRect3D& HawkRect3D::operator += (const HawkVector3D& oVec)
	{
		Point += oVec;
		return *this;
	}

	HawkRect3D HawkRect3D::operator - (const HawkVector3D& oVec) const
	{
		return HawkRect3D(Point-oVec,Edge[0],Edge[1]);
	}

	HawkRect3D HawkRect3D::operator -= (const HawkVector3D& oVec)
	{
		Point -= oVec;
		return *this;
	}

	HawkVector3D HawkRect3D::operator [](Int32 iIdx) const
	{
		A_Exception(iIdx>= 0 && iIdx<=3);

		if (iIdx == 0) 
		{
			return Point;
		}
		else if (iIdx == 1) 
		{
			return Point + Edge[0];
		}
		else if (iIdx == 2)
		{
			return Point + Edge[0] + Edge[1];
		}
		else if (iIdx == 3)
		{
			return Point + Edge[1];
		}
		return HawkVector3D();
	}
	
	HawkVector3D HawkRect3D::GetPoint(Int32 iIdx) const
	{
		return (*this)[iIdx];
	}

	//�������������
	Float HawkRect3D::GetArea() const
	{
		return HawkMath::Abs(Edge[0].DotProduct(Edge[1]));
	}

	//���ö���
	void HawkRect3D::SetOriginPoint(const HawkVector3D& oPt)
	{
		Point = oPt;
	}

	const HawkVector3D& HawkRect3D::GetOriginPoint() const
	{
		return Point;
	}

	//���ñ�
	void HawkRect3D::SetEdge(Int32 iIdx,const HawkVector3D& oEdge)
	{
		A_Exception(iIdx>= 0 && iIdx<=1);

		Edge[iIdx] = oEdge[iIdx];
	}

	//��ñ�
	const HawkVector3D& HawkRect3D::GetEdge(Int32 iIdx) const
	{
		A_Exception(iIdx>= 0 && iIdx<=1);

		return Edge[iIdx];
	}
	
	//���������η���(��������ϵ,Ϊ������)
	HawkVector3D HawkRect3D::GetNormal() const
	{
		return Edge[0].CrossProduct(Edge[1]).Normalize();
	}
	
	Bool HawkRect3D::IsFrontFacing(const HawkVector3D& vVec3) const
	{
		HawkVector3D vNormal = GetNormal();
		Float fDis = vNormal.DotProduct(vVec3);
		return fDis >= 0.0f;
	}

	//�����������ڲ�
	Bool HawkRect3D::IsPointInside(const HawkVector3D& oVec3) const
	{
		HawkTriangle3D oTri1(Point,Edge[0],Edge[1]);
		HawkTriangle3D oTri2(GetPoint(2),-Edge[0],-Edge[1]);

		return oTri1.IsPointInside(oVec3) || oTri2.IsPointInside(oVec3);
	}
}

