#ifndef HAWK_COUNTER_H
#define HAWK_COUNTER_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ������,Ҳ����������ʱ                                                */
	/************************************************************************/
	class UTIL_API HawkCounter : public HawkRefCounter
	{
	public:
		//���������
		HawkCounter(UInt32 iPeriod = 0);

		//����
		virtual ~HawkCounter();

	public:
		//��������
		DEFINE_PROPERTY(UInt32,	m_iPeriod,  Period);
		//��ǰ����
		DEFINE_PROPERTY(UInt32,	m_iCounter, Counter);
		//��ͣ״̬(Ĭ�Ϸ���ͣ)
		DEFINE_PROPERTY(Bool,	m_bPause,   IsPause);

	public:
		//�����Ƿ�ﵽ��ֵ
		virtual Bool IsFull() const;

		//����(bFull:�Ѽ����������)
		virtual void Reset(bool bFull = false);

		//��ͣ
		virtual void SetPause(bool bPause);
		
		//���Ӽ���
		virtual Bool IncCounter(UInt32 iCounter = 1);
		
		//���ټ���
		virtual void DecCounter(UInt32 iCounter = 1);
	};
}
#endif

