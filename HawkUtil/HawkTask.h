#ifndef HAWK_TASK_H
#define HAWK_TASK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �߳��������ÿ�������ʾһ���߳���ִ�е�����                       */
	/************************************************************************/
	class UTIL_API HawkTask : public HawkRefCounter 
	{
	public:
		//����������
		typedef PVoid (*PThreadRoutine)(void* pArgs);

	public:
		//����
		HawkTask(PThreadRoutine pRoutine = 0, PVoid pParam = 0);

		//����
		virtual ~HawkTask();

	public:
		//��ȡ�ص�����
		virtual PVoid  GetParam();

		//ִ���߳�����
		virtual PVoid  Run(void* pArgs = 0);

		//�ر�����,�ͷ���Դ
		virtual Int32  Close();

	protected:
		//����ص����
		PThreadRoutine  m_pRoutine;
		//����ص�����
		PVoid           m_pParam;
	};
	//������ж���
	typedef deque<HawkTask*> TaskQueue;
}
#endif
