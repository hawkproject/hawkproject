#ifndef HAWK_TRANSFORM_H
#define HAWK_TRANSFORM_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* 空间变换操作,主要包含:平移,旋转,缩放,空间变换(matrix)                */
	/* 方法包含:Translate,Rotate,Scale,Transform                            */
	/************************************************************************/
	class GEO_API HawkTransform
	{
	//二维空间的各种变换
	public:
		//平移变换
		static void Translate(HawkVector2D& oVec2,const HawkVector2D& oTrans);
		static void Translate(HawkLine2D& oLine,const HawkVector2D& oTrans);
		static void Translate(HawkRay2D& oRay,const HawkVector2D& oTrans);
		static void Translate(HawkSegment2D& oSegmen,const HawkVector2D& oTrans);
		static void Translate(HawkTriangle2D& oTrig,const HawkVector2D& oTrans);
		static void Translate(HawkRect2D& oRect,const HawkVector2D& oTrans);

		//旋转变换(绕参考的旋转弧度角)
		static void Rotate(HawkVector2D& oVec2,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkLine2D& oLine,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkRay2D& oRay,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkRect2D& oRect,const HawkVector2D& oRefPt,const HawkRadian& oRad);

		//缩放变换(绕参考的缩放倍数)
		static void Scale(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,float fScale);
		static void Scale(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,float fScale);
		static void Scale(HawkRect2D& oRect,const HawkVector2D& oRefPt,float fScale);

		//矩阵变换
		static void Transform(HawkVector2D& oVec2,const HawkMatrix2& oMat2);

		
	//三维空间的各种变换
	public:
		//平移变换
		static void Translate(HawkVector3D& oVec3,const HawkVector3D& oTrans);
		static void Translate(HawkLine3D& oLine,const HawkVector3D& oTrans);
		static void Translate(HawkRay3D& oRay,const HawkVector3D& oTrans);
		static void Translate(HawkSegment3D& oSegmen,const HawkVector3D& oTrans);
		static void Translate(HawkTriangle3D& oTrig,const HawkVector3D& oTrans);
		static void Translate(HawkRect3D& oRect,const HawkVector3D& oTrans);

		//旋转变换(绕参考的旋转弧度角:分别为绕x,y,z轴的弧度)
		static void Rotate(HawkVector3D& oVec3,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkLine3D& oLine,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkRay3D& oRay,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkSegment3D& oSegmen,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkTriangle3D& oTrig,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkRect3D& oRect,const HawkVector3D& oRefAxis,const HawkRadian& oRad);

		//缩放变换(绕参考的缩放倍数)
		static void Scale(HawkSegment3D& oSegmen,const HawkVector3D& oRefPt,float fScale);
		static void Scale(HawkTriangle3D& oTrig,const HawkVector3D& oRefPt,float fScale);
		static void Scale(HawkRect3D& oRect,const HawkVector3D& oRefPt,float fScale);

		//矩阵变换
		static void Transform(HawkVector3D& oVec3,const HawkMatrix3& oMat3);
	};
}
#endif
