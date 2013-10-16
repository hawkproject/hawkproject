#ifndef HAWK_DEGREE_H
#define HAWK_DEGREE_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 数学计算中的角度值                                                   */
	/************************************************************************/
	class GEO_API HawkDegree
	{
	public:
		//属性定义
		DEFINE_PROPERTY(Float,Degree,Degree);

	public:
		//构造
		HawkDegree(Float fDeg = 0);

		HawkDegree(const HawkDegree& oDeg);

		HawkDegree(const HawkRadian& oRad);

		//获得角度数值
		Float  Value() const;

		//弧度转换为角度
		HawkDegree  operator = (const HawkRadian& oRad);

		//各种运算符重载 (+ - * / += -= *= /=)
		HawkDegree& operator = ( const HawkDegree& oDeg );

		HawkDegree  operator + ( Float fDeg ) const;

		HawkDegree& operator += ( Float fDeg );

		HawkDegree  operator + ( const HawkDegree& oDeg ) const ;

		HawkDegree& operator += ( const HawkDegree& oDeg ) ;

		HawkDegree  operator - () const;

		HawkDegree  operator - ( Float fDeg ) const;

		HawkDegree& operator -= ( Float fDeg );

		HawkDegree  operator - ( const HawkDegree& oDeg ) const;

		HawkDegree& operator -= ( const HawkDegree& oDeg );

		HawkDegree  operator * ( Float fScale ) const;

		HawkDegree  operator * ( const HawkDegree& oDeg ) const;

		HawkDegree& operator *= ( Float fScale );

		HawkDegree& operator *= ( const HawkDegree& oDeg );

		HawkDegree  operator / ( Float fScale ) const;

		HawkDegree& operator /= ( Float fScale );

		HawkDegree  operator / ( const HawkDegree& oDeg ) const;

		HawkDegree& operator /= ( const HawkDegree& oDeg );

		Bool operator <  ( const HawkDegree& oDeg ) const;

		Bool operator <= ( const HawkDegree& oDeg ) const;

		Bool operator == ( const HawkDegree& oDeg ) const ;

		Bool operator != ( const HawkDegree& oDeg ) const;

		Bool operator >= ( const HawkDegree& oDeg ) const;

		Bool operator >  ( const HawkDegree& oDeg ) const;
	};
}
#endif
