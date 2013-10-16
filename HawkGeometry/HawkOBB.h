#ifndef HAWK_OBB_H
#define HAWK_OBB_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	class GEO_API HawkOBB
	{
	public:		
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,XAxis,XAxis);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,YAxis,YAxis);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,ZAxis,ZAxis);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Center,Center);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Extent,Extent);

	public:
		HawkOBB();
		~HawkOBB();

	public:
		HawkVector3D* GetAxis();
		const HawkVector3D* GetAxis() const;
		void  GetProjection(const HawkVector3D& vAxis,Float& fMin,Float& fMax) const;

	public:
		//œ‡Ωª≈–∂œ
		Bool  Intersect(const HawkOBB& oBox) const;
		Bool  Intersect(const HawkSphere& oSphere) const;
		Bool  Intersect(const HawkPlane3D& oPlane) const;
		Bool  Intersect(const HawkTriangle3D& oTriangle) const;
	};
}
#endif
