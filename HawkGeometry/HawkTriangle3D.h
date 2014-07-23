#ifndef HAWK_TRIANGLE3D_H
#define HAWK_TRIANGLE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά��������                                                         */
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge2                              */
	/************************************************************************/
	class GEO_API HawkTriangle3D
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkTriangle3D();

		//���½�����������߹���
		HawkTriangle3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2);

		//��������
		HawkTriangle3D(const HawkTriangle3D& oTrig);

		//����������
		Bool operator == (const HawkTriangle3D& oTrig) const;

		Bool operator != (const HawkTriangle3D& oTrig) const;

		HawkTriangle3D  operator + (const HawkVector3D& oVec) const;

		HawkTriangle3D& operator += (const HawkVector3D& oVec);

		HawkTriangle3D  operator - (const HawkVector3D& oVec) const;

		HawkTriangle3D  operator -= (const HawkVector3D& oVec);

		HawkVector3D    operator [](Int32 iIdx) const;

		//�������������
		Float GetArea() const;

		//���ö���
		void  SetOriginPoint(const HawkVector3D& oPt);

		const HawkVector3D& GetOriginPoint() const;
		
		//���ñ�
		void  SetEdge(Int32 iIdx,const HawkVector3D& oEdge);

		//��ñ�
		const HawkVector3D& GetEdge(Int32 iIdx) const;

		//���������η���(��������ϵ,Ϊ������)
		HawkVector3D GetNormal() const;
		
		//���������εĶ���
		HawkVector3D GetPoint(Int32 iIdx) const;
		
		//��������������
		Bool IsFrontFacing(const HawkVector3D& vVec3) const;

		//�����������ڲ�
		Bool IsPointInside(const HawkVector3D& oVec3) const;
		
		//���������ε���������
		void SetByPoint(const HawkVector3D& vPt1,const HawkVector3D& vPt2,const HawkVector3D& vPt3);

		//��ȡ���ĵ�
		HawkVector3D  GetCenterPoint() const;

		//����߶�
		HawkSegment3D GetSegment(Int32 iIdx) const;

		void GetProjection(const HawkVector3D& vAxis,Float& fMin, Float& fMax) const;

		//������ֱ��ͬһ��
		static Bool  IsOnSameSide(const HawkVector3D& vP1, const HawkVector3D& vP2,const HawkVector3D& vA, const HawkVector3D& vB);

	public:
		//�����ζ����2����
		HawkVector3D  Point;
		HawkVector3D  Edge[2];
	};
}
#endif
