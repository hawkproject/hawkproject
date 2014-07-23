#include "HawkQuaternion.h"
#include "HawkVector3D.h"
#include "HawkMatrix3.h"
#include "HawkMatrix4.h"

namespace Hawk
{
	HawkQuaternion::HawkQuaternion (Float fW,Float fX, Float fY, Float fZ)
	{
		W = fW;
		X = fX;
		Y = fY;
		Z = fZ;
	}

	//选择矩阵构造
	HawkQuaternion::HawkQuaternion(const HawkMatrix3& oRotMat)
	{
		SetFromMatrix3(oRotMat);
	}

	HawkQuaternion::HawkQuaternion(const HawkMatrix4& oRotMat)
	{
		SetFromMatrix4(oRotMat);
	}

	//轴角对构造
	HawkQuaternion::HawkQuaternion(const HawkVector3D& vAxis,Float fRadian)
	{
		SetFromAxisAngle(vAxis,fRadian);
	}

	HawkQuaternion::HawkQuaternion(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis)
	{
		SetFromAxis(xAxis,yAxis,zAxis);
	}

	Float* HawkQuaternion::Ptr()
	{
		return &W;
	}

	const Float* HawkQuaternion::Ptr() const
	{
		return &W;
	}

	HawkQuaternion& HawkQuaternion::SetFromAxis(const HawkVector3D& xAxis, const HawkVector3D& yAxis, const HawkVector3D& zAxis)
	{	
		HawkMatrix3 mRot;
		mRot.SetFromAxis(xAxis,yAxis,zAxis);
		return SetFromMatrix3(mRot);
	}

	void HawkQuaternion::GetAsAxis(HawkVector3D& xAxis, HawkVector3D& yAxis, HawkVector3D& zAxis)
	{
		HawkMatrix3 mRot = GetAsMatrix3();
		memcpy(&xAxis,mRot[0],sizeof(HawkVector3D));
		memcpy(&yAxis,mRot[1],sizeof(HawkVector3D));
		memcpy(&zAxis,mRot[2],sizeof(HawkVector3D));
	}

	HawkQuaternion& HawkQuaternion::SetFromMatrix3(const HawkMatrix3& oRotMat)
	{
		HawkAssert(oRotMat.IsRotationMatrix());

		Float fTrace = oRotMat[0][0]+oRotMat[1][1]+oRotMat[2][2];
		Float fRoot  = 0;

		if ( fTrace > 0.0f )
		{
			fRoot = HawkMath::Sqrt(fTrace + 1.0f);
			W     = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			X = (oRotMat[1][2]-oRotMat[2][1])*fRoot;
			Y = (oRotMat[2][0]-oRotMat[0][2])*fRoot;
			Z = (oRotMat[0][1]-oRotMat[1][0])*fRoot;
		}
		else
		{
			Int32 iNext[3] = {1, 2, 0};
			Int32 i = 0;

			if (oRotMat[1][1] > oRotMat[0][0])
				i = 1;

			if (oRotMat[2][2] > oRotMat[i][i])
				i = 2;

			Int32 j = iNext[i];
			Int32 k = iNext[j];

			fRoot = HawkMath::Sqrt(oRotMat[i][i]-oRotMat[j][j]-oRotMat[k][k] + 1.0f);
			Float* apkQuat[3] = { &X, &Y, &Z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot       = 0.5f/fRoot;
			W = (oRotMat[j][k]-oRotMat[k][j])*fRoot;
			*apkQuat[j] = (oRotMat[i][j]+oRotMat[j][i])*fRoot;
			*apkQuat[k] = (oRotMat[i][k]+oRotMat[k][i])*fRoot;
		}
		return *this;
	}

	HawkQuaternion&  HawkQuaternion::SetFromMatrix4 (const HawkMatrix4& oRotMat)
	{
		HawkMatrix3 oMat3(oRotMat);
		return SetFromMatrix3(oMat3);
	}

	HawkMatrix3 HawkQuaternion::GetAsMatrix3 () const
	{
		HawkMatrix3 oRotMat;

		Float fNorm = GetNorm();
		Float fScale= (fNorm > 0? 2.0f/fNorm : 0);
		Float fTx   = fScale*X;
		Float fTy   = fScale*Y;
		Float fTz   = fScale*Z;
		Float fTwx  = fTx*W;
		Float fTwy  = fTy*W;
		Float fTwz  = fTz*W;
		Float fTxx  = fTx*X;
		Float fTxy  = fTy*X;
		Float fTxz  = fTz*X;
		Float fTyy  = fTy*Y;
		Float fTyz  = fTz*Y;
		Float fTzz  = fTz*Z;

		oRotMat[0][0] = 1.0f-(fTyy+fTzz);
		oRotMat[1][0] = fTxy-fTwz;
		oRotMat[2][0] = fTxz+fTwy;
		oRotMat[0][1] = fTxy+fTwz;
		oRotMat[1][1] = 1.0f-(fTxx+fTzz);
		oRotMat[2][1] = fTyz-fTwx;
		oRotMat[0][2] = fTxz-fTwy;
		oRotMat[1][2] = fTyz+fTwx;
		oRotMat[2][2] = 1.0f-(fTxx+fTyy);

		return oRotMat;
	}

	HawkMatrix4 HawkQuaternion::GetAsMatrix4 () const
	{
		HawkMatrix3 oMat = GetAsMatrix3();
		return HawkMatrix4(oMat);
	}
	
	void HawkQuaternion::SetFromAxisAngle(const HawkVector3D& vAxis,Float fRadian)
	{
		//cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		HawkVector3D oVec3 = vAxis;
		oVec3.Normalize();

		Float fHalfAngle = 0.5f*fRadian;
		Float fSin       = HawkMath::Sin(fHalfAngle);
		W = HawkMath::Cos(fHalfAngle);
		X = fSin*oVec3.X;
		Y = fSin*oVec3.Y;
		Z = fSin*oVec3.Z;
	}
	
	void HawkQuaternion::GetAsAxisAngle(HawkVector3D& vAxis,Float& fRadian) const
	{
		//cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		Float fSqrLength = X*X+Y*Y+Z*Z;
		if (fSqrLength > 0.0f)
		{
			fRadian = 2.0f*HawkMath::Acos(W);
			Float fInvLength = HawkMath::Invsqrt(fSqrLength);
			vAxis.X = X*fInvLength;
			vAxis.Y = Y*fInvLength;
			vAxis.Z = Z*fInvLength;
		}
		else
		{
			fRadian  = 0.0f;
			vAxis.X  = 1.0f;
			vAxis.Y  = 0.0f;
			vAxis.Z  = 0.0f;
		}

		vAxis.Normalize();
	}
	
	HawkQuaternion& HawkQuaternion::operator = (const HawkQuaternion& oQuat)
	{
		W = oQuat.W;
		X = oQuat.X;
		Y = oQuat.Y;
		Z = oQuat.Z;
		return *this;
	}

	HawkQuaternion HawkQuaternion::operator+ (const HawkQuaternion& oQuat) const
	{
		return HawkQuaternion(W+oQuat.W,X+oQuat.X,Y+oQuat.Y,Z+oQuat.Z);
	}
	
	HawkQuaternion HawkQuaternion::operator- (const HawkQuaternion& oQuat) const
	{
		return HawkQuaternion(W-oQuat.W,X-oQuat.X,Y-oQuat.Y,Z-oQuat.Z);
	}
	
	HawkQuaternion HawkQuaternion::operator* (const HawkQuaternion& oQuat) const
	{
		return HawkQuaternion
			(
			W * oQuat.W - X * oQuat.X - Y * oQuat.Y - Z * oQuat.Z,
			W * oQuat.X + X * oQuat.W + Y * oQuat.Z - Z * oQuat.Y,
			W * oQuat.Y + Y * oQuat.W + Z * oQuat.X - X * oQuat.Z,
			W * oQuat.Z + Z * oQuat.W + X * oQuat.Y - Y * oQuat.X
			);
	}
	
	HawkQuaternion HawkQuaternion::operator * (Float fVal) const
	{
		return HawkQuaternion(fVal*W,fVal*X,fVal*Y,fVal*Z);
	}
	
	HawkQuaternion operator* (Float fVal, const HawkQuaternion& oQuat)
	{
		return HawkQuaternion(fVal*oQuat.W,fVal*oQuat.X,fVal*oQuat.Y,fVal*oQuat.Z);
	}
	
	HawkQuaternion HawkQuaternion::operator - () const
	{
		return HawkQuaternion(-W,-X,-Y,-Z);
	}
	
	Bool HawkQuaternion::operator == (const HawkQuaternion& oQuat) const
	{
		return (oQuat.X == X) && (oQuat.Y == Y) && (oQuat.Z == Z) && (oQuat.W == W);
	}

	Bool HawkQuaternion::operator != (const HawkQuaternion& oQuat) const
	{
		return !operator == (oQuat);
	}

	void HawkQuaternion::SetZero()
	{
		W = 0.0f;
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	void HawkQuaternion::MakeIdentity()
	{
		W = 1.0f;
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	Float HawkQuaternion::DotProduct(const HawkQuaternion& oQuat) const
	{
		return W*oQuat.W+X*oQuat.X+Y*oQuat.Y+Z*oQuat.Z;
	}

	Float HawkQuaternion::Length() const
	{
		return HawkMath::Sqrt(W*W+X*X+Y*Y+Z*Z);
	}
	
	Float  HawkQuaternion::GetMagnitude() const
	{
		return Length();
	}

	Float HawkQuaternion::SQLength() const
	{
		return W*W+X*X+Y*Y+Z*Z;
	}
	
	Float  HawkQuaternion::GetNorm() const
	{
		return SQLength();
	}

	void HawkQuaternion::MakeInverse()
	{
		*this = Inverse();
	}

	HawkQuaternion HawkQuaternion::Inverse () const
	{
		Float fLen = Length();
		if ( fLen > 0.0f )
		{
			Float  fInvLen = 1.0f/fLen;
			return HawkQuaternion(W*fInvLen,-X*fInvLen,-Y*fInvLen,-Z*fInvLen);
		}
		else
		{
			A_Exception(false && "Quaternion Length Error.");
		}
		
		return HawkQuaternion(0,0,0,0);
	}
	
	HawkQuaternion HawkQuaternion::UnitInverse () const
	{
		HawkQuaternion oQuat(W,-X,-Y,-Z);
		return oQuat.Inverse();
	}
	
	HawkQuaternion HawkQuaternion::Exp () const
	{
		HawkQuaternion oQuat;
		Float fRadian = HawkMath::Sqrt(SQLength() - W*W);
		Float fSin    = HawkMath::Sin(fRadian);
		oQuat.W       = HawkMath::Cos(fRadian);

		if (!HawkMath::IsZero(fSin))
		{
			Float fCoeff = fSin/fRadian;
			oQuat.X      = fCoeff*X;
			oQuat.Y      = fCoeff*Y;
			oQuat.Z      = fCoeff*Z;
		}
		else
		{
			oQuat.X = X;
			oQuat.Y = Y;
			oQuat.Z = Z;
		}

		return oQuat;
	}
	
	HawkQuaternion HawkQuaternion::Log () const
	{
		HawkQuaternion oQuat;
		oQuat.W = 0.0f;

		if (HawkMath::Abs(W) < 1.0f)
		{
			Float fRadian = HawkMath::Acos(W);
			Float fSin    = HawkMath::Sin(fRadian);
			if (!HawkMath::IsZero(fSin))
			{
				Float fCoeff = fRadian/fSin;
				oQuat.X      = fCoeff*X;
				oQuat.Y      = fCoeff*Y;
				oQuat.Z      = fCoeff*Z;

				return oQuat;
			}
		}

		oQuat.X = X;
		oQuat.Y = Y;
		oQuat.Z = Z;

		return oQuat;
	}
	
	HawkVector3D HawkQuaternion::operator * (const HawkVector3D& oVec3) const
	{
		HawkVector3D uv, uuv;
		HawkVector3D qvec(X, Y, Z);
		uv  = qvec.CrossProduct(oVec3);
		uuv = qvec.CrossProduct(uv);
		uv  *= (2.0f * W);
		uuv *= 2.0f;

		return oVec3 + uv + uuv;
	}
	
	Bool HawkQuaternion::Equals(const HawkQuaternion& oQuat, Float fDisRadian) const
	{
		Float fCos    = DotProduct(oQuat);
		Float fRadian = HawkMath::Acos(fCos);

		return (HawkMath::Abs(fRadian) <= fDisRadian)|| (HawkMath::Abs(fRadian - HawkMath::PI) <= fDisRadian);
	}
	
	HawkQuaternion HawkQuaternion::Slerp (Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkQ, Bool bShortestPath)
	{
		Float fCos = rkP.DotProduct(rkQ);
		HawkQuaternion rkT;

		if (fCos < 0.0f && bShortestPath)
		{
			fCos = -fCos;
			rkT  = -rkQ;
		}
		else
		{
			rkT = rkQ;
		}

		if (HawkMath::Abs(fCos) < 1.0f - 1e-03)
		{
			Float fSin    = HawkMath::Sqrt(1.0f - HawkMath::Sqrt(fCos));
			Float fAngle  = HawkMath::Atan2(fSin, fCos);
			Float fInvSin = 1.0f / fSin;
			Float fCoeff0 = HawkMath::Sin((1.0f - fT) * fAngle) * fInvSin;
			Float fCoeff1 = HawkMath::Sin(fT * fAngle) * fInvSin;
			return fCoeff0 * rkP + fCoeff1 * rkT;
		}
		else
		{
			HawkQuaternion oQuat = (1.0f - fT) * rkP + fT * rkT;
			oQuat.Normalize();
			return oQuat;
		}
	}
	
	HawkQuaternion HawkQuaternion::SlerpExtraSpins (Float fT,const HawkQuaternion& rkP, const HawkQuaternion& rkQ, int iExtraSpins)
	{
		Float fCos   = rkP.DotProduct(rkQ);
		Float fAngle = HawkMath::Acos(fCos);

		if ( HawkMath::Abs(fAngle) < 1e-03 )
			return rkP;

		Float fSin    = HawkMath::Sin(fAngle);
		Float fPhase  = HawkMath::PI*iExtraSpins*fT;
		Float fInvSin = 1.0f/fSin;
		Float fCoeff0 = HawkMath::Sin((1.0f-fT)*fAngle - fPhase)*fInvSin;
		Float fCoeff1 = HawkMath::Sin(fT*fAngle + fPhase)*fInvSin;
		return fCoeff0*rkP + fCoeff1*rkQ;
	}
	
	HawkQuaternion HawkQuaternion::Squad (Float fT,const HawkQuaternion& rkP, const HawkQuaternion& rkA,const HawkQuaternion& rkB, const HawkQuaternion& rkQ, bool shortestPath)
	{
		Float fSlerpT = 2.0f*fT*(1.0f-fT);
		HawkQuaternion kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
		HawkQuaternion kSlerpQ = Slerp(fT, rkA, rkB);
		return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
	}
	
	Float HawkQuaternion::Normalize(void)
	{
		Float fLen = Length();
		Float fVal = 1.0f / fLen;
		*this = *this * fVal;
		return fLen;
	}
	
	Bool HawkQuaternion::IsZero() const
	{
		return GetNorm() < HawkMath::FLOAT_DIFF;
	}

	Bool HawkQuaternion::IsUnit() const
	{
		return HawkMath::Abs<Float>(GetNorm() - 1) < HawkMath::FLOAT_DIFF;
	}

	HawkQuaternion HawkQuaternion::Nlerp(Float fT, const HawkQuaternion& rkP,const HawkQuaternion& rkQ, Bool bShortestPath)
	{
		HawkQuaternion oQuat;
		Float fCos = rkP.DotProduct(rkQ);
		if (fCos < 0.0f && bShortestPath)
		{
			oQuat = rkP + fT * ((-rkQ) - rkP);
		}
		else
		{
			oQuat = rkP + fT * (rkQ - rkP);
		}
		oQuat.Normalize();
		return oQuat;
	}

	//Yaw Pitch Roll(z*x*y)
	HawkQuaternion& HawkQuaternion::SetFromYawPitchRoll(const HawkVector3D& oYawPitchRoll)
	{
		Float fYaw   = oYawPitchRoll.X;
		Float fPitch = oYawPitchRoll.Y;
		Float fRoll  = oYawPitchRoll.Z;

		Float sinHalfYaw   = HawkMath::Sin(fYaw / 2.0f);
		Float sinHalfPitch = HawkMath::Sin(fPitch / 2.0f);
		Float sinHalfRoll  = HawkMath::Sin(fRoll / 2.0f);

		Float cosHalfYaw   = HawkMath::Cos(fYaw / 2.0f);
		Float cosHalfPitch = HawkMath::Cos(fPitch / 2.0f);
		Float cosHalfRoll  = HawkMath::Cos(fRoll / 2.0f);

		W = cosHalfYaw*cosHalfPitch*cosHalfRoll + sinHalfYaw*sinHalfPitch*sinHalfRoll;
		X = cosHalfYaw*sinHalfPitch*cosHalfRoll + sinHalfYaw*cosHalfPitch*sinHalfRoll;
		Y = sinHalfYaw*cosHalfPitch*cosHalfRoll - cosHalfYaw*sinHalfPitch*sinHalfRoll;
		Z = cosHalfYaw*cosHalfPitch*sinHalfRoll - sinHalfYaw*sinHalfPitch*cosHalfRoll;

		return *this;
	}

	//Get Yaw Pitch Roll
	HawkVector3D  HawkQuaternion::GetAsYawPitchRoll() const
	{
		Float fYaw   = 0;
		Float fPitch = 0;
		Float fRoll  = 0;;

		Float sp = -2.0f * (Y*Z + W*X);
		//万向锁
		if(HawkMath::IsZero(HawkMath::Abs(sp) - 1.0f))
		{
			fPitch = HawkMath::HALF_PI * sp;
			fYaw   = HawkMath::Atan2(-X*Z + W*Y,0.5f - Y*Y - Z*Z);
			fRoll  = 0;
		}
		else
		{
			fPitch = HawkMath::Asin(sp);
			fYaw   = HawkMath::Atan2(X*Z + W*Y,0.5f - X*X - Y*Y);
			fRoll  = HawkMath::Atan2(X*Y + W*Z,0.5f - X*X - Z*Z);
		}

		return HawkVector3D(fYaw,fPitch,fRoll);
	}
}
