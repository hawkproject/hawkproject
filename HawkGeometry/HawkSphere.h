#ifndef HAWK_SPHERE_H
#define HAWK_SPHERE_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 3D球形封装                                                           */
	/************************************************************************/	
	class GEO_API HawkSphere 
	{
	public:
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Center,Center);
		DEFINE_PUBLIC_PROPERTY(Float,Radius,Radius);

	public:
		HawkSphere();
		HawkSphere(const HawkVector3D& vCenter,Float fRadius);

	public:		
		//空间变换
		void Transform(const HawkMatrix4& oMat4);

		void SetZero();

	public:
		Bool Intersect(const HawkSphere& oSphere) const;
		Bool Intersect(const HawkAxisAlignedBox& oAxisAlignedBox) const;
		Bool Intersect(const HawkPlane3D& oPlane) const;		
	};
}
#endif
