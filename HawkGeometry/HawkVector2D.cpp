#include "HawkVector2D.h"
#include "HawkDegree.h"
#include "HawkRadian.h"
#include "HawkMatrix2.h"
#include "HawkPoint2D.h"

namespace Hawk
{
	//默认构造函数
	HawkVector2D::HawkVector2D(Float fVal) : X(fVal), Y(fVal) {}

	//构造函数,参数表示向量的数值
	HawkVector2D::HawkVector2D(Float fX, Float fY) : X(fX), Y(fY) {}

	//拷贝构造函数
	HawkVector2D::HawkVector2D(const HawkVector2D& oVec2) : X(oVec2.X), Y(oVec2.Y) {}

	//点构造
	HawkVector2D::HawkVector2D(const HawkPoint2D& oPt2) : X(oPt2.X), Y(oPt2.Y) {}

	//向量变负
	HawkVector2D HawkVector2D::operator - () const 
	{ 
		return HawkVector2D(-X, -Y);
	}

	//赋值函数
	HawkVector2D& HawkVector2D::operator = (const HawkVector2D& oVec2) 
	{ 
		X = oVec2.X;
		Y = oVec2.Y;
		return *this;
	}

	//其他各种操作符重载 = - * / += -= *= /= = ==
	HawkVector2D HawkVector2D::operator +  (const HawkVector2D& oVec2) const 
	{ 
		return HawkVector2D(X + oVec2.X, Y + oVec2.Y); 
	}

	HawkVector2D& HawkVector2D::operator += (const HawkVector2D& oVec2) 
	{ 
		X += oVec2.X; 
		Y += oVec2.Y; 
		return *this; 
	}

	HawkVector2D HawkVector2D::operator + (Float fVal) const
	{ 
		return HawkVector2D(X + fVal, Y + fVal);
	}

	HawkVector2D& HawkVector2D::operator += (Float fVal) 
	{ 
		X += fVal; 
		Y += fVal;
		return *this;
	}

	HawkVector2D HawkVector2D::operator - (const HawkVector2D& oVec2) const
	{ 
		return HawkVector2D(X - oVec2.X, Y - oVec2.Y);
	}

	HawkVector2D& HawkVector2D::operator -= (const HawkVector2D& oVec2) 
	{ 
		X -= oVec2.X;
		Y -= oVec2.Y; 
		return *this; 
	}

	HawkVector2D HawkVector2D::operator -(Float fVal) const
	{ 
		return HawkVector2D(X - fVal, Y - fVal);
	}

	HawkVector2D& HawkVector2D::operator -= (Float fVal) 
	{ 
		X -= fVal;
		Y -= fVal; 
		return *this;
	}

	HawkVector2D HawkVector2D::operator * (const HawkVector2D& oVec2) const 
	{ 
		return HawkVector2D(X * oVec2.X, Y * oVec2.Y);
	}

	HawkVector2D& HawkVector2D::operator *= (const HawkVector2D& oVec2) 
	{ 
		X *= oVec2.X;
		Y *= oVec2.Y;
		return *this; 
	}

	HawkVector2D HawkVector2D::operator * (Float fVal) const 
	{ 
		return HawkVector2D(X * fVal, Y * fVal); 
	}

	HawkVector2D& HawkVector2D::operator *= (Float fVal) 
	{
		X *= fVal; 
		Y *= fVal; 
		return *this; 
	}

	HawkVector2D HawkVector2D::operator / (const HawkVector2D& oVec2) const 
	{ 
		A_Exception(!HawkMath::IsZero(oVec2.X) && !HawkMath::IsZero(oVec2.Y));

		return HawkVector2D(X / oVec2.X, Y / oVec2.Y); 
	}

	HawkVector2D& HawkVector2D::operator /= (const HawkVector2D& oVec2) 
	{
		A_Exception(!HawkMath::IsZero(oVec2.X) && !HawkMath::IsZero(oVec2.Y));

		X /= oVec2.X;
		Y /= oVec2.Y;
		return *this; 
	}

	HawkVector2D HawkVector2D::operator / (Float fVal) const 
	{ 
		A_Exception(!HawkMath::IsZero(fVal));

		return HawkVector2D(X / fVal, Y / fVal);
	}

	HawkVector2D& HawkVector2D::operator /= (Float fVal)
	{ 
		A_Exception(!HawkMath::IsZero(fVal));

		X /= fVal;
		Y /= fVal;
		return *this; 
	}

	Bool HawkVector2D::operator == (const HawkVector2D& oVec2) const
	{
		return HawkMath::IsZero(X - oVec2.X) && HawkMath::IsZero(Y - oVec2.Y);
	}

	Bool HawkVector2D::operator != (const HawkVector2D& oVec2) const
	{ 
		return !(*this == oVec2);
	}

	//获得向量长度
	Float HawkVector2D::Length() const 
	{ 
		return HawkMath::Sqrt( X*X + Y*Y ); 
	}

	//获得向量长度的平方
	Float HawkVector2D::SQLength() const 
	{ 
		return X*X + Y*Y;
	}

	//二维向量点乘
	 Float HawkVector2D::DotProduct(const HawkVector2D& oVec2) const
	{
		return X*oVec2.X + Y*oVec2.Y;
	}

	//获得两点之间的距离
	 Float HawkVector2D::GetDistanceFrom(const HawkVector2D& oVec2) const
	{
		return HawkVector2D(X - oVec2.X, Y - oVec2.Y).Length();
	}

	//获得两点之间的距离的平方
	 Float HawkVector2D::GetSQDistanceFrom(const HawkVector2D& oVec2) const
	{
		return HawkVector2D(X - oVec2.X, Y - oVec2.Y).SQLength();
	}

	//向量单位化
	HawkVector2D& HawkVector2D::Normalize()
	{
		Float fLen = Length();

		if (HawkMath::IsZero(fLen))
		{
			X = 0;Y = 0;
			return *this;
		}

		fLen = HawkMath::Reciprocal(fLen) ;
		X = (Float)(X * fLen);
		Y = (Float)(Y * fLen);

		return *this;
	}

	//与另外一个向量的中点
	HawkVector2D HawkVector2D::MidPoint( const HawkVector2D& oVec2 ) const
	{
		return HawkVector2D(( X + oVec2.X ) * 0.5f,( Y + oVec2.Y ) * 0.5f );
	}

	//差乘
	Float HawkVector2D::CrossProduct( const HawkVector2D& oVec2 ) const
	{
		return X * oVec2.Y - Y * oVec2.X;
	}

	//判断在两点之间
	Bool HawkVector2D::IsBetweenPoints(const HawkVector2D& oBegin, const HawkVector2D& oEnd) const
	{
		if (oBegin.X != oEnd.X)
		{
			return ((oBegin.X <= X && X <= oEnd.X) || (oBegin.X >= X && X >= oEnd.X));
		}
		else
		{
			return ((oBegin.Y <= Y && Y <= oEnd.Y) || (oBegin.Y >= Y && Y >= oEnd.Y));
		}
		return false;
	}

	//与另外一个向量进行插值计算,插值系数为fVal[0,1]
	 HawkVector2D HawkVector2D::GetInterpolated(const HawkVector2D& oVec2, Float fVal) const
	{
		Float fInv = 1.0f - fVal;
		return HawkVector2D((Float)(oVec2.X*fInv + X*fVal), (Float)(oVec2.Y*fInv + Y*fVal));
	}
	
	//平移
	HawkVector2D& HawkVector2D:: Move(const HawkVector2D& oMove)
	{
		X += oMove.X;
		Y += oMove.Y;
		return *this;
	}

	//缩放
	HawkVector2D&  HawkVector2D::Scale(Float fX,Float fY)
	{
		X *= fX;
		Y *= fY;
		
		return *this;
	}

	//缩放
	HawkVector2D&  HawkVector2D::Scale(const HawkVector2D& oScale)
	{
		return Scale(oScale.X,oScale.Y);
	}

	//旋转
	HawkVector2D&  HawkVector2D::Rotate(const HawkDegree& oDegree)
	{
		HawkRadian oRadian(oDegree);
		return Rotate(oRadian);
	}

	//旋转
	HawkVector2D&  HawkVector2D::Rotate(const HawkRadian& oRadian)
	{
		Float fSin = HawkMath::Sin(oRadian.Value());
		Float fCos = HawkMath::Cos(oRadian.Value());
		
		X = X*fCos - Y*fSin;
		Y = X*fSin + Y*fCos;

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	//友员函数
	HawkVector2D operator * (Float fVal,const HawkVector2D& oVec2) 
	{ 
		return HawkVector2D(oVec2.X * fVal, oVec2.Y * fVal); 
	}

	//矩阵乘,进行坐标变换
	HawkVector2D operator * (const HawkVector2D& oVec2,const HawkMatrix2& oMat2)
	{
		return HawkVector2D(oVec2.X * oMat2.M00 + oVec2.Y * oMat2.M10 , oVec2.X * oMat2.M01 + oVec2.Y * oMat2.M11); 
	}
}

