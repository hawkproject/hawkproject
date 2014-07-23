#ifndef HAWK_MATRIX4_H
#define HAWK_MATRIX4_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 4*4�����ʾ(��������ϵ)                                               */
	/************************************************************************/
	class GEO_API HawkMatrix4
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkMatrix4();		

		//�������캯��
		HawkMatrix4(const HawkMatrix4& oMat);
		explicit HawkMatrix4(const HawkMatrix3& oMat3);

		//���е��±���ʲ���
		Float& operator()(Int32 iRow, Int32 iCol);

		//�����еĵ��±����
		Float* operator[](Int32 iIdx);

		const Float& operator()(Int32 iRow, Int32 iCol) const;

		//�����еĵ��±����
		const Float* operator[](Int32 iIdx) const;

		//��ֵ����������
		HawkMatrix4& operator = (const HawkMatrix4& oMat4);

		//��������ָ��
		Float* Ptr() ;
		const Float* Ptr() const;

		//�����������������
		Bool operator==(const HawkMatrix4 &oMat4) const;

		//!=����������
		Bool operator!=(const HawkMatrix4 &oMat4) const;

		//+����������
		HawkMatrix4  operator+(const HawkMatrix4& oMat4) const;

		//+=����������
		HawkMatrix4& operator+=(const HawkMatrix4& oMat4);

		//-����������
		HawkMatrix4  operator-(const HawkMatrix4& oMat4) const;

		//-=����������
		HawkMatrix4& operator-=(const HawkMatrix4& oMat4);

		//*����������
		HawkMatrix4  operator * (const HawkMatrix4& oMat4) const;

		//*=����������
		HawkMatrix4& operator *= (const HawkMatrix4& oMat4);

		//�����ҳ�����(��Ӧopengl)
		HawkVector4D operator * (const HawkVector4D& oVec4) const;

		HawkVector3D operator * (const HawkVector3D& oVec3) const;

		HawkPlane3D  operator * (const HawkPlane3D& oPlane) const;

		void SetUpperLeft2x2(const HawkMatrix2& oMat2);	
		void SetUpperLeft3x3(const HawkMatrix3& oMat3);	
		HawkVector3D MultiplyByUpperLeft3x3(const HawkVector3D& oVec3) const;

		//�ж�Ϊ�������
		Bool IsAffine(void) const;

		//��λ��
		HawkMatrix4& MakeIdentity();

		//�ж��Ƿ�Ϊ��λ����
		Bool  IsIdentity() const;

		//��������ʽ
		Float GetDeterminant() const;

		//���þ����ƽ����
		HawkMatrix4& SetTranslation( const HawkVector3D& oVec3 );

		//��ȡƽ����
		HawkVector3D GetTranslation() const;

		//���������� X,Y,ZΪ������
		HawkMatrix4& SetScale( const HawkVector3D& oScale );

		//����������
		HawkMatrix4& SetScale( Float fScale );

		//��ȡ������
		HawkVector3D GetScale() const;

		//ƽ��һ������
		void TranslateVect( HawkVector3D& oVec3 ) const;

		//����ƽ������
		void InverseTranslateVect( HawkVector3D& oVec3 ) const;

		//��ת��ά����
		void RotateVect( HawkVector3D& oVec3 ) const;

		//������ת��ά����
		void InverseRotateVect( HawkVector3D& oVec3 ) const;

		//�仯һ����ά����
		void TransformVect( HawkVector3D& oVec3) const;

		//�����
		Bool MakeInverse();

		void SetZero();

		//��ȡ�����
		Bool GetInverse(HawkMatrix4& oMat4) const;
		HawkMatrix4 GetInverse() const;

		//��ȡת�þ���
		HawkMatrix4 GetTranspose() const;

		//��ת�����ļ���
		//Pitch
		HawkMatrix4& SetRotationX(Float fRadian);

		//Yaw
		HawkMatrix4& SetRotationY(Float fRadian);

		//Roll
		HawkMatrix4& SetRotationZ(Float fRadian);

		//Yaw Pitch Roll(z*x*y)
		HawkMatrix4& SetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll);

		//Axis Rotation
		HawkMatrix4& SetRotationAxis(const HawkVector3D& oRefAxis,Float fRadian);

		//Quaternion
		HawkMatrix4& SetRotation(const HawkQuaternion& oQuat);

		//Get Yaw Pitch Roll
		void  GetYawPitchRoll(Float& fYaw,Float& fPitch,Float& fRoll);

		HawkVector3D  GetRow(Int32 iRow) const;
		HawkVector3D  GetColumn(Int32 iCol) const;

		void  SetRow(Int32 iRow,const HawkVector3D& oVec);
		void  SetColumn(Int32 iCol,const HawkVector3D& oVec);

	 public:
		//����Look_At����
		HawkMatrix4& MakeMatrixLookAtLH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp);
		
		//����Look_At����
		HawkMatrix4& MakeMatrixLookAtRH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp);
		
		//������������
		//����һ������Ļ����Ϊ����ԭ�������ϵ����ͶӰ����
		HawkMatrix4& MakeMatrixOrthoLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//������������
		//����һ������Ļ����Ϊ����ԭ�������ϵ����ͶӰ����
		HawkMatrix4& MakeMatrixOrthoRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//������������
		//����һ���Զ���2d����ϵ������ͶӰ����
		HawkMatrix4& MakeMatrixOrthoOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//������������
		//����һ���Զ���2d����ϵ������ͶӰ����
		HawkMatrix4& MakeMatrixOrthoOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveFovLH(Float fFovy,Float fAspect,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveFovRH(Float fFovy,Float fAspect,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//����͸��ͶӰ����
		HawkMatrix4& MakeMatrixPerspectiveOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//ƽ��ͶӰ����
		HawkMatrix4& MakeMatrixReflect(const HawkPlane3D& oPlane);

		//X����ת
		//fRadian are measured clockwise when looking along the rotation axis toward the origin.
		HawkMatrix4& MakeMatrixRotationX(Float fRadian);

		//Y����ת
		HawkMatrix4& MakeMatrixRotationY(Float fRadian);

		//Z����ת
		HawkMatrix4& MakeMatrixRotationZ(Float fRadian);

		//��������ת
		HawkMatrix4& MakeMatrixRotationAxis(const HawkVector3D& oAxis,Float fRadian);

		//Builds a matrix with a specified yaw, pitch, and roll
		//The order of transformations is roll(z) first, then pitch(x), then yaw(y).
		HawkMatrix4& MakeMatrixRotationYawPitchRoll(Float fYaw,Float fPitch,Float fRoll);

		//����
		HawkMatrix4& MakeMatrixScaling(Float fX,Float fY,Float fZ);

		//ƽ�淽��ͶӰ����		
		HawkMatrix4& MakeMatrixShadow(const HawkVector4D& oLightVec,const HawkPlane3D& oPlane);

		//ƽ�ƾ���
		HawkMatrix4& MakeMatrixTranslation(Float fX,Float fY,Float fZ);

		//�任����
		HawkMatrix4& MakeMatrixTransform(const HawkVector3D& sPos, const HawkVector3D& sDir, const HawkVector3D& sUp);

	public:
		//���������������Ϣ
		//��������
		union
		{
			struct
			{
				Float M00,M01,M02,M03;
				Float M10,M11,M12,M13;
				Float M20,M21,M22,M23;
				Float M30,M31,M32,M33;
			};
			struct 
			{
				Float _M[4][4];
			};
			Float M[16];
		};
	};

}
#endif
