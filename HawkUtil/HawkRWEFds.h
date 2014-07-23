#ifndef HAWK_RWEFDS_H
#define HAWK_RWEFDS_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ���socket������¼����Ϸ�װ                                         */
	/************************************************************************/
	class UTIL_API HawkRWEfds : public HawkRefCounter
	{
	public:
		//����
		HawkRWEfds();

		//����
		virtual ~HawkRWEfds();

	public:
		//���ü����¼�
		virtual void    EnableEvent(SOCKET hSocket, UInt32 iEvent);		

		//�ȴ��¼�����
		virtual Int32   SelectEvent(Int32 iTimeout = 0);

		//�ж��ض��������Ƿ����¼�
		virtual Bool    IsSetEvent(SOCKET hSocket, UInt32 iEvent);

		//��ȡ�ļ�����������
		virtual Int32   GetFdNum();

		//��������
		virtual void    Reset();

	protected:
		//���¼���������
		fd_set  m_sRfds;
		//д�¼���������
		fd_set	m_sWfds;
		//�����¼���������
		fd_set	m_sEfds;
		//�ļ�����������
		Int32	m_iFdNum;
		//����ļ�������
		SOCKET	m_iMaxFd;
	};
}
#endif
