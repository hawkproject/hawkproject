#include "HawkMatrix4.h"
#include "HawkMatrix2.h"
#include "HawkMatrix3.h"
#include "HawkVector2D.h"
#include "HawkVector3D.h"
#include "HawkVector4D.h"
#include "HawkPlane3D.h"
#include "HawkQuaternion.h"

namespace Hawk
{
	//默认构造函数
	HawkMatrix4::HawkMatrix4()
	{
		memset(M,0,16*sizeof(Float));
		M[0] = 1.0f;M[5] = 1.0f;M[10] = 1.0f;M[15] = 1.0f;
	}

	HawkMatrix4::HawkMatrix4(const HawkMatrix3& oMat3)
	{
		memset(M,0,16*sizeof(Float));
		for (Int32 i=0;i<3;i++)
		{
			M[i*4 + 0] = oMat3.M[i*3 + 0];
			M[i*4 + 1] = oMat3.M[i*3 + 1];
			M[i*4 + 2] = oMat3.M[i*3 + 2];
		}
		M[15] = 1.0f;
	}

	//拷贝构造函数
	HawkMatrix4::HawkMatrix4(const HawkMatrix4& oMat)
	{
		memcpy(M,oMat.M,16*sizeof(Float));
	}

	//行列的下标访问操作
	Float& HawkMatrix4::operator()(Int32 iRow, Int32 iCol)
	{ 
		A_Exception(iRow>=0 && iRow<4 && iCol>=0 && iCol<4);

		return M[ iRow * 4 + iCol ];
	}

	const Float& HawkMatrix4::operator()(Int32 iRow, Int32 iCol) const
	{ 
		A_Exception(iRow>=0 && iRow<4 && iCol>=0 && iCol<4);

		return M[iRow * 4 + iCol];
	}

	//矩阵的单下标访问
	Float* HawkMatrix4::operator[](Int32 iIdx)
	{ 
		A_Exception(iIdx>=0 && iIdx<4);

		return _M[iIdx];
	}

	const Float* HawkMatrix4::operator[](Int32 iIdx) const 
	{ 
		A_Exception(iIdx>=0 && iIdx<4);

		return _M[iIdx]; 
	}

	//赋值操作符重载
	HawkMatrix4& HawkMatrix4::operator = (const HawkMatrix4& oMat4)
	{
		if (this == &oMat4)
			return *this;

		memcpy(M, oMat4.M, 16*sizeof(Float));

		return *this;
	}

	//矩阵数据指针
	Float* HawkMatrix4::Ptr() 
	{ 
		return M;
	}

	const Float* HawkMatrix4::Ptr() const
	{
		return M;
	}

	//其他运算操作符重载
	Bool HawkMatrix4::operator==(const HawkMatrix4 &oMat4) const
	{
		if (this == &oMat4)
			return true;

		for (Int32 i = 0; i < 16; ++i)
			if (!HawkMath::FloatEqual(M[i],oMat4.M[i]))
				return false;

		return true;
	}

	//!=操作符重载
	Bool HawkMatrix4::operator!=(const HawkMatrix4 &oMat4) const
	{
		return !(*this == oMat4);
	}

	//+操作符重载
	HawkMatrix4 HawkMatrix4::operator+(const HawkMatrix4& oMat4) const
	{
		HawkMatrix4 oTmp;

		for (Int32 i=0;i<16;i++)
		{
			oTmp.M[i] = M[i] + oMat4.M[i];
		}

		return oTmp;
	}

	//+=操作符重载
	HawkMatrix4& HawkMatrix4::operator+=(const HawkMatrix4& oMat4)
	{
		for (Int32 i=0;i<16;i++)
		{
			M[i] += oMat4.M[i];
		}

		return *this;
	}

	//-操作符重载
	HawkMatrix4 HawkMatrix4::operator-(const HawkMatrix4& oMat4) const
	{
		HawkMatrix4 oTmp;

		for (Int32 i=0;i<16;i++)
		{
			oTmp.M[i] = M[i] - oMat4.M[i];
		}

		return oTmp;
	}

	//-=操作符重载
	HawkMatrix4& HawkMatrix4::operator-=(const HawkMatrix4& oMat4)
	{
		for (Int32 i=0;i<16;i++)
		{
			M[i] -= oMat4.M[i];
		}

		return *this;
	}

	//*操作符重载
	HawkMatrix4 HawkMatrix4::operator * (const HawkMatrix4& oMat4) const
	{
		if ( IsIdentity() )
			return oMat4;
		if ( oMat4.IsIdentity() )
			return *this;

		HawkMatrix4 oTmp;

		for (Int32 iRow = 0; iRow < 4; iRow++)
		{
			for (Int32 iCol = 0; iCol < 4; iCol++)
			{
				oTmp(iRow,iCol) = (*this)(iRow,0)*oMat4(0,iCol) + 
					(*this)(iRow,1)*oMat4(1,iCol) + 
					(*this)(iRow,2)*oMat4(2,iCol) +
					(*this)(iRow,3)*oMat4(3,iCol);
			}
		}

		return oTmp;
	}

	//*=操作符重载
	HawkMatrix4& HawkMatrix4::operator  *= (const HawkMatrix4& oMat4)
	{
		HawkMatrix4 oTmp = *this*oMat4;
		oTmp = oTmp;

		return *this;
	}

	//矩阵右乘向量(适应opengl)
	HawkVector4D HawkMatrix4::operator* (const HawkVector4D& oVec4) const
	{
		HawkVector4D kProd;
		for (Int32 iRow = 0; iRow < 4; iRow++)
		{
			kProd[iRow] =
				(*this)(iRow,0)*oVec4[0] +
				(*this)(iRow,1)*oVec4[1] +
				(*this)(iRow,2)*oVec4[2] +
				(*this)(iRow,3)*oVec4[3];
		}
		return kProd;
	}

	HawkVector3D HawkMatrix4::operator * (const HawkVector3D& oVec3) const
	{
		HawkVector3D oTmp;

		//Float fInvW = 1.0f / ( M30* oVec3.X + M31 * oVec3.Y + M32 * oVec3.Z + M33 );

		oTmp.X = ( M00 * oVec3.X + M01 * oVec3.Y + M02 * oVec3.Z + M03 );
		oTmp.Y = ( M10 * oVec3.X + M11 * oVec3.Y + M12 * oVec3.Z + M13 );
		oTmp.Z = ( M20 * oVec3.X + M21 * oVec3.Y + M22 * oVec3.Z + M23 );

		return oTmp;
	}

	HawkPlane3D  HawkMatrix4::operator * (const HawkPlane3D& oPlane) const
	{
		HawkPlane3D  ret;
		HawkMatrix4  invTrans;
		GetInverse(invTrans);
		invTrans = invTrans.GetTranspose();

		HawkVector4D v4(oPlane.Normal.X,oPlane.Normal.Y,oPlane.Normal.Z,oPlane.Constant);
		v4 = invTrans * v4;
		ret.Normal.X = v4.X; 
		ret.Normal.Y = v4.Y; 
		ret.Normal.Z = v4.Z;
		ret.Constant = v4.W / ret.Normal.Length();

		return ret;
	}

	void HawkMatrix4::SetUpperLeft2x2(const HawkMatrix2& oMat2)
	{
		MakeIdentity();

		M00 = oMat2.M00; M01 = oMat2.M01;
		M10 = oMat2.M10; M11 = oMat2.M11;
	}

	void HawkMatrix4::SetUpperLeft3x3(const HawkMatrix3& oMat3)
	{
		MakeIdentity();

		M00 = oMat3.M00; M01 = oMat3.M01; M02 = oMat3.M02;
		M10 = oMat3.M10; M11 = oMat3.M11; M12 = oMat3.M12;
		M20 = oMat3.M20; M21 = oMat3.M21; M22 = oMat3.M22;
	}

	HawkVector3D HawkMatrix4::MultiplyByUpperLeft3x3(const HawkVector3D& oVec3) const
	{
		return HawkVector3D(
			M00 * oVec3.X + M01 * oVec3.Y + M02 * oVec3.Z,
			M10 * oVec3.X + M11 * oVec3.Y + M12 * oVec3.Z,
			M20 * oVec3.X + M21 * oVec3.Y + M22 * oVec3.Z
			);
	}

	//判断为仿射矩阵
	Bool HawkMatrix4::IsAffine(void) const
	{
		return M[12] == 0 && M[13] == 0 && M[14] == 0 && M[15] == 1;
	}

	//单位化
	HawkMatrix4& HawkMatrix4::MakeIdentity()
	{
		SetZero();
		M[0] = M[5] = M[10] = M[15] = 1.0f;
		return *this;
	}

	//判断是否为单位矩阵
	Bool HawkMatrix4::IsIdentity() const
	{
		if (!HawkMath::IsZero(M[0] - 1.0f) || !HawkMath::IsZero(M[5] - 1.0f) || !HawkMath::IsZero(M[10] - 1.0f) || !HawkMath::IsZero(M[15] - 1.0f))
			return false;

		for (int i=0; i<4; ++i)
		{
			for (int j=0; j<4; ++j)
			{
				if ((j != i) && (!HawkMath::IsZero((*this)(i,j))))
					return false;
			}
		}
		return true;
	}

	//矩阵行列式
	Float HawkMatrix4::GetDeterminant() const
	{
		Float det1 = M12 * M23 - M22 * M13;
		Float det2 = M11 * M23 - M21 * M13;
		Float det3 = M11 * M22 - M21 * M12;
		Float det4 = M10 * M23 - M20 * M13;
		Float det5 = M10 * M22 - M20 * M12;
		Float det6 = M10 * M21 - M20 * M11;

		return -M30 * (M01 * det1 - M02 * det2 + M03 * det3) +
				M31 * (M00 * det1 - M02 * det4 + M03 * det5) -
				M32 * (M00 * det2 - M01 * det4 + M03 * det6) +
				M33 * (M00 * det3 - M01 * det5 + M02 * det6);
	}

	//设置矩阵的平移量
	HawkMatrix4& HawkMatrix4::SetTranslation( const HawkVector3D& oVec3 )
	{
		MakeIdentity();
		
		M[12] = oVec3.X;
		M[13] = oVec3.Y;
		M[14] = oVec3.Z;
		M[15] = 1.0f;

		return *this;
	}

	//获取平移量
	HawkVector3D HawkMatrix4::GetTranslation() const
	{
		return HawkVector3D(M[12],M[13],M[14]);
	}

	//设置缩放量 X,Y,Z为缩放轴
	HawkMatrix4& HawkMatrix4::SetScale( const HawkVector3D& oScale )
	{
		MakeIdentity();

		M[0]  = oScale.X;
		M[5]  = oScale.Y;
		M[10] = oScale.Z;
		M[15] = 1.0f;

		return *this;
	}

	//设置缩放量
	HawkMatrix4& HawkMatrix4::SetScale( Float fScale ) 
	{ 
		return SetScale(HawkVector3D(fScale,fScale,fScale)); 
	}

	//获取缩放量
	HawkVector3D HawkMatrix4::GetScale() const
	{
		if(HawkMath::IsZero(M[1]) && 
			HawkMath::IsZero(M[2]) &&
			HawkMath::IsZero(M[4]) && 
			HawkMath::IsZero(M[6]) &&
			HawkMath::IsZero(M[8]) &&
			HawkMath::IsZero(M[9]))
			return HawkVector3D(M[0], M[5], M[10]);

		return HawkVector3D(HawkMath::Sqrt(M[0] * M[0] + M[1] * M[1] + M[2] * M[2]),
			HawkMath::Sqrt(M[4] * M[4] + M[5] * M[5] + M[6] * M[6]),
			HawkMath::Sqrt(M[8] * M[8] + M[9] * M[9] + M[10] * M[10]));
	}

	//平移一个向量
	void HawkMatrix4::TranslateVect( HawkVector3D& oVec3 ) const
	{
		oVec3.X += M[12];
		oVec3.Y += M[13];
		oVec3.Z += M[14];
	}

	//逆向平移向量
	void HawkMatrix4::InverseTranslateVect( HawkVector3D& oVec3 ) const
	{
		oVec3.X -= M[12];
		oVec3.Y -= M[13];
		oVec3.Z -= M[14];
	}

	//逆向旋转三维向量
	void HawkMatrix4::InverseRotateVect( HawkVector3D& oVec3 ) const
	{
		HawkVector3D oTmp = oVec3;
		oVec3.X = oTmp.X*M[0] + oTmp.Y*M[1] + oTmp.Z*M[2];
		oVec3.Y = oTmp.X*M[4] + oTmp.Y*M[5] + oTmp.Z*M[6];
		oVec3.Z = oTmp.X*M[8] + oTmp.Y*M[9] + oTmp.Z*M[10];
	}

	//旋转三维向量
	void HawkMatrix4::RotateVect( HawkVector3D& oVec3 ) const
	{
		HawkVector3D oTmp = oVec3;
		oVec3.X = oTmp.X*M[0] + oTmp.Y*M[4] + oTmp.Z*M[8];
		oVec3.Y = oTmp.X*M[1] + oTmp.Y*M[5] + oTmp.Z*M[9];
		oVec3.Z = oTmp.X*M[2] + oTmp.Y*M[6] + oTmp.Z*M[10];
	}

	//变化一个三维向量
	void HawkMatrix4::TransformVect( HawkVector3D& oVec3) const
	{
		Float oTmp[3];

		oTmp[0] = oVec3.X*M[0] + oVec3.Y*M[4] + oVec3.Z*M[8] + M[12];
		oTmp[1] = oVec3.X*M[1] + oVec3.Y*M[5] + oVec3.Z*M[9] + M[13];
		oTmp[2] = oVec3.X*M[2] + oVec3.Y*M[6] + oVec3.Z*M[10] + M[14];

		oVec3.X = oTmp[0];
		oVec3.Y = oTmp[1];
		oVec3.Z = oTmp[2];
	}

	//计算逆矩阵
	Bool HawkMatrix4::MakeInverse()
	{
		HawkMatrix4 oTmp;

		if (GetInverse(oTmp))
		{
			*this = oTmp;
			return true;
		}

		return false;
	}

	void HawkMatrix4::SetZero()
	{
		memset(M,0,16*sizeof(16));
	}

	HawkMatrix4 HawkMatrix4::GetInverse() const
	{
		HawkMatrix4 mat4;
		mat4.MakeIdentity();
		GetInverse(mat4);
		return mat4;
	}

	//获取逆矩阵
	Bool HawkMatrix4::GetInverse(HawkMatrix4& oMat4) const
	{
		if (IsIdentity())
		{
			oMat4 = *this;
			return true;
		}

		Float det = 0.0f;

		//计算2*2子矩阵的模
		Float det01 = M12 * M23 - M13 * M22;	
		Float det02 = M11 * M23 - M13 * M21;
		Float det03 = M11 * M22 - M12 * M21;
		Float det04 = M10 * M23 - M13 * M20;
		Float det05 = M10 * M22 - M12 * M20;
		Float det06 = M10 * M21 - M11 * M20;

		det =  -M30 * (M01 * det01 - M02 * det02 + M03 * det03) +
			M31 * (M00 * det01 - M02 * det04 + M03 * det05) -
			M32 * (M00 * det02 - M01 * det04 + M03 * det06) +
			M33 * (M00 * det03 - M01 * det05 + M02 * det06);

		if (HawkMath::IsZero(det))
		{
			oMat4.SetZero();
			return false;
		}
		else
		{
			Float det07 = M22 * M33 - M23 * M32;
			Float det08 = M23 * M31 - M21 * M33;
			Float det09 = M21 * M32 - M22 * M31;
			Float det10 = M02 * M33 - M03 * M32;
			Float det11 = M03 * M31 - M01 * M33;
			Float det12 = M01 * M32 - M02 * M31;
			Float det13 = M02 * M13 - M03 * M12;
			Float det14 = M03 * M11 - M01 * M13;
			Float det15 = M01 * M12 - M02 * M11;
			Float det16 = M20 * M33 - M23 * M30;
			Float det17 = M22 * M30 - M20 * M32;
			Float det18 = M00 * M33 - M03 * M30;
			Float det19 = M02 * M30 - M00 * M32;
			Float det20 = M00 * M13 - M03 * M10;
			Float det21 = M02 * M10 - M00 * M12;
			Float det22 = M20 * M31 - M21 * M30;
			Float det23 = M00 * M31 - M01 * M30;
			Float det24 = M00 * M11 - M01 * M10;

			det = 1.0f / det;

			oMat4.M00 = det * (M11 * det07 + M12 * det08 + M13 * det09);
			oMat4.M01 = det * (M21 * det10 + M22 * det11 + M23 * det12);
			oMat4.M02 = det * (M31 * det13 + M32 * det14 + M33 * det15);
			oMat4.M03 = det * (M01 * -det01 + M02 * det02 + M03 * -det03);

			oMat4.M10 = det * (M12 * det16 + M13 * det17 - M10 * det07);
			oMat4.M11 = det * (M22 * det18 + M23 * det19 - M20 * det10);
			oMat4.M12 = det * (M32 * det20 + M33 * det21 - M30 * det13);
			oMat4.M13 = det * (M02 * -det04 + M03 * det05 + M00 * det01);

			oMat4.M20 = det * (M13 * det22 - M10 * det08 - M11 * det16);
			oMat4.M21 = det * (M23 * det23 - M20 * det11 - M21 * det18);
			oMat4.M22 = det * (M33 * det24 - M30 * det14 - M31 * det20);
			oMat4.M23 = det * (M03 * -det06 + M00 * -det02 + M01 * det04);

			oMat4.M30 = det * (M10 * -det09 - M11 * det17 - M12 * det22);
			oMat4.M31 = det * (M20 * -det12 - M21 * det19 - M22 * det23);
			oMat4.M32 = det * (M30 * -det15 - M31 * det21 - M32 * det24);
			oMat4.M33 = det * (M00 * det03 + M01 * -det05 + M02 * det06);
		}

		return true;
	}

	//获取转置矩阵
	HawkMatrix4 HawkMatrix4::GetTranspose() const
	{
		HawkMatrix4 oMat4;
		oMat4.M[ 0] = M[ 0];
		oMat4.M[ 1] = M[ 4];
		oMat4.M[ 2] = M[ 8];
		oMat4.M[ 3] = M[12];

		oMat4.M[ 4] = M[ 1];
		oMat4.M[ 5] = M[ 5];
		oMat4.M[ 6] = M[ 9];
		oMat4.M[ 7] = M[13];

		oMat4.M[ 8] = M[ 2];
		oMat4.M[ 9] = M[ 6];
		oMat4.M[10] = M[10];
		oMat4.M[11] = M[14];

		oMat4.M[12] = M[ 3];
		oMat4.M[13] = M[ 7];
		oMat4.M[14] = M[11];
		oMat4.M[15] = M[15];
		return oMat4;
	}

	//旋转参数的计算
	HawkMatrix4& HawkMatrix4::SetRotationX(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M00 = M33 = 1.0f;
		M11 = M22 = cosAlpha;
		M12 = sinAlpha;
		M21 = -sinAlpha;

		return *this;
	}

	HawkMatrix4& HawkMatrix4::SetRotationY(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M11 = M33 = 1.0f;
		M02 = -sinAlpha;
		M00 = M22 = cosAlpha;
		M20 = sinAlpha;

		return *this;
	}

	HawkMatrix4& HawkMatrix4::SetRotationZ(Float fRadian)
	{
		MakeIdentity();

		Float sinAlpha = HawkMath::Sin(fRadian);
		Float cosAlpha = HawkMath::Cos(fRadian);

		M22 = M33 = 1.0f;
		M01 = sinAlpha;
		M00 = M11 = cosAlpha;
		M10 = -sinAlpha;

		return *this;
	}

	//设置 Yaw Pitch Roll
	HawkMatrix4& HawkMatrix4::SetYawPitchRoll(Float fYaw,Float fPitch,Float fRoll)
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

		M33 = 1.0f;

		return *this;
	}

	void HawkMatrix4::GetYawPitchRoll(Float& fYaw,Float& fPitch,Float& fRoll)
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

	HawkVector3D  HawkMatrix4::GetRow(Int32 iRow) const
	{
		A_Exception(iRow>=0 && iRow<4);
		return HawkVector3D(_M[iRow][0],_M[iRow][1],_M[iRow][2]);
	}

	HawkVector3D  HawkMatrix4::GetColumn(Int32 iCol) const
	{
		A_Exception(iCol>=0 && iCol<4);
		return HawkVector3D(_M[0][iCol],_M[1][iCol],_M[2][iCol]);
	}

	void  HawkMatrix4::SetRow(Int32 iRow,const HawkVector3D& oVec)
	{
		A_Exception(iRow>=0 && iRow<4);
		_M[iRow][0] = oVec.X;
		_M[iRow][1] = oVec.Y;
		_M[iRow][2] = oVec.Z;
	}

	void  HawkMatrix4::SetColumn(Int32 iCol,const HawkVector3D& oVec)
	{
		A_Exception(iCol>=0 && iCol<4);
		_M[0][iCol] = oVec.X;
		_M[1][iCol] = oVec.Y;
		_M[2][iCol] = oVec.Z;
	}

	HawkMatrix4& HawkMatrix4::SetRotationAxis(const HawkVector3D& oRefAxis,Float fRadian)
	{
		MakeIdentity();

		Float _nx = oRefAxis.X;
		Float _ny = oRefAxis.Y;
		Float _nz = oRefAxis.Z;

		Float _cos= HawkMath::Cos(fRadian);
		Float _sin= HawkMath::Sin(fRadian);

		(*this)(0,0)	= _nx*_nx*(1-_cos) + _cos;
		(*this)(0, 1)	=_nx*_ny*(1-_cos) + _nz*_sin;
		(*this)(0, 2)	= _nx*_nz*(1-_cos) - _ny*_sin;
		(*this)(0, 3)	= 0.0f;

		(*this)(1,0)	= _nx*_ny*(1-_cos) - _nz*_sin;	
		(*this)(1, 1)	= _ny*_ny*(1-_cos) + _cos;
		(*this)(1, 2)	= _ny*_nz*(1-_cos) + _nx*_sin;			
		(*this)(1, 3)	= 0.0f;

		(*this)(2,0)	= _nx*_nz*(1-_cos) + _ny*_sin;
		(*this)(2, 1)	= _ny*_nz*(1-_cos) - _nx*_sin;
		(*this)(2, 2)	= _nz*_nz*(1-_cos) + _cos;
		(*this)(2, 3)	= 0.0f;

		(*this)(3,0)	= 0.0f;	
		(*this)(3, 1)	= 0.0f;	
		(*this)(3, 2)	= 0.0f;	
		(*this)(3, 3)	= 1.0f;

		return *this;
	}

	HawkMatrix4& HawkMatrix4::SetRotation(const HawkQuaternion& oQuat)
	{
		HawkQuaternion oTmp = oQuat;
		oTmp.MakeIdentity();
		*this = oTmp.GetAsMatrix4();
		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixTransform(const HawkVector3D& sPos, const HawkVector3D& sDir, const HawkVector3D& sUp)
	{
		HawkVector3D zaxis = HawkVector3D(sDir).Normalize();
		HawkVector3D xaxis = sUp.CrossProduct(zaxis).Normalize();
		HawkVector3D yaxis = zaxis.CrossProduct(xaxis);

		(*this)(0,0) = xaxis.X; (*this)(0, 1) = xaxis.Y; (*this)(0, 2) = xaxis.Z; (*this)(0, 3) = 0.0f;
		(*this)(1,0) = yaxis.X; (*this)(1, 1) = yaxis.Y; (*this)(1, 2) = yaxis.Z; (*this)(1, 3) = 0.0f;
		(*this)(2,0) = zaxis.X; (*this)(2, 1) = zaxis.Y; (*this)(2, 2) = zaxis.Z; (*this)(2, 3) = 0.0f;
		(*this)(3,0) = sPos.X;	(*this)(3, 1) = sPos.Y;	 (*this)(3, 2) = sPos.Z;  (*this)(3, 3) = 1.0f;

		return *this;
	}		

	/*视矩阵(左手)
	zaxis = normal(At - Eye)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) l
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixLookAtLH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp)
	{
		HawkVector3D zaxis = (oAt - oEye).Normalize();
		HawkVector3D xaxis = oUp.CrossProduct(zaxis).Normalize();
		HawkVector3D yaxis = zaxis.CrossProduct(xaxis);

		Float x = -xaxis.DotProduct(oEye);
		Float y = -yaxis.DotProduct(oEye);
		Float z = -zaxis.DotProduct(oEye);

		(*this)(0,0) = xaxis.X; (*this)(0, 1) = yaxis.X; (*this)(0, 2) = zaxis.X; (*this)(0, 3) = 0.0f;
		(*this)(1,0) = xaxis.Y; (*this)(1, 1) = yaxis.Y; (*this)(1, 2) = zaxis.Y; (*this)(1, 3) = 0.0f;
		(*this)(2,0) = xaxis.Z; (*this)(2, 1) = yaxis.Z; (*this)(2, 2) = zaxis.Z; (*this)(2, 3) = 0.0f;
		(*this)(3,0) = x;		(*this)(3, 1) = y;	     (*this)(3, 2) = z;		  (*this)(3, 3) = 1.0f;

		return *this;
	}
	
	/*视矩阵(右手)
	zaxis = normal(Eye - At)
	xaxis = normal(cross(Up, zaxis))
	yaxis = cross(zaxis, xaxis)

	xaxis.x           yaxis.x           zaxis.x          0
	xaxis.y           yaxis.y           zaxis.y          0
	xaxis.z           yaxis.z           zaxis.z          0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye) l
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixLookAtRH(const HawkVector3D& oEye,const HawkVector3D& oAt,const HawkVector3D& oUp)
	{
		HawkVector3D zaxis = (oEye - oAt).Normalize();
		HawkVector3D xaxis = oUp.CrossProduct(zaxis).Normalize();
		HawkVector3D yaxis = zaxis.CrossProduct(xaxis);

		Float x = -xaxis.DotProduct(oEye);
		Float y = -yaxis.DotProduct(oEye);
		Float z = -zaxis.DotProduct(oEye);

		(*this)(0,0) = xaxis.X; (*this)(0, 1) = yaxis.X; (*this)(0, 2) = zaxis.X; (*this)(0, 3) = 0.0f;
		(*this)(1,0) = xaxis.Y; (*this)(1, 1) = yaxis.Y; (*this)(1, 2) = zaxis.Y; (*this)(1, 3) = 0.0f;
		(*this)(2,0) = xaxis.Z; (*this)(2, 1) = yaxis.Z; (*this)(2, 2) = zaxis.Z; (*this)(2, 3) = 0.0f;
		(*this)(3,0) = x;		(*this)(3, 1) = y;	     (*this)(3, 2) = z;		  (*this)(3, 3) = 1.0f;

		return *this;
	}

	/*
	2/w  0    0           0
	0    2/h  0           0
	0    0    1/(zf-zn)   0
	0    0    zn/(zn-zf)  1
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixOrthoLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f/fWidth; (*this)(0, 1) = 0;            (*this)(0, 2) = 0;					(*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0;           (*this)(1, 1) = 2.0f/fHeight; (*this)(1, 2) = 0;					(*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0;           (*this)(2, 1) = 0;			  (*this)(2, 2) = 1.0f/(fZFar-fZNear);  (*this)(2, 3) = 0.0f;
		(*this)(3,0) = 0;		    (*this)(3, 1) = 0;	          (*this)(3, 2) = fZNear/(fZNear-fZFar);(*this)(3, 3) = 1.0f;
		
		return *this;
	}

	/*
	2/w  0    0           0
	0    2/h  0           0
	0    0    1/(zn-zf)   0
	0    0    zn/(zn-zf)  1
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixOrthoRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f/fWidth; (*this)(0, 1) = 0;            (*this)(0, 2) = 0;					(*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0;           (*this)(1, 1) = 2.0f/fHeight; (*this)(1, 2) = 0;					(*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0;           (*this)(2, 1) = 0;			  (*this)(2, 2) = 1.0f/(fZNear-fZFar);  (*this)(2, 3) = 0.0f;
		(*this)(3,0) = 0;		    (*this)(3, 1) = 0;	          (*this)(3, 2) = fZNear/(fZNear-fZFar);(*this)(3, 3) = 1.0f;

		return *this;
	}

	/*
	2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zf-zn)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixOrthoOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f/(fRight-fLeft);			 (*this)(0, 1) = 0;							   (*this)(0, 2) = 0;					 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0;							 (*this)(1, 1) = 2.0f/(fTop-fBottom);		   (*this)(1, 2) = 0;					 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0;							 (*this)(2, 1) = 0;							   (*this)(2, 2) = 1.0f/(fZFar-fZNear);  (*this)(2, 3) = 0.0f;
		(*this)(3,0) = (fLeft+fRight)/(fLeft-fRight);(*this)(3, 1) = (fTop+fBottom)/(fBottom-fTop);(*this)(3, 2) = fZNear/(fZNear-fZFar);(*this)(3, 3) = 1.0f;

		return *this;
	}

	/*
	2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zn-zf)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixOrthoOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f/(fRight-fLeft);			 (*this)(0, 1) = 0;							   (*this)(0, 2) = 0;					 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0;							 (*this)(1, 1) = 2.0f/(fTop-fBottom);		   (*this)(1, 2) = 0;					 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0;							 (*this)(2, 1) = 0;							   (*this)(2, 2) = 1.0f/(fZNear-fZFar);  (*this)(2, 3) = 0.0f;
		(*this)(3,0) = (fLeft+fRight)/(fLeft-fRight);(*this)(3, 1) = (fTop+fBottom)/(fBottom-fTop);(*this)(3, 2) = fZNear/(fZNear-fZFar);(*this)(3, 3) = 1.0f;

		return *this;
	}

	/*	
	xScale     0          0               0
	0        yScale       0               0
	0          0       zf/(zf-zn)         1
	0          0       -zn*zf/(zf-zn)     0
	where:
	yScale = cot(fovY/2)
	xScale = yScale / aspect ratio
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveFovLH(Float fFovy,Float fAspect,Float fZNear,Float fZFar)
	{
		Float xScale = HawkMath::Cos(fFovy*0.5f) / HawkMath::Sin(fFovy*0.5f);
		Float yScale = xScale / fAspect;

		(*this)(0,0) = xScale;	(*this)(0, 1) = 0.0f;	(*this)(0, 2) = 0.0f;						  (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;	(*this)(1, 1) = yScale;	(*this)(1, 2) = 0.0f;						  (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0.0f;	(*this)(2, 1) = 0.0f;	(*this)(2, 2) = fZFar/(fZFar-fZNear);		  (*this)(2, 3) = 1.0f;
		(*this)(3,0) = 0.0f;	(*this)(3, 1) = 0.0f;	(*this)(3, 2) = -fZNear*fZFar/(fZFar-fZNear); (*this)(3, 3) = 0.0f;

		return *this;
	}

	/*	
	xScale     0          0              0
	0        yScale       0              0
	0        0        zf/(zn-zf)        -1
	0        0        zn*zf/(zn-zf)      0
	where:
	yScale = cot(fovY/2)
	xScale = yScale / aspect ratio
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveFovRH(Float fFovy,Float fAspect,Float fZNear,Float fZFar)
	{
		Float xScale = HawkMath::Cos(fFovy*0.5f) / HawkMath::Sin(fFovy*0.5f);
		Float yScale = xScale / fAspect;

		(*this)(0,0) = xScale;	(*this)(0, 1) = 0.0f;	(*this)(0, 2) = 0.0f;						  (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;	(*this)(1, 1) = yScale;	(*this)(1, 2) = 0.0f;						  (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0.0f;	(*this)(2, 1) = 0.0f;	(*this)(2, 2) = fZFar/(fZNear-fZFar);		  (*this)(2, 3) = 1.0f;
		(*this)(3,0) = 0.0f;	(*this)(3, 1) = 0.0f;	(*this)(3, 2) = fZNear*fZFar/(fZNear-fZFar); (*this)(3, 3) = 0.0f;

		return *this;
	}

	/*
	2*zn/w  0       0              0
	0       2*zn/h  0              0
	0       0       zf/(zf-zn)     1
	0       0       zn*zf/(zn-zf)  0
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveLH(Float fWidth,Float fHeight,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f*fZNear/fWidth; (*this)(0, 1) = 0.0f;				(*this)(0, 2) = 0.0f;						 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;			   (*this)(1, 1) = 2.0f*fZNear/fHeight;	(*this)(1, 2) = 0.0f;						 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0.0f;			   (*this)(2, 1) = 0.0f;				(*this)(2, 2) = fZFar/(fZFar-fZNear);		 (*this)(2, 3) = 1.0f;
		(*this)(3,0) = 0.0f;		       (*this)(3, 1) = 0.0f;				(*this)(3, 2) = -fZNear*fZFar/(fZNear-fZFar);(*this)(3, 3) = 0.0f;

		return *this;
	}

	/*
	2*zn/w  0       0              0
	0       2*zn/h  0              0
	0       0       zf/(zn-zf)    -1
	0       0       zn*zf/(zn-zf)  0
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveRH(Float fWidth,Float fHeight,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f*fZNear/fWidth; (*this)(0, 1) = 0.0f;				(*this)(0, 2) = 0.0f;						 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;			   (*this)(1, 1) = 2.0f*fZNear/fHeight;	(*this)(1, 2) = 0.0f;						 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = 0.0f;			   (*this)(2, 1) = 0.0f;				(*this)(2, 2) = fZFar/(fZNear-fZFar);		 (*this)(2, 3) = -1.0f;
		(*this)(3,0) = 0.0f;		       (*this)(3, 1) = 0.0f;				(*this)(3, 2) = fZNear*fZFar/(fZNear-fZFar); (*this)(3, 3) = 0.0f;

		return *this;
	}

	/*
	2*zn/(r-l)   0            0              0
	0            2*zn/(t-b)   0              0
	(l+r)/(l-r)  (t+b)/(b-t)  zf/(zf-zn)     1
	0            0            zn*zf/(zn-zf)  0
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveOffCenterLH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f*fZNear/(fRight-fLeft);	 (*this)(0, 1) = 0.0f;							(*this)(0, 2) = 0.0f;						 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;						 (*this)(1, 1) = 2.0f*fZNear/(fTop-fBottom);	(*this)(1, 2) = 0.0f;						 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = (fLeft+fRight)/(fLeft-fRight);(*this)(2, 1) = (fTop+fBottom)/(fBottom-fTop);	(*this)(2, 2) = fZFar/(fZFar-fZNear);		 (*this)(2, 3) = 1.0f;
		(*this)(3,0) = 0.0f;						 (*this)(3, 1) = 0.0f;							(*this)(3, 2) = fZNear*fZFar/(fZNear-fZFar); (*this)(3, 3) = 0.0f;

		return *this;
	}

	/*
	2*zn/(r-l)   0            0                0
	0            2*zn/(t-b)   0                0
	(l+r)/(r-l)  (t+b)/(t-b)  zf/(zn-zf)      -1
	0            0            zn*zf/(zn-zf)    0
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixPerspectiveOffCenterRH(Float fLeft,Float fRight,Float fBottom,Float fTop,Float fZNear,Float fZFar)
	{
		(*this)(0,0) = 2.0f*fZNear/(fRight-fLeft);	 (*this)(0, 1) = 0.0f;							(*this)(0, 2) = 0.0f;						 (*this)(0, 3) = 0.0f;
		(*this)(1,0) = 0.0f;						 (*this)(1, 1) = 2.0f*fZNear/(fTop-fBottom);	(*this)(1, 2) = 0.0f;						 (*this)(1, 3) = 0.0f;
		(*this)(2,0) = (fLeft+fRight)/(fRight-fLeft);(*this)(2, 1) = (fTop+fBottom)/(fTop-fBottom);	(*this)(2, 2) = fZFar/(fZNear-fZFar);		 (*this)(2, 3) = -1.0f;
		(*this)(3,0) = 0.0f;						 (*this)(3, 1) = 0.0f;							(*this)(3, 2) = fZNear*fZFar/(fZNear-fZFar); (*this)(3, 3) = 0.0f;

		return *this;
	}

	/*
	P = normalize(Plane);
	-2 * P.a * P.a + 1  -2 * P.b * P.a      -2 * P.c * P.a        0
	-2 * P.a * P.b      -2 * P.b * P.b + 1  -2 * P.c * P.b        0
	-2 * P.a * P.c      -2 * P.b * P.c      -2 * P.c * P.c + 1    0
	-2 * P.a * P.d      -2 * P.b * P.d      -2 * P.c * P.d        l
	*/
	HawkMatrix4& HawkMatrix4::MakeMatrixReflect(const HawkPlane3D& oPlane)
	{
		HawkPlane3D P = oPlane;
		P.Normalize();

		(*this)(0,0) = -2.0f*P.Normal.X*P.Normal.X+1.0f;(*this)(0, 1) = -2.0f*P.Normal.Y*P.Normal.X;	 (*this)(0, 2) = -2.0f*P.Normal.Z*P.Normal.X;	  (*this)(0, 3) = 0.0f;
		(*this)(1,0) = -2.0f*P.Normal.X*P.Normal.Y;		(*this)(1, 1) = -2.0f*P.Normal.Y*P.Normal.Y+1.0f;(*this)(1, 2) = -2.0f*P.Normal.Z*P.Normal.Y;     (*this)(1, 3) = 0.0f;
		(*this)(2,0) = -2.0f*P.Normal.X*P.Normal.Z;		(*this)(2, 1) = -2.0f*P.Normal.Y*P.Normal.Z;	 (*this)(2, 2) = -2.0f*P.Normal.Z*P.Normal.Z+1.0f;(*this)(2, 3) = 0.0f;
		(*this)(3,0) = -2.0f*P.Normal.X*P.Constant;		(*this)(3, 1) = -2.0f*P.Normal.Y*P.Constant;     (*this)(3, 2) = -2.0f*P.Normal.Z*P.Constant;	  (*this)(3, 3) = 1.0f;

		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixRotationX(Float fRadian)
	{
		SetRotationX(fRadian);
		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixRotationY(Float fRadian)
	{
		SetRotationY(fRadian);
		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixRotationZ(Float fRadian)
	{
		SetRotationZ(fRadian);
		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixRotationAxis(const HawkVector3D& oAxis,Float fRadian)
	{
		SetRotationAxis(oAxis,fRadian);
		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixRotationYawPitchRoll(Float fYaw,Float fPitch,Float fRoll)
	{
		HawkMatrix4 x_mat;
		x_mat.SetRotationX(fPitch);

		HawkMatrix4 y_mat;
		x_mat.SetRotationY(fYaw);

		HawkMatrix4 z_mat;
		x_mat.SetRotationZ(fRoll);

		HawkMatrix4 mat = z_mat*x_mat;
		mat *= y_mat;

		memcpy(M,mat.M,16*sizeof(Float));

		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixScaling(Float fX,Float fY,Float fZ)
	{
		SetScale(HawkVector3D(fX,fY,fZ));
		return *this;
	}

	/*
	P = normalize(Plane);
	L = Light;
	d = dot(P, L)
	P.a * L.x + d  P.a * L.y      P.a * L.z      P.a * L.w  
	P.b * L.x      P.b * L.y + d  P.b * L.z      P.b * L.w  
	P.c * L.x      P.c * L.y      P.c * L.z + d  P.c * L.w  
	P.d * L.x      P.d * L.y      P.d * L.z      P.d * L.w + d
	*/
	//影子本质上是把物体按照灯光照射方向平行地投射到平面n*p+d=0之上。
	//我们能够使用一个矩阵来表示从一个顶点p变换到平面n*p=d=0上的s的变化。而且，我们能够用同一个矩阵来表现正交投影和透视投影。
	//我们用一个4D向量（nx, ny, nz, d）来表示将要用于投射阴影平面的平面等式中的各个系数。让4D向量L=（Lx, Ly, Lz, Lw）来表示平行光的照射方向或点光源的位置。我们用w来区别：
	//1．假如w＝0， 那么L表示平行光的照射方向。
	//2．假如w＝1 ，那么L表示点光源的位置。
	HawkMatrix4& HawkMatrix4::MakeMatrixShadow(const HawkVector4D& oLightVec,const HawkPlane3D& oPlane)
	{
		HawkPlane3D  P = oPlane;
		P.Normalize();
		HawkVector4D L = oLightVec;
		Float d = P.DotProduct(oLightVec);

		(*this)(0,0) = -P.Normal.X*L.X+d; (*this)(0, 1) = P.Normal.X*L.Y;  (*this)(0, 2) = P.Normal.X * L.Z;  (*this)(0, 3) = P.Normal.X*L.W;
		(*this)(1,0) = P.Normal.Y *L.X;	  (*this)(1, 1) = P.Normal.Y*L.Y+d;(*this)(1, 2) = P.Normal.Y * L.Z;  (*this)(1, 3) = P.Normal.Y*L.W;
		(*this)(2,0) = P.Normal.Z *L.X;	  (*this)(2, 1) = P.Normal.Z*L.Y;  (*this)(2, 2) = P.Normal.Z * L.Z+d;(*this)(2, 3) = P.Normal.Z*L.W;
		(*this)(3,0) = P.Constant *L.X;	  (*this)(3, 1) = P.Constant*L.Y;  (*this)(3, 2) = P.Constant * L.Z;  (*this)(3, 3) = P.Constant*L.W+d;

		return *this;
	}

	HawkMatrix4& HawkMatrix4::MakeMatrixTranslation(Float fX,Float fY,Float fZ)
	{
		SetTranslation(HawkVector3D(fX,fY,fZ));
		return *this;
	}
}

