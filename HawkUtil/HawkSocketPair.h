#ifndef HAWK_SOCKETPAIR_H
#define HAWK_SOCKETPAIR_H

#include "HawkSocket.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* Socket�Բ�����װ,��Ҫ�����̼߳���Ϣ֪ͨ                              */
	/************************************************************************/
	class UTIL_API HawkSocketPair : public HawkRefCounter
	{		
	public:		
		//����
		HawkSocketPair();

		//����
		virtual ~HawkSocketPair();

	public:
		//�����׽��ֶ�
		virtual Bool	Create(Bool bThreadSafe = false);

		//�Ƿ���Ч
		virtual Bool    IsValid() const;

		//������Ϣ, ע: (Send�ѻ����� (Recv����ʱ) ���������
		virtual Bool	Send(const void* pData, Size_t& iSize, Int32 iFlags = 0);

		//������Ϣ
		virtual Bool	Receive(void* pData, Size_t& iSize, Int32 iFlags = 0);

		//����δ���¼�
		virtual Bool    Clear();

		//�ر�
		virtual Bool    Close();

	public:
		//��ȡдFD
		virtual SOCKET	GetWriteFd() const;

		//��ȡ��FD
		virtual SOCKET  GetReadFd() const;

	protected:
		//Socket���
		SOCKET		m_hSocket[2];
		//���Ͷ˵��̰߳�ȫ��
		HawkMutex*  m_pMutex;
		//������
		UInt64		m_iSend;
		//������
		UInt64		m_iRecv;
	};	

	////Э�����ͼ�㶨��
	typedef HawkSocketPair SocketPair;
}
#endif
