#ifndef HAWK_LINE2D_H
#define HAWK_LINE2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�ռ�ֱ������                                                     */
	/************************************************************************/
	class GEO_API HawkLine2D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Direction,Direction);

	public:
		//Ĭ�Ϲ���
		HawkLine2D();

		//ʹ�������㹹��
		HawkLine2D(const HawkVector2D& oPt1,const HawkVector2D& oPt2);

		//��������
		HawkLine2D(const HawkLine2D& oLine);

		//�����ߵĵ�����
		void  Set(const HawkVector2D& oPt1,const HawkVector2D& oPt2);

		//����һ�����ֱ�ߵ�λ�ù�ϵ
		PtRelation GetPtRelation(const HawkVector2D& oVec2) const;

		//�ж����Ƿ���������(�����ߵ�������Ҫ����)
		Bool  IsPointOnLine(const HawkVector2D& oVec2) const;
		
		//���������oVec�������ĵ�
		HawkVector2D GetClosestPoint(const HawkVector2D& oVec2) const;

		//�㵽ֱ�ߵľ���
		Float GetDistance(const HawkVector2D& oVec2) const;

		//�жϵ���ֱ�����
		Bool IsOnLeft(const HawkVector2D& oVec2) const;		

		//���������������
		HawkLine2D  operator + (const HawkVector2D& oVec2) const;

		HawkLine2D& operator += (const HawkVector2D& oVec2);

		HawkLine2D  operator - (const HawkVector2D& oVec2) const ;

		HawkLine2D& operator -= (const HawkVector2D& oVec2);

		Bool operator == (const HawkLine2D& oLine) const;

		Bool operator != (const HawkLine2D& oLine) const;

	public:
		//ֱ���ཻ�Ľ���
		Bool Intersect(const HawkLine2D& oLine,Vec2IntrResult* pResult = 0) const;

		//ֱ�ߺ��߶��ཻ�Ľ���
		Bool Intersect(const HawkSegment2D& oSegment,Vec2IntrResult* pResult = 0) const;

		//ֱ�ߺ������ཻ�Ľ���
		Bool Intersect(const HawkRay2D& oRay,Vec2IntrResult* pResult = 0) const;

		//ֱ�ߺ��������ཻ�Ľ���
		Bool Intersect(const HawkTriangle2D& oTriangle,Vec2IntrResult* pResult = 0) const;

		//ֱ�ߺ;����ཻ�Ľ���
		Bool Intersect(const HawkRect2D& oRect,Vec2IntrResult* pResult = 0) const;
	};
}
#endif
