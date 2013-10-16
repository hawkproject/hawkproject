#include "HawkRect.h"
#include "HawkVector2D.h"
#include "HawkPoint2D.h"

namespace Hawk
{
	//默认构造函数
	HawkRect::HawkRect(): Top(0),Bottom(0),Left(0),Right(0),ZOrder(0){};
	
	//上下左右构造
	HawkRect::HawkRect(Float l,Float t,Float r,Float b) : Top(t),Bottom(b),Left(l),Right(r),ZOrder(0){}

	//拷贝构造
	HawkRect::HawkRect(const HawkRect& oRc)
	{
		Left   = oRc.Left;
		Top    = oRc.Top;
		Right  = oRc.Right;
		Bottom = oRc.Bottom;
		ZOrder = oRc.ZOrder;
	}

	//操作符重载
	Bool HawkRect::operator == (const HawkRect& oRc) const
	{
		return  Left   == oRc.Left  &&
				Top    == oRc.Top   &&
				Right  == oRc.Right &&
				Bottom == oRc.Bottom&& 
				ZOrder == oRc.ZOrder;
	}

	Bool HawkRect::operator != (const HawkRect& oRc) const
	{
		return !(*this == oRc);
	}

	HawkRect& HawkRect::Move(Float fX,Float fY)
	{
		Left   += fX;
		Right  += fX;
		Top    += fY;
		Bottom += fY;

		return *this;
	}

	HawkRect& HawkRect::Move(HawkVector2D& oVec)
	{
		Left   += oVec.X;
		Right  += oVec.X;
		Top    += oVec.Y;
		Bottom += oVec.Y;

		return *this;
	}

	Bool HawkRect::IsValid() const
	{
		return Right >= Left && Bottom >= Top;
	}

	Float HawkRect::GetArea()
	{
		if(IsValid())
			return (Right-Left)*(Bottom-Top);
		return 0;
	}

	Bool HawkRect::PtInRect(const HawkPoint2D& xPt)
	{
		return xPt.X == HawkMath::Clamp(xPt.X,Left,Right) &&
			xPt.Y == HawkMath::Clamp(xPt.Y,Top,Bottom);
	}

	Bool HawkRect::PtInRect(const HawkVector2D& oVec)
	{
		return oVec.X == HawkMath::Clamp(oVec.X,Left,Right) &&
			oVec.Y == HawkMath::Clamp(oVec.Y,Top,Bottom);
	}
}

