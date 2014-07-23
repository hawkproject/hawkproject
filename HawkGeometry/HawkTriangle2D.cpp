#include "HawkTriangle2D.h"
#include "HawkVector2D.h"
#include "HawkLine2D.h"
#include "HawkRay2D.h"
#include "HawkSegment2D.h"
#include "HawkPoint2D.h"
#include "HawkRect2D.h"

namespace Hawk
{
	//默认构造函数
	HawkTriangle2D::HawkTriangle2D(){};

	//左下角坐标和两个边构造
	HawkTriangle2D::HawkTriangle2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2)
	{
		Point	= oPt;
		Edge[0] = oEdge1;
		Edge[1] = oEdge2;
	}

	//拷贝构造
	HawkTriangle2D::HawkTriangle2D(const HawkTriangle2D& oTrig)
	{
		Point	= oTrig.Point;
		Edge[0] = oTrig.Edge[0];
		Edge[1] = oTrig.Edge[1];
	}

	//操作符重载
	Bool HawkTriangle2D::operator == (const HawkTriangle2D& oTrig) const
	{
		return  Point == oTrig.Point && Edge[0]==oTrig.Edge[0] && Edge[1]==oTrig.Edge[1];
	}

	Bool HawkTriangle2D::operator != (const HawkTriangle2D& oTrig) const
	{
		return !(*this == oTrig);
	}

	HawkTriangle2D HawkTriangle2D::operator + (const HawkVector2D& oVec2) const
	{
		return HawkTriangle2D(Point+oVec2,Edge[0],Edge[1]);
	}

	HawkTriangle2D& HawkTriangle2D::operator += (const HawkVector2D& oVec2)
	{
		Point += oVec2;
		return *this;
	}

	HawkTriangle2D HawkTriangle2D::operator - (const HawkVector2D& oVec2) const
	{
		return HawkTriangle2D(Point-oVec2,Edge[0],Edge[1]);
	}

	HawkTriangle2D HawkTriangle2D::operator -= (const HawkVector2D& oVec2)
	{
		Point -= oVec2;
		return *this;
	}

	HawkVector2D HawkTriangle2D::operator [](Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=2);

		if (iIdx == 0) 
			return Point;
		else
			return Point + Edge[iIdx - 1];
	}

	//计算三角形面积
	Float HawkTriangle2D::GetArea() const
	{
		return HawkMath::Abs(Edge[0].DotProduct(Edge[1]) * 0.5f);
	}

	//设置顶点
	void HawkTriangle2D::SetOriginPoint(const HawkVector2D& oPt)
	{
		Point = oPt;
	}

	const HawkVector2D& HawkTriangle2D::GetOriginPoint() const
	{
		return Point;
	}

	//设置边
	void HawkTriangle2D::SetEdge(Int32 iIdx,const HawkVector2D& oEdge)
	{
		A_Exception(iIdx>=0 && iIdx<=1);

		Edge[iIdx] = oEdge;
	}

	//获得边
	const HawkVector2D& HawkTriangle2D::GetEdge(Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=1);

		return Edge[iIdx];
	}

	//某一特定顶点在三角形之内
	Bool HawkTriangle2D::IsPointInside(const HawkVector2D& oVec2) const
	{ 
		Bool bLeft0 = HawkLine2D(GetPoint(0),GetPoint(1)).IsOnLeft(oVec2);
		Bool bLeft1 = HawkLine2D(GetPoint(1),GetPoint(2)).IsOnLeft(oVec2);
		Bool bLeft2 = HawkLine2D(GetPoint(2),GetPoint(0)).IsOnLeft(oVec2);

		return (bLeft0 && bLeft1 && bLeft2) || (!bLeft0 && !bLeft1 && !bLeft2);
	}

	//某一特定顶点在三角形之内
	Bool  HawkTriangle2D::IsPointInside(const HawkPoint2D& oPt2) const
	{
		HawkVector2D oVec2(oPt2.X,oPt2.Y);
		return IsPointInside(oVec2);
	}
	
	//获得顶点
	HawkVector2D HawkTriangle2D::GetPoint(Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=2);

		if (iIdx == 0) 
			return Point;
		else
			return Point + Edge[iIdx - 1];
	}

	//设置三角形的三个顶点
	void  HawkTriangle2D::SetByPoint(const HawkVector2D& vPt1,const HawkVector2D& vPt2,const HawkVector2D& vPt3)
	{
		Point   = vPt1;
		Edge[0] = vPt2 - vPt1;
		Edge[1] = vPt3 - vPt1;
	}	

	//获取包围中心点(非外接圆)
	HawkVector2D HawkTriangle2D::GetBoundCenter() const
	{
		HawkVector2D vPt = Point + Edge[0] + Edge[1];
		return (vPt + Point) * 0.5f;
	}

	//获取包围半径
	Float HawkTriangle2D::GetBoundRadius() const
	{
		HawkVector2D vPt = Point + Edge[0] + Edge[1];
		Float fRadius0 = (vPt-Point).SQLength();
		Float fRadius1 = (GetPoint(1) - GetPoint(2)).SQLength();

		return fRadius0 > fRadius1? HawkMath::Sqrt(fRadius0) * 0.5f : HawkMath::Sqrt(fRadius1) * 0.5f;
	}

	Bool HawkTriangle2D::IsClockWise() const
	{
		return Edge[0].CrossProduct(Edge[1]) < 0;
	}

	HawkSegment2D HawkTriangle2D::GetSegment(Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<=2);
		if (iIdx == 0)
		{
			return HawkSegment2D(GetPoint(0),GetPoint(1));
		}
		else if (iIdx == 1)
		{
			return HawkSegment2D(GetPoint(1),GetPoint(2));
		}
		return HawkSegment2D(GetPoint(2),GetPoint(0));
	}

	//////////////////////////////////////////////////////////////////////////
	//直线和三角形相交判断
	Bool  HawkTriangle2D::Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult) const
	{
		return oLine.Intersect(*this,pResult);
	}

	//射线和三角形相交判断
	Bool  HawkTriangle2D::Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult) const
	{
		return oRay.Intersect(*this,pResult);
	}

	//线段和三角形相交判断
	Bool  HawkTriangle2D::Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult) const
	{
		return oSegment.Intersect(*this,pResult);
	}

	//三角形和三角形相交判断
	Bool  HawkTriangle2D::Intersect(const HawkTriangle2D& oTriangle) const
	{
		Float fDis = (GetBoundCenter() - oTriangle.GetBoundCenter()).Length();
		if (fDis > GetBoundRadius() + oTriangle.GetBoundRadius())
		{
			return false;
		}
		
		for (Int32 i=0;i<3;i++)
		{
			HawkLine2D oLine(GetPoint(i),GetPoint((i+1)%3));
			PtRelation eRel = IsClockWise()?PT_LEFT:PT_RIGHT;

			if (oLine.GetPtRelation(oTriangle.GetPoint(0)) == eRel && 
				oLine.GetPtRelation(oTriangle.GetPoint(1)) == eRel &&
				oLine.GetPtRelation(oTriangle.GetPoint(2)) == eRel)
				return false;
		}

		for (Int32 i=0;i<3;i++)
		{
			HawkLine2D oLine(oTriangle.GetPoint(i),oTriangle.GetPoint((i+1)%3));
			PtRelation eRel = oTriangle.IsClockWise()?PT_LEFT:PT_RIGHT;

			if (oLine.GetPtRelation(GetPoint(0)) == eRel && 
				oLine.GetPtRelation(GetPoint(1)) == eRel &&
				oLine.GetPtRelation(GetPoint(2)) == eRel)
				return false;
		}

		return true;
	}

	//三角形和矩形相交判断
	Bool  HawkTriangle2D::Intersect(const HawkRect2D& oRect) const
	{
		Float fDis = (GetBoundCenter() - oRect.GetBoundCenter()).Length();
		if (fDis > GetBoundRadius() + oRect.GetBoundRadius())
		{
			return false;
		}

		for (Int32 i=0;i<3;i++)
		{
			HawkLine2D oLine(GetPoint(i),GetPoint((i+1)%3));
			PtRelation eRel = IsClockWise()?PT_LEFT:PT_RIGHT;

			if (oLine.GetPtRelation(oRect.GetPoint(0)) == eRel && 
				oLine.GetPtRelation(oRect.GetPoint(1)) == eRel &&
				oLine.GetPtRelation(oRect.GetPoint(2)) == eRel &&
				oLine.GetPtRelation(oRect.GetPoint(3)) == eRel)
				return false;
		}

		for (Int32 i=0;i<4;i++)
		{
			HawkLine2D oLine(oRect.GetPoint(i),oRect.GetPoint((i+1)%4));
			PtRelation eRel = oRect.IsClockWise()?PT_LEFT:PT_RIGHT;

			if (oLine.GetPtRelation(GetPoint(0)) == eRel && 
				oLine.GetPtRelation(GetPoint(1)) == eRel &&
				oLine.GetPtRelation(GetPoint(2)) == eRel)
				return false;
		}

		return true;
	}
}

