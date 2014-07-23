#ifndef HAWK_CAPSULE_H
#define HAWK_CAPSULE_H

#include "HawkGeoDef.h"
#include "HawkSegment3D.h"

namespace Hawk
{
	/************************************************************************/
	/* �������װ                                                           */
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
		//�ཻ�ж�
		Bool  Intersect(const HawkCapsule& oCapsule) const;
		Bool  Intersect(const HawkPlane3D& oPlane) const;
	};
}
#endif
