#ifndef HAWK_VECTOR2D_H
#define HAWK_VECTOR2D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��άƽ���еĵ�,ʹ�ø�������Ϊ������������                            */
	/************************************************************************/
	class GEO_API HawkVector2D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);

	public:
		//Ĭ�Ϲ��캯��
		HawkVector2D(Float fVal = 0);

		//���캯��,������ʾ��������ֵ
		HawkVector2D(Float fX, Float fY);

		//�������캯��
		HawkVector2D(const HawkVector2D& oVec2);

		//�㹹��
		HawkVector2D(const HawkPoint2D& oPt2);

		//�����为
		HawkVector2D  operator - () const ;

		//��ֵ����
		HawkVector2D& operator = (const HawkVector2D& oVec2);

		//�������ֲ��������� = - * / += -= *= /= = ==
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

		//�߼����������
		Bool operator == (const HawkVector2D& oVec2) const;

		Bool operator != (const HawkVector2D& oVec2) const;

		//�����������
		Float Length() const;

		//����������ȵ�ƽ��
		Float SQLength() const ;

		//��ά�������
		Float DotProduct(const HawkVector2D& oVec2) const;

		//�������֮��ľ���
		Float GetDistanceFrom(const HawkVector2D& oVec2) const;

		//�������֮��ľ����ƽ��
		Float GetSQDistanceFrom(const HawkVector2D& oVec2) const;

		//������λ��
		HawkVector2D& Normalize();

		//������һ���������е�
		HawkVector2D  MidPoint( const HawkVector2D& oVec2 ) const;

		//���
		Float CrossProduct( const HawkVector2D& oVec2 ) const;

		//�ж�������֮��
		Bool  IsBetweenPoints(const HawkVector2D& oBegin, const HawkVector2D& oEnd) const;

		//������һ���������в�ֵ����,��ֵϵ��ΪfVal[0,1]
		HawkVector2D GetInterpolated(const HawkVector2D& oVec2, Float fVal) const;

		//ƽ��
		HawkVector2D& Move(const HawkVector2D& oMove);

		//����
		HawkVector2D& Scale(Float fX,Float fY);

		//����
		HawkVector2D& Scale(const HawkVector2D& oScale);

		//��ת
		HawkVector2D& Rotate(const HawkDegree& oDegree);

		//��ת
		HawkVector2D& Rotate(const HawkRadian& oRadian);

		//��������
		GEO_API friend HawkVector2D operator * (Float fVal,const HawkVector2D& oVec2);

		//�����,��������任
		GEO_API friend HawkVector2D operator * (const HawkVector2D& oVec2,const HawkMatrix2& oMat2);
	};
}
#endif
