#ifndef HAWK_RADIAN_H
#define HAWK_RADIAN_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 数学计算中的弧度值                                                   */
	/************************************************************************/
	class GEO_API HawkRadian
	{
	public:
		//属性定义
		DEFINE_PROPERTY(Float,Radian,Radian);

	public:
		//构造
		HawkRadian(Float fRad = 0);

		HawkRadian(const HawkRadian& oRad);

		HawkRadian(const HawkDegree& oDeg);

		//获得弧度数值
		Float  Value() const;

		//角度转换为弧度
		HawkRadian  operator = (const HawkDegree& oDeg);

		//各种运算符重载 (+ - * / += -= *= /=)
		HawkRadian& operator = ( const HawkRadian& oRad );

		HawkRadian  operator + ( Float fRad ) const;

		HawkRadian& operator += ( Float fRad ) ;

		HawkRadian  operator + ( const HawkRadian& oRad ) const;

		HawkRadian& operator += ( const HawkRadian& oRad );

		HawkRadian  operator - () const ;

		HawkRadian  operator - ( Float fRad ) const;

		HawkRadian& operator -= ( Float fRad ) ;

		HawkRadian  operator - ( const HawkRadian& oRad ) const;

		HawkRadian& operator -= ( const HawkRadian& oRad );

		HawkRadian  operator * ( Float fScale ) const;

		HawkRadian  operator * ( const HawkRadian& oRad ) const;

		HawkRadian& operator *= ( Float fScale );

		HawkRadian& operator *= ( const HawkRadian& oRad );

		HawkRadian  operator / ( Float fScale ) const;

		HawkRadian& operator /= ( Float fScale ) ;

		HawkRadian  operator / ( const HawkRadian& oRad ) const;

		HawkRadian& operator /= ( const HawkRadian& oRad );

		Bool operator <  ( const HawkRadian& oRad ) const ;

		Bool operator <= ( const HawkRadian& oRad ) const;

		Bool operator == ( const HawkRadian& oRad ) const;

		Bool operator != ( const HawkRadian& oRad ) const;

		Bool operator >= ( const HawkRadian& oRad ) const;

		Bool operator >  ( const HawkRadian& oRad ) const;
	};
}
#endif
