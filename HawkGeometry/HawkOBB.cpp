#include "HawkOBB.h"
#include "HawkSphere.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"
#include "HawkPlane3D.h"
#include "HawkTriangle3D.h"
#include "HawkVector3D.h"

namespace Hawk
{
	HawkOBB::HawkOBB() : XAxis(1,0,0),YAxis(0,1,0),ZAxis(0,0,1),Center(0,0,0),Extent(0,0,0)
	{

	}

	HawkOBB::~HawkOBB()
	{

	}

	HawkVector3D* HawkOBB::GetAxis()
	{
		return &XAxis;
	}

	const HawkVector3D* HawkOBB::GetAxis() const
	{
		return &XAxis;
	}

	void  HawkOBB::GetProjection(const HawkVector3D& vAxis,Float& fMin,Float& fMax) const
	{
		Float origin = vAxis.DotProduct(Center);
		Float maximumExtent =
			HawkMath::Abs<Float>(Extent[0]*vAxis.DotProduct(XAxis)) +
			HawkMath::Abs<Float>(Extent[1]*vAxis.DotProduct(YAxis)) +
			HawkMath::Abs<Float>(Extent[2]*vAxis.DotProduct(ZAxis));

		fMin = origin - maximumExtent;
		fMax = origin + maximumExtent;
	}

	Bool  HawkOBB::Intersect(const HawkOBB& oBox) const
	{
		const Float cutoff = (Float)1 - HawkMath::FLOAT_DIFF;
		bool existsParallelPair = false;
		int i;

		// Convenience variables.
		const HawkVector3D* A = GetAxis();
		const HawkVector3D* B = oBox.GetAxis();
		const Float* EA = Extent.Ptr();
		const Float* EB = oBox.Extent.Ptr();

		// Compute difference of box centers, D = C1-C0.
		HawkVector3D D = oBox.Center - Center;

		Float C[3][3];     // matrix C = A^T B, c_{ij} = Dot(A_i,B_j)
		Float AbsC[3][3];  // |c_{ij}|
		Float AD[3];       // Dot(A_i,D)
		Float r0, r1, r;   // interval radii and distance between centers
		Float r01;         // = R0 + R1

		// axis C0+t*A0
		for (i = 0; i < 3; ++i)
		{
			C[0][i] = A[0].DotProduct(B[i]);
			AbsC[0][i] = HawkMath::Abs<Float>(C[0][i]);
			if (AbsC[0][i] > cutoff)
			{
				existsParallelPair = true;
			}
		}
		AD[0] = A[0].DotProduct(D);
		r = HawkMath::Abs<Float>(AD[0]);
		r1 = EB[0]*AbsC[0][0] + EB[1]*AbsC[0][1] + EB[2]*AbsC[0][2];
		r01 = EA[0] + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A1
		for (i = 0; i < 3; ++i)
		{
			C[1][i] = A[1].DotProduct(B[i]);
			AbsC[1][i] = HawkMath::Abs<Float>(C[1][i]);
			if (AbsC[1][i] > cutoff)
			{
				existsParallelPair = true;
			}
		}
		AD[1] = A[1].DotProduct(D);
		r = HawkMath::Abs<Float>(AD[1]);
		r1 = EB[0]*AbsC[1][0] + EB[1]*AbsC[1][1] + EB[2]*AbsC[1][2];
		r01 = EA[1] + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A2
		for (i = 0; i < 3; ++i)
		{
			C[2][i] = A[2].DotProduct(B[i]);
			AbsC[2][i] = HawkMath::Abs<Float>(C[2][i]);
			if (AbsC[2][i] > cutoff)
			{
				existsParallelPair = true;
			}
		}
		AD[2] = A[2].DotProduct(D);
		r = HawkMath::Abs<Float>(AD[2]);
		r1 = EB[0]*AbsC[2][0] + EB[1]*AbsC[2][1] + EB[2]*AbsC[2][2];
		r01 = EA[2] + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*B0
		r = HawkMath::Abs<Float>(B[0].DotProduct(D));
		r0 = EA[0]*AbsC[0][0] + EA[1]*AbsC[1][0] + EA[2]*AbsC[2][0];
		r01 = r0 + EB[0];
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*B1
		r = HawkMath::Abs<Float>(B[1].DotProduct(D));
		r0 = EA[0]*AbsC[0][1] + EA[1]*AbsC[1][1] + EA[2]*AbsC[2][1];
		r01 = r0 + EB[1];
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*B2
		r = HawkMath::Abs<Float>(B[2].DotProduct(D));
		r0 = EA[0]*AbsC[0][2] + EA[1]*AbsC[1][2] + EA[2]*AbsC[2][2];
		r01 = r0 + EB[2];
		if (r > r01)
		{
			return false;
		}

		// At least one pair of box axes was parallel, so the separation is
		// effectively in 2D where checking the "edge" normals is sufficient for
		// the separation of the boxes.
		if (existsParallelPair)
		{
			return true;
		}

		// axis C0+t*A0xB0
		r = HawkMath::Abs<Float>(AD[2]*C[1][0] - AD[1]*C[2][0]);
		r0 = EA[1]*AbsC[2][0] + EA[2]*AbsC[1][0];
		r1 = EB[1]*AbsC[0][2] + EB[2]*AbsC[0][1];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A0xB1
		r = HawkMath::Abs<Float>(AD[2]*C[1][1] - AD[1]*C[2][1]);
		r0 = EA[1]*AbsC[2][1] + EA[2]*AbsC[1][1];
		r1 = EB[0]*AbsC[0][2] + EB[2]*AbsC[0][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A0xB2
		r = HawkMath::Abs<Float>(AD[2]*C[1][2] - AD[1]*C[2][2]);
		r0 = EA[1]*AbsC[2][2] + EA[2]*AbsC[1][2];
		r1 = EB[0]*AbsC[0][1] + EB[1]*AbsC[0][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A1xB0
		r = HawkMath::Abs<Float>(AD[0]*C[2][0] - AD[2]*C[0][0]);
		r0 = EA[0]*AbsC[2][0] + EA[2]*AbsC[0][0];
		r1 = EB[1]*AbsC[1][2] + EB[2]*AbsC[1][1];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A1xB1
		r = HawkMath::Abs<Float>(AD[0]*C[2][1] - AD[2]*C[0][1]);
		r0 = EA[0]*AbsC[2][1] + EA[2]*AbsC[0][1];
		r1 = EB[0]*AbsC[1][2] + EB[2]*AbsC[1][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A1xB2
		r = HawkMath::Abs<Float>(AD[0]*C[2][2] - AD[2]*C[0][2]);
		r0 = EA[0]*AbsC[2][2] + EA[2]*AbsC[0][2];
		r1 = EB[0]*AbsC[1][1] + EB[1]*AbsC[1][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A2xB0
		r = HawkMath::Abs<Float>(AD[1]*C[0][0] - AD[0]*C[1][0]);
		r0 = EA[0]*AbsC[1][0] + EA[1]*AbsC[0][0];
		r1 = EB[1]*AbsC[2][2] + EB[2]*AbsC[2][1];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A2xB1
		r = HawkMath::Abs<Float>(AD[1]*C[0][1] - AD[0]*C[1][1]);
		r0 = EA[0]*AbsC[1][1] + EA[1]*AbsC[0][1];
		r1 = EB[0]*AbsC[2][2] + EB[2]*AbsC[2][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		// axis C0+t*A2xB2
		r = HawkMath::Abs<Float>(AD[1]*C[0][2] - AD[0]*C[1][2]);
		r0 = EA[0]*AbsC[1][2] + EA[1]*AbsC[0][2];
		r1 = EB[0]*AbsC[2][1] + EB[1]*AbsC[2][0];
		r01 = r0 + r1;
		if (r > r01)
		{
			return false;
		}

		return true;
	}

	Bool  HawkOBB::Intersect(const HawkSphere& oSphere) const
	{
		HawkVector3D cdiff = oSphere.Center - Center;
		const HawkVector3D* Axis = GetAxis();

		Float ax = HawkMath::Abs<Float>(cdiff.DotProduct(Axis[0]));
		Float ay = HawkMath::Abs<Float>(cdiff.DotProduct(Axis[1]));
		Float az = HawkMath::Abs<Float>(cdiff.DotProduct(Axis[2]));
		Float dx = ax - Extent[0];
		Float dy = ay - Extent[1];
		Float dz = az - Extent[2];

		if (ax <= Extent[0])
		{
			if (ay <= Extent[1])
			{
				if (az <= Extent[2])
				{
					// Sphere center inside box.
					return true;
				}
				else
				{
					// Potential sphere-face intersection with face z.
					return dz <= oSphere.Radius;
				}
			}
			else
			{
				if (az <= Extent[2])
				{
					// Potential sphere-face intersection with face y.
					return dy <= oSphere.Radius;
				}
				else
				{
					// Potential sphere-edge intersection with edge formed
					// by faces y and z.
					Float rsqr = oSphere.Radius*oSphere.Radius;
					return dy*dy + dz*dz <= rsqr;
				}
			}
		}
		else
		{
			if (ay <= Extent[1])
			{
				if (az <= Extent[2])
				{
					// Potential sphere-face intersection with face x.
					return dx <= oSphere.Radius;
				}
				else
				{
					// Potential sphere-edge intersection with edge formed
					// by faces x and z.
					Float rsqr = oSphere.Radius*oSphere.Radius;
					return dx*dx + dz*dz <= rsqr;
				}
			}
			else
			{
				if (az <= Extent[2])
				{
					// Potential sphere-edge intersection with edge formed
					// by faces x and y.
					Float rsqr = oSphere.Radius*oSphere.Radius;
					return dx*dx + dy*dy <= rsqr;
				}
				else
				{
					// Potential sphere-vertex intersection at corner formed
					// by faces x,y,z.
					Float rsqr = oSphere.Radius*oSphere.Radius;
					return dx*dx + dy*dy + dz*dz <= rsqr;
				}
			}
		}
	}

	Bool  HawkOBB::Intersect(const HawkTriangle3D& oTriangle) const
	{
		Float min0, max0, min1, max1;
		HawkVector3D D, edge[3];

		const HawkVector3D* Axis = GetAxis();

		// Test direction of triangle normal.
		edge[0] = oTriangle[1] - oTriangle[0];
		edge[1] = oTriangle[2] - oTriangle[0];
		D = edge[0].CrossProduct(edge[1]);
		min0 = D.DotProduct(oTriangle[0]);
		max0 = min0;
		GetProjection(D,min1, max1);
		if (max1 < min0 || max0 < min1)
		{
			return false;
		}

		// Test direction of box faces.
		for (int i = 0; i < 3; ++i)
		{
			D = Axis[i];
			oTriangle.GetProjection(D,min0, max0);
			Float DdC = D.DotProduct(Center);
			min1 = DdC - Extent[i];
			max1 = DdC + Extent[i];
			if (max1 < min0 || max0 < min1)
			{
				return false;
			}
		}

		// Test direction of triangle-box edge cross products.
		edge[2] = edge[1] - edge[0];
		for (int i0 = 0; i0 < 3; ++i0)
		{
			for (int i1 = 0; i1 < 3; ++i1)
			{
				D = edge[i0].CrossProduct(Axis[i1]);
				oTriangle.GetProjection(D,min0, max0);
				GetProjection(D,min1, max1);
				if (max1 < min0 || max0 < min1)
				{
					return false;
				}
			}
		}

		return true;
	}

	Bool  HawkOBB::Intersect(const HawkPlane3D& oPlane) const
	{
		const HawkVector3D* Axis = GetAxis();

		Float tmp[3] =
		{
			Extent[0]*(oPlane.Normal.DotProduct(Axis[0])),
			Extent[1]*(oPlane.Normal.DotProduct(Axis[1])),
			Extent[2]*(oPlane.Normal.DotProduct(Axis[2]))
		};

		Float radius = HawkMath::Abs<Float>(tmp[0]) + HawkMath::Abs<Float>(tmp[1]) +
			HawkMath::Abs<Float>(tmp[2]);

		Float signedDistance = oPlane.GetDistance(Center);
		return HawkMath::Abs<Float>(signedDistance) <= radius;
	}
}
