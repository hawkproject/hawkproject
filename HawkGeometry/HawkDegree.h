#ifndef HAWK_DEGREE_H
#define HAWK_DEGREE_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ѧ�����еĽǶ�ֵ                                                   */
	/************************************************************************/
	class GEO_API HawkDegree
	{
	public:
		//���Զ���
		DEFINE_PROPERTY(Float,Degree,Degree);

	public:
		//����
		HawkDegree(Float fDeg = 0);

		HawkDegree(const HawkDegree& oDeg);

		HawkDegree(const HawkRadian& oRad);

		//��ýǶ���ֵ
		Float  Value() const;

		//����ת��Ϊ�Ƕ�
		HawkDegree  operator = (const HawkRadian& oRad);

		//������������� (+ - * / += -= *= /=)
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
