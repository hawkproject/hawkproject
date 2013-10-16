#ifndef HAWK_TRIANGLE2D_H
#define HAWK_TRIANGLE2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维三角形类	                                                        */
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge2                              */
	/************************************************************************/
	class GEO_API HawkTriangle2D
	{
	public:
		//默认构造函数
		HawkTriangle2D();

		//左下角坐标和两个边构造
		HawkTriangle2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2);

		//拷贝构造
		HawkTriangle2D(const HawkTriangle2D& oTrig);

		//操作符重载
		Bool operator == (const HawkTriangle2D& oTrig) const;

		Bool operator != (const HawkTriangle2D& oTrig) const;

		HawkTriangle2D operator + (const HawkVector2D& oVec2) const;

		HawkTriangle2D& operator += (const HawkVector2D& oVec2);

		HawkTriangle2D operator - (const HawkVector2D& oVec2) const;

		HawkTriangle2D operator -= (const HawkVector2D& oVec2);

		//获得顶点
		HawkVector2D GetPoint(Int32 iIdx) const;

		//获得索引对应的顶点
		HawkVector2D operator [](Int32 iIdx) const;

		//计算三角形面积
		Float GetArea() const;

		//设置顶点(原始点)
		void  SetOriginPoint(const HawkVector2D& oPt);

		const HawkVector2D& GetOriginPoint() const;

		//设置边
		void  SetEdge(Int32 iIdx,const HawkVector2D& oEdge);

		//获得边
		const HawkVector2D& GetEdge(Int32 iIdx) const;

		//设置三角形的三个顶点
		void  SetByPoint(const HawkVector2D& vPt1,const HawkVector2D& vPt2,const HawkVector2D& vPt3);

		//获取包围中心点(非外接圆)
		HawkVector2D  GetBoundCenter() const;

		//获取包围半径
		Float         GetBoundRadius() const;

		//获取顶点方向(顺,逆时针)
		Bool		  IsClockWise() const;

		//获取线段
		HawkSegment2D GetSegment(Int32 iIdx) const;

	public:
		//某一特定顶点在三角形之内
		Bool  IsPointInside(const HawkVector2D& oVec2) const;

		//某一特定顶点在三角形之内
		Bool  IsPointInside(const HawkPoint2D& oPt2) const;
	
	public:
		//直线和三角形相交判断
		Bool  Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//射线和三角形相交判断
		Bool  Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//线段和三角形相交判断
		Bool  Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//三角形和三角形相交判断
		Bool  Intersect(const HawkTriangle2D& oTriangle) const;

		//三角形和矩形相交判断
		Bool  Intersect(const HawkRect2D& oRect) const;

	public:
		//三角形顶点和2条边
		HawkVector2D  Point;
		HawkVector2D  Edge[2];
	};
}
#endif
