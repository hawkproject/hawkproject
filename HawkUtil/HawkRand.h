#ifndef HAWK_RAND_H
#define HAWK_RAND_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �����������                                                         */
	/************************************************************************/
	class UTIL_API HawkRand
	{
	public:
		//�����������
		static Int32 RandInt();

		//���ɴ����޵��������
		static Int32 RandInt(Int32 iMax);

		//���ɴ������޵��������
		static Int32 RandInt(Int32 iLow, Int32 iHigh);

		//�������������
		static Float RandFloat();

		//���ɴ����޵����������
		static Float RandFloat(Float fMax);

		//���ɴ������޵����������
		static Float RandFloat(Float fLow, Float fHigh);
	};
}
#endif

