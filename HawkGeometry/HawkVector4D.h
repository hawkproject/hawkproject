#ifndef HAWK_VECTOR4D_H
#define HAWK_VECTOR4D_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ˼ά�ռ�������,�����������;������                                */
	/************************************************************************/
	class GEO_API HawkVector4D
	{
	public:
		//��������
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);
		DEFINE_PUBLIC_PROPERTY(Float,Z,Z);
		DEFINE_PUBLIC_PROPERTY(Float,W,W);

	public:
		HawkVector4D();

		HawkVector4D(Float fX, Float fY, Float fZ, Float fW );

		HawkVector4D(const HawkVector3D& oVec3);

		HawkVector4D(const HawkVector4D& oVec4);

		//�������������
		HawkVector4D& operator = ( const HawkVector4D& oVec4 );

		HawkVector4D& operator = ( const Float fVal);

		//���ݵ�ַָ��
		Float*  Ptr();
		
		//���ݵ�ַָ��
		const Float*  Ptr() const;

		//������������
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

		//�����������
		Float  Length() const;

		//����������ȵ�ƽ��
		Float  SQLength() const;

		//��ά�������
		Float  DotProduct(const HawkVector4D& oVec4) const;

		//��λ��
		HawkVector4D&  Normalize();

		//������һ���������в�ֵ����,��ֵϵ��ΪfVal[0,1]
		HawkVector4D   GetInterpolated(const HawkVector4D& oVec4, Float fVal) const;

		//�������任
		GEO_API friend HawkVector4D operator * (const HawkVector4D& oVec4,const HawkMatrix4& oMat4);
	};
}
#endif
