#include "HawkAxisAlignedBox.h"
#include "HawkVector3D.h"
#include "HawkMatrix4.h"
#include "HawkSphere.h"
#include "HawkPlane3D.h"
#include "HawkRay3D.h"
#include "HawkLine3D.h"

namespace Hawk
{
	HawkAxisAlignedBox::HawkAxisAlignedBox() : Minimum(-0.5,-0.5,-0.5),Maximum(0.5,0.5,0.5)
	{
	}

	HawkAxisAlignedBox::HawkAxisAlignedBox(const HawkAxisAlignedBox& oAxisAlignedBox) 
		: Minimum(oAxisAlignedBox.Minimum),Maximum(oAxisAlignedBox.Maximum)
	{
	}

	HawkAxisAlignedBox::HawkAxisAlignedBox(const HawkVector3D& vMin, const HawkVector3D& vMax) 
		: Minimum(vMin),Maximum(vMax)
	{
	}

	HawkAxisAlignedBox& HawkAxisAlignedBox::operator = (const HawkAxisAlignedBox& oAxisAlignedBox)
	{
		Minimum = oAxisAlignedBox.Minimum;
		Maximum = oAxisAlignedBox.Maximum;
		return *this;
	}

	HawkAxisAlignedBox::~HawkAxisAlignedBox()
	{
	}

	HawkVector3D HawkAxisAlignedBox::GetCorner(CornerEnum eCorner) const
	{
		switch(eCorner)
		{
		case FAR_LEFT_BOTTOM:
			return Minimum;
		case FAR_LEFT_TOP:
			return HawkVector3D(Minimum.X, Maximum.Y, Minimum.Z);
		case FAR_RIGHT_TOP:
			return HawkVector3D(Maximum.X, Maximum.Y, Minimum.Z);
		case FAR_RIGHT_BOTTOM:
			return HawkVector3D(Maximum.X, Minimum.Y, Minimum.Z);
		case NEAR_RIGHT_BOTTOM:
			return HawkVector3D(Maximum.X, Minimum.Y, Maximum.Z);
		case NEAR_LEFT_BOTTOM:
			return HawkVector3D(Minimum.X, Minimum.Y, Maximum.Z);
		case NEAR_LEFT_TOP:
			return HawkVector3D(Minimum.X, Maximum.Y, Maximum.Z);
		case NEAR_RIGHT_TOP:
			return Maximum;
		default:
			return HawkVector3D();
		}
	}

	void HawkAxisAlignedBox::SetCenter(const HawkVector3D& oCenter)
	{
		Minimum = oCenter - GetExtents();
		Maximum = oCenter + GetExtents();
	}

	void HawkAxisAlignedBox::SetExtents(const HawkVector3D& oExtents)
	{
		HawkAssert(oExtents.X > 0 && oExtents.Y > 0 && oExtents.Z > 0);
		HawkVector3D vCenter = GetCenter();
		Minimum = vCenter - oExtents;
		Maximum = vCenter + oExtents;
	}

	HawkVector3D HawkAxisAlignedBox::GetExtents() const
	{
		return (Maximum - Minimum) * 0.5f;
	}

	void HawkAxisAlignedBox::SetBound(const HawkSphere& oSphere)
	{
		Float fRadius = oSphere.GetRadius();
		HawkVector3D vPt(fRadius, fRadius, fRadius);

		Minimum = oSphere.GetCenter() - vPt;
		Maximum = oSphere.GetCenter() + vPt;
	}

	void HawkAxisAlignedBox::Merge(const HawkSphere& oSphere)
	{
		HawkAxisAlignedBox aabb;
		aabb.SetBound(oSphere);
		Merge(aabb);
	}

	void HawkAxisAlignedBox::Merge(const HawkAxisAlignedBox& oAxisAlignedBox)
	{
		Maximum.MakeCeil(oAxisAlignedBox.Maximum);
		Minimum.MakeFloor(oAxisAlignedBox.Minimum);
	}

	void HawkAxisAlignedBox::Merge(const HawkVector3D& oVec3)
	{
		Maximum.MakeCeil(oVec3);
		Minimum.MakeFloor(oVec3);
	}

	void HawkAxisAlignedBox::Transform(const HawkMatrix4& oMat4)
	{
		HawkVector3D oldMin, oldMax, vCorner;

		oldMin = Minimum;
		oldMax = Maximum;

		vCorner = oldMin;
		Merge(vCorner * oMat4);

		vCorner.Z = oldMax.Z;
		Merge(vCorner * oMat4);

		vCorner.Y = oldMax.Y;
		Merge(vCorner * oMat4);

		vCorner.Z = oldMin.Z;
		Merge(vCorner * oMat4);

		vCorner.X = oldMax.X;
		Merge(vCorner * oMat4);

		vCorner.Z = oldMax.Z;
		Merge(vCorner * oMat4);

		vCorner.Y = oldMin.Y;
		Merge(vCorner * oMat4);

		vCorner.Z = oldMin.Z;
		Merge(vCorner * oMat4); 
	}

	Float HawkAxisAlignedBox::GetVolume() const
	{
		HawkVector3D vDiff = Maximum - Minimum;
		return vDiff.X * vDiff.Y * vDiff.Z;
	}

	void HawkAxisAlignedBox::Scale(const HawkVector3D& oScale)
	{
		Minimum = Minimum*oScale;
		Maximum = Maximum*oScale;
	}

	Bool HawkAxisAlignedBox::IsNull() const
	{
		return GetSize().Length() == 0;
	}

	void HawkAxisAlignedBox::SetZero()
	{
		memset(this,0,sizeof(*this));
	}

	HawkVector3D HawkAxisAlignedBox::GetCenter(void) const
	{
		return (Minimum + Maximum) * 0.5f;
	}

	HawkVector3D HawkAxisAlignedBox::GetSize(void) const
	{
		return Maximum - Minimum;
	}

	Bool HawkAxisAlignedBox::Contain(const HawkVector3D& oVec3) const
	{
		return Minimum.X <= oVec3.X && oVec3.X <= Maximum.X &&
			Minimum.Y <= oVec3.Y && oVec3.Y <= Maximum.Y &&
			Minimum.Z <= oVec3.Z && oVec3.Z <= Maximum.Z;
	}

	Bool HawkAxisAlignedBox::Contain(const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		return Minimum.X <= oAxisAlignedBox.Minimum.X &&
			Minimum.Y <= oAxisAlignedBox.Minimum.Y &&
			Minimum.Z <= oAxisAlignedBox.Minimum.Z &&
			oAxisAlignedBox.Maximum.X <= Maximum.X &&
			oAxisAlignedBox.Maximum.Y <= Maximum.Y &&
			oAxisAlignedBox.Maximum.Z <= Maximum.Z;
	}

	Bool HawkAxisAlignedBox::operator == (const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		return Minimum == oAxisAlignedBox.Minimum && Maximum == oAxisAlignedBox.Maximum;
	}

	Bool HawkAxisAlignedBox::operator != (const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		return !(*this == oAxisAlignedBox);
	}

	//////////////////////////////////////////////////////////////////////////
	Bool HawkAxisAlignedBox::Intersect(const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		if (Maximum.X < oAxisAlignedBox.Minimum.X)
			return false;
		if (Maximum.Y < oAxisAlignedBox.Minimum.Y)
			return false;
		if (Maximum.Z < oAxisAlignedBox.Minimum.Z)
			return false;

		if (Minimum.X > oAxisAlignedBox.Maximum.X)
			return false;
		if (Minimum.Y > oAxisAlignedBox.Maximum.Y)
			return false;
		if (Minimum.Z > oAxisAlignedBox.Maximum.Z)
			return false;

		return true;

	}	

	Bool HawkAxisAlignedBox::Intersect(const HawkSphere& oSphere) const
	{
		if (IsNull()) return false;
		
		//From Ogre
		Float s, d = 0;
		for (Int32 i = 0; i < 3; ++i)
		{
			if (oSphere.Center[i] < Minimum[i])
			{
				s = oSphere.Center[i] - Minimum[i];
				d += s * s; 
			}
			else if(oSphere.Center[i] > Maximum[i])
			{
				s = oSphere.Center[i] - Maximum[i];
				d += s * s; 
			}
		}
		return d <= oSphere.Radius * oSphere.Radius;
	}	
}
