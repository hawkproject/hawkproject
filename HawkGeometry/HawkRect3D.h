#ifndef HAWK_RECT3D_H
#define HAWK_RECT3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

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
	class GEO_API HawkRect3D
	{
	public:
		//默认构造函数
		HawkRect3D();

		//左下角坐标和两个边构造
		HawkRect3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2);

		//拷贝构造
		HawkRect3D(const HawkRect3D& oRc);

		//操作符重载
		Bool operator == (const HawkRect3D& oRc) const;

		Bool operator != (const HawkRect3D& oRc) const;

		HawkRect3D   operator + (const HawkVector3D& oVec) const;

		HawkRect3D&  operator += (const HawkVector3D& oVec);

		HawkRect3D   operator - (const HawkVector3D& oVec) const;

		HawkRect3D   operator -= (const HawkVector3D& oVec);

		HawkVector3D operator [](Int32 iIdx) const;

		//计算三角形面积
		Float GetArea() const;

		//设置顶点
		void  SetOriginPoint(const HawkVector3D& oPt);

		const HawkVector3D& GetOriginPoint() const;

		//设置边
		void  SetEdge(Int32 iIdx,const HawkVector3D& oEdge);

		//获得边
		const HawkVector3D& GetEdge(Int32 iIdx) const;

		//计算三角形法线(左手坐标系,为正方向)
		HawkVector3D GetNormal() const;

		//获取矩形顶点
		HawkVector3D GetPoint(Int32 iIdx) const;

		//点在三角形正面
		Bool IsFrontFacing(const HawkVector3D& vVec3) const;

		//点在三角形内部
		Bool IsPointInside(const HawkVector3D& oVec3) const;

	public:
		//三角形顶点和2条边
		HawkVector3D  Point;
		HawkVector3D  Edge[2];
	};
}
#endif
