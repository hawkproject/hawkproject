#ifndef HAWK_GW_CONTROLER_H
#define HAWK_GW_CONTROLER_H

#include "HawkGateDef.h"

namespace Hawk
{	
	/************************************************************************/
	/* ���ط��������������													*/
	/************************************************************************/
	class HawkGateway;
	class GATE_API HawkGwControler : public HawkRefCounter
	{
	public:
		//����
		HawkGwControler(HawkGateway* pGateway = 0);

		//����
		virtual ~HawkGwControler();

	public:
		//��ʼ��������
		virtual Bool  Init(const AString& sAddr);

		//���¿�����״̬
		virtual Bool  UpdateEvent();

		//ֹͣ
		virtual Bool  Stop();

	protected:
		//Э�鴦��
		virtual Bool  OnCommand(const GateCommand* pCmd, const OctetsStream* pIdentify);

	protected:
		//���������ض���
		HawkGateway*	m_pGateway;
		//����ͨ�Ŷ���
		HawkZmq*		m_pControler;
		//��ʶ������
		OctetsStream*	m_pIdentify;
		//ͨ�û�����
		OctetsStream*	m_pCommand;
	};
}
#endif
