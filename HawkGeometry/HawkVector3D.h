#ifndef HAWK_VECTOR3D_H
#define HAWK_VECTOR3D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维向量表示                                                         */
	/************************************************************************/
	class GEO_API HawkVector3D
	{
	public:
		//三维向量属性
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);
		DEFINE_PUBLIC_PROPERTY(Float,Z,Z);

	public:
		//默认构造函数
		HawkVector3D(Float fVal = 0);

		//构造函数,参数表示向量的数值
		HawkVector3D(Float fX, Float fY, Float fZ);

		//拷贝构造函数
		HawkVector3D(const HawkVector3D& oVec3);

		//向量变负
		HawkVector3D operator - () const;

		//赋值函数
		HawkVector3D& operator = (const HawkVector3D& oVec3);

		//其他各种操作符重载 + - * / += *= /= = ==
		HawkVector3D  operator +  (const HawkVector3D& oVec3) const ;

		HawkVector3D& operator += (const HawkVector3D& oVec3);

		HawkVector3D  operator + (Float fVal) const;

		HawkVector3D& operator += (Float fVal);

		HawkVector3D  operator - (const HawkVector3D& oVec3) const;

		HawkVector3D& operator -= (const HawkVector3D& oVec3);

		HawkVector3D  operator -(Float fVal) const;

		HawkVector3D& operator -= (Float fVal);

		HawkVector3D  operator * (const HawkVector3D& oVec3) const;

		HawkVector3D& operator *= (const HawkVector3D& oVec3);

		HawkVector3D  operator * (Float fVal) const;

		HawkVector3D& operator *= (Float fVal);

		HawkVector3D  operator / (const HawkVector3D& oVec3) const ;

		HawkVector3D& operator /= (const HawkVector3D& oVec3);

		HawkVector3D  operator / (Float fVal) const;

		HawkVector3D& operator /= (Float fVal);

		//逻辑运算符重载
		Bool  operator <= (const HawkVector3D& oVec3) const;

		Bool  operator >= (const HawkVector3D& oVec3) const;

		Bool  operator < (const HawkVector3D& oVec3) const;

		Bool  operator > (const HawkVector3D& oVec3) const;

		Bool  operator == (const HawkVector3D& oVec3) const;

		Bool  operator != (const HawkVector3D& oVec3) const;

		//获得向量长度
		Float  Length() const;

		//获得向量长度的平方
		Float  SQLength() const;

		//向量点乘
		Float  DotProduct(const HawkVector3D& oVec3) const;

		//数据地址指针
		Float* Ptr();

		//数据地址指针
		const  Float* Ptr() const;

		//索引访问数据
		Float  operator [] (int iIdx) const;

		Float& operator [] (int iIdx);

		//获得两点之间的距离
		Float  GetDistanceFrom(const HawkVector3D& oVec3) const;

		//两点只见的水平距离
		Float  GetHoriDistance(const HawkVector3D& oVec3) const;

		//两点只见的垂直距离
		Float  GetVertDistance(const HawkVector3D& oVec3) const;

		//获得两点之间的距离的平方
		Float  GetSQDistanceFrom(const HawkVector3D& oVec3) const;

		//向量单位化
		HawkVector3D&  Normalize();

		//置0
		void   MakeZero();

		void   MakeFloor(const HawkVector3D& oVec3);

		void   MakeCeil(const HawkVector3D& oVec3);

		//与另外一个向量的中点
		HawkVector3D  MidPoint( const HawkVector3D& oVec3 ) const;
		//差乘
		HawkVector3D  CrossProduct( const HawkVector3D& oVec3 ) const;

		//判断在两点之间
		Bool IsBetweenPoints(const HawkVector3D& oBegin, const HawkVector3D& oEnd) const;

		//与另外一个向量进行插值计算,插值系数为fVal[0,1]
		HawkVector3D GetInterpolated(const HawkVector3D& oVec3, Float fVal) const;
	
		//和另一向量的夹角,返回弧度值
		Float AngleBetween(const HawkVector3D& oVec3);
		
		//到另一向量的旋转四元数
		HawkQuaternion GetRotationTo(const HawkVector3D& oVec3) const;

		//右乘
		GEO_API friend  HawkVector3D operator * (Float fVal,const HawkVector3D& oVec3);

		//矩阵乘(用于坐标变换)
		GEO_API friend  HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix3& oMat3);

		//矩阵乘(用于坐标变换)
		GEO_API friend  HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix4& oMat4);		
	};
}
#endif
