#ifndef HAWK_SIGNAL_H
#define HAWK_SIGNAL_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �źŲ�����װ(ע: ��Notify��Wait�ᵼ����Ч,�źŶ�ʧ,��ˮƽ����ģʽ)   */
	/************************************************************************/
	class UTIL_API HawkSignal : public HawkRefCounter
	{
	public:
		//����
		HawkSignal();

		//����
		virtual ~HawkSignal();

	public:
		//iTime��ʾ�ȴ���ʱʱ��,-1��ʾ���޵ȴ�,��λMS
		//�ȴ�Notify���¼��������¼�δ����״̬
		virtual Bool  Wait(Int32 iWaitTime = -1);

		//�¼�֪ͨ(�Ƿ�㲥)
		//�Դ�֮ǰwait���źŲ���Ч
		virtual Bool  Notify(Bool bBroadcast = false);

	protected:
		PVoid   m_pMutex;
		PVoid   m_pCond;
	};
}
#endif
