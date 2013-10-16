#ifndef HAWK_MATRIX3_H
#define HAWK_MATRIX3_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 三维矩阵数据，用于空间变换(左手坐标系)                               */
	/************************************************************************/
	class GEO_API HawkMatrix3
	{
	public:
		//构造
		HawkMatrix3();

		HawkMatrix3(const HawkMatrix3& oMat3);
		HawkMatrix3(const HawkMatrix4& oMat4);

		//数据指针
		const Float* Ptr() const ;

		Float* Ptr();

		//操作符重载
		Float& operator()(Int32 iRow, Int32 iCol);

		//矩阵行的单下标访问
		Float* operator[] (Int32 iIdx);

		const Float& operator()(Int32 iRow, Int32 iCol) const;

		//矩阵行的单下标访问
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

		//3*3矩阵相乘
		HawkMatrix3& operator *= (const HawkMatrix3& oMat3);

		//矩阵右乘向量
		HawkVector3D operator * (const HawkVector3D& oVec3) const;

		HawkMatrix3  operator * (const Float& fScale) const;

		HawkMatrix3& operator *= (const Float& fScale);

		void SetUpperLeft2x2(const HawkMatrix2& oMat2);

		HawkVector2D MultiplyByUpperLeft2x2(const HawkVector2D& oVec2) const;

		//判断是否为单位矩阵
		Bool   IsIdentity() const;

		void   SetZero();
		
		//为正交矩阵
		Bool   IsOrthogonalMatrix() const;

		//为旋转矩阵
		Bool   IsRotationMatrix() const;

		//矩阵行列式
		Float  GetDeterminant() const;

		//矩阵基本运算(转置，求逆，模运算，向量乘，单位化......)
		//单位化
		HawkMatrix3& MakeIdentity();

		//获取转置矩阵
		HawkMatrix3  GetTranspose() const;

		//逆矩阵
		HawkMatrix3	 GetInverse() const;

		HawkMatrix3& MakeInverse();

		//转置
		HawkMatrix3& MakeTranspose();

		//平移
		HawkMatrix3& SetTranslation(Float fX, Float fY);

		HawkMatrix3& SetTranslation(const HawkVector2D& oVec2);

		//X,Y,Z为缩放轴
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

		//获得行
		HawkVector3D GetRow(Int32 iRow) const;

		//获得列
		HawkVector3D GetColumn(Int32 iCol) const;

		//设置行
		void   SetRow(Int32 iRow,const HawkVector3D& oVec3);

		//设置列
		void   SetColumn(Int32 iCol,const HawkVector3D& oVec3);

		//转换为四元数
		HawkQuaternion  GetQuaternion();

	public:
		//矩阵数据
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
