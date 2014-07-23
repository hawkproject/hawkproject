#ifndef HAWK_PROTOCOLLISTENER_H
#define HAWK_PROTOCOLLISTENER_H

#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* ��Ϣ������                                                           */
	/************************************************************************/
	class UTIL_API HawkProtocolListener
	{
	public:
		//����
		HawkProtocolListener();

		//����
		virtual ~HawkProtocolListener();

		//��Ϣע���
		typedef map<ProtoType, ProtoType> ProtoTypeMap;

	public:
		//ע����Ϣ����
		virtual Bool   MonitorProto(ProtoType iType);

		//��Ϣ����
		virtual Bool   OnProtocol(HawkProtocol* pProto);

		//�Ƴ���Ϣ����(����Ϊ0��ʾ�Ƴ�����)
		virtual Bool   AbandonProto(ProtoType iType = 0);

	protected:
		//ע�����Ϣӳ��
		ProtoTypeMap  m_mProtoType;
	};	
}
#endif
