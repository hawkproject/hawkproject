#include "HawkRect2D.h"
#include "HawkRect.h"
#include "HawkVector2D.h"
#include "HawkLine2D.h"
#include "HawkRay2D.h"
#include "HawkSegment2D.h"
#include "HawkTriangle2D.h"

namespace Hawk
{
	//默认构造函数
	HawkRect2D::HawkRect2D()
	{ 
		Point = HawkVector2D(0,0);
		Edge[0] = HawkVector2D(1,0);
		Edge[1] = HawkVector2D(0,1);
	};

	//左下角坐标和两个边构造
	HawkRect2D::HawkRect2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2)
	{
		Point	= oPt;
		Edge[0] = oEdge1;
		Edge[1] = oEdge2;
	}

	//拷贝构造
	HawkRect2D::HawkRect2D(const HawkRect2D& oRc)
	{
		Point	= oRc.Point;
		Edge[0] = oRc.Edge[0];
		Edge[1] = oRc.Edge[1];
	}

	HawkRect2D::HawkRect2D(const HawkRect& oRc)
	{
		A_Exception(oRc.IsValid());

		Point.X	  = oRc.Left;
		Point.Y	  = oRc.Top;
		Edge[0].X = (Float)oRc.Right - (Float)oRc.Left;
		Edge[0].Y = 0;
		Edge[1].X = 0;
		Edge[1].Y = (Float)oRc.Bottom - (Float)oRc.Top;
	}

	//操作符重载
	Bool HawkRect2D::operator == (const HawkRect2D& oRc) const
	{
		return  Point == oRc.Point && Edge[0]==oRc.Edge[0] && Edge[1]==oRc.Edge[1];
	}

	Bool HawkRect2D::operator != (const HawkRect2D& oRc) const
	{
		return !(*this == oRc);
	}

	HawkRect2D HawkRect2D::operator + (const HawkVector2D& oVec) const
	{
		return HawkRect2D(Point+oVec,Edge[0],Edge[1]);
	}

	HawkRect2D& HawkRect2D::operator += (const HawkVector2D& oVec)
	{
		Point += oVec;
		return *this;
	}

	HawkRect2D HawkRect2D::operator - (const HawkVector2D& oVec) const
	{
		return HawkRect2D(Point-oVec,Edge[0],Edge[1]);
	}

	HawkRect2D HawkRect2D::operator -= (const HawkVector2D& oVec)
	{
		Point -= oVec;
		return *this;
	}

	HawkVector2D HawkRect2D::operator [](Int32 iIdx) const
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
		return HawkVector2D();
	}
	
	//获得其中一个顶点
	HawkVector2D HawkRect2D::GetPoint(Int32 iIdx) const
	{
		return (*this)[iIdx];
	}

	HawkSegment2D HawkRect2D::GetSegment(Int32 iIdx) const
	{
		A_Exception(iIdx>= 0 && iIdx<=3);

		if (iIdx == 0) 
		{
			return HawkSegment2D(GetPoint(0),GetPoint(1));
		}
		else if (iIdx == 1) 
		{
			return HawkSegment2D(GetPoint(1),GetPoint(2));
		}
		else if (iIdx == 2)
		{
			return HawkSegment2D(GetPoint(2),GetPoint(3));
		}
		return HawkSegment2D(GetPoint(3),GetPoint(0));
	}

	//计算三角形面积
	Float HawkRect2D::GetArea() const
	{
		return HawkMath::Abs(Edge[0].DotProduct(Edge[1]));
	}

	//设置顶点
	void HawkRect2D::SetOriginPoint(const HawkVector2D& oPt)
	{
		Point = oPt;
	}

	const HawkVector2D& HawkRect2D::GetOriginPoint() const
	{
		return Point;
	}

	void  HawkRect2D::SetCenterPoint(const HawkVector2D& oPt)
	{
		HawkVector2D vCenter = GetCenterPoint();
		HawkVector2D vDelta  = vCenter - oPt;
		Point = Point + vDelta;
	}

	HawkVector2D HawkRect2D::GetCenterPoint() const
	{
		return Point + (Edge[0] + Edge[1]) * 0.5f;
	}

	//获取包围中心点(非外接圆)
	HawkVector2D HawkRect2D::GetBoundCenter() const
	{
		return GetCenterPoint();
	}

	//获取包围半径
	Float HawkRect2D::GetBoundRadius() const
	{
		Float fRadius0 = (GetPoint(2)-GetPoint(0)).SQLength();
		Float fRadius1 = (GetPoint(1) - GetPoint(3)).SQLength();

		return fRadius0 > fRadius1? HawkMath::Sqrt(fRadius0) * 0.5f : HawkMath::Sqrt(fRadius1) * 0.5f;
	}

	//设置边
	void HawkRect2D::SetEdge(Int32 iIdx,const HawkVector2D& oEdge)
	{
		HawkAssert(iIdx>=0 && iIdx<=1);

		Edge[iIdx] = oEdge;
	}

	//获得边
	const HawkVector2D& HawkRect2D::GetEdge(Int32 iIdx) const
	{
		HawkAssert(iIdx>=0 && iIdx<=1);

		return Edge[iIdx];
	}

	void  HawkRect2D::Clear()
	{
		Point = HawkVector2D(0,0);
		Edge[0] = HawkVector2D(1,0);
		Edge[1] = HawkVector2D(0,1);
	}

	//某一特定顶点在三角形之内
	Bool HawkRect2D::IsPointInside(const HawkVector2D& oVec) const
	{ 
		HawkTriangle2D oTrig1(Point,Edge[0],Edge[1]);
		HawkTriangle2D oTrig2(GetPoint(2),-Edge[0],-Edge[1]);

		return oTrig1.IsPointInside(oVec) || oTrig2.IsPointInside(oVec);
	}

	Bool HawkRect2D::IsClockWise() const
	{
		return Edge[0].CrossProduct(Edge[1]) < 0;
	}

	//宽度计算
	Float HawkRect2D::GetWidth() const
	{
		return Edge[0].Length();
	}

	void HawkRect2D::SetWidth(Float fWidth)
	{
		Edge[0] = fWidth*Edge[0].Normalize();
	}

	//高度计算
	Float HawkRect2D::GetHeight() const
	{
		return Edge[1].Length();
	}

	void HawkRect2D::SetHeight(Float fHeight)
	{
		Edge[1] = fHeight*Edge[1].Normalize();
	}

	//////////////////////////////////////////////////////////////////////////
	//直线和矩形相交判断
	Bool  HawkRect2D::Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult) const
	{
		return oLine.Intersect(*this,pResult);
	}

	//射线和矩形相交判断
	Bool  HawkRect2D::Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult) const
	{
		return oRay.Intersect(*this,pResult);
	}

	//线段和矩形相交判断
	Bool  HawkRect2D::Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult) const
	{
		return oSegment.Intersect(*this,pResult);
	}

	//三角形和矩形相交判断
	Bool  HawkRect2D::Intersect(const HawkTriangle2D& oTriangle) const
	{
		return oTriangle.Intersect(*this);
	}

	//矩形和矩形相交判断
	Bool  HawkRect2D::Intersect(const HawkRect2D& oRect) const
	{
		Float fDis = (GetBoundCenter() - oRect.GetBoundCenter()).Length();
		if (fDis > GetBoundRadius() + oRect.GetBoundRadius())
		{
			return false;
		}

		for (Int32 i=0;i<4;i++)
		{
			HawkLine2D oLine(GetPoint(i),GetPoint((i+1)%4));
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
				oLine.GetPtRelation(GetPoint(2)) == eRel &&
				oLine.GetPtRelation(GetPoint(3)) == eRel)
				return false;
		}

		return true;
	}
}
