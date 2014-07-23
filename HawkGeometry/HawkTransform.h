#ifndef HAWK_TRANSFORM_H
#define HAWK_TRANSFORM_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* �ռ�任����,��Ҫ����:ƽ��,��ת,����,�ռ�任(matrix)                */
	/* ��������:Translate,Rotate,Scale,Transform                            */
	/************************************************************************/
	class GEO_API HawkTransform
	{
	//��ά�ռ�ĸ��ֱ任
	public:
		//ƽ�Ʊ任
		static void Translate(HawkVector2D& oVec2,const HawkVector2D& oTrans);
		static void Translate(HawkLine2D& oLine,const HawkVector2D& oTrans);
		static void Translate(HawkRay2D& oRay,const HawkVector2D& oTrans);
		static void Translate(HawkSegment2D& oSegmen,const HawkVector2D& oTrans);
		static void Translate(HawkTriangle2D& oTrig,const HawkVector2D& oTrans);
		static void Translate(HawkRect2D& oRect,const HawkVector2D& oTrans);

		//��ת�任(�Ʋο�����ת���Ƚ�)
		static void Rotate(HawkVector2D& oVec2,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkLine2D& oLine,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkRay2D& oRay,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,const HawkRadian& oRad);
		static void Rotate(HawkRect2D& oRect,const HawkVector2D& oRefPt,const HawkRadian& oRad);

		//���ű任(�Ʋο������ű���)
		static void Scale(HawkSegment2D& oSegmen,const HawkVector2D& oRefPt,float fScale);
		static void Scale(HawkTriangle2D& oTrig,const HawkVector2D& oRefPt,float fScale);
		static void Scale(HawkRect2D& oRect,const HawkVector2D& oRefPt,float fScale);

		//����任
		static void Transform(HawkVector2D& oVec2,const HawkMatrix2& oMat2);

		
	//��ά�ռ�ĸ��ֱ任
	public:
		//ƽ�Ʊ任
		static void Translate(HawkVector3D& oVec3,const HawkVector3D& oTrans);
		static void Translate(HawkLine3D& oLine,const HawkVector3D& oTrans);
		static void Translate(HawkRay3D& oRay,const HawkVector3D& oTrans);
		static void Translate(HawkSegment3D& oSegmen,const HawkVector3D& oTrans);
		static void Translate(HawkTriangle3D& oTrig,const HawkVector3D& oTrans);
		static void Translate(HawkRect3D& oRect,const HawkVector3D& oTrans);

		//��ת�任(�Ʋο�����ת���Ƚ�:�ֱ�Ϊ��x,y,z��Ļ���)
		static void Rotate(HawkVector3D& oVec3,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkLine3D& oLine,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkRay3D& oRay,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkSegment3D& oSegmen,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkTriangle3D& oTrig,const HawkVector3D& oRefAxis,const HawkRadian& oRad);
		static void Rotate(HawkRect3D& oRect,const HawkVector3D& oRefAxis,const HawkRadian& oRad);

		//���ű任(�Ʋο������ű���)
		static void Scale(HawkSegment3D& oSegmen,const HawkVector3D& oRefPt,float fScale);
		static void Scale(HawkTriangle3D& oTrig,const HawkVector3D& oRefPt,float fScale);
		static void Scale(HawkRect3D& oRect,const HawkVector3D& oRefPt,float fScale);

		//����任
		static void Transform(HawkVector3D& oVec3,const HawkMatrix3& oMat3);
	};
}
#endif
