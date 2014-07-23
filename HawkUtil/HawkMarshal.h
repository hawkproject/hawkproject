#ifndef HAWK_MARSHAL_H
#define HAWK_MARSHAL_H

#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* �����������л��ͷ����л������װ                                     */
	/************************************************************************/
	class UTIL_API HawkMarshal : public HawkRefCounter
	{
	public:
		//���ݴ��(����������)
		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS);

		//���ݽ��(����������)
		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS) ;

		//��������
		virtual Bool Clear();
	};
}
#endif
