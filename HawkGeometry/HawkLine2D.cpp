#include "HawkLine2D.h"
#include "HawkSegment2D.h"
#include "HawkRay2D.h"
#include "HawkTriangle2D.h"
#include "HawkRect2D.h"

namespace Hawk
{
	//Ĭ�Ϲ���
	HawkLine2D::HawkLine2D() : Point(HawkVector2D()),Direction(HawkVector2D())
	{
	}

	//ʹ�������㹹��
	HawkLine2D::HawkLine2D(const HawkVector2D& oPt1,const HawkVector2D& oPt2)
	{
		Set(oPt1,oPt2);
	}

	//��������
	HawkLine2D::HawkLine2D(const HawkLine2D& oLine) : Point(oLine.Point),Direction(oLine.Direction)
	{
	}

	//�����ߵĵ�����
	void  HawkLine2D::Set(const HawkVector2D& oPt1,const HawkVector2D& oPt2)
	{
		Point	  = oPt1;
		Direction = oPt2 - oPt1;
		Direction.Normalize();
	}

	PtRelation HawkLine2D::GetPtRelation(const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp = oVec2 - Point;
		Float fVal = oTmp.CrossProduct(Direction);
		if (fVal > HawkMath::FLOAT_DIFF)
		{
			return PT_RIGHT;
		}
		else if (fVal < -HawkMath::FLOAT_DIFF)
		{
			return PT_LEFT;
		}
		return PT_ON;
	}

	//�ж����Ƿ���������(�����ߵ�������Ҫ����)
	Bool HawkLine2D::IsPointOnLine(const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp = oVec2 - Point;
		oTmp.Normalize();
		return (oTmp == Direction) || (oTmp == -Direction);
	}
	
	//���������oVec�������ĵ�
	HawkVector2D HawkLine2D::GetClosestPoint(const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp = oVec2 - Point;
		return Point + Direction*oTmp.DotProduct(Direction);
	}
	
	Float HawkLine2D::GetDistance(const HawkVector2D& oVec2) const
	{
		HawkVector2D vPt = GetClosestPoint(oVec2);
		return (oVec2 - vPt).Length();
	}

	//�жϵ���ֱ�����
	Bool HawkLine2D::IsOnLeft(const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp = oVec2 - Point;
		Float fVal = oTmp.CrossProduct(Direction);

		return fVal < 0.0f;
	}

	//���������������
	HawkLine2D HawkLine2D::operator + (const HawkVector2D& oVec2) const 
	{ 
		HawkLine2D oTmp = *this;
		oTmp.Point += oVec2;
		return oTmp; 
	}

	HawkLine2D& HawkLine2D::operator += (const HawkVector2D& oVec2) 
	{ 
		Point += oVec2; 
		return *this; 
	}

	HawkLine2D  HawkLine2D::operator - (const HawkVector2D& oVec2) const 
	{ 
		HawkLine2D oTmp = *this;
		oTmp.Point -= oVec2;
		return oTmp; 
	}

	HawkLine2D& HawkLine2D::operator -= (const HawkVector2D& oVec2) 
	{ 
		Point -= oVec2;
		return *this; 
	}

	Bool HawkLine2D::operator == (const HawkLine2D& oLine) const
	{ 
		return (Point==oLine.Point) && (Direction == oLine.Direction || Direction == -oLine.Direction);
	}

	Bool HawkLine2D::operator != (const HawkLine2D& oLine) const
	{
		return !(*this == oLine);
	}

	//////////////////////////////////////////////////////////////////////////
	//ֱ���ཻ�Ľ���
	Bool HawkLine2D::Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff = oLine.Point - Point;
		Float fCross = Direction.CrossProduct(oLine.Direction);

		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(oLine.Direction) / fCross;
			fFactor1 = vDiff.CrossProduct(Direction) / fCross;

			//�����ཻ
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Point + fFactor0*Direction;
			}
			return true;
		}

		//�غ�
		if (oLine.IsPointOnLine(Point))
		{
			if (pResult)
			{
				pResult->Factor = 0;
				pResult->Point  = Point;
			}
			return true;
		}
		
		//ƽ��
		return false;
	}

	//ֱ�ߺ��߶��ཻ�Ľ���
	Bool HawkLine2D::Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff   = oSegment.Start - Point;
		HawkVector2D  vSegDir = oSegment.GetDirection();

		Float fCross = Direction.CrossProduct(vSegDir);

		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(vSegDir) / fCross;
			fFactor1 = vDiff.CrossProduct(Direction) / fCross;

			//�����ཻ
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Point + fFactor0*Direction;
			}
			return true;
		}

		if (IsPointOnLine(oSegment.Start))
		{
			if (pResult)
			{
				if(!HawkMath::IsZero(Direction.X))
				{
					pResult->Factor = (oSegment.Start.X-Point.X) / Direction.X;
				}
				else if(!HawkMath::IsZero(Direction.Y))
				{
					pResult->Factor = (oSegment.Start.Y-Point.Y) / Direction.Y;
				}
				else
				{
					T_Exception("Direction Is Zero.");
				}

				pResult->Point  = oSegment.Start;
			}
			return true;
		}
		else if (IsPointOnLine(oSegment.End))
		{
			if (pResult)
			{
				if(!HawkMath::IsZero(Direction.X))
				{
					pResult->Factor = (oSegment.End.X-Point.X) / Direction.X;
				}
				else if(!HawkMath::IsZero(Direction.Y))
				{
					pResult->Factor = (oSegment.End.Y-Point.Y) / Direction.Y;
				}
				else
				{
					T_Exception("Direction Is Zero.");
				}

				pResult->Point  = oSegment.End;
			}
			return true;
		}

		return false;
	}

	//ֱ�ߺ������ཻ�Ľ���
	Bool HawkLine2D::Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult) const
	{
		HawkVector2D  vDiff = oRay.Point - Point;
		Float fCross = Direction.CrossProduct(oRay.Direction);

		if (HawkMath::Abs<Float>(fCross) > HawkMath::FLOAT_DIFF)
		{
			Float fFactor0 = 0, fFactor1 = 0;
			fFactor0 = vDiff.CrossProduct(oRay.Direction) / fCross;
			fFactor1 = vDiff.CrossProduct(Direction) / fCross;

			//�����ཻ
			if (pResult)
			{
				pResult->Factor = fFactor0;
				pResult->Point  = Point + fFactor0*Direction;
			}

			return fFactor0 >= 0;
		}

		//�غ�
		if (IsPointOnLine(oRay.Point))
		{
			if (pResult)
			{
				if(!HawkMath::IsZero(Direction.X))
				{
					pResult->Factor = (oRay.Point.X-Point.X) / Direction.X;
				}
				else if(!HawkMath::IsZero(Direction.Y))
				{
					pResult->Factor = (oRay.Point.Y-Point.Y) / Direction.Y;
				}
				else
				{
					T_Exception("Direction Is Zero.");
				}

				pResult->Point  = oRay.Point;
			}
			return true;
		}

		//ƽ��
		return false;
	}

	//ֱ�ߺ��������ཻ�Ľ���
	Bool HawkLine2D::Intersect(const HawkTriangle2D& oTriangle,Vec2IntrResult* pResult) const
	{
		//��ΧԲ���Ӧֱ�ߵĹ�ϵ
		HawkVector2D vCenter = oTriangle.GetBoundCenter();
		Float        fRadius = oTriangle.GetBoundRadius();
		if (GetDistance(vCenter) > fRadius)
		{
			return false;
		}

		PtRelation eRel[3] = {PT_NONE};
		for (Int32 i=0;i<3;i++)
		{
			HawkVector2D oVec2 = oTriangle.GetPoint(i);
			eRel[i] = GetPtRelation(oVec2);
			if (eRel[i] == PT_ON)
			{
				if (pResult)
				{
					if(!HawkMath::IsZero(Direction.X))
					{
						pResult->Factor = (oVec2.X-Point.X) / Direction.X;
					}
					else if(!HawkMath::IsZero(Direction.Y))
					{
						pResult->Factor = (oVec2.Y-Point.Y) / Direction.Y;
					}
					else
					{
						T_Exception("Direction Is Zero.");
					}

					pResult->Point  = oVec2;
				}
				return true;
			}
		}

		for (Int32 i=0;i<3;i++)
		{
			if (eRel[i] != eRel[(i+1)%3])
			{
				HawkSegment2D oSeg(oTriangle.GetPoint(i),oTriangle.GetPoint((i+1)%3));
				if(Intersect(oSeg,pResult)) 
					return true;
			}
		}

		return false;
	}

	//ֱ�ߺ;����ཻ�Ľ���
	Bool HawkLine2D::Intersect(const HawkRect2D& oRect,Vec2IntrResult* pResult) const
	{
		//��ΧԲ���Ӧֱ�ߵĹ�ϵ
		HawkVector2D vCenter = oRect.GetBoundCenter();
		Float        fRadius = oRect.GetBoundRadius();
		if (GetDistance(vCenter) > fRadius)
		{
			return false;
		}

		PtRelation eRel[4] = {PT_NONE};
		for (Int32 i=0;i<4;i++)
		{
			HawkVector2D oVec2 = oRect.GetPoint(i);
			eRel[i] = GetPtRelation(oVec2);
			if (eRel[i] == PT_ON)
			{
				if (pResult)
				{
					if(!HawkMath::IsZero(Direction.X))
					{
						pResult->Factor = (oVec2.X-Point.X) / Direction.X;
					}
					else if(!HawkMath::IsZero(Direction.Y))
					{
						pResult->Factor = (oVec2.Y-Point.Y) / Direction.Y;
					}
					else
					{
						T_Exception("Direction Is Zero.");
					}

					pResult->Point  = oVec2;
				}
				return true;
			}
		}

		for (Int32 i=0;i<4;i++)
		{
			if (eRel[i] != eRel[(i+1)%4])
			{
				HawkSegment2D oSeg(oRect.GetPoint(i),oRect.GetPoint((i+1)%4));
				if(Intersect(oSeg,pResult)) 
					return true;
			}
		}

		return false;
	}
}

