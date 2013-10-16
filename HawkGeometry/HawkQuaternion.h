#ifndef HAWK_QUATERNION_H
#define HAWK_QUATERNION_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 四元数表示方式(左手坐标系)                                            */
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

		//选择矩阵构造
		explicit HawkQuaternion(const HawkMatrix3& oRotMat);
		explicit HawkQuaternion(const HawkMatrix4& oRotMat);

		//轴角对构造
		HawkQuaternion(const HawkVector3D& vAxis,Float fRadian);

		//局部正交轴构造
		HawkQuaternion(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis);

		HawkQuaternion& operator  = (const HawkQuaternion& oQuat);
		HawkQuaternion  operator  + (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  - (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  * (const HawkQuaternion& oQuat) const;
		HawkQuaternion  operator  * (Float fVal) const;
		HawkQuaternion  operator  - () const;

		//旋转向量
		HawkVector3D    operator * (const HawkVector3D& oVec3) const;

		Bool operator == (const HawkQuaternion& oQuat) const;
		Bool operator != (const HawkQuaternion& oQuat) const;

		GEO_API friend HawkQuaternion operator* (Float fVal,const HawkQuaternion& oQuat);	

		Float* Ptr();

		const  Float* Ptr() const;

		//旋转矩阵和四元数
		HawkQuaternion&  SetFromMatrix3 (const HawkMatrix3& oRotMat);
		HawkQuaternion&  SetFromMatrix4 (const HawkMatrix4& oRotMat);
		HawkMatrix3      GetAsMatrix3 () const;
		HawkMatrix4      GetAsMatrix4 () const;

		//轴角对和四元数
		void  SetFromAxisAngle (const HawkVector3D& vAxis,Float fRadian);
		void  GetAsAxisAngle (HawkVector3D& vAxis,Float& fRadian) const;

		//旋转Yaw,Pitch,Row(z*x*y)
		HawkQuaternion& SetFromYawPitchRoll(const HawkVector3D& oYawPitchRoll);
		HawkVector3D    GetAsYawPitchRoll() const;

		//本地坐标轴
		HawkQuaternion& SetFromAxis(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis);
		void            GetAsAxis(HawkVector3D& xAxis, HawkVector3D& yAxis, HawkVector3D& zAxis);

		//是否为0
		bool   IsZero() const;
		//是否为单位四元数
		bool   IsUnit() const;
		//单位化四元数,返回单位化之前的模
		Float  Normalize(void); 
		//点乘
		Float  DotProduct (const HawkQuaternion& oQuat) const;
		//模
		Float  Length () const;
		Float  GetMagnitude() const;
		//模的平方
		Float  SQLength () const;
		Float  GetNorm() const;
		//置0
		void   SetZero();
		//置(1,0,0,0)
		void   MakeIdentity();		
		//相等(弧度容差)
		Bool   Equals(const HawkQuaternion& oQuat, Float fDisRadian) const;
		//变逆
		void   MakeInverse();

		//逆四元数
		HawkQuaternion Inverse () const;
		//单位逆四元数
		HawkQuaternion UnitInverse () const;
		//指数运算
		HawkQuaternion Exp () const;
		//对数运算
		HawkQuaternion Log () const;

	public:
		//线性插值
		static HawkQuaternion Slerp (Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkQ, Bool bShortestPath = false);

		static HawkQuaternion SlerpExtraSpins (Float fT,const HawkQuaternion& rkP, const HawkQuaternion& rkQ,Int32 iExtraSpins);

		//二次插值
		static HawkQuaternion Squad (Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkA, const HawkQuaternion& rkB,const HawkQuaternion& rkQ, Bool bShortestPath = false);

		//单位化的线性插值
		static HawkQuaternion Nlerp(Float fT, const HawkQuaternion& rkP, const HawkQuaternion& rkQ, Bool bShortestPath = false);
	};
}
#endif
