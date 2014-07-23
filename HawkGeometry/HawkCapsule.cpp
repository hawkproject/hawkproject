#include "HawkCapsule.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"
#include "HawkPlane3D.h"
#include "HawkSegment3D.h"
#include "HawkCapsule.h"

namespace Hawk
{
	HawkCapsule::HawkCapsule() : Radius(0)
	{

	}

	HawkCapsule::~HawkCapsule()
	{

	}

	HawkCapsule::HawkCapsule(const HawkSegment3D& oSegment, Float fRadius)
	{
		Segment = oSegment;
		Radius  = fRadius;
	}

	Bool  HawkCapsule::Intersect(const HawkCapsule& oCapsule) const
	{
		Float distance = Segment.GetDistance(oCapsule.Segment);
		Float rSum     = Radius + oCapsule.Radius;
		return distance <= rSum;
	}

	Bool  HawkCapsule::Intersect(const HawkPlane3D& oPlane) const
	{
		Float pDist = oPlane.GetDistance(Segment.End);
		Float nDist = oPlane.GetDistance(Segment.Start);
		if (pDist*nDist <= HawkMath::FLOAT_DIFF)
			return true;

		return HawkMath::Abs<Float>(pDist) <= Radius || HawkMath::Abs<Float>(nDist) <= Radius;
	}
}
