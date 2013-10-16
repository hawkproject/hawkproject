#include "HawkLine3D.h"
#include "HawkTriangle3D.h"
#include "HawkRect3D.h"
#include "HawkAxisAlignedBox.h"
#include "HawkSphere.h"
#include "HawkPlane3D.h"
#include "HawkCapsule.h"
#include "HawkVector3D.h"
#include "HawkOBB.h"

namespace Hawk
{
	Bool geoClip(Float denom, Float numer, Float& t0, Float& t1)
	{
		if (denom > 0.0f)
		{
			if (numer > denom*t1)
				return false;

			if (numer > denom*t0)
				t0 = numer / denom;

			return true;
		}
		else if (denom < 0.0f)
		{
			if (numer > denom*t0)
				return false;

			if (numer > denom*t1)
				t1 = numer / denom;

			return true;
		}

		return numer <= 0.0f;
	}

	void GenComplementBasis (HawkVector3D& u, HawkVector3D& v,const HawkVector3D& w)
	{
		Float invLength = 0.0f;

		if (HawkMath::Abs<Float>(w[0]) >= HawkMath::Abs<Float>(w[1]))
		{
			invLength = 1.0f / HawkMath::Sqrt(w[0]*w[0] + w[2]*w[2]);
			u[0] = -w[2]*invLength;
			u[1] = 0.0f;
			u[2] = +w[0]*invLength;
			v[0] = w[1]*u[2];
			v[1] = w[2]*u[0] - w[0]*u[2];
			v[2] = -w[1]*u[0];
		}
		else
		{
			invLength = 1.0f / HawkMath::Sqrt(w[1]*w[1] + w[2]*w[2]);
			u[0] = 0.0f;
			u[1] = +w[2]*invLength;
			u[2] = -w[1]*invLength;
			v[0] = w[1]*u[2] - w[2]*u[1];
			v[1] = -w[0]*u[2];
			v[2] = w[0]*u[1];
		}
	}	

	//默认构造
	HawkLine3D::HawkLine3D() : Point(HawkVector3D()),Direction(HawkVector3D())
	{
	}

	//使用两个点构造
	HawkLine3D::HawkLine3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2)
	{
		Set(oPt1,oPt2);
	}

	//拷贝构造
	HawkLine3D::HawkLine3D(const HawkLine3D& oLine) : Point(oLine.Point),Direction(oLine.Direction)
	{
	}

	//设置线的点数据
	void HawkLine3D::Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2)
	{
		Point = oPt1;
		Direction = oPt2 - oPt1;
		Direction.Normalize();
	}

	//判读点是否在线上面(各种线的特性需要考虑)
	Bool HawkLine3D::IsPointOnLine(const HawkVector3D& oVec) const
	{
		HawkVector3D oTmp = oVec - Point;
		oTmp.Normalize();

		return (oTmp == Direction) || (oTmp == -Direction);
	}
	
	//计算线段上面和oVec相隔最近的点
	HawkVector3D HawkLine3D::GetClosestPoint(const HawkVector3D& oVec) const
	{
		HawkVector3D oTmp = oVec - Point;
		
		return Point + Direction*oTmp.DotProduct(Direction);
	}

	Float HawkLine3D::GetDistance(const HawkVector3D& oVec) const
	{
		HawkVector3D vPt = GetClosestPoint(oVec);
		return (oVec - vPt).Length();
	}

	//各种运算符的重载
	HawkLine3D HawkLine3D::operator + (const HawkVector3D& oVec) const 
	{ 
		HawkLine3D oTmp = *this;
		oTmp.Point += oVec;
		return oTmp; 
	}

	HawkLine3D& HawkLine3D::operator += (const HawkVector3D& oVec) 
	{ 
		Point += oVec; 
		return *this; 
	}

	HawkLine3D HawkLine3D::operator - (const HawkVector3D& oVec) const 
	{ 
		HawkLine3D oTmp = *this;
		oTmp.Point -= oVec;
		return oTmp; 
	}

	HawkLine3D& HawkLine3D::operator -= (const HawkVector3D& oVec) 
	{ 
		Point -= oVec;
		return *this; 
	}

	Bool HawkLine3D::operator == (const HawkLine3D& oLine) const
	{ 
		return (Point==oLine.Point) && (Direction == oLine.Direction || Direction == -oLine.Direction);
	}

	Bool HawkLine3D::operator != (const HawkLine3D& oLine) const
	{
		return !(*this == oLine);
	}

	//////////////////////////////////////////////////////////////////////////
	//直线和三角形相交
	Bool HawkLine3D::Intersect(const HawkTriangle3D& oTriangle,Vec3IntrResult* pResult) const
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
					if (pResult)
					{
						Float QdN = -fSign*vDiff.DotProduct(normal);
						pResult->Factor = QdN / DdN;
						pResult->Point = Point + pResult->Factor*Direction;
					}					
					return true;
				}
			}
		}

		return false;
	}

	//直线和矩形相交
	Bool HawkLine3D::Intersect(const HawkRect3D& oRect,Vec3IntrResult* pResult) const
	{
		HawkTriangle3D oTriangle1 = HawkTriangle3D(oRect.GetPoint(0),oRect.GetEdge(0),oRect.GetEdge(1));
		HawkTriangle3D oTriangle2 = HawkTriangle3D(oRect.GetPoint(2),-oRect.GetEdge(0),-oRect.GetEdge(1));

		if (Intersect(oTriangle1,pResult))
			return true;

		return Intersect(oTriangle2,pResult);
	}

	Bool HawkLine3D::Intersect(const HawkPlane3D& oPlane, Hawk::Vec3IntrResult *pResult) const
	{
		Float DdN = Direction.DotProduct(oPlane.Normal);
		Float signedDistance = oPlane.GetDistance(Point);
		if (HawkMath::Abs<Float>(DdN) > HawkMath::FLOAT_DIFF)
		{
			if (pResult)
			{
				pResult->Factor = -signedDistance/DdN;
				pResult->Point  = Point + pResult->Factor * Direction;
			}
			return true;
		}

		// The Line and plane are parallel.  Determine if they are numerically
		// close enough to be coincident.
		if (HawkMath::Abs<Float>(signedDistance) <= HawkMath::FLOAT_DIFF)
		{
			if (pResult)
			{
				pResult->Factor = 0;
				pResult->Point  = Point;
			}

			return true;
		}

		return true;
	}

	//直线和AABB盒相交
	Bool HawkLine3D::Intersect(const HawkAxisAlignedBox& oAABB,Vec3IntrResult* pResult) const
	{
		if (oAABB.IsNull()) return false;

		HawkVector3D vDiff = Point - oAABB.GetCenter();
		HawkVector3D vExt  = oAABB.GetExtents();

		Float t0 = -FLT_MAX;
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
	Bool HawkLine3D::Intersect(const HawkSphere& oSphere,Vec3IntrResult* pResult) const
	{
		HawkVector3D vDiff = Point - oSphere.Center;
		Float a0  = vDiff.DotProduct(vDiff) - oSphere.Radius*oSphere.Radius;
		Float a1  = Direction.DotProduct(vDiff);
		Float dis = a1*a1 - a0;

		if (dis >= 0)
		{
			if (pResult)
			{
				if (dis >= HawkMath::FLOAT_DIFF)
				{
					Float root = HawkMath::Sqrt(dis);
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
			}			

			return true;
		}

		return false;
	}

	Bool  HawkLine3D::Intersect(const HawkOBB& oBox,Vec3IntrResult* pResult) const
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

		Float t0 = -FLT_MAX,t1 = FLT_MAX;
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


	Bool  HawkLine3D::Intersect(const HawkCapsule& oCapsule,Vec3IntrResult* pResult) const
	{
		Float t[2] = {0};

		HawkVector3D U, V, W = oCapsule.Segment.GetDirection();
		GenComplementBasis(U, V, W);
		Float rSqr   = oCapsule.Radius * oCapsule.Radius;
		Float extent = oCapsule.Segment.GetExtent();

		// Convert incoming line origin to capsule coordinates.
		HawkVector3D diff = Point - oCapsule.Segment.GetCenter();
		HawkVector3D P(U.DotProduct(diff), V.DotProduct(diff), W.DotProduct(diff));

		// Get the z-value, in capsule coordinates, of the incoming line's
		// unit-length direction.
		Float dz = W.DotProduct(Direction);
		if (HawkMath::Abs<Float>(dz) >= 1.0f - HawkMath::FLOAT_DIFF)
		{
			// The line is parallel to the capsule axis.  Determine whether the
			// line intersects the capsule hemispheres.
			Float radialSqrDist = rSqr - P.X*P.X - P.Y*P.Y;
			if (radialSqrDist < 0.0f)
			{
				// Line outside the cylinder of the capsule, no intersection.
				return false;
			}

			// line intersects the hemispherical caps
			Float zOffset = HawkMath::Sqrt(radialSqrDist) + extent;
			if (dz > 0.0f)
			{
				t[0] = -P.Z - zOffset;
				t[1] = -P.Z + zOffset;
			}
			else
			{
				t[0] = P.Z - zOffset;
				t[1] = P.Z + zOffset;
			}

			//2交点
			if (pResult)
			{
				pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
				pResult->Point  = Point + Direction * pResult->Factor;
			}
			return true;
		}

		// Convert incoming line unit-length direction to capsule coordinates.
		HawkVector3D D(U.DotProduct(Direction), V.DotProduct(Direction), dz);

		// Test intersection of line P+t*D with infinite cylinder x^2+y^2 = r^2.
		// This reduces to computing the roots of a quadratic equation.  If
		// P = (px,py,pz) and D = (dx,dy,dz), then the quadratic equation is
		//   (dx^2+dy^2)*t^2 + 2*(px*dx+py*dy)*t + (px^2+py^2-r^2) = 0
		Float a0 = P.X*P.X + P.Y*P.Y - rSqr;
		Float a1 = P.X*D.X + P.Y*D.Y;
		Float a2 = D.X*D.X + D.Y*D.Y;
		Float discr = a1*a1 - a0*a2;
		if (discr < (Float)0)
		{
			// Line does not intersect infinite cylinder.
			return false;
		}

		Float root, inv, tValue, zValue;
		int quantity = 0;
		if (discr > HawkMath::FLOAT_DIFF)
		{
			// Line intersects infinite cylinder in two places.
			root = HawkMath::Sqrt(discr);
			inv = ((Float)1)/a2;
			tValue = (-a1 - root)*inv;
			zValue = P.Z + tValue*D.Z;
			if (HawkMath::Abs<Float>(zValue) <= extent)
			{
				t[quantity++] = tValue;
			}

			tValue = (-a1 + root)*inv;
			zValue = P.Z + tValue*D.Z;
			if (HawkMath::Abs<Float>(zValue) <= extent)
			{
				t[quantity++] = tValue;
			}

			if (quantity == 2)
			{
				//2交点
				if (pResult)
				{
					pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
					pResult->Point  = Point + Direction * pResult->Factor;
				}
				return true;
			}
		}
		else
		{
			// Line is tangent to infinite cylinder.
			tValue = -a1/a2;
			zValue = P.Z + tValue*D.Z;
			if (HawkMath::Abs<Float>(zValue) <= extent)
			{
				t[0] = tValue;

				//1交点
				if (pResult)
				{
					pResult->Factor = t[0];
					pResult->Point  = Point + Direction * pResult->Factor;
				}
				return true;
			}
		}

		// Test intersection with bottom hemisphere.  The quadratic equation is
		//   t^2 + 2*(px*dx+py*dy+(pz+e)*dz)*t + (px^2+py^2+(pz+e)^2-r^2) = 0
		// Use the fact that currently a1 = px*dx+py*dy and a0 = px^2+py^2-r^2.
		// The leading coefficient is a2 = 1, so no need to include in the
		// construction.
		Float PZpE = P.Z + extent;
		a1 += PZpE*D.Z;
		a0 += PZpE*PZpE;
		discr = a1*a1 - a0;
		if (discr > HawkMath::FLOAT_DIFF)
		{
			root = HawkMath::Sqrt(discr);
			tValue = -a1 - root;
			zValue = P.Z + tValue*D.Z;
			if (zValue <= -extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}

			tValue = -a1 + root;
			zValue = P.Z + tValue*D.Z;
			if (zValue <= -extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}
		}
		else if (HawkMath::Abs<Float>(discr) <= HawkMath::FLOAT_DIFF)
		{
			tValue = -a1;
			zValue = P.Z + tValue*D.Z;
			if (zValue <= -extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}
		}

		// Test intersection with top hemisphere.  The quadratic equation is
		//   t^2 + 2*(px*dx+py*dy+(pz-e)*dz)*t + (px^2+py^2+(pz-e)^2-r^2) = 0
		// Use the fact that currently a1 = px*dx+py*dy+(pz+e)*dz and
		// a0 = px^2+py^2+(pz+e)^2-r^2.  The leading coefficient is a2 = 1, so
		// no need to include in the construction.
		a1 -= ((Float)2)*extent*D.Z;
		a0 -= ((Float)4)*extent*P.Z;
		discr = a1*a1 - a0;
		if (discr > HawkMath::FLOAT_DIFF)
		{
			root = HawkMath::Sqrt(discr);
			tValue = -a1 - root;
			zValue = P.Z + tValue*D.Z;
			if (zValue >= extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}

			tValue = -a1 + root;
			zValue = P.Z + tValue*D.Z;
			if (zValue >= extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}
		}
		else if (HawkMath::Abs<Float>(discr) <= HawkMath::FLOAT_DIFF)
		{
			tValue = -a1;
			zValue = P.Z + tValue*D.Z;
			if (zValue >= extent)
			{
				t[quantity++] = tValue;
				if (quantity == 2)
				{
					if (t[0] > t[1])
					{
						Float save = t[0];
						t[0] = t[1];
						t[1] = save;
					}

					//2交点
					if (pResult)
					{
						pResult->Factor = HawkMath::Abs<Float>(t[0]) < HawkMath::Abs<Float>(t[1])? t[0] : t[1];
						pResult->Point  = Point + Direction * pResult->Factor;
					}
					return true;
				}
			}
		}

		return false;
	}

}
