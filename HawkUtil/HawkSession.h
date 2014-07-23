#ifndef HAWK_SESSION_H
#define HAWK_SESSION_H

#include "HawkSecurity.h"
#include "HawkSocket.h"
#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* ����Ự�����װ                                                     */
	/************************************************************************/
	class UTIL_API HawkSession : public HawkRefCounter
	{
	public:
		//����
		HawkSession();
		
		//����
		virtual ~HawkSession();

	public:
		//��ʼ��
		virtual Bool	Init(const SocketAddr& sAddr, Int32 iTimeout = DEFAULT_TIMEOUT);		

		//���»Ự,��ȡ����IO����(����false��ʾ����Ͽ�)
		virtual Bool    Tick(UInt32 iPeriod);

		//�����뻺�����н�����Э��
		virtual Bool    DecodeProtocol(Protocol*& pProto);

		//��������(��Ҫ�������üӽ������)
		virtual Bool    Reconnect();

		//�رջỰ
		virtual Bool	Close();

	public:
		//���ö���������
		virtual void	SetISecurity(HawkSecurity* pSecurity);

		//����д���������
		virtual void	SetOSecurity(HawkSecurity* pSecurity);

		//����ԭʼ����
		virtual Bool	SendRawData(void* pData, Int32 iSize, Bool bFlush = true);

		//����Э��
		virtual Bool	SendProtocol(Protocol* pProto, Bool bFlush = true);	

		//д�����ͻ�����(֡����ʱ����)
		virtual Bool	FlushOutput();

		//��ȡ����Buffer
		OctetsStream*	GetInputBuf();

		//��ȡ�ỰSocket
		HawkSocket*		GetSocket();

		//��ȡ�Զ˵�ַ
		SocketAddr*		GetAddr();

	protected:
		//�Ƿ���Ч
		virtual Bool    IsValid() const;

	protected:
		//�Ự�׽���
		HawkSocket		m_sSocket;
		//��ַ
		SocketAddr		m_sAddress;	
		//���ܶ���
		HawkSecurity*	m_pISecurity;
		//���ܶ���
		HawkSecurity*	m_pOSecurity;
		//���뻺����
		OctetsStream*	m_pInputBuf;
		//���������
		OctetsStream*	m_pOutputBuf;
		//��ʱ������
		OctetsStream*	m_pCommonBuf;
	};
}
#endif
