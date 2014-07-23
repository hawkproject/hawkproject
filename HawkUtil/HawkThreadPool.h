#ifndef HAWK_THREADPOOL_H
#define HAWK_THREADPOOL_H

#include "HawkThread.h"

namespace Hawk
{
	/************************************************************************/
	/* �̳߳ز�����װ                                                       */
	/************************************************************************/
	class UTIL_API HawkThreadPool : public HawkRefCounter
	{
	public:
		//�̳߳ع���
		HawkThreadPool();

		//�̳߳�����
		virtual ~HawkThreadPool();

		//�߳��б����Ͷ���
		typedef vector<HawkThread*> ThreadList;

	public:
		//��ʼ��(PoolSize��ʾ�߳���, PoolQueue��ʾ�Ƿ�ʹ�ó��������)
		virtual Bool   InitPool(Int32 iPoolSize = 2, Bool bPoolQueue = true);

		//���ִ������(iThread�����ڷ�PoolQueueģʽ��ָ���߳�ִ��)
		virtual Bool   AddTask(HawkTask* pTask, Int32 iThreadIdx = -1, Bool bFirst = false);

		//��ʼִ��
		virtual Bool   Start();

		//��������߳���
		virtual Int32  GetThreadNum();

		//��ȡ�߳�ID
		virtual Int32  GetThreadId(Int32 iIdx);

		//���������߳�
		virtual void   Close(Bool bWaitBreak = false);

		//��ѯ�Ƿ�ʼ����(������)
		virtual Bool   IsRunning() const;

		//��ѯ�Ƿ�ȴ�����ִ�����˳�
		virtual Bool   IsWaitBreak() const;

		//��ȡPush��ȥ��������
		virtual UInt64 GetPushTaskCnt() const;

		//��ȡPop������������
		virtual UInt64 GetPopTaskCnt() const;

		//�������е�һ������(����ֵ��ʾ�Ƿ��������)
		virtual Bool   PopTask(HawkTask** ppTask);

	protected:
		//�߳��б�
		ThreadList		m_vThread;
		//�̳߳ص��߳���Ŀ
		Int32			m_iThreadNum;
		//�Ƿ�ӵ���Լ����������
		Bool			m_bPoolQueue;
		//�̳߳�����״̬
		volatile Bool   m_bRunning;	
		//�̳߳صȴ�����ִ�����˳�
		volatile Bool   m_bWaitBreak;	
		//��ǰʹ�õ��߳�����
		volatile UInt32	m_iTurnIdx;
		//�߳��������
		TaskQueue		m_qTask;
		//������л�����
		HawkSpinLock*	m_pLock;
		//��ӵ���������
		UInt64			m_iPushTaskCnt;
		//ִ�е���������
		UInt64			m_iPopTaskCnt;
	};
}
#endif
