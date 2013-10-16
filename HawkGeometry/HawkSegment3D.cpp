#include "HawkSegment3D.h"
#include "HawkVector3D.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"

namespace Hawk
{
	//构造
	HawkSegment3D::HawkSegment3D() : Start(HawkVector3D()),End(HawkVector3D())
	{
	}

	//构造
	HawkSegment3D::HawkSegment3D(const HawkVector3D& oStart,const HawkVector3D& oEnd)
	{
		Set(oStart,oEnd);
	}

	//拷贝构造
	HawkSegment3D::HawkSegment3D(const HawkSegment3D& oSegment) : Start(oSegment.Start),End(oSegment.End)
	{
	}

	//设置线的点数据
	void  HawkSegment3D::Set(const HawkVector3D& oStart,const HawkVector3D& oEnd)
	{
		Start	= oStart;
		End		= oEnd;
	}
	
	//长度
	Float HawkSegment3D::GetLength() const
	{
		return (End - Start).Length();
	}

	Float HawkSegment3D::GetExtent() const
	{
		return GetLength() * 0.5f;
	}

	HawkVector3D HawkSegment3D::GetCenter() const
	{
		return (Start + End) * 0.5f;
	}

	//获得方向
	HawkVector3D HawkSegment3D::GetDirection() const
	{
		return (End - Start).Normalize();
	}

	//判读点是否在线上面(各种线的特性需要考虑)
	Bool  HawkSegment3D::IsPointOnSegment(const HawkVector3D& oVec) const
	{
		return (oVec - Start).Normalize() == (End - oVec).Normalize();
	}

	//各种运算符的重载
	HawkSegment3D HawkSegment3D::operator + (const HawkVector3D& oVec) const 
	{ 
		return HawkSegment3D(Start+oVec,End+oVec); 
	}

	HawkSegment3D& HawkSegment3D::operator += (const HawkVector3D& oVec) 
	{ 
		Start += oVec; 
		End   += oVec; 
		return *this; 
	}

	HawkSegment3D HawkSegment3D::operator - (const HawkVector3D& oVec) const 
	{ 
		return HawkSegment3D(Start-oVec,End-oVec); 
	}

	HawkSegment3D& HawkSegment3D::operator -= (const HawkVector3D& oVec) 
	{ 
		Start -= oVec; 
		End   -= oVec; 
		return *this; 
	}

	Bool HawkSegment3D::operator == (const HawkSegment3D& oSegment) const
	{ 
		return ((Start==oSegment.Start) && (End == oSegment.End)) ||
			((Start==oSegment.End) && (End == oSegment.Start));
	}

	Bool HawkSegment3D::operator != (const HawkSegment3D& oSegment) const
	{
		return !(*this == oSegment);
	}

	Float HawkSegment3D::GetDistance(const HawkSegment3D& oSegment) const
	{
		HawkVector3D diff = GetCenter() -oSegment.GetCenter();
		Float a01 = -GetDirection().DotProduct(oSegment.GetDirection());
		Float b0 = diff.DotProduct(GetDirection());
		Float b1 = -diff.DotProduct(oSegment.GetDirection());
		Float c = diff.SQLength();
		Float det =HawkMath::Abs<Float>(1.0f - a01*a01);
		Float s0, s1, sqrDist, extDet0, extDet1, tmpS0, tmpS1;

		if (det >= HawkMath::FLOAT_DIFF)
		{
			// Segments are not parallel.
			s0 = a01*b1 - b0;
			s1 = a01*b0 - b1;
			extDet0 = GetExtent()*det;
			extDet1 = oSegment.GetExtent()*det;

			if (s0 >= -extDet0)
			{
				if (s0 <= extDet0)
				{
					if (s1 >= -extDet1)
					{
						if (s1 <= extDet1)  // region 0 (interior)
						{
							// Minimum at interior points of segments.
							Float invDet = ((Float)1)/det;
							s0 *= invDet;
							s1 *= invDet;
							sqrDist = s0*(s0 + a01*s1 + ((Float)2)*b0) +
								s1*(a01*s0 + s1 + ((Float)2)*b1) + c;
						}
						else  // region 3 (side)
						{
							s1 = oSegment.GetExtent();
							tmpS0 = -(a01*s1 + b0);
							if (tmpS0 < -GetExtent())
							{
								s0 = -GetExtent();
								sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
									s1*(s1 + ((Float)2)*b1) + c;
							}
							else if (tmpS0 <= GetExtent())
							{
								s0 = tmpS0;
								sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
							}
							else
							{
								s0 = GetExtent();
								sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
									s1*(s1 + ((Float)2)*b1) + c;
							}
						}
					}
					else  // region 7 (side)
					{
						s1 = -oSegment.GetExtent();
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 < -GetExtent())
						{
							s0 = -GetExtent();
							sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
								s1*(s1 + ((Float)2)*b1) + c;
						}
						else if (tmpS0 <= GetExtent())
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
						}
						else
						{
							s0 = GetExtent();
							sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
								s1*(s1 + ((Float)2)*b1) + c;
						}
					}
				}
				else
				{
					if (s1 >= -extDet1)
					{
						if (s1 <= extDet1)  // region 1 (side)
						{
							s0 = GetExtent();
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 < -oSegment.GetExtent())
							{
								s1 = -oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
							else if (tmpS1 <= oSegment.GetExtent())
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
							}
							else
							{
								s1 = oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
						}
						else  // region 2 (corner)
						{
							s1 = oSegment.GetExtent();
							tmpS0 = -(a01*s1 + b0);
							if (tmpS0 < -GetExtent())
							{
								s0 = -GetExtent();
								sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
									s1*(s1 + ((Float)2)*b1) + c;
							}
							else if (tmpS0 <= GetExtent())
							{
								s0 = tmpS0;
								sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
							}
							else
							{
								s0 = GetExtent();
								tmpS1 = -(a01*s0 + b1);
								if (tmpS1 < -oSegment.GetExtent())
								{
									s1 = -oSegment.GetExtent();
									sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
										s0*(s0 + ((Float)2)*b0) + c;
								}
								else if (tmpS1 <= oSegment.GetExtent())
								{
									s1 = tmpS1;
									sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
								}
								else
								{
									s1 = oSegment.GetExtent();
									sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
										s0*(s0 + ((Float)2)*b0) + c;
								}
							}
						}
					}
					else  // region 8 (corner)
					{
						s1 = -oSegment.GetExtent();
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 < -GetExtent())
						{
							s0 = -GetExtent();
							sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
								s1*(s1 + ((Float)2)*b1) + c;
						}
						else if (tmpS0 <= GetExtent())
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
						}
						else
						{
							s0 = GetExtent();
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 > oSegment.GetExtent())
							{
								s1 = oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
							else if (tmpS1 >= -oSegment.GetExtent())
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
							}
							else
							{
								s1 = -oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
						}
					}
				}
			}
			else 
			{
				if (s1 >= -extDet1)
				{
					if (s1 <= extDet1)  // region 5 (side)
					{
						s0 = -GetExtent();
						tmpS1 = -(a01*s0 + b1);
						if (tmpS1 < -oSegment.GetExtent())
						{
							s1 = -oSegment.GetExtent();
							sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
								s0*(s0 + ((Float)2)*b0) + c;
						}
						else if (tmpS1 <= oSegment.GetExtent())
						{
							s1 = tmpS1;
							sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
						}
						else
						{
							s1 = oSegment.GetExtent();
							sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
								s0*(s0 + ((Float)2)*b0) + c;
						}
					}
					else  // region 4 (corner)
					{
						s1 = oSegment.GetExtent();
						tmpS0 = -(a01*s1 + b0);
						if (tmpS0 > GetExtent())
						{
							s0 = GetExtent();
							sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
								s1*(s1 + ((Float)2)*b1) + c;
						}
						else if (tmpS0 >= -GetExtent())
						{
							s0 = tmpS0;
							sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
						}
						else
						{
							s0 = -GetExtent();
							tmpS1 = -(a01*s0 + b1);
							if (tmpS1 < -oSegment.GetExtent())
							{
								s1 = -oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
							else if (tmpS1 <= oSegment.GetExtent())
							{
								s1 = tmpS1;
								sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
							}
							else
							{
								s1 = oSegment.GetExtent();
								sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
									s0*(s0 + ((Float)2)*b0) + c;
							}
						}
					}
				}
				else   // region 6 (corner)
				{
					s1 = -oSegment.GetExtent();
					tmpS0 = -(a01*s1 + b0);
					if (tmpS0 > GetExtent())
					{
						s0 = GetExtent();
						sqrDist = s0*(s0 - ((Float)2)*tmpS0) +
							s1*(s1 + ((Float)2)*b1) + c;
					}
					else if (tmpS0 >= -GetExtent())
					{
						s0 = tmpS0;
						sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
					}
					else
					{
						s0 = -GetExtent();
						tmpS1 = -(a01*s0 + b1);
						if (tmpS1 < -oSegment.GetExtent())
						{
							s1 = -oSegment.GetExtent();
							sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
								s0*(s0 + ((Float)2)*b0) + c;
						}
						else if (tmpS1 <= oSegment.GetExtent())
						{
							s1 = tmpS1;
							sqrDist = -s1*s1 + s0*(s0 + ((Float)2)*b0) + c;
						}
						else
						{
							s1 = oSegment.GetExtent();
							sqrDist = s1*(s1 - ((Float)2)*tmpS1) +
								s0*(s0 + ((Float)2)*b0) + c;
						}
					}
				}
			}
		}
		else
		{
			// The segments are parallel.  The average b0 term is designed to
			// ensure symmetry of the function.  That is, dist(seg0,seg1) and
			// dist(seg1,seg0) should produce the same number.
			Float e0pe1 = GetExtent() + oSegment.GetExtent();
			Float sign = (a01 > (Float)0 ? (Float)-1 : (Float)1);
			Float b0Avr = ((Float)0.5)*(b0 - sign*b1);
			Float lambda = -b0Avr;
			if (lambda < -e0pe1)
			{
				lambda = -e0pe1;
			}
			else if (lambda > e0pe1)
			{
				lambda = e0pe1;
			}

			s1 = -sign*lambda*oSegment.GetExtent()/e0pe1;
			s0 = lambda + sign*s1;
			sqrDist = lambda*(lambda + ((Float)2)*b0Avr) + c;
		}

		HawkVector3D vClosestPt0 = GetCenter() + s0*GetDirection();
		HawkVector3D vClosestPt1 = oSegment.GetCenter() + s1*oSegment.GetDirection();
		Float fSegment0Parameter = 0, fSegment1Parameter = 0;
		fSegment0Parameter = s0;
		fSegment1Parameter = s1;
		
		// Account for numerical round-off errors.
		if (sqrDist < (Float)0)
			sqrDist = (Float)0;

		return HawkMath::Sqrt(sqrDist);
	}

	Float HawkSegment3D::GetDistance(const HawkLine3D& oLine) const
	{
		HawkVector3D diff = oLine.Point - GetCenter();
		Float a01 = -oLine.Direction.DotProduct(GetDirection());
		Float b0 = diff.DotProduct(oLine.Direction);
		Float c = diff.SQLength();
		Float det = HawkMath::Abs<Float>(1.0f - a01*a01);
		Float b1, s0, s1, sqrDist, extDet;

		if (det >= HawkMath::FLOAT_DIFF)
		{
			// The line and segment are not parallel.
			b1 = -diff.DotProduct(GetDirection());
			s1 = a01*b0 - b1;
			extDet = GetExtent()*det;

			if (s1 >= -extDet)
			{
				if (s1 <= extDet)
				{
					// Two interior points are closest, one on the line and one
					// on the segment.
					Float invDet = ((Float)1)/det;
					s0 = (a01*b1 - b0)*invDet;
					s1 *= invDet;
					sqrDist = s0*(s0 + a01*s1 + ((Float)2)*b0) +
						s1*(a01*s0 + s1 + ((Float)2)*b1) + c;
				}
				else
				{
					// The endpoint e1 of the segment and an interior point of
					// the line are closest.
					s1 = GetExtent();
					s0 = -(a01*s1 + b0);
					sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
				}
			}
			else
			{
				// The end point e0 of the segment and an interior point of the
				// line are closest.
				s1 = -GetExtent();
				s0 = -(a01*s1 + b0);
				sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
			}
		}
		else
		{
			// The line and segment are parallel.  Choose the closest pair so that
			// one point is at segment center.
			s1 = (Float)0;
			s0 = -b0;
			sqrDist = b0*s0 + c;
		}

		HawkVector3D vClosestPt0 = oLine.Point + s0*oLine.Direction;
		HawkVector3D vClosestPt1 = GetCenter() + s1*GetDirection();
		Float fSegment0Parameter = 0, fSegment1Parameter = 0;
		fSegment0Parameter = s0;
		fSegment1Parameter = s1;

		// Account for numerical round-off errors.
		if (sqrDist < HawkMath::FLOAT_DIFF)
			sqrDist = 0;

		return HawkMath::Sqrt(sqrDist);
	}

	Float HawkSegment3D::GetDistance(const Hawk::HawkRay3D &oRay) const
	{
		HawkVector3D diff = oRay.Point - GetCenter();
		Float a01 = -oRay.Direction.DotProduct(GetDirection());
		Float b0 = diff.DotProduct(oRay.Direction);
		Float b1 = -diff.DotProduct(GetDirection());
		Float c = diff.SQLength();
		Float det = HawkMath::Abs<Float>((Float)1.0f - a01*a01);
		Float s0, s1, sqrDist, extDet;

		if (det >= HawkMath::FLOAT_DIFF)
		{
			// The ray and segment are not parallel.
			s0 = a01*b1 - b0;
			s1 = a01*b0 - b1;
			extDet = GetExtent()*det;

			if (s0 >= (Float)0)
			{
				if (s1 >= -extDet)
				{
					if (s1 <= extDet)  // region 0
					{
						// Minimum at interior points of ray and segment.
						Float invDet = ((Float)1)/det;
						s0 *= invDet;
						s1 *= invDet;
						sqrDist = s0*(s0 + a01*s1 + ((Float)2)*b0) +
							s1*(a01*s0 + s1 + ((Float)2)*b1) + c;
					}
					else  // region 1
					{
						s1 = GetExtent();
						s0 = -(a01*s1 + b0);
						if (s0 > (Float)0)
						{
							sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
						}
						else
						{
							s0 = (Float)0;
							sqrDist = s1*(s1 + ((Float)2)*b1) + c;
						}
					}
				}
				else  // region 5
				{
					s1 = -GetExtent();
					s0 = -(a01*s1 + b0);
					if (s0 > (Float)0)
					{
						sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
					}
					else
					{
						s0 = (Float)0;
						sqrDist = s1*(s1 + ((Float)2)*b1) + c;
					}
				}
			}
			else
			{
				if (s1 <= -extDet)  // region 4
				{
					s0 = -(-a01*GetExtent() + b0);
					if (s0 > (Float)0)
					{
						s1 = -GetExtent();
						sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
					}
					else
					{
						s0 = (Float)0;
						s1 = -b1;
						if (s1 < -GetExtent())
						{
							s1 = -GetExtent();
						}
						else if (s1 > GetExtent())
						{
							s1 = GetExtent();
						}
						sqrDist = s1*(s1 + ((Float)2)*b1) + c;
					}
				}
				else if (s1 <= extDet)  // region 3
				{
					s0 = (Float)0;
					s1 = -b1;
					if (s1 < -GetExtent())
					{
						s1 = -GetExtent();
					}
					else if (s1 > GetExtent())
					{
						s1 = GetExtent();
					}
					sqrDist = s1*(s1 + ((Float)2)*b1) + c;
				}
				else  // region 2
				{
					s0 = -(a01*GetExtent() + b0);
					if (s0 > (Float)0)
					{
						s1 = GetExtent();
						sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
					}
					else
					{
						s0 = (Float)0;
						s1 = -b1;
						if (s1 < -GetExtent())
						{
							s1 = -GetExtent();
						}
						else if (s1 > GetExtent())
						{
							s1 = GetExtent();
						}
						sqrDist = s1*(s1 + ((Float)2)*b1) + c;
					}
				}
			}
		}
		else
		{
			// Ray and segment are parallel.
			if (a01 > (Float)0)
			{
				// Opposite direction vectors.
				s1 = -GetExtent();
			}
			else
			{
				// Same direction vectors.
				s1 = GetExtent();
			}

			s0 = -(a01*s1 + b0);
			if (s0 > (Float)0)
			{
				sqrDist = -s0*s0 + s1*(s1 + ((Float)2)*b1) + c;
			}
			else
			{
				s0 = (Float)0;
				sqrDist = s1*(s1 + ((Float)2)*b1) + c;
			}
		}

		HawkVector3D vClosestPt0 = oRay.Point + s0*oRay.Direction;
		HawkVector3D vClosestPt1 = GetCenter() + s1*GetDirection();
		Float fSegment0Parameter = 0, fSegment1Parameter = 0;
		fSegment0Parameter = s0;
		fSegment1Parameter = s1;

		if (sqrDist < HawkMath::FLOAT_DIFF)
			sqrDist = 0;

		return HawkMath::Sqrt(sqrDist);
	}
}


