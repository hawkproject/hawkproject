#ifndef HAWK_APPGATEPROXY_H
#define HAWK_APPGATEPROXY_H

#include "HawkGateProxy.h"

namespace Hawk
{
	class HawkAppFrame;
	/************************************************************************/
	/* �����ʹ�����ش�����                                                 */
	/************************************************************************/
	class FRAME_API HawkAppGateProxy : public HawkGateProxy
	{
	public:
		//����
		HawkAppGateProxy(HawkAppFrame* pApp = 0);

		//����
		virtual ~HawkAppGateProxy();

	public:
		//��ʼ��
		virtual Bool  Init();

	public:
		//����������
		virtual Bool  OnProxyConnected();

		//�������Ͽ�
		virtual Bool  OnProxyDisconnect();

		//���յ�Э��
		virtual Bool  OnRecvProtocol(const GateMsgHeader& sHeader, HawkProtocol* pProto);

	public:
		//�Ƿ�����״̬
		virtual Bool  IsConnect() const;

	protected:
		//App����
		HawkAppFrame*	m_pApp;
		//����״̬
		Bool			m_bConnect;
	};
}
#endif
