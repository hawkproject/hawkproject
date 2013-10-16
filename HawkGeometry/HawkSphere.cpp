#include "HawkSphere.h"
#include "HawkVector3D.h"
#include "HawkRay3D.h"
#include "HawkAxisAlignedBox.h"
#include "HawkPlane3D.h"
#include "HawkMatrix4.h"
#include "HawkLine3D.h"
#include "HawkVector4D.h"

namespace Hawk
{
	HawkSphere::HawkSphere()
	{
		Center.X = 0;Center.Y = 0;Center.Z = 0;
		Radius = 0;
	}

	HawkSphere::HawkSphere(const HawkVector3D& vCenter,Float fRadius)
	{
		Center = vCenter;
		Radius = fRadius;
	}

	void  HawkSphere::SetZero()
	{
		Center = HawkVector3D(0,0,0);
		Radius = 0;
	}

	void HawkSphere::Transform(const HawkMatrix4& oMat4)
	{
		Center = Center * oMat4;
		HawkVector4D oScale = oMat4.GetColumn(3);
		Float fScale = oScale.X;
		fScale = fScale < oScale.Y? oScale.Y : fScale;
		fScale = fScale < oScale.Z? oScale.Z : fScale;
		Radius = Radius * fScale;
	}

	//////////////////////////////////////////////////////////////////////////
	Bool HawkSphere::Intersect(const HawkSphere& oSphere) const
	{
		return (oSphere.Center - Center).SQLength() <= HawkMath::Sqr(oSphere.Radius + Radius);
	}

	Bool HawkSphere::Intersect(const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		return oAxisAlignedBox.Intersect(*this);
	}

	Bool HawkSphere::Intersect(const HawkPlane3D& oPlane) const
	{
		return oPlane.Intersect(*this);
	}
}

