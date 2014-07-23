#include "HawkVector3D.h"
#include "HawkMatrix3.h"
#include "HawkMatrix4.h"
#include "HawkQuaternion.h"

namespace Hawk
{
	//默认构造函数
	HawkVector3D::HawkVector3D(Float fVal) : X(fVal), Y(fVal), Z(fVal) {}

	//构造函数,参数表示向量的数值
	HawkVector3D::HawkVector3D(Float fX, Float fY, Float fZ) : X(fX), Y(fY),Z(fZ) {}

	//拷贝构造函数
	HawkVector3D::HawkVector3D(const HawkVector3D& oVec3) : X(oVec3.X), Y(oVec3.Y), Z(oVec3.Z) {}

	//向量变负
	HawkVector3D HawkVector3D::operator - () const 
	{ 
		return HawkVector3D(-X, -Y,-Z);
	}

	//赋值函数
	HawkVector3D& HawkVector3D::operator = (const HawkVector3D& oVec3) 
	{ 
		X = oVec3.X;
		Y = oVec3.Y;
		Z = oVec3.Z;
		return *this;
	}

	//其他各种操作符重载 + - * / += *= /= = ==
	HawkVector3D HawkVector3D::operator +  (const HawkVector3D& oVec3) const 
	{ 
		return HawkVector3D(X + oVec3.X, Y + oVec3.Y, Z + oVec3.Z); 
	}

	HawkVector3D& HawkVector3D::operator += (const HawkVector3D& oVec3) 
	{ 
		X += oVec3.X; 
		Y += oVec3.Y; 
		Z += oVec3.Z; 
		return *this; 
	}

	HawkVector3D HawkVector3D::operator + (Float fVal) const
	{ 
		return HawkVector3D(X + fVal, Y + fVal, Z + fVal);
	}

	HawkVector3D& HawkVector3D::operator += (Float fVal) 
	{ 
		X += fVal; 
		Y += fVal;
		Z += fVal;
		return *this;
	}

	HawkVector3D HawkVector3D::operator - (const HawkVector3D& oVec3) const
	{ 
		return HawkVector3D(X - oVec3.X, Y - oVec3.Y, Z - oVec3.Z);
	}

	HawkVector3D& HawkVector3D::operator -= (const HawkVector3D& oVec3) 
	{ 
		X -= oVec3.X;
		Y -= oVec3.Y; 
		Z -= oVec3.Z;
		return *this; 
	}

	HawkVector3D HawkVector3D::operator -(Float fVal) const
	{ 
		return HawkVector3D(X - fVal, Y - fVal, Z - fVal);
	}

	HawkVector3D& HawkVector3D::operator -= (Float fVal) 
	{ 
		X -= fVal;
		Y -= fVal; 
		Z -= fVal; 
		return *this;
	}

	HawkVector3D HawkVector3D::operator * (const HawkVector3D& oVec3) const 
	{ 
		return HawkVector3D(X * oVec3.X, Y * oVec3.Y, Z * oVec3.Z);
	}

	HawkVector3D& HawkVector3D::operator *= (const HawkVector3D& oVec3) 
	{ 
		X *= oVec3.X;
		Y *= oVec3.Y;
		Z *= oVec3.Z;
		return *this; 
	}

	HawkVector3D HawkVector3D::operator * (Float fVal) const 
	{ 
		return HawkVector3D(X * fVal, Y * fVal, Z * fVal); 
	}

	HawkVector3D& HawkVector3D::operator *= (Float fVal) 
	{
		X *= fVal; 
		Y *= fVal; 
		Z *= fVal; 
		return *this; 
	}

	HawkVector3D HawkVector3D::operator / (const HawkVector3D& oVec3) const 
	{ 
		A_Exception(!HawkMath::IsZero(oVec3.X) && !HawkMath::IsZero(oVec3.Y) && !HawkMath::IsZero(oVec3.Z));

		return HawkVector3D(X / oVec3.X, Y / oVec3.Y, Z / oVec3.Z); 
	}

	HawkVector3D& HawkVector3D::operator /= (const HawkVector3D& oVec3) 
	{
		A_Exception(!HawkMath::IsZero(oVec3.X) && !HawkMath::IsZero(oVec3.Y) && !HawkMath::IsZero(oVec3.Z));
		
		X /= oVec3.X;
		Y /= oVec3.Y;
		Z /= oVec3.Z;
		return *this; 
	}

	HawkVector3D HawkVector3D::operator / (Float fVal) const 
	{ 
		A_Exception(!HawkMath::IsZero(fVal));

		return HawkVector3D(X / fVal, Y / fVal, Z / fVal);
	}

	HawkVector3D& HawkVector3D::operator /= (Float fVal)
	{ 
		A_Exception(!HawkMath::IsZero(fVal));

		X /= fVal;
		Y /= fVal;
		Z /= fVal;
		return *this; 
	}

	//逻辑运算符重载
	Bool HawkVector3D::operator <= (const HawkVector3D& oVec3) const 
	{
		return X <= oVec3.X && Y <= oVec3.Y && Z <= oVec3.Z; 
	}

	Bool HawkVector3D::operator >= (const HawkVector3D& oVec3) const
	{
		return X >= oVec3.X && Y >= oVec3.Y && Z >= oVec3.Z; 
	}

	Bool HawkVector3D::operator < (const HawkVector3D& oVec3) const
	{ 
		return X < oVec3.X && Y < oVec3.Y && Z < oVec3.Z;
	}

	Bool HawkVector3D::operator > (const HawkVector3D& oVec3) const
	{ 
		return X > oVec3.X && Y > oVec3.Y && Z > oVec3.Z; 
	}

	Bool HawkVector3D::operator == (const HawkVector3D& oVec3) const
	{
		return HawkMath::IsZero(X - oVec3.X) && HawkMath::IsZero(Y - oVec3.Y) && HawkMath::IsZero(Z - oVec3.Z);
	}

	Bool HawkVector3D::operator != (const HawkVector3D& oVec3) const
	{ 
		return !(*this == oVec3);
	}

	//获得向量长度
	Float HawkVector3D::Length() const 
	{ 
		return HawkMath::Sqrt( X*X + Y*Y + Z*Z); 
	}

	//获得向量长度的平方
	Float HawkVector3D::SQLength() const 
	{ 
		return X*X + Y*Y + Z*Z;
	}

	//向量点乘
	Float HawkVector3D::DotProduct(const HawkVector3D& oVec3) const
	{
		return X*oVec3.X + Y*oVec3.Y + Z*oVec3.Z;
	}

	//数据地址指针
	Float* HawkVector3D::Ptr()
	{
		return (Float*)this;
	}

	const  Float* HawkVector3D::Ptr() const
	{
		return (const  Float*)this;
	}

	//索引访问数据
	Float HawkVector3D::operator [] (int iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<3);
	
		return Ptr()[iIdx];
	}

	Float& HawkVector3D::operator [] (int iIdx)
	{
		A_Exception(iIdx>=0 && iIdx<3);

		return Ptr()[iIdx];
	}

	void HawkVector3D::MakeFloor(const HawkVector3D& oVec3)
	{
		if( oVec3.X < X ) X = oVec3.X;
		if( oVec3.Y < Y ) Y = oVec3.Y;
		if( oVec3.Z < Z ) Z = oVec3.Z;
	}

	void HawkVector3D::MakeCeil(const HawkVector3D& oVec3)
	{
		if( oVec3.X > X ) X = oVec3.X;
		if( oVec3.Y > Y ) Y = oVec3.Y;
		if( oVec3.Z > Z ) Z = oVec3.Z;
	}

	//获得两点之间的距离
	Float HawkVector3D::GetDistanceFrom(const HawkVector3D& oVec3) const
	{
		return HawkVector3D(X - oVec3.X, Y - oVec3.Y, Z - oVec3.Z).Length();
	}

	//两点只见的水平距离
	Float  HawkVector3D::GetHoriDistance(const HawkVector3D& oVec3) const
	{
		return HawkVector3D(X - oVec3.X, 0, Z - oVec3.Z).Length();
	}

	//两点只见的垂直距离
	Float  HawkVector3D::GetVertDistance(const HawkVector3D& oVec3) const
	{
		return HawkMath::Abs(Y - oVec3.Y);
	}

	//获得两点之间的距离的平方
	Float HawkVector3D::GetSQDistanceFrom(const HawkVector3D& oVec3) const
	{
		return HawkVector3D(X - oVec3.X, Y - oVec3.Y, Z - oVec3.Z).SQLength();
	}

	void HawkVector3D::MakeZero()
	{
		X = 0; Y = 0; Z = 0;
	}

	//向量单位化
	HawkVector3D& HawkVector3D::Normalize()
	{
		Float fLen = Length();

		if (HawkMath::IsZero(fLen))
			return *this;

		fLen = HawkMath::Reciprocal(fLen);

		X = (Float)(X * fLen);
		Y = (Float)(Y * fLen);
		Z = (Float)(Z * fLen);

		return *this;
	}

	//与另外一个向量的中点
	HawkVector3D HawkVector3D::MidPoint( const HawkVector3D& oVec3 ) const
	{
		return HawkVector3D(( X + oVec3.X ) * 0.5f,( Y + oVec3.Y ) * 0.5f ,( Z + oVec3.Z ) * 0.5f);
	}

	//差乘
	HawkVector3D HawkVector3D::CrossProduct( const HawkVector3D& oVec3 ) const
	{
		return HawkVector3D(Y * oVec3.Z - Z * oVec3.Y, Z * oVec3.X - X * oVec3.Z, X * oVec3.Y - Y * oVec3.X);
	}

	//和另一向量的夹角
	Float HawkVector3D::AngleBetween(const HawkVector3D& oVec3)
	{
		Float fProduct = Length() * oVec3.Length();
		if(fProduct < HawkMath::FLOAT_DIFF)
			fProduct = HawkMath::FLOAT_DIFF;

		Float fVal = DotProduct(oVec3) / fProduct;

		fVal = HawkMath::Clamp<Float>(fVal,-1.0f,1.0f);
		return HawkMath::Acos(fVal);

	}

	//到另一向量的旋转四元数
	HawkQuaternion HawkVector3D::GetRotationTo(const HawkVector3D& oVec3) const
	{
		HawkQuaternion oQuat(1.0f,0,0,0);

		HawkVector3D v0 = *this;
		HawkVector3D v1 = oVec3;
		v0.Normalize();
		v1.Normalize();
		Float fProduct = v0.DotProduct(v1);

		if (fProduct >= 1.0f)
			return oQuat;

		if (fProduct < (HawkMath::FLOAT_DIFF - 1.0f))
		{
			HawkVector3D vAxis = HawkVector3D(1.0f,0,0).CrossProduct(*this);
			if (vAxis.Length() <= HawkMath::FLOAT_DIFF)
				vAxis =  HawkVector3D(0,1.0f,0).CrossProduct(*this);
			vAxis.Normalize();
			oQuat.SetFromAxisAngle(vAxis,HawkMath::PI);
		}
		else
		{
			Float fVal = HawkMath::Sqrt((1.0f+fProduct)*2.0f);
			Float fInvVal = 1.0f / fVal;

			HawkVector3D v2 = v0.CrossProduct(v1);

			oQuat.X = v2.X * fInvVal;
			oQuat.Y = v2.Y * fInvVal;
			oQuat.Z = v2.Z * fInvVal;
			oQuat.W = fVal * 0.5f;
			oQuat.Normalize();
		}
		return oQuat;
	}

	//判断在两点之间
	Bool HawkVector3D::IsBetweenPoints(const HawkVector3D& oBegin, const HawkVector3D& oEnd) const
	{
		Float fVal = (oEnd - oBegin).SQLength();
		return GetSQDistanceFrom(oBegin) <= fVal && GetSQDistanceFrom(oEnd) <= fVal;
	}

	//与另外一个向量进行插值计算,插值系数为fVal[0,1]
	 HawkVector3D HawkVector3D::GetInterpolated(const HawkVector3D& oVec3, Float fVal) const
	{
		Float fInv = 1.0f - fVal;
		return HawkVector3D(oVec3.X*fInv + X*fVal, oVec3.Y*fInv + Y*fVal, oVec3.Z*fInv + Z*fVal);
	}
	
	HawkVector3D operator * (Float fVal,const HawkVector3D& oVec3) 
	{ 
		return HawkVector3D(oVec3.X * fVal, oVec3.Y * fVal, oVec3.Z * fVal); 
	}

	//矩阵乘
	HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix3& oMat3)
	{
		HawkVector3D oTmp;
		oTmp.X = oVec3.X*oMat3.M00 + oVec3.Y*oMat3.M10 + oVec3.Z*oMat3.M20;
		oTmp.Y = oVec3.X*oMat3.M01 + oVec3.Y*oMat3.M11 + oVec3.Z*oMat3.M21;
		oTmp.Z = oVec3.X*oMat3.M02 + oVec3.Y*oMat3.M12 + oVec3.Z*oMat3.M22;
		return oTmp;
	}

	//矩阵乘
	HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix4& oMat4)
	{
		HawkVector3D oTmp;

		Float fInvW = 1.0f / (oMat4[3][0]*oVec3.X + oMat4[3][1]*oVec3.Y + oMat4[3][2]*oVec3.Z + oMat4[3][3]);

		oTmp.X = ( oMat4[0][0] * oVec3.X + oMat4[0][1] * oVec3.Y + oMat4[0][2] * oVec3.Z + oMat4[0][3] ) * fInvW;
		oTmp.Y = ( oMat4[1][0] * oVec3.X + oMat4[1][1] * oVec3.Y + oMat4[1][2] * oVec3.Z + oMat4[1][3] ) * fInvW;
		oTmp.Z = ( oMat4[2][0] * oVec3.X + oMat4[2][1] * oVec3.Y + oMat4[2][2] * oVec3.Z + oMat4[2][3] ) * fInvW;

		return oTmp;
	}
}
