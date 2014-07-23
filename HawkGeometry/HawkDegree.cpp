#include "HawkDegree.h"
#include "HawkRadian.h"

namespace Hawk
{
	//����
	HawkDegree::HawkDegree ( Float fDeg) : Degree(fDeg) {}

	HawkDegree::HawkDegree ( const HawkDegree& oDeg ) : Degree(oDeg.Degree) {}

	HawkDegree::HawkDegree(const HawkRadian& oRad)
	{
		Degree = oRad.Value()*HawkMath::RAD2DEG;
	}

	//����ת��Ϊ�Ƕ�
	HawkDegree  HawkDegree::operator = (const HawkRadian& oRad)
	{
		return HawkDegree(oRad.Value()*HawkMath::RAD2DEG);
	}

	//��ýǶ���ֵ
	Float  HawkDegree::Value() const
	{
		return Degree;
	}

	//������������� (+ - * / += -= *= /=)
	HawkDegree& HawkDegree::operator = ( const HawkDegree& oDeg )
	{
		Degree = oDeg.Degree; 
		return *this; 
	}

	HawkDegree HawkDegree::operator + ( Float fDeg ) const 
	{ 
		return HawkDegree ( Degree + fDeg ); 
	}

	HawkDegree& HawkDegree::operator += ( Float fDeg ) 
	{ 
		Degree += fDeg;
		return *this;
	}

	HawkDegree HawkDegree::operator + ( const HawkDegree& oDeg ) const 
	{ 
		return HawkDegree ( Degree + oDeg.Degree ); 
	}

	HawkDegree& HawkDegree::operator += ( const HawkDegree& oDeg ) 
	{ 
		Degree += oDeg.Degree;
		return *this;
	}

	HawkDegree HawkDegree::operator - () const 
	{ 
		return HawkDegree(-Degree);
	}

	HawkDegree HawkDegree::operator - ( Float fDeg ) const
	{
		return HawkDegree ( Degree - fDeg ); 
	}

	HawkDegree& HawkDegree::operator -= ( Float fDeg ) 
	{
		Degree -= fDeg; 
		return *this; 
	}

	HawkDegree HawkDegree::operator - ( const HawkDegree& oDeg ) const
	{
		return HawkDegree ( Degree - oDeg.Degree ); 
	}

	HawkDegree& HawkDegree::operator -= ( const HawkDegree& oDeg ) 
	{
		Degree -= oDeg.Degree; 
		return *this; 
	}

	HawkDegree HawkDegree::operator * ( Float fScale ) const 
	{
		return HawkDegree ( Degree * fScale );
	}

	HawkDegree HawkDegree::operator * ( const HawkDegree& oDeg ) const
	{ 
		return HawkDegree ( Degree * oDeg.Degree ); 
	}

	HawkDegree& HawkDegree::operator *= ( Float fScale )
	{ 
		Degree *= fScale; 
		return *this; 
	}

	HawkDegree& HawkDegree::operator *= ( const HawkDegree& oDeg )
	{ 
		Degree *= oDeg.Degree; 
		return *this; 
	}

	HawkDegree HawkDegree::operator / ( Float fScale ) const
	{
		A_Exception(!HawkMath::IsZero(fScale));

		return HawkDegree ( Degree / fScale );
	}

	HawkDegree& HawkDegree::operator /= ( Float fScale ) 
	{ 
		A_Exception(!HawkMath::IsZero(fScale));

		Degree /= fScale;
		return *this;
	}

	HawkDegree HawkDegree::operator / ( const HawkDegree& oDeg ) const
	{
		A_Exception(!HawkMath::IsZero(oDeg.Value()));

		return HawkDegree ( Degree / oDeg.Degree );
	}

	HawkDegree& HawkDegree::operator /= ( const HawkDegree& oDeg ) 
	{ 
		A_Exception(!HawkMath::IsZero(oDeg.Value()));

		Degree /= oDeg.Degree;
		return *this;
	}

	Bool HawkDegree::operator <  ( const HawkDegree& oDeg ) const 
	{ 
		return Degree <  oDeg.Degree; 
	}

	Bool HawkDegree::operator <= ( const HawkDegree& oDeg ) const 
	{ 
		return Degree <= oDeg.Degree;
	}

	Bool HawkDegree::operator == ( const HawkDegree& oDeg ) const 
	{ 
		return HawkMath::FloatEqual(Degree,oDeg.Degree); 
	}

	Bool HawkDegree::operator != ( const HawkDegree& oDeg ) const
	{ 
		return Degree != oDeg.Degree; 
	}

	Bool HawkDegree::operator >= ( const HawkDegree& oDeg ) const 
	{ 
		return Degree >= oDeg.Degree; 
	}

	Bool HawkDegree::operator >  ( const HawkDegree& oDeg ) const 
	{ 
		return Degree >  oDeg.Degree;
	}
}

