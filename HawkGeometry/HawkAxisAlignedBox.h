#ifndef HAWK_AXISALIGNEDBOX_H
#define HAWK_AXISALIGNEDBOX_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* HawkAxisAlignedBox封装                                               */
	/************************************************************************/
	class GEO_API HawkAxisAlignedBox
	{
	public:
		/*
		6-----7
		/|    /|
		/ |   / |
		2-----3  |
		|  4--|--5
		| /   | /
		|/    |/
		0-----1
		*/
		enum CornerEnum
		{						
			NEAR_LEFT_BOTTOM  = 0,
			NEAR_RIGHT_BOTTOM = 1,
			NEAR_LEFT_TOP     = 2,
			NEAR_RIGHT_TOP    = 3,

			FAR_LEFT_BOTTOM   = 4,
			FAR_RIGHT_BOTTOM  = 5,
			FAR_LEFT_TOP      = 6,
			FAR_RIGHT_TOP     = 7,

			CORNER_NUM        = 8,
		};

		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Minimum,Minimum);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Maximum,Maximum);

	public:
		HawkAxisAlignedBox();

		HawkAxisAlignedBox(const HawkAxisAlignedBox& oAxisAlignedBox);

		HawkAxisAlignedBox(const HawkVector3D& vMin,const HawkVector3D& vMax);

		HawkAxisAlignedBox& operator = (const HawkAxisAlignedBox& oAxisAlignedBox);

		~HawkAxisAlignedBox();

	public:
		//是否为空
		Bool  IsNull() const;
		void  SetZero();

		void  SetCenter(const HawkVector3D& center);	
		void  SetExtents(const HawkVector3D& extents);
						
		void  SetBound(const HawkSphere& oSphere);				

		//合并
		void  Merge(const HawkAxisAlignedBox& oAxisAlignedBox);		
		void  Merge(const HawkVector3D& oVec3);	
		void  Merge(const HawkSphere& oSphere);	

		//变换
		void  Transform(const HawkMatrix4& oMat4);

		//体积
		Float GetVolume() const;

		//缩放
		void  Scale(const HawkVector3D& oScale);		
		
		//包含判断
		Bool  Contain(const HawkVector3D& oVec3) const;

		Bool  Contain(const HawkAxisAlignedBox& oAxisAlignedBox) const;

		Bool  operator == (const HawkAxisAlignedBox& oAxisAlignedBox) const;

		Bool  operator != (const HawkAxisAlignedBox& oAxisAlignedBox) const;

	public:
		HawkVector3D GetCorner(CornerEnum eCorner) const;
		HawkVector3D GetExtents() const;
		HawkVector3D GetCenter(void) const;
		HawkVector3D GetSize(void) const;

	public:
		//相交判断
		Bool  Intersect(const HawkAxisAlignedBox& oAxisAlignedBox) const;
		Bool  Intersect(const HawkSphere& oSphere) const;	
	};
	typedef HawkAxisAlignedBox AABBox;
}
#endif
