#ifndef HAWK_APPGATEWAY_H
#define HAWK_APPGATEWAY_H

#include "HawkGateway.h"

namespace Hawk
{
	class HawkAppFrame;
	/************************************************************************/
	/* �����ʹ����Ƕ���ط�����                                             */
	/************************************************************************/	
	class FRAME_API HawkAppGateway : public HawkGateway
	{
	public:
		//����
		HawkAppGateway(HawkAppFrame* pApp = 0);

		//����
		virtual ~HawkAppGateway();

	public:
		//��ʼ��
		virtual Bool    Init();

		//ֹͣ
		virtual Bool	Stop();

	protected:
		//App����
		HawkAppFrame*	m_pApp;
		//���ط������߳�
		HawkThread*		m_pThread;
	};
}
#endif
