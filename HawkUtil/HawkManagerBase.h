#ifndef HAWK_MANAGERBASE_H
#define HAWK_MANAGERBASE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ����������Ļ��۶���                                                 */
	/************************************************************************/
	class UTIL_API HawkManagerBase : public HawkRefCounter
	{
	public:
		//����
		HawkManagerBase();

		//����
		virtual ~HawkManagerBase();

	public:
		//����������
		virtual Bool  Start();

		//���¹�����
		virtual Bool  Tick(UInt32 iPeriod);

		//�жϹ������Ƿ�����
		virtual Bool  IsRunning() const;
		
		//ֹͣ����������,������Stop�������Դ�ͷ�
		virtual Bool  Stop();

	protected:
		volatile Bool m_bRunning;
	};
}
#endif
