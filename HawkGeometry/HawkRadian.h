#ifndef HAWK_RADIAN_H
#define HAWK_RADIAN_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ѧ�����еĻ���ֵ                                                   */
	/************************************************************************/
	class GEO_API HawkRadian
	{
	public:
		//���Զ���
		DEFINE_PROPERTY(Float,Radian,Radian);

	public:
		//����
		HawkRadian(Float fRad = 0);

		HawkRadian(const HawkRadian& oRad);

		HawkRadian(const HawkDegree& oDeg);

		//��û�����ֵ
		Float  Value() const;

		//�Ƕ�ת��Ϊ����
		HawkRadian  operator = (const HawkDegree& oDeg);

		//������������� (+ - * / += -= *= /=)
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
