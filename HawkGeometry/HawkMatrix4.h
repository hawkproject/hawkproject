#ifndef HAWK_MATRIX4_H
#define HAWK_MATRIX4_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 4*4矩阵表示(左手坐标系)                                               */
	/************************************************************************/
	class GEO_API HawkMatrix4
	{
	public:
		//默认构造函数
		HawkMatrix4();		

		//拷贝构造函数
		HawkMatrix4(const HawkMatrix4& oMat);
		explicit HawkMatrix4(const HawkMatrix3& oMat3);

		//行列的下标访问操作
		Float& operator()(Int32 iRow, Int32 iCol);

		//矩阵行的单下标访问
		Float* operator[](Int32 iIdx);

		const Float& operator()(Int32 iRow, Int32 iCol) const;

		//矩阵行的单下标访问
		const Float* operator[](Int32 iIdx) const;

		//赋值操作符重载
		HawkMatrix4& operator = (const HawkMatrix4& oMat4);

		//矩阵数据指针
		Float* Ptr() ;
		const Float* Ptr() const;

		//其他运算操作符重载
		Bool operator==(const HawkMatrix4 &oMat4) const;

		//!=操作符重载
		Bool operator!=(const HawkMatrix4 &oMat4) const;

		//+操作符重载
		HawkMatrix4  operator+(const HawkMatrix4& oMat4) const;

		//+=操作符重载
		HawkMatrix4& operator+=(const HawkMatrix4& oMat4);

		//-操作符重载
		HawkMatrix4  operator-(const HawkMatrix4& oMat4) const;

		//-=操作符重载
		HawkMatrix4& operator-=(const HawkMatrix4& oMat4);

		//*操作符重载
		HawkMatrix4  operator * (const HawkMatrix4& oMat4) const;

		//*=操作符重载
		HawkMatrix4& operator *= (const HawkMatrix4& oMat4);

		//矩阵右乘向量(适应opengl)
		HawkVector4D operator * (const HawkVector4D& oVec4) const;

		HawkVector3D operator * (const HawkVector3D& oVec3) const;

		HawkPlane3D  operator * (const HawkPlane3D& oPlane) const;

		void SetUpperLeft2x2(const HawkMatrix2& oMat2);	
		void SetUpperLeft3x3(const HawkMatrix3& oMat3);	
		HawkVector3D MultiplyByUpperLeft3x3(const HawkVector3D& oVec3) const;

		//判断为仿射矩阵
		Bool IsAffine(void) const;

		//单位化
		HawkMatrix4& MakeIdentity();

		//判断是否为单位矩阵
		Bool  IsIdentity() const;

		//矩阵行列式
		Float GetDeterminant() const;

		//设置矩阵的平移量
		HawkMatrix4& SetTranslation( const HawkVector3D& oVec3 );

		//获取平移量
		HawkVector3D GetTranslation() const;

		//设置缩放量 X,Y,Z为缩放轴
		HawkMatrix4& SetScale( const HawkVector3D& oScale );

		//设置缩放量
		HawkMatrix4& SetScale( Float fScale );

		//获取缩放量
		HawkVector3D GetScale() const;

		//平移一个向量
		void TranslateVect( HawkVector3D& oVec3 ) const;

		//逆向平移向量
		void InverseTranslateVect( HawkVector3D& oVec3 ) const;

		//旋转三维向量
		void RotateVect( HawkVector3D& oVec3 ) const;

		//逆向旋转三维向量
		void InverseRotateVect( HawkVector3D& oVec3 ) const;

		//变化一个三维向量
		void TransformVect( HawkVector3D& oVec3) const;

		//逆矩阵
		Bool MakeInverse();

		void SetZero();

		//获取逆矩阵
		Bool GetInverse(HawkMatrix4& oMat4) const;
		HawkMatrix4 GetInverse() const;

		//获取转置矩阵
		HawkMatrix4 GetTranspose() const;

		//旋转参数的计算
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
		//左手Look_At矩阵
		HawkMatrix4& MakeMatrixLookAtLH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp);
		
		//右手Look_At矩阵
		HawkMatrix4& MakeMatrixLookAtRH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp);
		
		//左手正交矩阵
		//创建一个以屏幕中心为坐标原点的左手系正交投影矩阵
		HawkMatrix4& MakeMatrixOrthoLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//右手正交矩阵
		//创建一个以屏幕中心为坐标原点的左手系正交投影矩阵
		HawkMatrix4& MakeMatrixOrthoRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//左手正交矩阵
		//创建一个自定义2d坐标系的正交投影矩阵
		HawkMatrix4& MakeMatrixOrthoOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//右手正交矩阵
		//创建一个自定义2d坐标系的正交投影矩阵
		HawkMatrix4& MakeMatrixOrthoOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//左手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveFovLH(Float fFovy,Float fAspect,Float fZNear,Float fZFar);

		//右手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveFovRH(Float fFovy,Float fAspect,Float fZNear,Float fZFar);

		//左手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//右手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar);

		//左手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//右手透视投影矩阵
		HawkMatrix4& MakeMatrixPerspectiveOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar);

		//平面投影矩阵
		HawkMatrix4& MakeMatrixReflect(const HawkPlane3D& oPlane);

		//X轴旋转
		//fRadian are measured clockwise when looking along the rotation axis toward the origin.
		HawkMatrix4& MakeMatrixRotationX(Float fRadian);

		//Y轴旋转
		HawkMatrix4& MakeMatrixRotationY(Float fRadian);

		//Z轴旋转
		HawkMatrix4& MakeMatrixRotationZ(Float fRadian);

		//任意轴旋转
		HawkMatrix4& MakeMatrixRotationAxis(const HawkVector3D& oAxis,Float fRadian);

		//Builds a matrix with a specified yaw, pitch, and roll
		//The order of transformations is roll(z) first, then pitch(x), then yaw(y).
		HawkMatrix4& MakeMatrixRotationYawPitchRoll(Float fYaw,Float fPitch,Float fRoll);

		//缩放
		HawkMatrix4& MakeMatrixScaling(Float fX,Float fY,Float fZ);

		//平面方向投影矩阵		
		HawkMatrix4& MakeMatrixShadow(const HawkVector4D& oLightVec,const HawkPlane3D& oPlane);

		//平移矩阵
		HawkMatrix4& MakeMatrixTranslation(Float fX,Float fY,Float fZ);

		//变换矩阵
		HawkMatrix4& MakeMatrixTransform(const HawkVector3D& sPos, const HawkVector3D& sDir, const HawkVector3D& sUp);

	public:
		//矩阵的行列数据信息
		//矩阵数据
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
