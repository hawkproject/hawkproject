#ifndef HAWK_PLANE3D_H
#define HAWK_PLANE3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ά�ռ��е�ƽ��,ʹ�ù̶���ͷ��߱�ʾ  N * p + C = 0                 */
	/************************************************************************/
	class GEO_API HawkPlane3D
	{
	public:
		//ƽ�淽��
		//Normal * pt + Distance = 0

		//���Զ���
		DEFINE_PUBLIC_PROPERTY(HawkVector3D,Normal,Normal);
		DEFINE_PUBLIC_PROPERTY(Float,Constant,Constant);
	
		//ƽ��ķ��� 
		enum PlaneSide
		{
			//ƽ����
			PLANESIDE_ON,
			//������
			PLANESIDE_POSITIVE,
			//������
			PLANESIDE_NEGATIVE,
			//Both
			PLANESIDE_BOTH
		};

	public:
		//Ĭ�Ϲ��캯��
		HawkPlane3D ();

		//�������캯��
		HawkPlane3D (const HawkPlane3D& oPlane);

		//�������������ڵ�ƽ��
		HawkPlane3D (const HawkTriangle3D& oTrig);

		//ʹ��ƽ�淽�̵��ĸ���������ƽ��
		HawkPlane3D (Float fA, Float fB, Float fC, Float fD);

		//ʹ��ƽ�淽�̵��ĸ���������ƽ��
		HawkPlane3D (const HawkVector4D& oVec4);

		//ʹ�÷��ߺ͹̶��㹹��ƽ��
		HawkPlane3D (const HawkVector3D& oNormal, const HawkVector3D& oVec3);

		//ʹ����ά�ռ���������㹹��ƽ��
		HawkPlane3D (const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3);

		//�жϲ���������
		Bool operator==(const HawkPlane3D& oPlane) const;

		Bool operator!=(const HawkPlane3D& oPlane) const;

		//������ƽ���λ�ù�ϵ
		PlaneSide GetSide (const HawkVector3D& oVec3) const;

		PlaneSide GetSide (const HawkVector3D& vCenter, const HawkVector3D& vHalfSize) const;
		
		//������Ӻ�ƽ���λ�ù�ϵ
		PlaneSide GetSide (const HawkAxisAlignedBox& oAxisAlignedBox) const;

		//����㵽ƽ��ľ���
		Float GetDistance(const HawkVector3D& oVec3) const;

		//�������ض���ƽ��
		void  Redefine(const HawkVector3D& oPt1, const HawkVector3D& oPt2,const HawkVector3D& oPt3);

		//һ�����һ�����߶���ƽ��
		void  Redefine(const HawkVector3D& oNormal, const HawkVector3D& oPt);

		//ͶӰһ��������ƽ��
		HawkVector3D ProjectVector(const HawkVector3D& oVec3) const;

		//ƽ�浥λ��
		Float  Normalize(void);

		//���
		Float  DotProduct(const HawkVector4D& oVec4);

	public:
		Bool  Intersect(const HawkTriangle3D& oTriangle) const;
		Bool  Intersect(const HawkRect3D& oRect) const;
		Bool  Intersect(const HawkPlane3D& oPlane) const;
		Bool  Intersect(const HawkAxisAlignedBox& oBox) const;
		Bool  Intersect(const HawkSphere& oSphere) const;
		Bool  Intersect(const HawkOBB& oBox) const;
		Bool  Intersect(const HawkCapsule& oCapsule) const;
	};
}
#endif
