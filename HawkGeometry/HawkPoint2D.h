#ifndef HAWK_POINT2D_H
#define HAWK_POINT2D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��άƽ���ϵĵ�(��Vector2D����)                                       */
	/************************************************************************/
	class GEO_API HawkPoint2D
	{
	public:
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);

	public:
		//Ĭ�Ϲ��캯��
		HawkPoint2D(Float fX = 0,Float fY = 0);

		//�������캯��
		HawkPoint2D(const HawkPoint2D& xPt);

		//Vector2D����
		HawkPoint2D(const HawkVector2D& oVec2);

		void  Move(Float fX,Float fY);

		Float DistanceTo(const HawkPoint2D& xPt);
	};
}
#endif
