#include "HawkSegment2D.h"
#include "HawkVector2D.h"
#include "HawkLine2D.h"
#include "HawkSegment2D.h"
#include "HawkRay2D.h"
#include "HawkRect2D.h"
#include "HawkTriangle2D.h"

namespace Hawk
{
	//构造
	HawkSegment2D::HawkSegment2D() : Start(HawkVector2D()),End(HawkVector2D())
	{
	}

	//构造
	HawkSegment2D::HawkSegment2D(const HawkVector2D& oStart,const HawkVector2D& oEnd)
	{
		Set(oStart,oEnd);
	}

	//拷贝构造
	HawkSegment2D::HawkSegment2D(const HawkSegment2D& oSegment) : Start(oSegment.Start),End(oSegment.End)
	{
	}

	//设置线的点数据
	void  HawkSegment2D::Set(const HawkVector2D& oStart,const HawkVector2D& oEnd)
	{
		Start	= oStart;
		End		= oEnd;
	}

	//判读点是否在线上面(各种线的特性需要考虑)
	Bool HawkSegment2D::IsPointOnSegment(const HawkVector2D& oVec2) const
	{
		return (oVec2 - Start).Normalize() == (End - oVec2).Normalize();
	}

	HawkVector2D HawkSegment2D::GetClosestPoint(const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp = oVec2 - Start;
		return Start + GetDirection()*oTmp.DotProduct(GetDirection());
	}

	Float HawkSegment2D::GetDistance(const HawkVector2D& oVec2) const
	{
		return (oVec2 - GetClosestPoint(oVec2)).Length();
	}

	//中点
	HawkVector2D HawkSegment2D::MidPoint() const
	{
		return 0.5f*(Start+End);
	}
	
	//获得方向
	HawkVector2D HawkSegment2D::GetDirection() const
	{
		return End - Start;
	}
	
	//长度
	Float HawkSegment2D::GetLength()
	{
		return (End - Start).Length();
	}

	//各种运算符的重载
	HawkSegment2D HawkSegment2D::operator + (const HawkVector2D& oVec2) const 
	{ 
		return HawkSegment2D(Start+oVec2,End+oVec2); 
	}

	HawkSegment2D& HawkSegment2D::operator += (const HawkVector2D& oVec2) 
	{ 
		Start += oVec2; 
		End   += oVec2; 
		return *this; 
	}

	HawkSegment2D HawkSegment2D::operator - (const HawkVector2D& oVec2) const 
	{ 
		return HawkSegment2D(Start-oVec2,End-oVec2); 
	}

	HawkSegment2D& HawkSegment2D::operator -= (const HawkVector2D& oVec2) 
	{ 
		Start -= oVec2; 
		End   -= oVec2; 
		return *this; 
	}

	Bool HawkSegment2D::operator == (const HawkSegment2D& oSegment) const
	{ 
		return ((Start==oSegment.Start) && (End == oSegment.End)) ||
			((Start==oSegment.End) && (End == oSegment.Start));
	}

	Bool HawkSegment2D::operator != (const HawkSegment2D& oSegment) const
	{
		return !(*this == oSegment);
	}

	//////////////////////////////////////////////////////////////////////////
	Bool HawkSegment2D::Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff = oLine.Point - Start;
		HawkVector2D  vDir  = GetDirection();

		Float fCross = vDir.CrossProduct(oLine.Direction);
		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(oLine.Direction) / fCross;
			fFactor1 = vDiff.CrossProduct(vDir) / fCross;

			//单点相交
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Start + fFactor0*vDir;
			}
			return fFactor0 >= 0.0f && fFactor0 <= 1.0f;
		}

		//重合
		if (oLine.IsPointOnLine(Start))
		{
			if (pResult)
			{
				pResult->Factor = 0;
				pResult->Point  = Start;
			}
			return true;
		}

		//平行
		return false;
	}

	Bool HawkSegment2D::Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff   = oSegment.Start - Start;
		HawkVector2D  vDir    = GetDirection();
		HawkVector2D  vSegDir = oSegment.GetDirection();

		Float fCross = vDir.CrossProduct(vSegDir);
		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(vSegDir) / fCross;
			fFactor1 = vDiff.CrossProduct(vDir) / fCross;

			//单点相交
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Start + fFactor0*vDir;
			}
			return fFactor0 >= 0.0f && fFactor0 <= 1.0f;
		}

		//重合
		if (oSegment.IsPointOnSegment(Start))
		{
			if (pResult)
			{
				pResult->Factor = 0;
				pResult->Point  = Start;
			}
			return true;
		}
		else if (oSegment.IsPointOnSegment(End))
		{
			if (pResult)
			{
				if(!HawkMath::IsZero(vDir.X))
				{
					pResult->Factor = (End.X-Start.X) / vDir.X;
				}
				else if(!HawkMath::IsZero(vDir.Y))
				{
					pResult->Factor = (End.Y-Start.Y) / vDir.Y;
				}
				else
				{
					T_Exception("Direction Is Zero.");
				}
				pResult->Point  = End;
			}
			return true;
		}

		//平行
		return false;
	}

	Bool HawkSegment2D::Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff = oRay.Point - Start;
		HawkVector2D  vDir  = GetDirection();

		Float fCross = vDir.CrossProduct(oRay.Direction);
		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(oRay.Direction) / fCross;
			fFactor1 = vDiff.CrossProduct(vDir) / fCross;

			//单点相交
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Start + fFactor0*vDir;
			}
			return fFactor0 >= 0.0f && fFactor0 <= 1.0f && fFactor1 >= 0.0f;
		}

		//重合
		if (oRay.IsPointOnRay(Start))
		{
			if (pResult)
			{
				pResult->Factor = 0;
				pResult->Point  = Start;
			}
			return true;
		}
		else if (oRay.IsPointOnRay(End))
		{
			if (pResult)
			{
				if(!HawkMath::IsZero(vDir.X))
				{
					pResult->Factor = (End.X-Start.X) / vDir.X;
				}
				else if(!HawkMath::IsZero(vDir.Y))
				{
					pResult->Factor = (End.Y-Start.Y) / vDir.Y;
				}
				else
				{
					T_Exception("Direction Is Zero.");
				}
				pResult->Point  = End;
			}
			return true;
		}

		//平行
		return false;
	}
	
	Bool HawkSegment2D::Intersect(const HawkTriangle2D& oTriangle,Vec2IntrResult* pResult) const
	{
		//包围圆与对应直线的关系
		HawkVector2D vCenter = oTriangle.GetBoundCenter();
		Float        fRadius = oTriangle.GetBoundRadius();
		if (GetDistance(vCenter) > fRadius)
		{
			return false;
		}

		for (Int32 i=0;i<3;i++)
		{
			HawkSegment2D oSeg = oTriangle.GetSegment(i);
			if (Intersect(oSeg,pResult))
				return true;
		}
		return false;
	}

	Bool HawkSegment2D::Intersect(const HawkRect2D& oRect,Vec2IntrResult* pResult) const
	{
		//包围圆与对应直线的关系
		HawkVector2D vCenter = oRect.GetBoundCenter();
		Float        fRadius = oRect.GetBoundRadius();
		if (GetDistance(vCenter) > fRadius)
		{
			return false;
		}

		for (Int32 i=0;i<3;i++)
		{
			HawkSegment2D oSeg = oRect.GetSegment(i);
			if (Intersect(oSeg,pResult))
				return true;
		}
		return false;
	}
}

