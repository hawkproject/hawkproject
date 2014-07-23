#include "HawkSession.h"
#include "HawkProtocolManager.h"
#include "HawkLoggerManager.h"

namespace Hawk
{	
	HawkSession::HawkSession() : m_pISecurity(0), m_pOSecurity(0), m_pInputBuf(0)
	{
		m_pInputBuf  = new OctetsStream(RCVBUF_SIZE);		
		m_pOutputBuf = new OctetsStream(SNDBUF_SIZE);
		m_pCommonBuf = new OctetsStream(PAGE_SIZE);
	}

	HawkSession::~HawkSession()
	{
		if (m_sSocket.IsValid())
			m_sSocket.Close();

		HAWK_RELEASE(m_pISecurity);
		HAWK_RELEASE(m_pOSecurity);
		HAWK_RELEASE(m_pInputBuf);
		HAWK_RELEASE(m_pOutputBuf);
		HAWK_RELEASE(m_pCommonBuf);		
	}

	Bool HawkSession::Init(const SocketAddr& sAddr, Int32 iTimeout)
	{
		//�ȹر�
		Close();

		//����ָ����ַ
		if (!m_sSocket.IsValid() && !m_sSocket.InitTcpClient(sAddr, true, iTimeout))
		{
#ifdef _DEBUG
			HawkFmtLog("InitTcpClient Failed, Addr: %s", sAddr.ToString().c_str());
#endif
			return false;
		}
		m_sAddress = sAddr;

		return true;
	}

	Bool HawkSession::Reconnect()
	{
		//�ȹر�
		Close();	

		//����ָ����ַ
		if (!m_sSocket.IsValid() && !m_sSocket.InitTcpClient(m_sAddress))
			return false;

		return true;
	}

	Bool HawkSession::Close()
	{
		//�ر��Ѵ����׽���
		if (m_sSocket.IsValid())
			m_sSocket.Close();

		//�ͷż������
		HAWK_RELEASE(m_pISecurity);
		HAWK_RELEASE(m_pOSecurity);
		
		//������ݻ�����
		if (m_pInputBuf)
			m_pInputBuf->Clear();

		if (m_pOutputBuf)
			m_pOutputBuf->Clear();

		if (m_pCommonBuf)
			m_pCommonBuf->Clear();		

		return true;
	}	

	Bool HawkSession::IsValid() const
	{
		return m_sSocket.IsValid();
	}

	OctetsStream* HawkSession::GetInputBuf()
	{
		return m_pInputBuf;
	}

	HawkSocket* HawkSession::GetSocket()
	{
		return &m_sSocket;
	}

	SocketAddr*	HawkSession::GetAddr()
	{
		return &m_sAddress;
	}

	void HawkSession::SetISecurity(HawkSecurity* pSecurity)
	{
		HAWK_RELEASE(m_pISecurity);
		if (pSecurity)
		{
			m_pISecurity = pSecurity;
			m_pISecurity->AddRef();
		}
	}

	void HawkSession::SetOSecurity(HawkSecurity* pSecurity)
	{
		HAWK_RELEASE(m_pOSecurity);
		if (pSecurity)
		{
			m_pOSecurity = pSecurity;
			m_pOSecurity->AddRef();
		}
	}

	Bool HawkSession::SendRawData(void* pData, Int32 iSize, Bool bFlush)
	{
		if (IsValid())
		{
			//�������
			HawkOctetsStream xOS(pData, iSize);
			if(m_pOSecurity)
				m_pOSecurity->Update(xOS);

			//�����������, ��д��
			if (m_pOutputBuf->EmptyCap() < xOS.Size())
				FlushOutput();

			//������
			HawkAssert(m_pOutputBuf->EmptyCap() >= xOS.Size());

			//��ӵ����ͻ�����
			m_pOutputBuf->Insert(m_pOutputBuf->End(), xOS.Begin(), xOS.End());

			//ʵʱ����
			if (bFlush)
				FlushOutput();
		
			return true;
		}
		return false;
	}

	Bool HawkSession::SendProtocol(Protocol* pProto, Bool bFlush)
	{
		if (IsValid())
		{
			//Э����Ϊ������
			HawkOctetsStream xOS;
			if(!pProto->Encode(xOS))
				return false;

			return SendRawData(xOS.Begin(), xOS.Size(), bFlush);
		}
		return false;
	}

	Bool HawkSession::FlushOutput()
	{
		if (IsValid())
		{
			if (m_pOutputBuf && m_pOutputBuf->Size())
			{
				//��������
				Size_t iSendSize = (Size_t)m_pOutputBuf->Size();
				if (!m_sSocket.Send(m_pOutputBuf->Begin(), iSendSize))
				{
					m_sSocket.Close();
					return false;
				}

				//�Ƴ��ѷ��͵�����
				m_pOutputBuf->Erase(m_pOutputBuf->Begin(), (UInt32)iSendSize);
			}
			return true;
		}
		return false;
	}

	Bool HawkSession::Tick(UInt32 iPeriod)
	{
		//�ǻ�Ự
		if (!IsValid())
			return false;

		//����ͨ�û�����������ԭʼ����
		m_pCommonBuf->Clear();
		Size_t iRecv = m_pCommonBuf->Capacity();

		//��������
		if (!m_sSocket.Receive(m_pCommonBuf->Begin(), iRecv))
			return false;

		//���ܲ����������뻺����
		if (iRecv > 0)
		{
			//����ͨ�û�����
			m_pCommonBuf->Resize((UInt32)iRecv);

			//����
			if (m_pISecurity)
				m_pISecurity->Update(*m_pCommonBuf);

			//��ӵ����뻺����
			m_pInputBuf->Insert(m_pInputBuf->End(), m_pCommonBuf->Begin(), m_pCommonBuf->Size());	
		}

		return true;
	}

	Bool HawkSession::DecodeProtocol(Protocol*& pProto)
	{
		try
		{
			pProto = P_ProtocolManager->Decode(*m_pInputBuf);
		}
		catch (HawkException& rhsExcep)
		{
			//�쳣�˳�
			HawkPrint(rhsExcep.GetMsg());

			//�ͷ�Э��
			P_ProtocolManager->ReleaseProto(pProto);
			pProto = 0;

			return false;
		}

		//�Ƴ����뻺���ǰ�οհ�
		m_pInputBuf->RemoveBlank();	
		return true;
	}	
}
