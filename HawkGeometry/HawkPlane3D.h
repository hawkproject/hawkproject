#ifndef HAWK_PLANE3D_H
#define HAWK_PLANE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维空间中的平面,使用固定点和法线表示  N * p + C = 0                 */
	/************************************************************************/
	class GEO_API HawkPlane3D
	{
	public:
		//平面方程
		//Normal * pt + Distance = 0

		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Normal,Normal);
		DEFINE_PUBLIC_PROPERTY(Float,Constant,Constant);
	
		//平面的方向 
		enum PlaneSide
		{
			//平面上
			PLANESIDE_ON,
			//正方向
			PLANESIDE_POSITIVE,
			//负方向
			PLANESIDE_NEGATIVE,
			//Both
			PLANESIDE_BOTH
		};

	public:
		//默认构造函数
		HawkPlane3D ();

		//拷贝构造函数
		HawkPlane3D (const HawkPlane3D& oPlane);

		//构造三角形所在的平面
		HawkPlane3D (const HawkTriangle3D& oTrig);

		//使用平面方程的四个参数构造平面
		HawkPlane3D (Float fA, Float fB, Float fC, Float fD);

		//使用平面方程的四个参数构造平面
		HawkPlane3D (const HawkVector4D& oVec4);

		//使用法线和固定点构造平面
		HawkPlane3D (const HawkVector3D& oNormal, const HawkVector3D& oVec3);

		//使用三维空间的三个顶点构造平面
		HawkPlane3D (const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3);

		//判断操作符重载
		Bool operator==(const HawkPlane3D& oPlane) const;

		Bool operator!=(const HawkPlane3D& oPlane) const;

		//计算点和平面的位置关系
		PlaneSide GetSide (const HawkVector3D& oVec3) const;

		PlaneSide GetSide (const HawkVector3D& vCenter, const HawkVector3D& vHalfSize) const;
		
		//计算盒子和平面的位置关系
		PlaneSide GetSide (const HawkAxisAlignedBox& oAxisAlignedBox) const;

		//计算点到平面的距离
		Float GetDistance(const HawkVector3D& oVec3) const;

		//三个点重定义平面
		void  Redefine(const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3);

		//一个点和一个法线定义平面
		void  Redefine(const HawkVector3D& oNormal, const HawkVector3D& oPt);

		//投影一个向量到平面
		HawkVector3D ProjectVector(const HawkVector3D& oVec3) const;

		//平面单位化
		Float  Normalize(void);

		//点乘
		Float  DotProduct(const HawkVector4D& oVec4);

	public:
		Bool  Intersect(const HawkTriangle3D& oTriangle) const;
		Bool  Intersect(const HawkRect3D& oRect) const;
		Bool  Intersect(const HawkPlane3D& oPlane) const;
		Bool  Intersect(const HawkAxisAlignedBox& oBox) const;
		Bool  Intersect(const HawkSphere& oSphere) const;
		Bool  Intersect(const HawkOBB& oBox) const;
		Bool  Intersect(const HawkCapsule& oCapsule) const;
	};
}
#endif
