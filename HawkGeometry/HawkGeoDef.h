#ifndef HAWK_GEODEF_H
#define HAWK_GEODEF_H

#include "HawkGeometry.h"

namespace Hawk
{
	/************************************************************************/
	/* 所有几何类型申明                                                     */
	/************************************************************************/
	class HawkAxisAlignedBox;
	class HawkCapsule;
	class HawkDegree;
	class HawkLine2D;
	class HawkLine3D;
	class HawkMatrix2;
	class HawkMatrix3;
	class HawkMatrix4;
	class HawkOBB;
	class HawkPlane3D;
	class HawkPoint2D;
	class HawkQuaternion;
	class HawkRadian;
	class HawkRay2D;
	class HawkRay3D;
	class HawkRect;
	class HawkRect2D;
	class HawkRect3D;
	class HawkSegment2D;
	class HawkSegment3D;
	class HawkSphere;
	class HawkTransform;
	class HawkTriangle2D;
	class HawkTriangle3D;
	class HawkVector2D;
	class HawkVector3D;
	class HawkVector4D;

	//点的位置关系
	enum PtRelation
	{
		PT_NONE = 0,
		PT_LEFT,
		PT_ON,
		PT_RIGHT,
	};

	//单一相交结果数据封装
	template<class VT> struct IntrResult
	{
		VT		Point;
		Float	Factor;	

		IntrResult() : Point(),Factor(0.0f) {}

		IntrResult<VT>& operator = (const IntrResult<VT>& sRes)
		{
			Point  = sRes.Point;
			Factor = sRes.Factor;
			return *this;
		}

		Bool operator < (const IntrResult<VT>& sRes) const
		{
			return Factor < sRes.Factor;
		}

		Bool operator == (const IntrResult<VT>& sRes) const
		{
			return HawkMath::Abs(Factor - sRes.Factor) <= HawkMath::FLOAT_DIFF && Point == sRes.Point;
		}
	};

	typedef IntrResult<HawkVector2D> Vec2IntrResult;
	typedef IntrResult<HawkVector3D> Vec3IntrResult;
}
#endif
