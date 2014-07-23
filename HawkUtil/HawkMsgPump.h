#ifndef HAWK_MSGPUMP_H
#define HAWK_MSGPUMP_H

#include "HawkManagerBase.h"
#include "HawkMsgListener.h"

namespace Hawk
{
	/************************************************************************/
	/* ��Ϣ�ô�����                                                         */
	/************************************************************************/
	class UTIL_API HawkMsgPump : public HawkManagerBase
	{
	public:
		//����
		HawkMsgPump();

		//����
		virtual ~HawkMsgPump();

		//��������������
		HAKW_SINGLETON_DECL(MsgPump);

		//��Ϣ�������б�
		typedef map<HawkMsgListener*, HawkMsgListener*> ListenerMap;

		//��Ϣ�����б�
		typedef map<Int32, ListenerMap>	MsgListenerMap;

	public:
		//ע����Ϣ������
		virtual Bool     AddListener(Int32 iMsg, HawkMsgListener* pListener);		

		//�Ƴ���Ϣ������
		virtual Bool     RemoveListener(Int32 iMsg, HawkMsgListener* pListener);

	public:
		//Ͷ����Ϣ(������)
		virtual Bool	 Post(HawkMsg* pMsg);

		//������Ϣ(����)
		virtual Bool     Send(HawkMsg* pMsg);

		//�ַ�������Ϣ
		virtual Bool     Tick();

	protected:
		//��Ϣ�����б�
		MsgListenerMap	  m_mListener;		
		//��Ϣ����
		vector<HawkMsg*>  m_vPumpMsg;
	};

	#define P_MsgPump  HawkMsgPump::GetInstance()
}
#endif
