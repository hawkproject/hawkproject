#ifndef HAWK_QUATERNION_H
#define HAWK_QUATERNION_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ��Ԫ����ʾ��ʽ(��������ϵ)                                            */
	/************************************************************************/
	class GEO_API HawkQuaternion
	{
	public:
		DEFINE_PUBLIC_PROPERTY(Float,W,W);
		DEFINE_PUBLIC_PROPERTY(Float,X,X);
		DEFINE_PUBLIC_PROPERTY(Float,Y,Y);
		DEFINE_PUBLIC_PROPERTY(Float,Z,Z);

	public:
		HawkQuaternion (Float fW = 1.0f,Float fX = 0.0f, Float fY = 0.0f, Float fZ = 0.0f);

		//ѡ�������
		explicit HawkQuaternion(const HawkMatrix3& oRotMat);
		explicit HawkQuaternion(const HawkMatrix4& oRotMat);

		//��ǶԹ���
		HawkQuaternion(const HawkVector3D& vAxis,Float fRadian);

		//�ֲ������ṹ��
		HawkQuaternion(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis);

		HawkQuaternion& operator  = (const HawkQuaternion& oQuat);
		HawkQuaternion  operator  + (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  - (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  * (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  * (Float fVal) const;
		HawkQuaternion  operator  - () const;

		//��ת����
		HawkVector3D    operator * (const HawkVector3D& oVec3) const;

		Bool operator == (const HawkQuaternion& oQuat) const;
		Bool operator != (const HawkQuaternion& oQuat) const;

		GEO_API friend HawkQuaternion operator* (Float fVal,const HawkQuaternion& oQuat);	

		Float* Ptr();

		const  Float* Ptr() const;

		//��ת�������Ԫ��
		HawkQuaternion&  SetFromMatrix3 (const HawkMatrix3& oRotMat);
		HawkQuaternion&  SetFromMatrix4 (const HawkMatrix4& oRotMat);
		HawkMatrix3      GetAsMatrix3 () const;
		HawkMatrix4      GetAsMatrix4 () const;

		//��ǶԺ���Ԫ��
		void  SetFromAxisAngle (const HawkVector3D& vAxis,Float fRadian);
		void  GetAsAxisAngle (HawkVector3D& vAxis,Float& fRadian) const;

		//��תYaw,Pitch,Row(z*x*y)
		HawkQuaternion& SetFromYawPitchRoll(const HawkVector3D& oYawPitchRoll);
		HawkVector3D    GetAsYawPitchRoll() const;

		//����������
		HawkQuaternion& SetFromAxis(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis);
		void            GetAsAxis(HawkVector3D& xAxis, HawkVector3D& yAxis, HawkVector3D& zAxis);

		//�Ƿ�Ϊ0
		bool   IsZero() const;
		//�Ƿ�Ϊ��λ��Ԫ��
		bool   IsUnit() const;
		//��λ����Ԫ��,���ص�λ��֮ǰ��ģ
		Float  Normalize(void); 
		//���
		Float  DotProduct (const HawkQuaternion& oQuat) const;
		//ģ
		Float  Length () const;
		Float  GetMagnitude() const;
		//ģ��ƽ��
		Float  SQLength () const;
		Float  GetNorm() const;
		//��0
		void   SetZero();
		//��(1,0,0,0)
		void   MakeIdentity();		
		//���(�����ݲ�)
		Bool   Equals(const HawkQuaternion& oQuat, Float fDisRadian) const;
		//����
		void   MakeInverse();

		//����Ԫ��
		HawkQuaternion Inverse () const;
		//��λ����Ԫ��
		HawkQuaternion UnitInverse () const;
		//ָ������
		HawkQuaternion Exp () const;
		//��������
		HawkQuaternion Log () const;

	public:
		//���Բ�ֵ
		static HawkQuaternion Slerp (Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkQ, Bool bShortestPath = false);

		static HawkQuaternion SlerpExtraSpins (Float fT,const HawkQuaternion& rkP, const HawkQuaternion& rkQ,Int32 iExtraSpins);

		//���β�ֵ
		static HawkQuaternion Squad (Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkA, const HawkQuaternion& rkB,const HawkQuaternion& rkQ, Bool bShortestPath = false);

		//��λ�������Բ�ֵ
		static HawkQuaternion Nlerp(Float fT, const HawkQuaternion& rkP, const HawkQuaternion& rkQ, Bool bShortestPath = false);
	};
}
#endif
