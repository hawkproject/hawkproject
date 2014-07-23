#ifndef HAWK_SEGMENT3D_H
#define HAWK_SEGMENT3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维空间线段数据                                                     */
	/************************************************************************/
	class GEO_API HawkSegment3D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Start,Start);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,End,End);

	public:
		//构造
		HawkSegment3D();

		//构造
		HawkSegment3D(const HawkVector3D& oStart,const HawkVector3D& oEnd);

		//拷贝构造
		HawkSegment3D(const HawkSegment3D& oSegment);

		//获得方向
		HawkVector3D GetDirection() const;

		//获取中点
		HawkVector3D GetCenter() const;

		//设置线的点数据
		void  Set(const HawkVector3D& oStart,const HawkVector3D& oEnd);

		//判读点是否在线上面(各种线的特性需要考虑)
		Bool  IsPointOnSegment(const HawkVector3D& oVec) const;
	
		//长度
		Float GetLength() const;

		//半长
		Float GetExtent() const;

		//距离
		Float GetDistance(const HawkSegment3D& oSegment) const;
		Float GetDistance(const HawkLine3D& oLine) const;
		Float GetDistance(const HawkRay3D& oRay) const;

		//各种运算符的重载
		HawkSegment3D  operator + (const HawkVector3D& oVec) const ;

		HawkSegment3D& operator += (const HawkVector3D& oVec);

		HawkSegment3D  operator - (const HawkVector3D& oVec) const;

		HawkSegment3D& operator -= (const HawkVector3D& oVec);

		Bool operator == (const HawkSegment3D& oSegment) const;

		Bool operator != (const HawkSegment3D& oSegment) const;
	};
}
#endif
