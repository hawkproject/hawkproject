#ifndef HAWK_RECT2D_H
#define HAWK_RECT2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* ������																*/
	/* ʹ�����½��������������Ϊ����										*/
	/*		   Edge1														*/
	/*		1 -----> 2														*/
	/*Edge2	|  		 |														*/
	/*		|		 |														*/
	/*		4 ------ 3														*/
	/* pt2 = pt1 + edge1  &  pt3 = pt1 + edge1 + edge2 & pt4 =  pt1 + edge2 */
	/************************************************************************/	
	class GEO_API HawkRect2D
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkRect2D();

		//���½�����������߹���
		HawkRect2D(const HawkVector2D& oPt,const HawkVector2D& oEdge1,const HawkVector2D& oEdge2);

		//��������
		HawkRect2D(const HawkRect2D& oRc);

		HawkRect2D(const HawkRect& oRc);

		//����������
		Bool operator == (const HawkRect2D& oRc) const;

		Bool operator != (const HawkRect2D& oRc) const;

		HawkRect2D   operator + (const HawkVector2D& oVec) const;

		HawkRect2D&  operator += (const HawkVector2D& oVec);

		HawkRect2D   operator - (const HawkVector2D& oVec) const;

		HawkRect2D   operator -= (const HawkVector2D& oVec);

		HawkVector2D operator [](Int32 iIdx) const;

		//�������������
		Float GetArea() const;

		//���ö���
		void  SetOriginPoint(const HawkVector2D& oPt);

		const HawkVector2D& GetOriginPoint() const;

		//�����е�
		void  SetCenterPoint(const HawkVector2D& oPt);

		HawkVector2D GetCenterPoint() const;

		//��ȡ��Χ���ĵ�(�����Բ)
		HawkVector2D GetBoundCenter() const;

		//��ȡ��Χ�뾶
		Float        GetBoundRadius() const;

		//��ȡ���㷽��(˳,��ʱ��)
		Bool		 IsClockWise() const;

		//���ñ�
		void  SetEdge(Int32 iIdx,const HawkVector2D& oEdge);

		//����߶�
		HawkSegment2D  GetSegment(Int32 iIdx) const;

		//��ñ�
		const HawkVector2D& GetEdge(Int32 iIdx) const;

		//��ȼ���
		Float GetWidth() const;

		void  SetWidth(Float fWidth);

		//�߶ȼ���
		Float GetHeight() const;

		void  SetHeight(Float fHeight);

		//�������һ������
		HawkVector2D GetPoint(Int32 iIdx) const;
	
		//�������
		void  Clear();

	public:
		//�ཻ���
		//ĳһ�ض������ھ���֮��
		Bool  IsPointInside(const HawkVector2D& oVec) const;

	public:
		//ֱ�ߺ;����ཻ�ж�
		Bool  Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//���ߺ;����ཻ�ж�
		Bool  Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//�߶κ;����ཻ�ж�
		Bool  Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//�����κ;����ཻ�ж�
		Bool  Intersect(const HawkTriangle2D& oTriangle) const;

		//���κ;����ཻ�ж�
		Bool  Intersect(const HawkRect2D& oRect) const;

	public:
		//�����ζ����2����
		HawkVector2D  Point;
		HawkVector2D  Edge[2];
	};
}
#endif
