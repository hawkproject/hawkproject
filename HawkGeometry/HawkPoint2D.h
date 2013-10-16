#ifndef HAWK_POINT2D_H
#define HAWK_POINT2D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维平面上的点(和Vector2D类似)                                       */
	/************************************************************************/
	class GEO_API HawkPoint2D
	{
	public:
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);

	public:
		//默认构造函数
		HawkPoint2D(Float fX = 0,Float fY = 0);

		//拷贝构造函数
		HawkPoint2D(const HawkPoint2D& xPt);

		//Vector2D构造
		HawkPoint2D(const HawkVector2D& oVec2);

		void  Move(Float fX,Float fY);

		Float DistanceTo(const HawkPoint2D& xPt);
	};
}
#endif
