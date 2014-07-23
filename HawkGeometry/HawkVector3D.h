#ifndef HAWK_VECTOR3D_H
#define HAWK_VECTOR3D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά������ʾ                                                         */
	/************************************************************************/
	class GEO_API HawkVector3D
	{
	public:
		//��ά��������
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);
		DEFINE_PUBLIC_PROPERTY(Float,Z,Z);

	public:
		//Ĭ�Ϲ��캯��
		HawkVector3D(Float fVal = 0);

		//���캯��,������ʾ��������ֵ
		HawkVector3D(Float fX, Float fY, Float fZ);

		//�������캯��
		HawkVector3D(const HawkVector3D& oVec3);

		//�����为
		HawkVector3D operator - () const;

		//��ֵ����
		HawkVector3D& operator = (const HawkVector3D& oVec3);

		//�������ֲ��������� + - * / += *= /= = ==
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

		//�߼����������
		Bool  operator <= (const HawkVector3D& oVec3) const;

		Bool  operator >= (const HawkVector3D& oVec3) const;

		Bool  operator < (const HawkVector3D& oVec3) const;

		Bool  operator > (const HawkVector3D& oVec3) const;

		Bool  operator == (const HawkVector3D& oVec3) const;

		Bool  operator != (const HawkVector3D& oVec3) const;

		//�����������
		Float  Length() const;

		//����������ȵ�ƽ��
		Float  SQLength() const;

		//�������
		Float  DotProduct(const HawkVector3D& oVec3) const;

		//���ݵ�ַָ��
		Float* Ptr();

		//���ݵ�ַָ��
		const  Float* Ptr() const;

		//������������
		Float  operator [] (int iIdx) const;

		Float& operator [] (int iIdx);

		//�������֮��ľ���
		Float  GetDistanceFrom(const HawkVector3D& oVec3) const;

		//����ֻ����ˮƽ����
		Float  GetHoriDistance(const HawkVector3D& oVec3) const;

		//����ֻ���Ĵ�ֱ����
		Float  GetVertDistance(const HawkVector3D& oVec3) const;

		//�������֮��ľ����ƽ��
		Float  GetSQDistanceFrom(const HawkVector3D& oVec3) const;

		//������λ��
		HawkVector3D&  Normalize();

		//��0
		void   MakeZero();

		void   MakeFloor(const HawkVector3D& oVec3);

		void   MakeCeil(const HawkVector3D& oVec3);

		//������һ���������е�
		HawkVector3D  MidPoint( const HawkVector3D& oVec3 ) const;
		//���
		HawkVector3D  CrossProduct( const HawkVector3D& oVec3 ) const;

		//�ж�������֮��
		Bool IsBetweenPoints(const HawkVector3D& oBegin, const HawkVector3D& oEnd) const;

		//������һ���������в�ֵ����,��ֵϵ��ΪfVal[0,1]
		HawkVector3D GetInterpolated(const HawkVector3D& oVec3, Float fVal) const;
	
		//����һ�����ļн�,���ػ���ֵ
		Float AngleBetween(const HawkVector3D& oVec3);
		
		//����һ��������ת��Ԫ��
		HawkQuaternion GetRotationTo(const HawkVector3D& oVec3) const;

		//�ҳ�
		GEO_API friend  HawkVector3D operator * (Float fVal,const HawkVector3D& oVec3);

		//�����(��������任)
		GEO_API friend  HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix3& oMat3);

		//�����(��������任)
		GEO_API friend  HawkVector3D operator * (const HawkVector3D& oVec3,const HawkMatrix4& oMat4);		
	};
}
#endif
