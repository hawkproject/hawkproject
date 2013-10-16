#ifndef HAWK_SEGMENT2D_H
#define HAWK_SEGMENT2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维空间线段数据                                                     */
	/************************************************************************/
	class GEO_API HawkSegment2D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Start,Start);
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,End,End);

	public:
		//构造
		HawkSegment2D();

		//构造
		HawkSegment2D(const HawkVector2D& oStart,const HawkVector2D& oEnd);

		//拷贝构造
		HawkSegment2D(const HawkSegment2D& oSegment);

		//设置线的点数据
		void  Set(const HawkVector2D& oStart,const HawkVector2D& oEnd);

		//判读点是否在线上面(各种线的特性需要考虑)
		Bool  IsPointOnSegment(const HawkVector2D& oVec2) const;		
		
		//计算上面和oVec相隔最近的点
		HawkVector2D GetClosestPoint(const HawkVector2D& oVec2) const;

		//点到直线的距离
		Float GetDistance(const HawkVector2D& oVec2) const;

		//长度
		Float GetLength();

		//中点
		HawkVector2D MidPoint() const;

		//获得方向
		HawkVector2D GetDirection() const;

		//各种运算符的重载
		HawkSegment2D  operator + (const HawkVector2D& oVec2) const ;

		HawkSegment2D& operator += (const HawkVector2D& oVec2);

		HawkSegment2D  operator - (const HawkVector2D& oVec2) const ;

		HawkSegment2D& operator -= (const HawkVector2D& oVec2) ;

		Bool operator == (const HawkSegment2D& oSegment) const;

		Bool operator != (const HawkSegment2D& oSegment) const;

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
