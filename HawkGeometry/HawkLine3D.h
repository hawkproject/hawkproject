#ifndef HAWK_LINE3D_H
#define HAWK_LINE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��άά�ռ�ֱ������                                                   */
	/************************************************************************/
	class GEO_API HawkLine3D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Direction,Direction);

	public:
		//Ĭ�Ϲ���
		HawkLine3D();

		//ʹ�������㹹��
		HawkLine3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//��������
		HawkLine3D(const HawkLine3D& oLine);

		//�����ߵĵ�����
		void  Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//�ж����Ƿ���������(�����ߵ�������Ҫ����)
		Bool  IsPointOnLine(const HawkVector3D& oVec) const;
		
		//�����߶������oVec�������ĵ�
		HawkVector3D GetClosestPoint(const HawkVector3D& oVec) const;
	
		//�㵽ֱ�ߵľ���
		Float GetDistance(const HawkVector3D& oVec) const;

		//���������������
		HawkLine3D  operator + (const HawkVector3D& oVec) const;

		HawkLine3D& operator += (const HawkVector3D& oVec);

		HawkLine3D  operator - (const HawkVector3D& oVec) const ;

		HawkLine3D& operator -= (const HawkVector3D& oVec);

		Bool operator == (const HawkLine3D& oLine) const;

		Bool operator != (const HawkLine3D& oLine) const;

	public:
		//ֱ�ߺ��������ཻ
		Bool Intersect(const HawkTriangle3D& oTriangle,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺ;����ཻ
		Bool Intersect(const HawkRect3D& oRect,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺ�ƽ���ཻ
		Bool Intersect(const HawkPlane3D& oPlane,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺ�AABB���ཻ
		Bool Intersect(const HawkAxisAlignedBox& oAABB,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺ����ཻ
		Bool Intersect(const HawkSphere& oSphere,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺ�OBB�ཻ
		Bool Intersect(const HawkOBB& oBox,Vec3IntrResult* pResult = 0) const;

		//ֱ�ߺͽ������ཻ
		Bool Intersect(const HawkCapsule& oCapsule,Vec3IntrResult* pResult = 0) const;
	};
}
#endif
