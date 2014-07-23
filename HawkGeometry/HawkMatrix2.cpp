#include "HawkMatrix2.h"
#include "HawkVector2D.h"
#include "HawkVector3D.h"

namespace Hawk
{
	//构造
	HawkMatrix2::HawkMatrix2()
	{
		MakeIdentity();
	}

	HawkMatrix2::HawkMatrix2(Float fM00,Float fM01,Float fM10,Float fM11) : M00(fM00),M01(fM01),M10(fM10),M11(fM11)
	{
	}

	HawkMatrix2::HawkMatrix2(const HawkMatrix2& oMat2) : M00(oMat2.M00),M01(oMat2.M01),M10(oMat2.M10),M11(oMat2.M11)
	{
	}

	//数据指针
	const Float* HawkMatrix2::Ptr() const 
	{ 
		return M;
	}

	Float* HawkMatrix2::Ptr() 
	{ 
		return M;
	}

	//操作符重载
	Float& HawkMatrix2::operator()(int iRow, int iCol)
	{ 
		A_Exception(iRow>=0 && iRow<2 && iCol>=0 && iCol<2);

		return M[ iRow * 2 + iCol ];
	}

	const Float& HawkMatrix2::operator()(int iRow, int iCol) const 
	{ 
		A_Exception(iRow>=0 && iRow<2 && iCol>=0 && iCol<2);

		return M[ iRow * 2 + iCol ];
	}

	Float* HawkMatrix2::operator[] (int iRow)
	{
		A_Exception(iRow>=0 && iRow<2);

		return (Float*)(&M[iRow*2]);
	}

	const Float* HawkMatrix2::operator[] (int iRow) const
	{
		A_Exception(iRow>=0 && iRow<2);

		return (Float*)(&M[iRow*2]);
	}

	HawkMatrix2& HawkMatrix2::operator = (const HawkMatrix2 &oMat2)
	{
		memcpy(M,oMat2.M,4*sizeof(Float));
		return *this;
	}

	HawkMatrix2& HawkMatrix2::operator  = (const Float& fScale)
	{
		M00 = fScale;M01 = fScale;
		M10 = fScale;M11 = fScale;
		return *this;
	}

	Bool HawkMatrix2::operator == (const HawkMatrix2& oMat2) const
	{
		return HawkMath::FloatEqual(M00,oMat2.M00) && 
			   HawkMath::FloatEqual(M01,oMat2.M01) && 
			   HawkMath::FloatEqual(M10,oMat2.M10) && 
			   HawkMath::FloatEqual(M11,oMat2.M11);
	}

	Bool HawkMatrix2::operator != (const HawkMatrix2& oMat2) const
	{
		return !(*this == oMat2);
	}

	HawkMatrix2 HawkMatrix2::operator + (const HawkMatrix2& oMat2) const
	{
		HawkMatrix2 oTmp;
		oTmp.M00 = M00+oMat2.M00;
		oTmp.M01 = M01+oMat2.M01;
		oTmp.M10 = M10+oMat2.M10;
		oTmp.M11 = M11+oMat2.M11;
		return oTmp;
	}

	HawkMatrix2& HawkMatrix2::operator += (const HawkMatrix2& oMat2)
	{
		M00 += oMat2.M00;
		M01 += oMat2.M01;
		M10 += oMat2.M10;
		M11 += oMat2.M11;
		return *this;
	}

	HawkMatrix2 HawkMatrix2::operator - (const HawkMatrix2& oMat2) const
	{
		HawkMatrix2 oTmp;
		oTmp.M00 = M00-oMat2.M00;
		oTmp.M01 = M00-oMat2.M01;
		oTmp.M10 = M00-oMat2.M10;
		oTmp.M11 = M00-oMat2.M11;
		return oTmp;
	}

	HawkMatrix2& HawkMatrix2::operator -= (const HawkMatrix2& oMat2)
	{
		M00 -= oMat2.M00;
		M01 -= oMat2.M01;
		M10 -= oMat2.M10;
		M11 -= oMat2.M11;
		return *this;
	}

	HawkMatrix2 HawkMatrix2::operator * (const HawkMatrix2& oMat2) const
	{
		if(IsIdentity())
		{
			return oMat2;
		}
		else if (oMat2.IsIdentity()) 
		{
			return *this;
		}

		HawkMatrix2 oTmp;
		for (int iRow = 0; iRow < 2; iRow++)
		{
			for (int iCol = 0; iCol < 2; iCol++)
			{
				oTmp[iRow][iCol] = (*this)[iRow][0]*oMat2[0][iCol] + (*this)[iRow][1]*oMat2[1][iCol];
			}
		}
		return oTmp;
	}

	HawkMatrix2& HawkMatrix2::operator *= (const HawkMatrix2& oMat2)
	{
		if(IsIdentity())
		{
			*this = oMat2;
			return *this;
		}
		else if (oMat2.IsIdentity()) 
		{
			return *this;
		}

		HawkMatrix2 oTmp;
		for (int iRow = 0; iRow < 2; iRow++)
		{
			for (int iCol = 0; iCol < 2; iCol++)
			{
				oTmp[iRow][iCol] = (*this)[iRow][0]*oMat2[0][iCol] + (*this)[iRow][1]*oMat2[1][iCol];
			}
		}
		memcpy(M,oTmp.M,4*sizeof(Float));
		return *this ;
	}

	HawkMatrix2 HawkMatrix2::operator * (const Float& fScale) const
	{
		HawkMatrix2 oTmp;
		for (int iRow = 0; iRow < 2; iRow++)
		{
			for (int iCol = 0; iCol < 2; iCol++)
			{
				oTmp[iRow][iCol] = (*this)[iRow][iCol]*fScale;
			}
		}
		return  oTmp;
	}
	
	HawkMatrix2& HawkMatrix2::operator *= (const Float& fScale)
	{
		for (int iRow = 0; iRow < 2; iRow++)
		{
			for (int iCol = 0; iCol < 2; iCol++)
			{
				(*this)[iRow][iCol] *= fScale;
			}
		}
		return *this;
	}

	HawkVector2D HawkMatrix2::operator * (const HawkVector2D& oVec2) const
	{
		HawkVector2D oTmp;
		oTmp.X = (*this)(0,0)*oVec2.X + (*this)(1,0)*oVec2.Y;
		oTmp.Y = (*this)(0,1)*oVec2.X + (*this)(1,1)*oVec2.Y;
		return oTmp;
	}

	//矩阵基本运算(转置，求逆，模运算，向量乘，单位化......)
	//单位化
	HawkMatrix2& HawkMatrix2::MakeIdentity()
	{
		memset(M, 0, 4*sizeof(Float));
		M[0] = M[2] = 1.0f;
		return *this;
	}

	//判断是否为单位矩阵
	Bool HawkMatrix2::IsIdentity() const
	{
		if (!HawkMath::IsZero(M[0] - 1.0f) || !HawkMath::IsZero(M[3] - 1.0f))
			return false;

		for (int i=0; i<2; ++i)
		{
			for (int j=0; j<2; ++j)
			{
				if ((j != i) && (!HawkMath::IsZero((*this)(i,j))))
					return false;
			}
		}
		return true;
	}

	//获取转置矩阵
	HawkMatrix2 HawkMatrix2::GetTranspose() const
	{
		HawkMatrix2 oTmp;
		GetTranspose(oTmp);
		return oTmp;
	}

	//获取转置矩阵
	void HawkMatrix2::GetTranspose( HawkMatrix2& oMat2 ) const
	{
		oMat2[0][0] = (*this)[0][0];
		oMat2[0][1] = (*this)[1][0];
		oMat2[1][0] = (*this)[0][1];
		oMat2[1][1] = (*this)[1][1];
	}

	//转置
	HawkMatrix2& HawkMatrix2::MakeTranspose()
	{
		*this = GetTranspose();
		return *this;
	}

	//零矩阵
	void  HawkMatrix2::SetZero()
	{
		memset(M,0,4*sizeof(Float));
	}

	//获取逆矩阵
	HawkMatrix2	HawkMatrix2::GetInverse() const
	{
		HawkMatrix2 oInv;
		Float fDet = GetDeterminant();

		if (HawkMath::IsZero(fDet))
		{
			oInv.SetZero();	
		}
		else
		{
			fDet = 1.0f / fDet;

			oInv.M00 = fDet * M11;
			oInv.M11 = fDet * M00;
			oInv.M01 = -fDet * M01;
			oInv.M10 = -fDet * M10;
		}

		return oInv;
	}
	
	//变逆矩阵
	HawkMatrix2& HawkMatrix2::MakeInverse()
	{
		*this = GetInverse();
		return *this;
	}

	//矩阵行列式
	Float HawkMatrix2::GetDeterminant() const
	{
		return M00 * M11 - M01 * M10;
	}

	//设置缩放量
	HawkMatrix2& HawkMatrix2::SetScale(Float fX, Float fY)
	{
		MakeIdentity();
		M00 = fX;
		M11 = fY;
		return *this;
	}

	//设置缩放量
	HawkMatrix2& HawkMatrix2::SetScale(const HawkVector2D& oVec2)
	{
		return SetScale(oVec2.X,oVec2.Y);
	}

	//设置缩放量
	HawkMatrix2& HawkMatrix2::SetScale(Float fScale)
	{
		return SetScale(fScale,fScale);
	}

	//设置旋转量
	HawkMatrix2& HawkMatrix2::SetRotation(Float fRadian)
	{
		MakeIdentity();
		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M01 = sinAlpha;
		M00 = M10 = cosAlpha;
		M10 = -sinAlpha;

		return *this;
	}

	//正交矩阵:逆矩阵 == 转置矩阵
	Bool HawkMatrix2::IsOrthogonalMatrix() const
	{
		return GetInverse() == GetTranspose();
	}
	
	//获得行
	HawkVector2D HawkMatrix2::GetRow(Int32 iRow)
	{	
		A_Exception(iRow>=0 && iRow<=1);
		return HawkVector2D((*this)(iRow,0),(*this)(iRow,1));
	}

	//获得列
	HawkVector2D HawkMatrix2::GetColumn(Int32 iCol)
	{
		A_Exception(iCol>=0 && iCol<=1);
		return HawkVector2D((*this)(0,iCol),(*this)(1,iCol));
	}

	void HawkMatrix2::SetRow(Int32 iRow,const HawkVector2D& oVec2)
	{
		(*this)(iRow,0) = oVec2.X;
		(*this)(iRow,1) = oVec2.Y;
	}

	void HawkMatrix2::SetColumn(Int32 iCol,const HawkVector2D& oVec2)
	{
		(*this)(0,iCol) = oVec2.X;
		(*this)(1,iCol) = oVec2.Y;
	}
}
