#ifndef HAWK_RECT3D_H
#define HAWK_RECT3D_H

#include "HawkGeoDef.h"
#include "HawkVector3D.h"

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
	class GEO_API HawkRect3D
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkRect3D();

		//���½�����������߹���
		HawkRect3D(const HawkVector3D& oPt,const HawkVector3D& oEdge1,const HawkVector3D& oEdge2);

		//��������
		HawkRect3D(const HawkRect3D& oRc);

		//����������
		Bool operator == (const HawkRect3D& oRc) const;

		Bool operator != (const HawkRect3D& oRc) const;

		HawkRect3D   operator + (const HawkVector3D& oVec) const;

		HawkRect3D&  operator += (const HawkVector3D& oVec);

		HawkRect3D   operator - (const HawkVector3D& oVec) const;

		HawkRect3D   operator -= (const HawkVector3D& oVec);

		HawkVector3D operator [](Int32 iIdx) const;

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

		//��ȡ���ζ���
		HawkVector3D GetPoint(Int32 iIdx) const;

		//��������������
		Bool IsFrontFacing(const HawkVector3D& vVec3) const;

		//�����������ڲ�
		Bool IsPointInside(const HawkVector3D& oVec3) const;

	public:
		//�����ζ����2����
		HawkVector3D  Point;
		HawkVector3D  Edge[2];
	};
}
#endif
