#ifndef HAWK_PROTOCOLPUMP_H
#define HAWK_PROTOCOLPUMP_H

#include "HawkManagerBase.h"
#include "HawkProtocolListener.h"

namespace Hawk
{
	/************************************************************************/
	/* ��Ϣ�ô�����                                                         */
	/************************************************************************/
	class UTIL_API HawkProtocolPump : public HawkManagerBase
	{
	public:
		//����
		HawkProtocolPump();

		//����
		virtual ~HawkProtocolPump();

		//��������������
		HAKW_SINGLETON_DECL(ProtocolPump);

		//��Ϣ�������б�
		typedef map<HawkProtocolListener*, HawkProtocolListener*> ListenerMap;

		//��Ϣ�����б�
		typedef map<Int32, ListenerMap> ProtocolListenerMap;

	public:
		//ע����Ϣ������
		virtual Bool     AddListener(Int32 iProtocol, HawkProtocolListener* pListener);

		//�Ƴ���Ϣ������
		virtual Bool     RemoveListener(Int32 iProtocol, HawkProtocolListener* pListener);

	public:
		//Ͷ����Ϣ(������)
		virtual Bool	 Post(HawkProtocol* pProtocol);

		//������Ϣ(����)
		virtual Bool     Send(HawkProtocol* pProtocol);

		//�ַ�������Ϣ
		virtual Bool     Tick();		

	protected:
		//�������м�����
		virtual Bool     RemoveAllListener();

		//����������Ϣ
		virtual Bool     RemoveAllProtocol();

	protected:
		//��Ϣ�����б�
		ProtocolListenerMap	  m_mListener;		
		//��Ϣ����
		vector<HawkProtocol*> m_vProtocol;
	};	

	#define P_ProtocolPump  HawkProtocolPump::GetInstance()
}
#endif
