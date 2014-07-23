#ifndef HAWK_ZMQMANAGER_H
#define HAWK_ZMQMANAGER_H

#include "HawkManagerBase.h"
#include "HawkZmq.h"

namespace Hawk
{
	/************************************************************************/
	/* ZMQ���������                                                        */
	/************************************************************************/
	class UTIL_API HawkZmqManager : public HawkManagerBase
	{
	protected:
		//���������
		HawkZmqManager();

		//����
		virtual ~HawkZmqManager();

		//��������
		HAKW_SINGLETON_DECL(ZmqManager);

		//��������Ͷ���
		typedef map<HawkZmq*, HawkZmq*> ZmqMap;

	public:
		//����������
		virtual Bool	 SetupZmqCtx(Int32 iThreads = 1);

		//��ȡ������
		virtual void*	 GetZmqCtx();

		//����ZMQ
		virtual HawkZmq* CreateZmq(Int32 iType);
		
		//�ر�ZMQ
		virtual Bool	 CloseZmq(HawkZmq*& pZmq);
		
		//����ZMQ����ͨ��,Bothway: ˫����, Timeout: ʱ�䳬ʱ(-1Ϊ���޵ȴ�), Once: ����һ��
		virtual Bool     ProxyZmq(HawkZmq* pFrontend, HawkZmq* pBackend, Bool bBothway, Int32 iTimeout, Bool bOnce);

	protected:
		//�����ľ��
		void*   m_pZmqCtx;
		//������Zmq�б�
		ZmqMap  m_mZmq;
	};

	//���׺궨��
	#define P_ZmqManager  HawkZmqManager::GetInstance()
}
#endif
