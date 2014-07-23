#ifndef HAWK_VECTOR4D_H
#define HAWK_VECTOR4D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 思维空间向量类,用于齐次坐标和矩阵计算                                */
	/************************************************************************/
	class GEO_API HawkVector4D
	{
	public:
		//向量属性
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);
		DEFINE_PUBLIC_PROPERTY(Float,Z,Z);
		DEFINE_PUBLIC_PROPERTY(Float,W,W);

	public:
		HawkVector4D();

		HawkVector4D(Float fX, Float fY, Float fZ, Float fW );

		HawkVector4D(const HawkVector3D& oVec3);

		HawkVector4D(const HawkVector4D& oVec4);

		//各类操作符重载
		HawkVector4D& operator = ( const HawkVector4D& oVec4 );

		HawkVector4D& operator = ( const Float fVal);

		//数据地址指针
		Float*  Ptr();
		
		//数据地址指针
		const Float*  Ptr() const;

		//索引访问数据
		Float  operator [] (int iIdx) const;

		Float& operator [] (int iIdx);

		Bool   operator == ( const HawkVector4D& oVec4 ) const;

		Bool   operator != ( const HawkVector4D& oVec4 ) const;

		HawkVector4D& operator = (const HawkVector3D& oVec3);

		HawkVector4D  operator + ( const HawkVector4D& oVec4) const;

		HawkVector4D  operator - ( const HawkVector4D& oVec4) const;

		HawkVector4D  operator * ( const Float fVal) const;

		HawkVector4D  operator * ( const HawkVector4D& oVec4) const;

		HawkVector4D  operator / ( const Float fVal) const;

		HawkVector4D  operator / ( const HawkVector4D& oVec4) const;

		const HawkVector4D& operator + () const;

		HawkVector4D  operator - () const;

		HawkVector4D& operator += ( const HawkVector4D& oVec4);

		HawkVector4D& operator -= ( const HawkVector4D& oVec4);

		HawkVector4D& operator *= ( const Float fVal);

		HawkVector4D& operator += ( const Float fVal);

		HawkVector4D& operator -= ( const Float fVal);

		HawkVector4D& operator *= ( const HawkVector4D& oVec4);

		HawkVector4D& operator /= ( const Float fVal);

		HawkVector4D& operator /= ( const HawkVector4D& oVec4);

		//获得向量长度
		Float  Length() const;

		//获得向量长度的平方
		Float  SQLength() const;

		//四维向量点乘
		Float  DotProduct(const HawkVector4D& oVec4) const;

		//单位化
		HawkVector4D&  Normalize();

		//与另外一个向量进行插值计算,插值系数为fVal[0,1]
		HawkVector4D   GetInterpolated(const HawkVector4D& oVec4, Float fVal) const;

		//齐次坐标变换
		GEO_API friend HawkVector4D operator * (const HawkVector4D& oVec4,const HawkMatrix4& oMat4);
	};
}
#endif
