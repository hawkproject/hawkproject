#ifndef HAWK_SEGMENT2D_H
#define HAWK_SEGMENT2D_H

#include "HawkGeoDef.h"
#include "HawkVector2D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�ռ��߶�����                                                     */
	/************************************************************************/
	class GEO_API HawkSegment2D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,Start,Start);
		DEFINE_PUBLIC_PROPERTY(HawkVector2D,End,End);

	public:
		//����
		HawkSegment2D();

		//����
		HawkSegment2D(const HawkVector2D& oStart,const HawkVector2D& oEnd);

		//��������
		HawkSegment2D(const HawkSegment2D& oSegment);

		//�����ߵĵ�����
		void  Set(const HawkVector2D& oStart,const HawkVector2D& oEnd);

		//�ж����Ƿ���������(�����ߵ�������Ҫ����)
		Bool  IsPointOnSegment(const HawkVector2D& oVec2) const;		
		
		//���������oVec�������ĵ�
		HawkVector2D GetClosestPoint(const HawkVector2D& oVec2) const;

		//�㵽ֱ�ߵľ���
		Float GetDistance(const HawkVector2D& oVec2) const;

		//����
		Float GetLength();

		//�е�
		HawkVector2D MidPoint() const;

		//��÷���
		HawkVector2D GetDirection() const;

		//���������������
		HawkSegment2D  operator + (const HawkVector2D& oVec2) const ;

		HawkSegment2D& operator += (const HawkVector2D& oVec2);

		HawkSegment2D  operator - (const HawkVector2D& oVec2) const ;

		HawkSegment2D& operator -= (const HawkVector2D& oVec2) ;

		Bool operator == (const HawkSegment2D& oSegment) const;

		Bool operator != (const HawkSegment2D& oSegment) const;

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
