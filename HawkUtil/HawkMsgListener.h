#ifndef HAWK_MSGLISTENER_H
#define HAWK_MSGLISTENER_H

#include "HawkMsg.h"

namespace Hawk
{
	/************************************************************************/
	/* ��Ϣ������                                                           */
	/************************************************************************/
	class UTIL_API HawkMsgListener
	{
	public:
		//����
		HawkMsgListener();

		//����
		virtual ~HawkMsgListener();

		//��Ϣע���
		typedef map<Int32, Int32> MsgTypeMap;

	public:
		//ע����Ϣ����
		virtual Bool   MonitorMsg(Int32 iMsg);

		//��Ϣ����
		virtual Bool   OnMessage(const HawkMsg& sMsg);

		//�Ƴ���Ϣ����(����Ϊ0��ʾ�Ƴ�����)
		virtual Bool   AbandonMsg(Int32 iMsg = 0);

	protected:
		//ע�����Ϣӳ��
		MsgTypeMap  m_mMsgType;
	};	
}
#endif
