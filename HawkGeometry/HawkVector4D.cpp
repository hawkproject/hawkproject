#include "HawkVector4D.h"
#include "HawkVector3D.h"
#include "HawkMatrix3.h"
#include "HawkMatrix4.h"

namespace Hawk
{
	HawkVector4D::HawkVector4D() {}

	HawkVector4D::HawkVector4D(Float fX, Float fY, Float fZ, Float fW ) : X( fX ), Y( fY ), Z( fZ ), W( fW ) {}

	HawkVector4D::HawkVector4D(const HawkVector3D& oVec3) : X(oVec3.X), Y(oVec3.Y), Z(oVec3.Z), W(1.0f) {}

	HawkVector4D::HawkVector4D(const HawkVector4D& oVec4) : X(oVec4.X), Y(oVec4.Y), Z(oVec4.Z), W(oVec4.W) {}

	//各类操作符重载
	HawkVector4D& HawkVector4D::operator = ( const HawkVector4D& oVec4 )
	{
		X = oVec4.X;
		Y = oVec4.Y;
		Z = oVec4.Z;
		W = oVec4.W;

		return *this;
	}

	HawkVector4D& HawkVector4D::operator = ( const Float fVal)
	{
		X = fVal;
		Y = fVal;
		Z = fVal;
		W = fVal;
		return *this;
	}

	//数据地址指针
	Float*  HawkVector4D::Ptr()
	{
		return (Float*)this;
	}

	//数据地址指针
	const Float*  HawkVector4D::Ptr() const
	{
		return (Float*)this;
	}

	//索引访问数据
	Float HawkVector4D::operator [] (int iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<4);

		return Ptr()[iIdx];
	}

	Float& HawkVector4D::operator [] (int iIdx)
	{
		A_Exception(iIdx>=0 && iIdx<4);

		return Ptr()[iIdx];
	}

	Bool HawkVector4D::operator == ( const HawkVector4D& oVec4 ) const
	{
		return (HawkMath::FloatEqual(X,oVec4.X) &&
			HawkMath::FloatEqual(Y,oVec4.Y) &&
			HawkMath::FloatEqual(Z,oVec4.Z) &&
			HawkMath::FloatEqual(W,oVec4.W) );
	}

	Bool HawkVector4D::operator != ( const HawkVector4D& oVec4 ) const
	{
		return !((*this) == oVec4);
	}

	HawkVector4D& HawkVector4D::operator = (const HawkVector3D& oVec3)
	{
		X = oVec3.X;
		Y = oVec3.Y;
		Z = oVec3.Z;
		W = 1.0f;
		return *this;
	}

	HawkVector4D HawkVector4D::operator + ( const HawkVector4D& oVec4 ) const
	{
		return HawkVector4D(
			X + oVec4.X,
			Y + oVec4.Y,
			Z + oVec4.Z,
			W + oVec4.W);
	}

	HawkVector4D HawkVector4D::operator - ( const HawkVector4D& oVec4 ) const
	{
		return HawkVector4D(
			X - oVec4.X,
			Y - oVec4.Y,
			Z - oVec4.Z,
			W - oVec4.W);
	}

	HawkVector4D HawkVector4D::operator * ( const Float fVal ) const
	{
		return HawkVector4D(
			X * fVal,
			Y * fVal,
			Z * fVal,
			W * fVal);
	}

	HawkVector4D HawkVector4D::operator * ( const HawkVector4D& oVec4) const
	{
		return HawkVector4D(
			oVec4.X * X,
			oVec4.Y * Y,
			oVec4.Z * Z,
			oVec4.W * W);
	}

	HawkVector4D HawkVector4D::operator / ( const Float fVal ) const
	{
		A_Exception(!HawkMath::IsZero(fVal));

		Float fInv = 1.0f / fVal;

		return HawkVector4D(
			X * fInv,
			Y * fInv,
			Z * fInv,
			W * fInv);
	}

	HawkVector4D HawkVector4D::operator / ( const HawkVector4D& oVec4) const
	{
		A_Exception(!HawkMath::IsZero(oVec4.X) && !HawkMath::IsZero(oVec4.Y) && !HawkMath::IsZero(oVec4.Z) && !HawkMath::IsZero(oVec4.W));

		return HawkVector4D(
			X / oVec4.X,
			Y / oVec4.Y,
			Z / oVec4.Z,
			W / oVec4.W);
	}

	const HawkVector4D& HawkVector4D::operator + () const
	{
		return *this;
	}

	HawkVector4D HawkVector4D::operator - () const
	{
		return HawkVector4D(-X, -Y, -Z, -W);
	}

	HawkVector4D& HawkVector4D::operator += ( const HawkVector4D& oVec4 )
	{
		X += oVec4.X;
		Y += oVec4.Y;
		Z += oVec4.Z;
		W += oVec4.W;

		return *this;
	}

	HawkVector4D& HawkVector4D::operator -= ( const HawkVector4D& oVec4 )
	{
		X -= oVec4.X;
		Y -= oVec4.Y;
		Z -= oVec4.Z;
		W -= oVec4.W;

		return *this;
	}

	HawkVector4D& HawkVector4D::operator *= ( const Float fVal )
	{
		X *= fVal;
		Y *= fVal;
		Z *= fVal;
		W *= fVal;
		return *this;
	}

	HawkVector4D& HawkVector4D::operator += ( const Float fVal )
	{
		X += fVal;
		Y += fVal;
		Z += fVal;
		W += fVal;
		return *this;
	}

	HawkVector4D& HawkVector4D::operator -= ( const Float fVal )
	{
		X -= fVal;
		Y -= fVal;
		Z -= fVal;
		W -= fVal;
		return *this;
	}

	HawkVector4D& HawkVector4D::operator *= ( const HawkVector4D& oVec4 )
	{
		X *= oVec4.X;
		Y *= oVec4.Y;
		Z *= oVec4.Z;
		W *= oVec4.W;

		return *this;
	}

	HawkVector4D& HawkVector4D::operator /= ( const Float fVal )
	{
		A_Exception( !HawkMath::IsZero(fVal));

		Float fInv = 1.0f / fVal;

		X *= fInv;
		Y *= fInv;
		Z *= fInv;
		W *= fInv;

		return *this;
	}

	HawkVector4D& HawkVector4D::operator /= ( const HawkVector4D& oVec4 )
	{
		A_Exception(!HawkMath::IsZero(oVec4.X) && !HawkMath::IsZero(oVec4.Y) && !HawkMath::IsZero(oVec4.Z) && !HawkMath::IsZero(oVec4.W));

		X /= oVec4.X;
		Y /= oVec4.Y;
		Z /= oVec4.Z;
		W /= oVec4.W;

		return *this;
	}

	//四维向量点乘
	Float HawkVector4D::DotProduct(const HawkVector4D& oVec4) const
	{
		return X * oVec4.X + Y * oVec4.Y + Z * oVec4.Z + W * oVec4.W;
	}

	//获得向量长度
	Float  HawkVector4D::Length() const
	{
		return  HawkMath::Sqrt(X*X + Y*Y + Z*Z + W*W);
	}

	//获得向量长度的平方
	Float  HawkVector4D::SQLength() const
	{
		return X*X + Y*Y + Z*Z + W*W;
	}

	HawkVector4D HawkVector4D::GetInterpolated(const HawkVector4D& oVec4, Float fVal) const
	{
		Float fInv = 1.0f - fVal;
		return HawkVector4D(oVec4.X*fInv + X*fVal, oVec4.Y*fInv + Y*fVal, oVec4.Z*fInv + Z*fVal, oVec4.W*fInv + W*fVal);
	}

	//单位化
	HawkVector4D& HawkVector4D::Normalize()
	{
		Float fLen = Length() ;

		if (HawkMath::IsZero(fLen))
			return *this;

		fLen = HawkMath::Reciprocal(fLen) ;

		X *= fLen;
		Y *= fLen;
		Z *= fLen;
		W *= fLen;

		return *this;
	}

	HawkVector4D operator * (const HawkVector4D& oVec4,const HawkMatrix4& oMat4)
	{
		HawkVector4D oTmp;
		oTmp.X = oVec4.X*oMat4.M00 + oVec4.Y*oMat4.M10 + oVec4.Z*oMat4.M20 + oVec4.W*oMat4.M30;
		oTmp.Y = oVec4.X*oMat4.M01 + oVec4.Y*oMat4.M11 + oVec4.Z*oMat4.M21 + oVec4.W*oMat4.M31;
		oTmp.Z = oVec4.X*oMat4.M02 + oVec4.Y*oMat4.M12 + oVec4.Z*oMat4.M22 + oVec4.W*oMat4.M32;
		oTmp.W = oVec4.X*oMat4.M03 + oVec4.Y*oMat4.M13 + oVec4.Z*oMat4.M23 + oVec4.W*oMat4.M33;
		return oTmp;
	}
}
