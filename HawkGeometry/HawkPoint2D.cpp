#include "HawkPoint2D.h"
#include "HawkVector2D.h"

namespace Hawk
{
	//Ĭ�Ϲ��캯��
	HawkPoint2D::HawkPoint2D(Float fX,Float fY):X(fX),Y(fY) {}

	//�������캯��
	HawkPoint2D::HawkPoint2D(const HawkPoint2D& xPt) : X(xPt.X),Y(xPt.Y) {}

	//Vector2D����
	HawkPoint2D::HawkPoint2D(const HawkVector2D& oVec2) : X(oVec2.X),Y(oVec2.Y) {}

	void HawkPoint2D::Move(Float fX,Float fY) 
	{
		X += fX;
		Y += fY;
	}

	Float HawkPoint2D::DistanceTo(const HawkPoint2D& xPt) 
	{
		return HawkMath::Sqrt((Float)((X-xPt.X)*(X-xPt.X) + (Y-xPt.Y)*(Y-xPt.Y)));
	}
}

