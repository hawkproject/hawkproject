#ifndef HAWK_MATRIX3_H
#define HAWK_MATRIX3_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�������ݣ����ڿռ�任(��������ϵ)                               */
	/************************************************************************/
	class GEO_API HawkMatrix3
	{
	public:
		//����
		HawkMatrix3();

		HawkMatrix3(const HawkMatrix3& oMat3);
		HawkMatrix3(const HawkMatrix4& oMat4);

		//����ָ��
		const Float* Ptr() const ;

		Float* Ptr();

		//����������
		Float& operator()(Int32 iRow, Int32 iCol);

		//�����еĵ��±����
		Float* operator[] (Int32 iIdx);

		const Float& operator()(Int32 iRow, Int32 iCol) const;

		//�����еĵ��±����
		const Float* operator[] (Int32 iIdx) const;

		HawkMatrix3& operator = (const HawkMatrix3 &oMat3);

		HawkMatrix3& operator  = (const Float& fScale);

		Bool operator == (const HawkMatrix3& oMat3) const;

		Bool operator != (const HawkMatrix3& oMat3) const;

		HawkMatrix3  operator + (const HawkMatrix3& oMat3) const;

		HawkMatrix3& operator += (const HawkMatrix3& oMat3);

		HawkMatrix3  operator - (const HawkMatrix3& oMat3) const;

		HawkMatrix3& operator -= (const HawkMatrix3& oMat3);

		HawkMatrix3  operator * (const HawkMatrix3& oMat3) const;

		//3*3�������
		HawkMatrix3& operator *= (const HawkMatrix3& oMat3);

		//�����ҳ�����
		HawkVector3D operator * (const HawkVector3D& oVec3) const;

		HawkMatrix3  operator * (const Float& fScale) const;

		HawkMatrix3& operator *= (const Float& fScale);

		void SetUpperLeft2x2(const HawkMatrix2& oMat2);

		HawkVector2D MultiplyByUpperLeft2x2(const HawkVector2D& oVec2) const;

		//�ж��Ƿ�Ϊ��λ����
		Bool   IsIdentity() const;

		void   SetZero();
		
		//Ϊ��������
		Bool   IsOrthogonalMatrix() const;

		//Ϊ��ת����
		Bool   IsRotationMatrix() const;

		//��������ʽ
		Float  GetDeterminant() const;

		//�����������(ת�ã����棬ģ���㣬�����ˣ���λ��......)
		//��λ��
		HawkMatrix3& MakeIdentity();

		//��ȡת�þ���
		HawkMatrix3  GetTranspose() const;

		//�����
		HawkMatrix3	 GetInverse() const;

		HawkMatrix3& MakeInverse();

		//ת��
		HawkMatrix3& MakeTranspose();

		//ƽ��
		HawkMatrix3& SetTranslation(Float fX, Float fY);

		HawkMatrix3& SetTranslation(const HawkVector2D& oVec2);

		//X,Y,ZΪ������
		HawkMatrix3& SetScale(Float fX, Float fY, Float fZ);

		HawkMatrix3& SetScale(const HawkVector3D& oVec3);

		HawkMatrix3& SetScale(Float fScale);

		//Pitch
		HawkMatrix3& SetRotationX(Float fRadian);

		//Yaw
		HawkMatrix3& SetRotationY(Float fRadian);

		//Roll
		HawkMatrix3& SetRotationZ(Float fRadian);

		//Yaw Pitch Roll(z*x*y)
		HawkMatrix3& SetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll);

		//Axis Rotation
		HawkMatrix3& SetRotationAxis(const HawkVector3D& oRefAxis,Float fRadian);

		//Get Yaw Pitch Roll
		void  GetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll);

		//XAxis,YAxis,ZAxis
		HawkMatrix3& SetFromAxis(const HawkVector3D& xAxis,const HawkVector3D& yAxis,const HawkVector3D& zAxis);

		//�����
		HawkVector3D GetRow(Int32 iRow) const;

		//�����
		HawkVector3D GetColumn(Int32 iCol) const;

		//������
		void   SetRow(Int32 iRow,const HawkVector3D& oVec3);

		//������
		void   SetColumn(Int32 iCol,const HawkVector3D& oVec3);

		//ת��Ϊ��Ԫ��
		HawkQuaternion  GetQuaternion();

	public:
		//��������
		union
		{
			struct
			{
				Float M00,M01,M02;
				Float M10,M11,M12;
				Float M20,M21,M22;
			};
			struct 
			{
				Float _M[3][3];
			};
			Float M[9];
		};
	};
}
#endif
