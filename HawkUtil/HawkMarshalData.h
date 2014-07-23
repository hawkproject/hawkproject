#ifndef HAWK_MARSHALDATA_H
#define HAWK_MARSHALDATA_H

#include "HawkMarshal.h"

namespace Hawk
{
	/************************************************************************/
	/* �����л����ݶ���,��Ҫ����Protocol�е����ݽṹ                        */
	/************************************************************************/
	class UTIL_API HawkMarshalData : public HawkMarshal
	{
	public:
		//�����л������¡����
		virtual HawkMarshalData* Clone() const;

		//�����л�����ֵ����
		virtual HawkMarshalData& operator = (const HawkMarshalData& rhsMD);
	};

	//�����Ͷ���
	typedef HawkMarshalData MarshalData;
}
#endif
