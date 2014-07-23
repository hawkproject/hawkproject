#ifndef HAWK_RECT2D_H
#define HAWK_RECT2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* 矩形类																*/
	/* 使用左下角坐标和两条边作为参数										*/
	/*		   Edge1														*/
	/*		1 -----> 2														*/
	/*Edge2	|  		 |														*/
	/*		|		 |														*/
	/*		4 ------ 3														*/
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge1 + edge2 & pt4 =  pt1 + edge2 */
	/************************************************************************/	
	class GEO_API HawkRect2D
	{
	public:
		//默认构造函数
		HawkRect2D();

		//左下角坐标和两个边构造
		HawkRect2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2);

		//拷贝构造
		HawkRect2D(const HawkRect2D& oRc);

		HawkRect2D(const HawkRect& oRc);

		//操作符重载
		Bool operator == (const HawkRect2D& oRc) const;

		Bool operator != (const HawkRect2D& oRc) const;

		HawkRect2D   operator + (const HawkVector2D& oVec) const;

		HawkRect2D&  operator += (const HawkVector2D& oVec);

		HawkRect2D   operator - (const HawkVector2D& oVec) const;

		HawkRect2D   operator -= (const HawkVector2D& oVec);

		HawkVector2D operator [](Int32 iIdx) const;

		//计算三角形面积
		Float GetArea() const;

		//设置顶点
		void  SetOriginPoint(const HawkVector2D& oPt);

		const HawkVector2D& GetOriginPoint() const;

		//设置中点
		void  SetCenterPoint(const HawkVector2D& oPt);

		HawkVector2D GetCenterPoint() const;

		//获取包围中心点(非外接圆)
		HawkVector2D GetBoundCenter() const;

		//获取包围半径
		Float        GetBoundRadius() const;

		//获取顶点方向(顺,逆时针)
		Bool		 IsClockWise() const;

		//设置边
		void  SetEdge(Int32 iIdx,const HawkVector2D& oEdge);

		//获得线段
		HawkSegment2D  GetSegment(Int32 iIdx) const;

		//获得边
		const HawkVector2D& GetEdge(Int32 iIdx) const;

		//宽度计算
		Float GetWidth() const;

		void  SetWidth(Float fWidth);

		//高度计算
		Float GetHeight() const;

		void  SetHeight(Float fHeight);

		//获得其中一个顶点
		HawkVector2D GetPoint(Int32 iIdx) const;
	
		//清除数据
		void  Clear();

	public:
		//相交检测
		//某一特定顶点在矩形之内
		Bool  IsPointInside(const HawkVector2D& oVec) const;

	public:
		//直线和矩形相交判断
		Bool  Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//射线和矩形相交判断
		Bool  Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//线段和矩形相交判断
		Bool  Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//三角形和矩形相交判断
		Bool  Intersect(const HawkTriangle2D& oTriangle) const;

		//矩形和矩形相交判断
		Bool  Intersect(const HawkRect2D& oRect) const;

	public:
		//三角形顶点和2条边
		HawkVector2D  Point;
		HawkVector2D  Edge[2];
	};
}
#endif
