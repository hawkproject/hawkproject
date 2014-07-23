#ifndef HAWK_RAY2D_H
#define HAWK_RAY2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维空间射线数据                                                     */
	/************************************************************************/
	class GEO_API HawkRay2D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Direction,Direction);

	public:
		//默认构造
		HawkRay2D();

		//使用两个点构造(顺序有关,决定方向)
		HawkRay2D(const HawkVector2D& oPt1,const HawkVector2D& oPt2);

		//拷贝构造
		HawkRay2D(const HawkRay2D& oRay);

		//设置线的点数据
		void  Set(const HawkVector2D& oPt1,const HawkVector2D& oPt2);

		//判读点是否在线上面(各种线的特性需要考虑)
		Bool  IsPointOnRay(const HawkVector2D& oVec2) const;

		//测试一个点和直线的位置关系
		PtRelation   GetPtRelation(const HawkVector2D& oVec2) const;

		//计算上面和oVec相隔最近的点
		HawkVector2D GetClosestPoint(const HawkVector2D& oVec2) const;

		//点到直线的距离
		Float  GetDistance(const HawkVector2D& oVec2) const;

		//各种运算符的重载
		HawkRay2D  operator + (const HawkVector2D& oVec2) const;

		HawkRay2D& operator += (const HawkVector2D& oVec2) ;

		HawkRay2D  operator - (const HawkVector2D& oVec2) const ;

		HawkRay2D& operator -= (const HawkVector2D& oVec2);

		Bool operator ==(const HawkRay2D& oRay) const;

		Bool operator!=(const HawkRay2D& oRay) const;

	public:
		//直线相交的交点
		Bool Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//直线和线段相交的交点
		Bool Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//直线和射线相交的交点
		Bool Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//直线和三角形相交的交点
		Bool Intersect(const HawkTriangle2D& oTriangle,Vec2IntrResult* pResult = 0) const;

		//直线和矩形相交的交点
		Bool Intersect(const HawkRect2D& oRect,Vec2IntrResult* pResult = 0) const;
	};
}
#endif
