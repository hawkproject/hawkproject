#include "HawkTransform.h"
#include "HawkRadian.h"
#include "HawkDegree.h"
#include "HawkLine2D.h"
#include "HawkRay2D.h"
#include "HawkSegment2D.h"
#include "HawkTriangle2D.h"
#include "HawkRect2D.h"
#include "HawkLine3D.h"
#include "HawkRay3D.h"
#include "HawkSegment3D.h"
#include "HawkTriangle3D.h"
#include "HawkRect3D.h"
#include "HawkVector2D.h"
#include "HawkVector3D.h"
#include "HawkVector4D.h"
#include "HawkMatrix2.h"
#include "HawkMatrix3.h"
#include "HawkMatrix4.h"
#include "HawkPlane3D.h"

namespace Hawk
{
	//平移变换
	void HawkTransform::Translate(HawkVector2D& oVec2,const HawkVector2D& oTrans)
	{
		oVec2 += oTrans;
	}

	void HawkTransform::Translate(HawkLine2D& oLine,const HawkVector2D& oTrans)
	{
		oLine.Point += oTrans;
	}

	void HawkTransform::Translate(HawkRay2D& oRay,const HawkVector2D& oTrans)
	{
		oRay.Point += oTrans;
	}

	void HawkTransform::Translate(HawkSegment2D& oSegmen,const HawkVector2D& oTrans)
	{
		oSegmen.Start += oTrans;
		oSegmen.End   += oTrans;
	}

	void HawkTransform::Translate(HawkTriangle2D& oTrig,const HawkVector2D& oTrans)
	{
		oTrig.Point += oTrans;
	}

	void HawkTransform::Translate(HawkRect2D& oRect,const HawkVector2D& oTrans)
	{
		oRect.Point += oTrans;
	}
	
	//旋转变换(绕参考的旋转弧度角)
	void HawkTransform::Rotate(HawkVector2D& oVec2,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		HawkVector2D oTmp = oVec2 - oRefPt;
		HawkMatrix2  oMat;
		oMat.SetRotation(oRad.Value());
		Transform(oTmp,oMat);
		oVec2 = oTmp + oRefPt;
	}

	void HawkTransform::Rotate(HawkLine2D& oLine,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		Rotate(oLine.Point,oRefPt,oRad);
		Rotate(oLine.Direction,HawkVector2D(0,0),oRad);
	}

	void HawkTransform::Rotate(HawkRay2D& oRay,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		Rotate(oRay.Point,oRefPt,oRad);
		Rotate(oRay.Direction,HawkVector2D(0,0),oRad);
	}

	void HawkTransform::Rotate(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		Rotate(oSegmen.Start,oRefPt,oRad);
		Rotate(oSegmen.End,oRefPt,oRad);
	}

	void HawkTransform::Rotate(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		Rotate(oTrig.Point,oRefPt,oRad);
		Rotate(oTrig.Edge[0],HawkVector2D(0,0),oRad);
		Rotate(oTrig.Edge[1],HawkVector2D(0,0),oRad);
	}

	void HawkTransform::Rotate(HawkRect2D& oRect,const HawkVector2D& oRefPt,const HawkRadian& oRad)
	{
		Rotate(oRect.Point,oRefPt,oRad);
		Rotate(oRect.Edge[0],HawkVector2D(0,0),oRad);
		Rotate(oRect.Edge[1],HawkVector2D(0,0),oRad);
	}
	
	//缩放变换(绕参考的缩放倍数)
	void HawkTransform::Scale(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,Float fScale)
	{
		if(oSegmen.IsPointOnSegment(oRefPt) && fScale>=0.0f)
		{
			HawkVector2D oTmp1 = oRefPt - oSegmen.Start;
			HawkVector2D oTmp2 = oRefPt - oSegmen.End;
			oSegmen.Start   = oRefPt + fScale*oTmp1;
			oSegmen.End     = oRefPt + fScale*oTmp2;
		}
	}

	void HawkTransform::Scale(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,Float fScale)
	{
		if(oTrig.IsPointInside(oRefPt) && fScale>=0.0f)
		{
			HawkVector2D oTmp = oRefPt - oTrig.Point;
			oTrig.Point	   = oRefPt + fScale*oTmp;
			oTrig.Edge[0] *= fScale;
			oTrig.Edge[1] *= fScale;
		}
	}

	void HawkTransform::Scale(HawkRect2D& oRect,const HawkVector2D& oRefPt,Float fScale)
	{
		if(oRect.IsPointInside(oRefPt) && fScale>=0.0f)
		{
			HawkVector2D oTmp = oRefPt - oRect.Point;
			oRect.Point	   = oRefPt + fScale*oTmp;
			oRect.Edge[0] *= fScale;
			oRect.Edge[1] *= fScale;
		}
	}
	
	//矩阵变换
	void HawkTransform::Transform(HawkVector2D& oVec2,const HawkMatrix2& oMat2)
	{
		HawkVector2D oTmp = oVec2;
		oVec2.X = oTmp.X*oMat2.M00 + oTmp.Y*oMat2.M10;
		oVec2.Y = oTmp.X*oMat2.M01 + oTmp.Y*oMat2.M11;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	//三维变换
	///////////////////////////////////////////////////////////////////////////////////////
	//平移变换
	void HawkTransform::Translate(HawkVector3D& oVec3,const HawkVector3D& oTrans)
	{
		oVec3 += oTrans;
	}

	void HawkTransform::Translate(HawkLine3D& oLine,const HawkVector3D& oTrans)
	{
		oLine.Point += oTrans;
	}

	void HawkTransform::Translate(HawkRay3D& oRay,const HawkVector3D& oTrans)
	{
		oRay.Point += oTrans;
	}

	void HawkTransform::Translate(HawkSegment3D& oSegmen,const HawkVector3D& oTrans)
	{
		oSegmen.Start += oTrans;
		oSegmen.End   += oTrans;
	}

	void HawkTransform::Translate(HawkTriangle3D& oTrig,const HawkVector3D& oTrans)
	{
		oTrig.Point += oTrans;
	}

	void HawkTransform::Translate(HawkRect3D& oRect,const HawkVector3D& oTrans)
	{
		oRect.Point += oTrans;
	}
	
	//旋转变换(绕参考的旋转弧度角)
	void HawkTransform::Rotate(HawkVector3D& oVec3,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		HawkMatrix3 oMat;
		oMat.SetRotationAxis(oRefAxis,oRad.Value());
		oVec3 = oVec3*oMat;
	}

	void HawkTransform::Rotate(HawkLine3D& oLine,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		Rotate(oLine.Point,oRefAxis,oRad);
		Rotate(oLine.Direction,oRefAxis,oRad);
	}

	void HawkTransform::Rotate(HawkRay3D& oRay,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		Rotate(oRay.Point,oRefAxis,oRad);
		Rotate(oRay.Direction,oRefAxis,oRad);
	}

	void HawkTransform::Rotate(HawkSegment3D& oSegmen,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		Rotate(oSegmen.Start,oRefAxis,oRad);
		Rotate(oSegmen.End,oRefAxis,oRad);
	}

	void HawkTransform::Rotate(HawkTriangle3D& oTrig,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		Rotate(oTrig.Point,oRefAxis,oRad);
		Rotate(oTrig.Edge[0],oRefAxis,oRad);
		Rotate(oTrig.Edge[1],oRefAxis,oRad);
	}

	void HawkTransform::Rotate(HawkRect3D& oRect,const HawkVector3D& oRefAxis,const HawkRadian& oRad)
	{
		Rotate(oRect.Point,oRefAxis,oRad);
		Rotate(oRect.Edge[0],oRefAxis,oRad);
		Rotate(oRect.Edge[1],oRefAxis,oRad);
	}
	
	//缩放变换(绕参考的缩放倍数)
	void HawkTransform::Scale(HawkSegment3D& oSegmen,const HawkVector3D& oRefPt,Float fScale)
	{
		if(oSegmen.IsPointOnSegment(oRefPt) && fScale>=0.0f)
		{
			HawkVector3D oTmp1 = oRefPt - oSegmen.Start;
			HawkVector3D oTmp3 = oRefPt - oSegmen.End;
			oSegmen.Start   = oRefPt + fScale*oTmp1;
			oSegmen.End     = oRefPt + fScale*oTmp3;
		}
	}

	void HawkTransform::Scale(HawkTriangle3D& oTrig,const HawkVector3D& oRefPt,Float fScale)
	{
		if(oTrig.IsPointInside(oRefPt) && fScale>=0.0f)
		{
			HawkVector3D oTmp = oRefPt - oTrig.Point;
			oTrig.Point	   = oRefPt + fScale*oTmp;
			oTrig.Edge[0] *= fScale;
			oTrig.Edge[1] *= fScale;
		}
	}

	void HawkTransform::Scale(HawkRect3D& oRect,const HawkVector3D& oRefPt,Float fScale)
	{
		if(oRect.IsPointInside(oRefPt) && fScale>=0.0f)
		{
			HawkVector3D oTmp = oRefPt - oRect.Point;
			oRect.Point	   = oRefPt + fScale*oTmp;
			oRect.Edge[0] *= fScale;
			oRect.Edge[1] *= fScale;
		}
	}
	
	//矩阵变换
	void HawkTransform::Transform(HawkVector3D& oVec3,const HawkMatrix3& oMat3)
	{
		HawkVector3D oTmp = oVec3;
		oVec3.X = oTmp.X*oMat3.M00 + oTmp.Y*oMat3.M10 + oTmp.Z*oMat3.M20;
		oVec3.Y = oTmp.X*oMat3.M01 + oTmp.Y*oMat3.M11 + oTmp.Z*oMat3.M21;
		oVec3.Z = oTmp.X*oMat3.M02 + oTmp.Y*oMat3.M12 + oTmp.Z*oMat3.M22;
	}
}


