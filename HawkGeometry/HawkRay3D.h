#ifndef HAWK_RAY3D_H
#define HAWK_RAY3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维空间射线	                                                        */
	/************************************************************************/
	class GEO_API HawkRay3D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Direction,Direction);

	public:
		//默认构造
		HawkRay3D();

		//使用两个点构造
		HawkRay3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//拷贝构造
		HawkRay3D(const HawkRay3D& oRay);

		//设置线的点数据
		void  Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//设置原点和方向
		void  SetPointAndDir(const HawkVector3D& oPoint,const HawkVector3D& oDir);

		//判读点是否在线上面(各种线的特性需要考虑)
		Bool  IsPointOnRay(const HawkVector3D& oVec) const;

		//各种运算符的重载
		HawkRay3D  operator + (const HawkVector3D& oVec) const;

		HawkRay3D& operator += (const HawkVector3D& oVec);

		HawkRay3D  operator - (const HawkVector3D& oVec) const;

		HawkRay3D& operator -= (const HawkVector3D& oVec);

		Bool operator ==(const HawkRay3D& oRay) const;

		Bool operator!=(const HawkRay3D& oRay) const;

	public:
		//直线和三角形相交
		Bool Intersect(const HawkTriangle3D& oTriangle,Vec3IntrResult* pResult = 0) const;

		//直线和矩形相交
		Bool Intersect(const HawkRect3D& oRect,Vec3IntrResult* pResult = 0) const;

		//直线和平面相交
		Bool Intersect(const HawkPlane3D& oPlane,Vec3IntrResult* pResult = 0) const;

		//直线和AABB盒相交
		Bool Intersect(const HawkAxisAlignedBox& oAABB,Vec3IntrResult* pResult = 0) const;

		//直线和球相交
		Bool Intersect(const HawkSphere& oSphere,Vec3IntrResult* pResult = 0) const;

		//直线和OBB相交
		Bool Intersect(const HawkOBB& oBox,Vec3IntrResult* pResult = 0) const;

		//直线和胶囊体相交
		Bool Intersect(const HawkCapsule& oCapsule,Vec3IntrResult* pResult = 0) const;
	};
}
#endif
