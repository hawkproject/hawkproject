#ifndef HAWK_VECTOR2D_H
#define HAWK_VECTOR2D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维平面中的点,使用浮点数作为坐标数据类型                            */
	/************************************************************************/
	class GEO_API HawkVector2D
	{
	public:
		//属性定义
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);

	public:
		//默认构造函数
		HawkVector2D(Float fVal = 0);

		//构造函数,参数表示向量的数值
		HawkVector2D(Float fX, Float fY);

		//拷贝构造函数
		HawkVector2D(const HawkVector2D& oVec2);

		//点构造
		HawkVector2D(const HawkPoint2D& oPt2);

		//向量变负
		HawkVector2D  operator - () const ;

		//赋值函数
		HawkVector2D& operator = (const HawkVector2D& oVec2);

		//其他各种操作符重载 = - * / += -= *= /= = ==
		HawkVector2D  operator +  (const HawkVector2D& oVec2) const;

		HawkVector2D& operator += (const HawkVector2D& oVec2);

		HawkVector2D  operator + (Float fVal) const;

		HawkVector2D& operator += (Float fVal);

		HawkVector2D  operator - (const HawkVector2D& oVec2) const;

		HawkVector2D& operator -= (const HawkVector2D& oVec2);

		HawkVector2D  operator -(Float fVal) const;

		HawkVector2D& operator -= (Float fVal);

		HawkVector2D  operator * (const HawkVector2D& oVec2) const;

		HawkVector2D& operator *= (const HawkVector2D& oVec2);

		HawkVector2D  operator * (Float fVal) const;

		HawkVector2D& operator *= (Float fVal);

		HawkVector2D  operator / (const HawkVector2D& oVec2) const;

		HawkVector2D& operator /= (const HawkVector2D& oVec2);

		HawkVector2D  operator / (Float fVal) const ;

		HawkVector2D& operator /= (Float fVal);

		//逻辑运算符重载
		Bool operator == (const HawkVector2D& oVec2) const;

		Bool operator != (const HawkVector2D& oVec2) const;

		//获得向量长度
		Float Length() const;

		//获得向量长度的平方
		Float SQLength() const ;

		//二维向量点乘
		Float DotProduct(const HawkVector2D& oVec2) const;

		//获得两点之间的距离
		Float GetDistanceFrom(const HawkVector2D& oVec2) const;

		//获得两点之间的距离的平方
		Float GetSQDistanceFrom(const HawkVector2D& oVec2) const;

		//向量单位化
		HawkVector2D& Normalize();

		//与另外一个向量的中点
		HawkVector2D  MidPoint( const HawkVector2D& oVec2 ) const;

		//差乘
		Float CrossProduct( const HawkVector2D& oVec2 ) const;

		//判断在两点之间
		Bool  IsBetweenPoints(const HawkVector2D& oBegin, const HawkVector2D& oEnd) const;

		//与另外一个向量进行插值计算,插值系数为fVal[0,1]
		HawkVector2D GetInterpolated(const HawkVector2D& oVec2, Float fVal) const;

		//平移
		HawkVector2D& Move(const HawkVector2D& oMove);

		//缩放
		HawkVector2D& Scale(Float fX,Float fY);

		//缩放
		HawkVector2D& Scale(const HawkVector2D& oScale);

		//旋转
		HawkVector2D& Rotate(const HawkDegree& oDegree);

		//旋转
		HawkVector2D& Rotate(const HawkRadian& oRadian);

		//向量数乘
		GEO_API friend HawkVector2D operator * (Float fVal,const HawkVector2D& oVec2);

		//矩阵乘,进行坐标变换
		GEO_API friend HawkVector2D operator * (const HawkVector2D& oVec2,const HawkMatrix2& oMat2);
	};
}
#endif
