#include "HawkMatrix3.h"
#include "HawkMatrix2.h"
#include "HawkMatrix4.h"
#include "HawkQuaternion.h"
#include "HawkVector2D.h"
#include "HawkVector3D.h"

namespace Hawk
{
	//构造
	HawkMatrix3::HawkMatrix3()
	{
		MakeIdentity();
	}

	HawkMatrix3::HawkMatrix3(const HawkMatrix3& oMat3)
	{
		memcpy(M,oMat3.M,9*sizeof(Float));
	}

	HawkMatrix3::HawkMatrix3(const HawkMatrix4& oMat4)
		:	M00(oMat4.M00), M01(oMat4.M01), M02(oMat4.M02),
			M10(oMat4.M10), M11(oMat4.M11), M12(oMat4.M12),
			M20(oMat4.M20), M21(oMat4.M21), M22(oMat4.M22)
	{
	}
	
	//数据指针
	const Float* HawkMatrix3::Ptr() const 
	{ 
		return M;
	}

	Float* HawkMatrix3::Ptr() 
	{ 
		return M;
	}

	//操作符重载
	Float& HawkMatrix3::operator()(Int32 iRow, Int32 iCol)
	{ 
		A_Exception(iRow>=0 && iRow<3 && iCol>=0 && iCol<3);

		return M[ iRow * 3 + iCol ];
	}

	const Float& HawkMatrix3::operator()(Int32 iRow, Int32 iCol) const 
	{ 
		A_Exception(iRow>=0 && iRow<3 && iCol>=0 && iCol<3);

		return M[ iRow * 3 + iCol ];
	}

	Float* HawkMatrix3::operator[] (Int32 iIdx)
	{
		A_Exception(iIdx>=0 && iIdx<3);

		return _M[iIdx];
	}

	const Float* HawkMatrix3::operator[] (Int32 iIdx) const
	{
		A_Exception(iIdx>=0 && iIdx<3);

		return _M[iIdx];
	}

	HawkMatrix3& HawkMatrix3::operator = (const HawkMatrix3 &oMat3)
	{
		memcpy(M,oMat3.M,9*sizeof(Float));

		return *this;
	}

	HawkMatrix3& HawkMatrix3::operator  = (const Float& fScale)
	{
		for (Int32 i=0;i<9;i++) 
			M[i] = fScale;

		return *this;
	}

	Bool HawkMatrix3::operator == (const HawkMatrix3& oMat3) const
	{
		for (Int32 i=0;i<9;i++)
		{
			if (!HawkMath::FloatEqual(M[i],oMat3.M[i])) 
				return false;
		}
		return true;
	}

	Bool HawkMatrix3::operator != (const HawkMatrix3& oMat3) const
	{
		return !(*this == oMat3);
	}

	HawkMatrix3 HawkMatrix3::operator + (const HawkMatrix3& oMat3) const
	{
		HawkMatrix3 oTmp;

		for (Int32 i=0;i<9;i++)
		{
			oTmp.M[i] = M[i] + oMat3.M[i];
		}

		return oTmp;
	}

	HawkMatrix3& HawkMatrix3::operator += (const HawkMatrix3& oMat3)
	{
		for (Int32 i=0;i<9;i++)
		{
			M[i] += oMat3.M[i];
		}

		return *this;
	}

	HawkMatrix3 HawkMatrix3::operator - (const HawkMatrix3& oMat3) const
	{
		HawkMatrix3 oTmp;

		for (Int32 i=0;i<9;i++)
		{
			oTmp.M[i] = M[i] - oMat3.M[i];
		}

		return oTmp;
	}

	HawkMatrix3& HawkMatrix3::operator -= (const HawkMatrix3& oMat3)
	{
		for (Int32 i=0;i<9;i++)
		{
			M[i] -= oMat3.M[i];
		}

		return *this;
	}

	HawkMatrix3 HawkMatrix3::operator * (const HawkMatrix3& oMat3) const
	{
		if(IsIdentity())
			return oMat3;

		if (oMat3.IsIdentity()) 
			return *this;

		HawkMatrix3 oTmp;

		for (Int32 iRow = 0; iRow < 3; iRow++)
		{
			for (Int32 iCol = 0; iCol < 3; iCol++)
			{
				oTmp(iRow,iCol) = (*this)(iRow,0)*oMat3(0,iCol) + 
					(*this)(iRow,1)*oMat3(1,iCol) + 
					(*this)(iRow,2)*oMat3(2,iCol);
			}
		}
		return oTmp;
	}

	//3*3矩阵相乘
	HawkMatrix3& HawkMatrix3::operator *= (const HawkMatrix3& oMat3)
	{
		*this = *this*oMat3;

		return *this ;
	}

	//矩阵右乘向量(适应opengl)
	HawkVector3D HawkMatrix3::operator* (const HawkVector3D& oVec3) const
	{
		HawkVector3D kProd;
		for (Int32 iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				(*this)(iRow,0)*oVec3[0] +
				(*this)(iRow,1)*oVec3[1] +
				(*this)(iRow,2)*oVec3[2];
		}
		return kProd;
	}

	HawkMatrix3 HawkMatrix3::operator * (const Float& fScale) const
	{
		HawkMatrix3 oTmp;
		for (Int32 iRow = 0; iRow < 3; iRow++)
		{
			for (Int32 iCol = 0; iCol < 3; iCol++)
			{
				oTmp(iRow,iCol) = (*this)(iRow,iCol)*fScale;
			}
		}
		return  oTmp;
	}

	HawkMatrix3& HawkMatrix3::operator *= (const Float& fScale)
	{
		for (Int32 iRow = 0; iRow < 3; iRow++)
		{
			for (Int32 iCol = 0; iCol < 3; iCol++)
			{
				(*this)(iRow,iCol) *= fScale;
			}
		}
		return *this;
	}

	//矩阵基本运算(转置，求逆，模运算，向量乘，单位化......)
	//单位化
	HawkMatrix3& HawkMatrix3::MakeIdentity()
	{
		memset(M, 0, 9*sizeof(Float));
		M[0] = M[4] = M[8] = 1.0f;
		return *this;
	}

	//判断是否为单位矩阵
	Bool HawkMatrix3::IsIdentity() const
	{
		if (!HawkMath::IsZero(M[0] - 1.0f) || !HawkMath::IsZero(M[4] - 1.0f) || !HawkMath::IsZero(M[8] - 1.0f))
			return false;

		for (int i=0; i<3; ++i)
		{
			for (int j=0; j<3; ++j)
			{
				if ((j != i) && (!HawkMath::IsZero((*this)(i,j))))
					return false;
			}
		}
		return true;
	}

	void HawkMatrix3::SetUpperLeft2x2(const HawkMatrix2& oMat2)
	{
		MakeIdentity();
		M00 = oMat2.M00; M01 = oMat2.M01;
		M10 = oMat2.M10; M11 = oMat2.M11;
	}

	HawkVector2D HawkMatrix3::MultiplyByUpperLeft2x2(const HawkVector2D& oVec2) const
	{
		return HawkVector2D(
			M00 * oVec2.X + M01 * oVec2.Y,
			M10 * oVec2.X + M11 * oVec2.Y
			);
	}

	//获取转置矩阵
	HawkMatrix3 HawkMatrix3::GetTranspose() const
	{
		HawkMatrix3 oMat3;
		oMat3(0,0) = (*this)(0,0);
		oMat3(0,1) = (*this)(1,0);
		oMat3(0,2) = (*this)(2,0);

		oMat3(1,0) = (*this)(0,1);
		oMat3(1,1) = (*this)(1,1);
		oMat3(1,2) = (*this)(2,1);

		oMat3(2,0) = (*this)(0,2);
		oMat3(2,1) = (*this)(1,2);
		oMat3(2,2) = (*this)(2,2);
		return oMat3;
	}

	//转置
	HawkMatrix3& HawkMatrix3::MakeTranspose()
	{
		*this = GetTranspose();
		return *this;
	}

	void  HawkMatrix3::SetZero()
	{
		memset(M,0,9*sizeof(Float));
	}

	//逆矩阵
	HawkMatrix3	HawkMatrix3::GetInverse() const
	{
		HawkMatrix3 oInv;
		Float fDet = 0;

		oInv.M00 = M11 * M22 - M21 * M12;
		oInv.M10 = M20 * M12 - M10 * M22;
		oInv.M20 = M10 * M21 - M20 * M11;

		fDet = M00 * oInv.M00 + M01 * oInv.M10 + M02 * oInv.M20;

		if (HawkMath::IsZero(fDet))
		{
			oInv.SetZero();	
		}
		else
		{
			fDet = 1.0f / fDet;

			oInv.M00 *= fDet;
			oInv.M10 *= fDet;
			oInv.M20 *= fDet;

			oInv.M01 = fDet * (M21 * M02 - M01 * M22);
			oInv.M11 = fDet * (M00 * M22 - M20 * M02);
			oInv.M21 = fDet * (M20 * M01 - M00 * M21);

			oInv.M02 = fDet * (M01 * M12 - M11 * M02);
			oInv.M12 = fDet * (M10 * M02 - M00 * M12);
			oInv.M22 = fDet * (M00 * M11 - M10 * M01);
		}

		return oInv;
	}

	HawkMatrix3& HawkMatrix3::MakeInverse()
	{
		*this = GetInverse();
		return *this;
	}

	//矩阵行列式
	Float HawkMatrix3::GetDeterminant() const
	{
		return M00 * (M11 * M22 - M21 * M12) - 
			M01 * (M10 * M22 - M20 * M12) +
			M02 * (M10 * M21 - M20 * M11);
	}

	HawkMatrix3& HawkMatrix3::SetTranslation(Float fX, Float fY)
	{
		MakeIdentity();
		M20 = fX; M21 = fY;
		return *this;
	}

	HawkMatrix3& HawkMatrix3::SetTranslation(const HawkVector2D& oVec2)
	{
		return SetTranslation(oVec2.X, oVec2.Y);
	}

	//X,Y,Z为缩放轴
	HawkMatrix3& HawkMatrix3::SetScale(Float fX, Float fY, Float fZ)
	{
		MakeIdentity();
		M00 = fX;
		M11 = fY;
		M22 = fZ;
		return *this;
	}

	HawkMatrix3& HawkMatrix3::SetScale(const HawkVector3D& oVec3)
	{
		return SetScale(oVec3.X,oVec3.Y,oVec3.Z);
	}

	HawkMatrix3& HawkMatrix3::SetScale(Float fScale)
	{
		return SetScale(fScale,fScale,fScale);
	}

	HawkMatrix3& HawkMatrix3::SetRotationX(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M00 = 1.0f;
		M11 = M22 = cosAlpha;
		M12 = sinAlpha;
		M21 = -sinAlpha;

		return *this;
	}

	HawkMatrix3& HawkMatrix3::SetRotationY(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M11 = 1.0f;
		M02 = -sinAlpha;
		M00 = M22 = cosAlpha;
		M20 = sinAlpha;

		return *this;
	}

	HawkMatrix3& HawkMatrix3::SetRotationZ(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M22 = 1.0f;
		M01 = sinAlpha;
		M00 = M11 = cosAlpha;
		M10 = -sinAlpha;

		return *this;
	}

	//设置 Yaw Pitch Roll
	HawkMatrix3& HawkMatrix3::SetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll)
	{
		//Refer: http://blogs.msdn.com/mikepelton/archive/2004/10/29/249501.aspx

		MakeIdentity();

		Float sinX = HawkMath::Sin(fYaw);
		Float cosX = HawkMath::Cos(fYaw);
		Float sinY = HawkMath::Sin(fPitch);
		Float cosY = HawkMath::Cos(fPitch);
		Float sinZ = HawkMath::Sin(fRoll);
		Float cosZ = HawkMath::Cos(fRoll);

		M00 = cosZ*cosY + sinZ*sinX*sinY;
		M01 = sinZ*cosX;
		M02 = cosZ*-sinY + sinZ*sinX*cosY;

		M10 = -sinZ*cosY + cosZ*sinX*sinY;
		M11 = cosZ*cosX;
		M12 = sinZ*sinY + cosZ*sinX*cosY;

		M20 = cosX*sinY;
		M21 = -sinX;
		M22 = cosX*cosY;

		return *this;
	}

	//Get Yaw Pitch Roll
	void HawkMatrix3::GetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll)
	{ 
		fPitch = HawkMath::Asin(-M21);

		Float fCos = HawkMath::Cos(fPitch); 

		if(!HawkMath::IsZero(fCos)) 
		{ 
			fRoll = HawkMath::Atan2(M01,M11);
			fYaw  = HawkMath::Atan2(M20,M22); 
		} 
		else 
		{ 
			fRoll = HawkMath::Atan2(M10,M00); 
			fYaw  = 0.0f; 
		} 
	} 

	HawkMatrix3& HawkMatrix3::SetFromAxis(const HawkVector3D& xAxis,const HawkVector3D& yAxis,const HawkVector3D& zAxis)
	{
		MakeIdentity();

		memcpy((*this)[0],&xAxis,sizeof(HawkVector3D));
		memcpy((*this)[1],&yAxis,sizeof(HawkVector3D));
		memcpy((*this)[2],&zAxis,sizeof(HawkVector3D));
		return *this;
	}

	HawkMatrix3& HawkMatrix3::SetRotationAxis(const HawkVector3D& oRefAxis,Float fRadian)
	{
		MakeIdentity();

		Float _nx = oRefAxis.X;
		Float _ny = oRefAxis.Y;
		Float _nz = oRefAxis.Z;

		Float _cos= HawkMath::Cos(fRadian);
		Float _sin= HawkMath::Sin(fRadian);

		(*this)(0,0)	= _nx*_nx*(1-_cos) + _cos;
		(*this)(0,1)	= _nx*_ny*(1-_cos) + _nz*_sin;
		(*this)(0,2)	= _nx*_nz*(1-_cos) - _ny*_sin;

		(*this)(1,0)	= _nx*_ny*(1-_cos) - _nz*_sin;	
		(*this)(1,1)	= _ny*_ny*(1-_cos) + _cos;
		(*this)(1,2)	= _ny*_nz*(1-_cos) + _nx*_sin;			

		(*this)(2,0)	= _nx*_nz*(1-_cos) + _ny*_sin;
		(*this)(2,1)	= _ny*_nz*(1-_cos) - _nx*_sin;
		(*this)(2,2)	= _nz*_nz*(1-_cos) + _cos;

		return *this;
	}

	//为正交矩阵
	Bool HawkMatrix3::IsOrthogonalMatrix() const
	{
		return GetInverse() == GetTranspose();
	}

	Bool HawkMatrix3::IsRotationMatrix() const
	{
		return (IsOrthogonalMatrix() && HawkMath::Abs<Float>(GetDeterminant() - 1) < HawkMath::FLOAT_DIFF);
	}

	//获得行
	HawkVector3D HawkMatrix3::GetRow(Int32 iRow) const
	{
		A_Exception(iRow>=0 && iRow<3);

		HawkVector3D oVec3;
		oVec3.X = (*this)(iRow,0);
		oVec3.Y = (*this)(iRow,1);
		oVec3.Z = (*this)(iRow,2);

		return oVec3;
	}

	//获得列
	HawkVector3D HawkMatrix3::GetColumn(Int32 iCol) const
	{
		A_Exception(iCol>=0 && iCol<3);

		HawkVector3D oVec3;
		oVec3.X = (*this)(0,iCol);
		oVec3.Y = (*this)(1,iCol);
		oVec3.Z = (*this)(2,iCol);

		return oVec3;
	}

	//设置行
	void   HawkMatrix3::SetRow(Int32 iRow,const HawkVector3D& oVec3)
	{
		A_Exception(iRow>=0 && iRow<3);

		(*this)(iRow,0) = oVec3.X;
		(*this)(iRow,1) = oVec3.Y;
		(*this)(iRow,2) = oVec3.Z;
	}

	//设置列
	void   HawkMatrix3::SetColumn(Int32 iCol,const HawkVector3D& oVec3)
	{
		A_Exception(iCol>=0 && iCol<3);

		(*this)(0,iCol) = oVec3.X;
		(*this)(1,iCol) = oVec3.Y;
		(*this)(2,iCol) = oVec3.Z;
	}

	HawkQuaternion  HawkMatrix3::GetQuaternion()
	{
		return HawkQuaternion(*this);
	}
}

