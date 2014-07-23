#ifndef HAWK_LINE3D_H
#define HAWK_LINE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维维空间直线数据                                                   */
	/************************************************************************/
	class GEO_API HawkLine3D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Direction,Direction);

	public:
		//默认构造
		HawkLine3D();

		//使用两个点构造
		HawkLine3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//拷贝构造
		HawkLine3D(const HawkLine3D& oLine);

		//设置线的点数据
		void  Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//判读点是否在线上面(各种线的特性需要考虑)
		Bool  IsPointOnLine(const HawkVector3D& oVec) const;
		
		//计算线段上面和oVec相隔最近的点
		HawkVector3D GetClosestPoint(const HawkVector3D& oVec) const;
	
		//点到直线的距离
		Float GetDistance(const HawkVector3D& oVec) const;

		//各种运算符的重载
		HawkLine3D  operator + (const HawkVector3D& oVec) const;

		HawkLine3D& operator += (const HawkVector3D& oVec);

		HawkLine3D  operator - (const HawkVector3D& oVec) const ;

		HawkLine3D& operator -= (const HawkVector3D& oVec);

		Bool operator == (const HawkLine3D& oLine) const;

		Bool operator != (const HawkLine3D& oLine) const;

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
