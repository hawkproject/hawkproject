#ifndef HAWK_MATRIX2_H
#define HAWK_MATRIX2_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 二维矩阵操作定义                                                     */
	/************************************************************************/	
	class GEO_API HawkMatrix2
	{
	public:
		//构造
		HawkMatrix2();

		HawkMatrix2(Float fM00,Float fM01,Float fM10,Float fM11);

		HawkMatrix2(const HawkMatrix2& oMat2);

		//数据指针
		const Float* Ptr() const;

		Float* Ptr();

		//操作符重载
		Float& operator()(int iRow, int iCol);

		const Float& operator()(int iRow, int iCol) const;

		Float* operator[] (int iRow);

		const Float* operator[] (int iRow) const;

		HawkMatrix2& operator = (const HawkMatrix2 &oMat2);

		HawkMatrix2& operator = (const Float& fScale);

		Bool operator == (const HawkMatrix2& oMat2) const;

		Bool operator != (const HawkMatrix2& oMat2) const;

		HawkMatrix2  operator + (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator += (const HawkMatrix2& oMat2);

		HawkMatrix2  operator - (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator -= (const HawkMatrix2& oMat2);

		HawkMatrix2  operator * (const HawkMatrix2& oMat2) const;

		HawkMatrix2& operator *= (const HawkMatrix2& oMat2);

		HawkMatrix2  operator * (const Float& fScale) const;

		HawkMatrix2& operator *= (const Float& fScale);

		//矩阵右乘向量(适应opengl)
		HawkVector2D operator * (const HawkVector2D& oVec2) const;

		//判断是否为单位矩阵
		Bool  IsIdentity() const;
		
		//获取转置矩阵
		void  GetTranspose( HawkMatrix2& oMat2 ) const;

		//零矩阵
		void  SetZero();
		
		//矩阵行列式
		Float GetDeterminant() const;

		//正交矩阵:逆矩阵 == 转置矩阵
		Bool  IsOrthogonalMatrix() const;		

		//矩阵基本运算(转置，求逆，模运算，向量乘，单位化......)
		//单位化
		HawkMatrix2& MakeIdentity();

		//获取转置矩阵
		HawkMatrix2  GetTranspose() const;

		//转置
		HawkMatrix2& MakeTranspose();

		//获取逆矩阵
		HawkMatrix2	 GetInverse() const;
		
		//变逆矩阵
		HawkMatrix2& MakeInverse();

		//设置缩放量
		HawkMatrix2& SetScale(Float fX, Float fY);

		//设置缩放量
		HawkMatrix2& SetScale(const HawkVector2D& oVec2);

		//设置缩放量
		HawkMatrix2& SetScale(Float fScale);

		//设置旋转量
		HawkMatrix2& SetRotation(Float fRadian);

		//获得行
		HawkVector2D GetRow(Int32 iRow);

		//获得列
		HawkVector2D GetColumn(Int32 iCol);
		
		//设置行
		void   SetRow(Int32 iRow,const HawkVector2D& oVec2);

		//设置列
		void   SetColumn(Int32 iCol,const HawkVector2D& oVec2);

	public:
		//矩阵数据
		union
		{
			struct
			{
				Float M00,M01;
				Float M10,M11;
			};
			Float M[4];
		};
	};
}
#endif
