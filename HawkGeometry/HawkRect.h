#ifndef HAWK_RECT_H
#define HAWK_RECT_H

#include "HawkGeoDef.h"

namespace Hawk
{
	/************************************************************************/
	/* ���������	    													*/
	/************************************************************************/
	class GEO_API HawkRect
	{
	public:
		DEFINE_PUBLIC_PROPERTY(Float,Top,Top);
		DEFINE_PUBLIC_PROPERTY(Float,Bottom,Bottom);
		DEFINE_PUBLIC_PROPERTY(Float,Left,Left);
		DEFINE_PUBLIC_PROPERTY(Float,Right,Right);
		DEFINE_PUBLIC_PROPERTY(Float,ZOrder,ZOrder);

	public:
		//Ĭ�Ϲ��캯��
		HawkRect();
		
		//�������ҹ���
		HawkRect(Float l,Float t,Float r,Float b);

		//��������
		HawkRect(const HawkRect& oRc);

		//����������
		Bool operator == (const HawkRect& oRc) const;

		Bool operator != (const HawkRect& oRc) const;

		HawkRect& Move(Float fX,Float fY);

		HawkRect& Move(HawkVector2D& oVec);

		Bool  IsValid() const;

		Float GetArea();
		
		Bool  PtInRect(const HawkPoint2D& xPt);

		Bool  PtInRect(const HawkVector2D& oVec);
	};
}
#endif
