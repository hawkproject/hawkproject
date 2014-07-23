#ifndef HAWK_RAY3D_H
#define HAWK_RAY3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�ռ�����	                                                        */
	/************************************************************************/
	class GEO_API HawkRay3D
	{
	public:
		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Point,Point);
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Direction,Direction);

	public:
		//Ĭ�Ϲ���
		HawkRay3D();

		//ʹ�������㹹��
		HawkRay3D(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//��������
		HawkRay3D(const HawkRay3D& oRay);

		//�����ߵĵ�����
		void  Set(const HawkVector3D& oPt1,const HawkVector3D& oPt2);

		//����ԭ��ͷ���
		void  SetPointAndDir(const HawkVector3D& oPoint,const HawkVector3D& oDir);

		//�ж����Ƿ���������(�����ߵ�������Ҫ����)
		Bool  IsPointOnRay(const HawkVector3D& oVec) const;

		//���������������
		HawkRay3D  operator + (const HawkVector3D& oVec) const;

		HawkRay3D& operator += (const HawkVector3D& oVec);

		HawkRay3D  operator - (const HawkVector3D& oVec) const;

		HawkRay3D& operator -= (const HawkVector3D& oVec);

		Bool operator ==(const HawkRay3D& oRay) const;

		Bool operator!=(const HawkRay3D& oRay) const;

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
