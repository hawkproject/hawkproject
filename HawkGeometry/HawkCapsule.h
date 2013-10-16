#ifndef HAWK_CAPSULE_H
#define HAWK_CAPSULE_H

#include "HawkGeoDef.h"
#include "HawkSegment3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 胶囊体封装                                                           */
	/************************************************************************/
	class GEO_API HawkCapsule
	{
	public:
		DEFINE_PUBLIC_PROPERTY(HawkSegment3D,Segment,Segment);
		DEFINE_PUBLIC_PROPERTY(Float,Radius,Radius);

	public:
		HawkCapsule();
		~HawkCapsule();
		HawkCapsule(const HawkSegment3D& oSegment, Float fRadius);

	public:
		//相交判断
		Bool  Intersect(const HawkCapsule& oCapsule) const;
		Bool  Intersect(const HawkPlane3D& oPlane) const;
	};
}
#endif
