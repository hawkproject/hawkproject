#include "HawkRadian.h"
#include "HawkDegree.h"

namespace Hawk
{
	//����
	HawkRadian::HawkRadian ( Float fRad) : Radian(fRad) {}

	HawkRadian::HawkRadian ( const HawkRadian& oRad ) : Radian(oRad.Radian) {}

	HawkRadian::HawkRadian(const HawkDegree& oDeg)
	{
		Radian = oDeg.Value()*HawkMath::DEG2RAD;
	}

	//�Ƕ�ת��Ϊ����
	HawkRadian  HawkRadian::operator = (const HawkDegree& oDeg)
	{
		return HawkRadian(oDeg.Value()*HawkMath::DEG2RAD);
	}

	//��û�����ֵ
	Float  HawkRadian::Value() const
	{
		return Radian;
	}

	//������������� (+ - * / += -= *= /=)
	HawkRadian& HawkRadian::operator = ( const HawkRadian& oRad )
	{
		Radian = oRad.Radian; 
		return *this; 
	}

	HawkRadian HawkRadian::operator + ( Float fRad ) const 
	{ 
		return HawkRadian ( Radian + fRad ); 
	}

	HawkRadian& HawkRadian::operator += ( Float fRad ) 
	{ 
		Radian += fRad;
		return *this;
	}

	HawkRadian HawkRadian::operator + ( const HawkRadian& oRad ) const 
	{ 
		return HawkRadian ( Radian + oRad.Radian ); 
	}

	HawkRadian& HawkRadian::operator += ( const HawkRadian& oRad ) 
	{ 
		Radian += oRad.Radian;
		return *this;
	}

	HawkRadian HawkRadian::operator - () const 
	{ 
		return HawkRadian(-Radian);
	}

	HawkRadian HawkRadian::operator - ( Float fRad ) const
	{
		return HawkRadian ( Radian - fRad ); 
	}

	HawkRadian& HawkRadian::operator -= ( Float fRad ) 
	{
		Radian -= fRad; 
		return *this; 
	}

	HawkRadian HawkRadian::operator - ( const HawkRadian& oRad ) const
	{
		return HawkRadian ( Radian - oRad.Radian ); 
	}

	HawkRadian& HawkRadian::operator -= ( const HawkRadian& oRad ) 
	{
		Radian -= oRad.Radian; 
		return *this; 
	}

	HawkRadian HawkRadian::operator * ( Float fScale ) const 
	{
		return HawkRadian ( Radian * fScale );
	}

	HawkRadian HawkRadian::operator * ( const HawkRadian& oRad ) const
	{ 
		return HawkRadian ( Radian * oRad.Radian ); 
	}

	HawkRadian& HawkRadian::operator *= ( Float fScale )
	{ 
		Radian *= fScale; 
		return *this; 
	}

	HawkRadian& HawkRadian::operator *= ( const HawkRadian& oRad )
	{ 
		Radian *= oRad.Radian; 
		return *this; 
	}

	HawkRadian HawkRadian::operator / ( Float fScale ) const
	{
		A_Exception(!HawkMath::IsZero(fScale));

		return HawkRadian ( Radian / fScale );
	}

	HawkRadian& HawkRadian::operator /= ( Float fScale ) 
	{ 
		A_Exception(!HawkMath::IsZero(fScale));

		Radian /= fScale;
		return *this;
	}

	HawkRadian HawkRadian::operator / ( const HawkRadian& oRad ) const
	{
		A_Exception(!HawkMath::IsZero(oRad.Value()));

		return HawkRadian ( Radian / oRad.Radian );
	}

	HawkRadian& HawkRadian::operator /= ( const HawkRadian& oRad ) 
	{ 
		A_Exception(!HawkMath::IsZero(oRad.Value()));

		Radian /= oRad.Radian;
		return *this;
	}

	Bool HawkRadian::operator <  ( const HawkRadian& oRad ) const 
	{ 
		return Radian <  oRad.Radian; 
	}

	Bool HawkRadian::operator <= ( const HawkRadian& oRad ) const 
	{ 
		return Radian <= oRad.Radian;
	}

	Bool HawkRadian::operator == ( const HawkRadian& oRad ) const 
	{ 
		return HawkMath::FloatEqual(Radian,oRad.Radian); 
	}

	Bool HawkRadian::operator != ( const HawkRadian& oRad ) const
	{ 
		return Radian != oRad.Radian; 
	}

	Bool HawkRadian::operator >= ( const HawkRadian& oRad ) const 
	{ 
		return Radian >= oRad.Radian; 
	}

	Bool HawkRadian::operator >  ( const HawkRadian& oRad ) const 
	{ 
		return Radian >  oRad.Radian;
	}
}
