#ifndef HAWK_SEGMENT3D_H
#define HAWK_SEGMENT3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�ռ��߶�����                                                     */
	/************************************************************************/
	class GEO_API HawkSegment3D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Start,Start);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,End,End);

	public:
		//����
		HawkSegment3D();

		//����
		HawkSegment3D(const HawkVector3D& oStart,const HawkVector3D& oEnd);

		//��������
		HawkSegment3D(const HawkSegment3D& oSegment);

		//��÷���
		HawkVector3D GetDirection() const;

		//��ȡ�е�
		HawkVector3D GetCenter() const;

		//�����ߵĵ�����
		void  Set(const HawkVector3D& oStart,const HawkVector3D& oEnd);

		//�ж����Ƿ���������(�����ߵ�������Ҫ����)
		Bool  IsPointOnSegment(const HawkVector3D& oVec) const;
	
		//����
		Float GetLength() const;

		//�볤
		Float GetExtent() const;

		//����
		Float GetDistance(const HawkSegment3D& oSegment) const;
		Float GetDistance(const HawkLine3D& oLine) const;
		Float GetDistance(const HawkRay3D& oRay) const;

		//���������������
		HawkSegment3D  operator + (const HawkVector3D& oVec) const ;

		HawkSegment3D& operator += (const HawkVector3D& oVec);

		HawkSegment3D  operator - (const HawkVector3D& oVec) const;

		HawkSegment3D& operator -= (const HawkVector3D& oVec);

		Bool operator == (const HawkSegment3D& oSegment) const;

		Bool operator != (const HawkSegment3D& oSegment) const;
	};
}
#endif
