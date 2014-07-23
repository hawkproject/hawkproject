#include "HawkRay3D.h"
#include "HawkVector3D.h"
#include "HawkTriangle3D.h"
#include "HawkLine3D.h"
#include "HawkRect3D.h"
#include "HawkPlane3D.h"
#include "HawkAxisAlignedBox.h"
#include "HawkSphere.h"
#include "HawkOBB.h"
#include "HawkCapsule.h"

namespace Hawk
{
	extern Bool geoClip(Float denom, Float numer, Float& t0, Float& t1);

	//默认构造
	HawkRay3D::HawkRay3D() : Point(HawkVector3D()),Direction(HawkVector3D())
	{
	}

	//使用两个点构造
	HawkRay3D::HawkRay3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2)
	{
		Set(oPt1,oPt2);
	}

	//拷贝构造
	HawkRay3D::HawkRay3D(const HawkRay3D& oRay) : Point(oRay.Point),Direction(oRay.Direction)
	{
	}

	//设置线的点数据
	void  HawkRay3D::Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2)
	{
		Point     = oPt1;
		Direction = oPt2 - oPt1;
		Direction.Normalize();
	}

	void  HawkRay3D::SetPointAndDir(const HawkVector3D& oPoint,const HawkVector3D& oDir)
	{
		Point     = oPoint;
		Direction = oDir;
		Direction.Normalize();
	}

	//判读点是否在线上面(各种线的特性需要考虑)
	Bool  HawkRay3D::IsPointOnRay(const HawkVector3D& oVec) const
	{
		HawkVector3D oTmp = oVec - Point;
		oTmp.Normalize();

		return oTmp == Direction;
	}

	//各种运算符的重载
	HawkRay3D HawkRay3D::operator + (const HawkVector3D& oVec) const 
	{ 
		HawkRay3D oTmp  = *this;
		oTmp.Point += oVec;
		return oTmp; 
	}

	HawkRay3D& HawkRay3D::operator += (const HawkVector3D& oVec) 
	{ 
		Point += oVec; 
		return *this; 
	}

	HawkRay3D HawkRay3D::operator - (const HawkVector3D& oVec) const 
	{ 
		HawkRay3D oTmp = *this;
		oTmp.Point -= oVec;
		return oTmp; 
	}

	HawkRay3D& HawkRay3D::operator -= (const HawkVector3D& oVec) 
	{ 
		Point -= oVec;
		return *this; 
	}

	Bool HawkRay3D::operator ==(const HawkRay3D& oRay) const
	{ 
		return (Point==oRay.Point) && (Direction == oRay.Direction);
	}

	Bool HawkRay3D::operator!=(const HawkRay3D& oRay) const
	{
		return !(*this == oRay);
	}

	//////////////////////////////////////////////////////////////////////////
	//直线和三角形相交
	Bool HawkRay3D::Intersect(const HawkTriangle3D& oTriangle,Vec3IntrResult* pResult) const
	{
		HawkVector3D vDiff   = Point - oTriangle.GetPoint(0);
		HawkVector3D edge1  = oTriangle.GetEdge(0);
		HawkVector3D edge2  = oTriangle.GetEdge(1);
		HawkVector3D normal = edge1.CrossProduct(edge2);

		Float DdN   = Direction.DotProduct(normal);
		Float fSign = 0.0f;
		if (DdN > HawkMath::FLOAT_DIFF)
		{
			fSign = 1.0f;
		}
		else if (DdN < -HawkMath::FLOAT_DIFF)
		{
			fSign = -1.0f;
			DdN  = -DdN;
		}
		else
		{
			return false;
		}

		Float DdQxE2 = fSign*Direction.DotProduct(vDiff.CrossProduct(edge2));
		if (DdQxE2 >= 0.0f)
		{
			Float DdE1xQ = fSign*Direction.DotProduct(edge1.CrossProduct(vDiff));
			if (DdE1xQ >= 0.0f)
			{
				if (DdQxE2 + DdE1xQ <= DdN)
				{					
					Float QdN = -fSign*vDiff.DotProduct(normal);
					if (QdN >= 0.0f)
					{
						if (pResult)
						{
							pResult->Factor = QdN / DdN;
							pResult->Point = Point + pResult->Factor*Direction;
						}
						return true;
					}
				}
			}
		}

		return false;
	}

	//直线和矩形相交
	Bool HawkRay3D::Intersect(const HawkRect3D& oRect,Vec3IntrResult* pResult) const
	{
		HawkTriangle3D oTriangle1 = HawkTriangle3D(oRect.GetPoint(0),oRect.GetEdge(0),oRect.GetEdge(1));
		HawkTriangle3D oTriangle2 = HawkTriangle3D(oRect.GetPoint(2),-oRect.GetEdge(0),-oRect.GetEdge(1));

		if (Intersect(oTriangle1,pResult))
			return true;

		return Intersect(oTriangle2,pResult);
	}

	//直线和平面相交
	Bool HawkRay3D::Intersect(const HawkPlane3D& oPlane,Vec3IntrResult* pResult) const
	{
		Float fDis = oPlane.Normal.DotProduct(Direction);
		if (fDis == 0)
			return false;

		Float fTmp = -(oPlane.Normal.DotProduct(Point) + oPlane.Constant) / fDis;
		if (fTmp >= 0)
		{
			if (pResult)
			{
				pResult->Factor = fTmp;
				pResult->Point  = Point + fTmp*Direction;
			}
			return true;
		}
		
		return false;
	}

	//直线和AABB盒相交
	Bool HawkRay3D::Intersect(const HawkAxisAlignedBox& oAABB,Vec3IntrResult* pResult) const
	{
		if (oAABB.IsNull()) return false;

		HawkVector3D vDiff = Point - oAABB.GetCenter();
		HawkVector3D vExt  = oAABB.GetExtents();

		Float t0 = 0;
		Float t1 = FLT_MAX;
		Float fT0 = t0, fT1 = t1;

		Bool bNotAllClip = false;

		if(geoClip(Direction.X, -vDiff.X-vExt[0], t0, t1) &&
			geoClip(-Direction.X, vDiff.X-vExt[0], t0, t1) &&
			geoClip(Direction.Y, -vDiff.Y-vExt[1], t0, t1) &&
			geoClip(-Direction.Y, vDiff.Y-vExt[1], t0, t1) &&
			geoClip(Direction.Z, -vDiff.Z-vExt[2], t0, t1) &&
			geoClip(-Direction.Z, vDiff.Z-vExt[2], t0, t1))
			bNotAllClip = true;

		if (bNotAllClip && (t0 != fT0 || t1 != fT1))
		{
			if (pResult)
			{
				if (t1 > t0)
				{
					pResult->Factor = HawkMath::Abs<Float>(t0) < HawkMath::Abs<Float>(t1)?t0:t1;
					pResult->Point  = Point + pResult->Factor*Direction;
				}
				else
				{
					pResult->Factor = t0;
					pResult->Point  = Point + t0*Direction;
				}
			}			

			return true;
		}

		return false;
	}

	//直线和球相交
	Bool HawkRay3D::Intersect(const HawkSphere& oSphere,Vec3IntrResult* pResult) const
	{
		HawkVector3D vDiff = Point - oSphere.Center;
		Float a0 = vDiff.DotProduct(vDiff) - oSphere.Radius*oSphere.Radius;
		Float a1, dis, root;

		//点在内部
		if (a0 <= 0.0f)
		{
			if(pResult)
			{
				a1 = Direction.DotProduct(vDiff);
				dis = a1*a1 - a0;
				root = HawkMath::Sqrt(dis);

				pResult->Factor = -a1 + root;
				pResult->Point  = Point + pResult->Factor*Direction;
			}
			return true;
		}

		a1 = Direction.DotProduct(vDiff);
		if (a1 >= 0.0f)
			return false;

		dis = a1*a1 - a0;
		if (dis < 0.0f)
			return false;
		
		if (dis >= HawkMath::FLOAT_DIFF)
		{
			root = HawkMath::Sqrt(dis);

			Float t0 = -a1 - root;
			Float t1 = -a1 + root;

			pResult->Factor = HawkMath::Abs<Float>(t0) < HawkMath::Abs<Float>(t1)?t0:t1;
			pResult->Point  = Point + pResult->Factor*Direction;
		}
		else
		{
			Float t0 = -a1;
			pResult->Factor = t0;
			pResult->Point  = Point + t0*Direction;
		}

		return true;
	}

	Bool  HawkRay3D::Intersect(const HawkOBB& oBox,Vec3IntrResult* pResult) const
	{
		const HawkVector3D* Axis = oBox.GetAxis();
		HawkVector3D diff        = Point - oBox.Center;
		HawkVector3D BOrigin(
			diff.DotProduct(Axis[0]),
			diff.DotProduct(Axis[1]),
			diff.DotProduct(Axis[2])
			);
		HawkVector3D BDirection(
			Direction.DotProduct(Axis[0]),
			Direction.DotProduct(Axis[1]),
			Direction.DotProduct(Axis[2])
			);

		Float t0 = 0,t1 = FLT_MAX;
		bool notAllClipped =
			geoClip(+BDirection.X, -BOrigin.X-oBox.Extent[0], t0, t1) &&
			geoClip(-BDirection.X, +BOrigin.X-oBox.Extent[0], t0, t1) &&
			geoClip(+BDirection.Y, -BOrigin.Y-oBox.Extent[1], t0, t1) &&
			geoClip(-BDirection.Y, +BOrigin.Y-oBox.Extent[1], t0, t1) &&
			geoClip(+BDirection.Z, -BOrigin.Z-oBox.Extent[2], t0, t1) &&
			geoClip(-BDirection.Z, +BOrigin.Z-oBox.Extent[2], t0, t1);

		if (notAllClipped)
		{
			//2交点
			if (t1 > t0)
			{
				if (pResult)
				{
					pResult->Factor = HawkMath::Abs<Float>(t0) < HawkMath::Abs<Float>(t1)? t0 : t1;
					pResult->Point  = Point + Direction * pResult->Factor;
				}
				return true;
			}
			//1交点
			else
			{
				if (pResult)
				{
					pResult->Factor = t0;
					pResult->Point  = Point + Direction * pResult->Factor;
				}
				return true;
			}
		}
		return false;
	}

	Bool  HawkRay3D::Intersect(const HawkCapsule& oCapsule,Vec3IntrResult* pResult) const
	{
		Vec3IntrResult  intrResult;
		HawkLine3D      oLine;
		oLine.Point     = Point;
		oLine.Direction = Direction;
		if (oLine.Intersect(oCapsule,&intrResult) && intrResult.Factor >= 0)
		{
			if (pResult)
			{
				pResult->Factor = intrResult.Factor;
				pResult->Point  = intrResult.Point;
			}
			return true;
		}
		return false;
	}
}
