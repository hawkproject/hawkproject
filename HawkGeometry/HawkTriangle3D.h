#ifndef HAWK_TRIANGLE3D_H
#define HAWK_TRIANGLE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维三角形类                                                         */
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge2                              */
	/************************************************************************/
	class GEO_API HawkTriangle3D
	{
	public:
		//默认构造函数
		HawkTriangle3D();

		//左下角坐标和两个边构造
		HawkTriangle3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2);

		//拷贝构造
		HawkTriangle3D(const HawkTriangle3D& oTrig);

		//操作符重载
		Bool operator == (const HawkTriangle3D& oTrig) const;

		Bool operator != (const HawkTriangle3D& oTrig) const;

		HawkTriangle3D  operator + (const HawkVector3D& oVec) const;

		HawkTriangle3D& operator += (const HawkVector3D& oVec);

		HawkTriangle3D  operator - (const HawkVector3D& oVec) const;

		HawkTriangle3D  operator -= (const HawkVector3D& oVec);

		HawkVector3D    operator [](Int32 iIdx) const;

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
		
		//计算三角形的顶点
		HawkVector3D GetPoint(Int32 iIdx) const;
		
		//点在三角形正面
		Bool IsFrontFacing(const HawkVector3D& vVec3) const;

		//点在三角形内部
		Bool IsPointInside(const HawkVector3D& oVec3) const;
		
		//设置三角形的三个顶点
		void SetByPoint(const HawkVector3D& vPt1,const HawkVector3D& vPt2,const HawkVector3D& vPt3);

		//获取中心点
		HawkVector3D  GetCenterPoint() const;

		//获得线段
		HawkSegment3D GetSegment(Int32 iIdx) const;

		void GetProjection(const HawkVector3D& vAxis,Float& fMin, Float& fMax) const;

		//两点在直线同一边
		static Bool  IsOnSameSide(const HawkVector3D& vP1, const HawkVector3D& vP2,const HawkVector3D& vA, const HawkVector3D& vB);

	public:
		//三角形顶点和2条边
		HawkVector3D  Point;
		HawkVector3D  Edge[2];
	};
}
#endif
