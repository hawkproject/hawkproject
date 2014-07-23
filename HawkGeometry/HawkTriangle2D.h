#ifndef HAWK_TRIANGLE2D_H
#define HAWK_TRIANGLE2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά��������	                                                        */
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge2                              */
	/************************************************************************/
	class GEO_API HawkTriangle2D
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkTriangle2D();

		//���½�����������߹���
		HawkTriangle2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2);

		//��������
		HawkTriangle2D(const HawkTriangle2D& oTrig);

		//����������
		Bool operator == (const HawkTriangle2D& oTrig) const;

		Bool operator != (const HawkTriangle2D& oTrig) const;

		HawkTriangle2D operator + (const HawkVector2D& oVec2) const;

		HawkTriangle2D& operator += (const HawkVector2D& oVec2);

		HawkTriangle2D operator - (const HawkVector2D& oVec2) const;

		HawkTriangle2D operator -= (const HawkVector2D& oVec2);

		//��ö���
		HawkVector2D GetPoint(Int32 iIdx) const;

		//���������Ӧ�Ķ���
		HawkVector2D operator [](Int32 iIdx) const;

		//�������������
		Float GetArea() const;

		//���ö���(ԭʼ��)
		void  SetOriginPoint(const HawkVector2D& oPt);

		const HawkVector2D& GetOriginPoint() const;

		//���ñ�
		void  SetEdge(Int32 iIdx,const HawkVector2D& oEdge);

		//��ñ�
		const HawkVector2D& GetEdge(Int32 iIdx) const;

		//���������ε���������
		void  SetByPoint(const HawkVector2D& vPt1,const HawkVector2D& vPt2,const HawkVector2D& vPt3);

		//��ȡ��Χ���ĵ�(�����Բ)
		HawkVector2D  GetBoundCenter() const;

		//��ȡ��Χ�뾶
		Float         GetBoundRadius() const;

		//��ȡ���㷽��(˳,��ʱ��)
		Bool		  IsClockWise() const;

		//��ȡ�߶�
		HawkSegment2D GetSegment(Int32 iIdx) const;

	public:
		//ĳһ�ض�������������֮��
		Bool  IsPointInside(const HawkVector2D& oVec2) const;

		//ĳһ�ض�������������֮��
		Bool  IsPointInside(const HawkPoint2D& oPt2) const;
	
	public:
		//ֱ�ߺ��������ཻ�ж�
		Bool  Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//���ߺ��������ཻ�ж�
		Bool  Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//�߶κ��������ཻ�ж�
		Bool  Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//�����κ��������ཻ�ж�
		Bool  Intersect(const HawkTriangle2D& oTriangle) const;

		//�����κ;����ཻ�ж�
		Bool  Intersect(const HawkRect2D& oRect) const;

	public:
		//�����ζ����2����
		HawkVector2D  Point;
		HawkVector2D  Edge[2];
	};
}
#endif
