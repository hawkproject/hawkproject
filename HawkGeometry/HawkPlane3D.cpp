#include "HawkPlane3D.h"
#include "HawkMatrix3.h"
#include "HawkVector3D.h"
#include "HawkTriangle3D.h"
#include "HawkRect3D.h"
#include "HawkLine3D.h"
#include "HawkSegment3D.h"
#include "HawkRay3D.h"
#include "HawkSphere.h"
#include "HawkAxisAlignedBox.h"
#include "HawkOBB.h"
#include "HawkCapsule.h"
#include "HawkVector4D.h"

namespace Hawk
{
	//默认构造函数
	HawkPlane3D::HawkPlane3D () : Normal(HawkVector3D(0,1,0)),Constant(0.0f){};

	//拷贝构造函数
	HawkPlane3D::HawkPlane3D (const HawkPlane3D& oPlane) : Normal(oPlane.Normal),Constant(oPlane.Constant){};

	//构造三角形所在的平面
	HawkPlane3D::HawkPlane3D (const HawkTriangle3D& oTrig)
	{
		Redefine(oTrig[0],oTrig[1],oTrig[2]);
	};

	//使用平面方程的四个参数构造平面
	HawkPlane3D::HawkPlane3D (Float fA, Float fB, Float fC, Float fD)
		:Normal(fA,fB,fC),Constant(fD){};

	HawkPlane3D::HawkPlane3D (const HawkVector4D& oVec4)
		:Normal(oVec4.X,oVec4.Y,oVec4.Z),Constant(oVec4.W){};

	//使用法线和固定点构造平面
	HawkPlane3D::HawkPlane3D (const HawkVector3D& oNormal, const HawkVector3D& oVec3)
	{
		Redefine(oNormal,oVec3);
	}

	//使用三维空间的三个顶点构造平面
	HawkPlane3D::HawkPlane3D (const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3)
	{
		Redefine(oPt1,oPt2,oPt3);
	}

	//判断操作符重载
	Bool HawkPlane3D::operator==(const HawkPlane3D& oPlane) const
	{
		return HawkMath::FloatEqual(oPlane.Constant,Constant) && 
			   oPlane.Normal == Normal;
	}

	Bool HawkPlane3D::operator!=(const HawkPlane3D& oPlane) const
	{
		return !(*this == oPlane);
	}

	//计算点和平面的位置关系
	HawkPlane3D::PlaneSide HawkPlane3D::GetSide (const HawkVector3D& oVec3) const
	{
		Float fDis = GetDistance(oVec3);

		//反面
		if ( fDis < -HawkMath::FLOAT_DIFF )
			return HawkPlane3D::PLANESIDE_NEGATIVE;

		if ( fDis > HawkMath::FLOAT_DIFF )
			return HawkPlane3D::PLANESIDE_POSITIVE;

		return HawkPlane3D::PLANESIDE_ON;
	}

	HawkPlane3D::PlaneSide HawkPlane3D::GetSide (const HawkVector3D& vCenter, const HawkVector3D& vHalfSize) const
	{
		Float fDist = GetDistance(vCenter);
		Float fMaxAbsDist = HawkMath::Abs(Normal.DotProduct(vHalfSize));

		if (fDist < -fMaxAbsDist)
			return HawkPlane3D::PLANESIDE_NEGATIVE;

		if (fDist > +fMaxAbsDist)
			return HawkPlane3D::PLANESIDE_POSITIVE;

		return HawkPlane3D::PLANESIDE_BOTH;
	}

	//计算盒子和平面的位置关系
	HawkPlane3D::PlaneSide HawkPlane3D::GetSide (const HawkAxisAlignedBox& oAxisAlignedBox) const
	{
		if (oAxisAlignedBox.IsNull()) return HawkPlane3D::PLANESIDE_ON;

		return GetSide(oAxisAlignedBox.GetCenter(), oAxisAlignedBox.GetExtents());
	}

	//计算点到平面的距离
	Float HawkPlane3D::GetDistance (const HawkVector3D& oVec3) const
	{
		return Normal.DotProduct(oVec3) + Constant;
	}

	//三个点重定义平面
	void HawkPlane3D::Redefine(const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3)
	{
		HawkVector3D kEdge1 = oPt2 - oPt1;
		HawkVector3D kEdge2 = oPt3 - oPt1;
		Normal = kEdge1.CrossProduct(kEdge2);
		Normal.Normalize();
		Constant = -Normal.DotProduct(oPt1);
	}

	//一个点和一个法线定义平面
	void HawkPlane3D::Redefine(const HawkVector3D& oNormal, const HawkVector3D& oPt)
	{
		Normal = oNormal;
		Constant = -oNormal.DotProduct(oPt);
	}

	//投影一个向量到平面
	HawkVector3D HawkPlane3D::ProjectVector(const HawkVector3D& oVec3) const
	{
		HawkMatrix3 oMat;
		oMat.M[0] = 1.0f - Normal.X * Normal.X;
		oMat.M[1] = -Normal.X * Normal.Y;
		oMat.M[2] = -Normal.X * Normal.Z;
		oMat.M[3] = -Normal.Y * Normal.X;
		oMat.M[4] = 1.0f - Normal.Y * Normal.Y;
		oMat.M[5] = -Normal.Z * Normal.Z;
		oMat.M[6] = -Normal.Z * Normal.X;
		oMat.M[7] = -Normal.Z * Normal.Y;
		oMat.M[8] = 1.0f - Normal.Z * Normal.Z;
		return oVec3*oMat;
	}

	//平面单位化
	Float HawkPlane3D::Normalize(void)
	{
		Float fLen = Normal.Length();

		if (fLen > HawkMath::FLOAT_DIFF)
		{
			Float fInvLen = 1.0f / fLen;
			Normal   *= fInvLen;
			Constant *= fInvLen;
		}

		return fLen;
	}

	//点乘
	Float  HawkPlane3D::DotProduct(const HawkVector4D& oVec4)
	{
		return Normal.X*oVec4.X + Normal.Y*oVec4.Y + Normal.Z*oVec4.Z + Constant*oVec4.W;
	}

	//////////////////////////////////////////////////////////////////////////
	//三角形和平面相交判断
	Bool  HawkPlane3D::Intersect(const HawkTriangle3D& oTriangle) const
	{
		PlaneSide sPS[3] = {PLANESIDE_ON};
		for(Int32 i=0;i<3;i++)
		{
			sPS[i] = GetSide(oTriangle.GetPoint(i));
			if (sPS[i] == PLANESIDE_ON)
				return true;		
		}

		return sPS[0] == sPS[1] && sPS[1] == sPS[2];
	}

	//矩形和平面相交判断
	Bool  HawkPlane3D::Intersect(const HawkRect3D& oRect) const
	{
		PlaneSide sPS[4] = {PLANESIDE_ON};
		for(Int32 i=0;i<4;i++)
		{
			sPS[i] = GetSide(oRect.GetPoint(i));
			if (sPS[i] == PLANESIDE_ON)
				return true;		
		}

		return sPS[0] == sPS[1] && sPS[1] == sPS[2] && sPS[2] == sPS[3];
	}

	//平面和平面相交判断
	Bool  HawkPlane3D::Intersect(const HawkPlane3D& oPlane) const
	{
		Float fDot = Normal.DotProduct(oPlane.Normal);
		return HawkMath::Abs<Float>(fDot) < 1.0f - HawkMath::FLOAT_DIFF;
	}

	//平面和AABB盒相交判断
	Bool  HawkPlane3D::Intersect(const HawkAxisAlignedBox& oBox) const
	{
		return GetSide(oBox) == HawkPlane3D::PLANESIDE_BOTH;
	}

	//平面和球相交判断
	Bool  HawkPlane3D::Intersect(const HawkSphere& oSphere) const
	{
		Float fDistance = GetDistance(oSphere.Center);
		return HawkMath::Abs<Float>(fDistance) <= oSphere.Radius;
	}

	Bool  HawkPlane3D::Intersect(const HawkOBB& oBox) const
	{
		return oBox.Intersect(*this);
	}

	Bool  HawkPlane3D::Intersect(const HawkCapsule& oCapsule) const
	{
		return oCapsule.Intersect(*this);
	}
}

