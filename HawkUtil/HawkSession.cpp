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
		//先关闭
		Close();

		//连接指定地址
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
		//先关闭
		Close();	

		//连接指定地址
		if (!m_sSocket.IsValid() && !m_sSocket.InitTcpClient(m_sAddress))
			return false;

		return true;
	}

	Bool HawkSession::Close()
	{
		//关闭已存在套接字
		if (m_sSocket.IsValid())
			m_sSocket.Close();

		//释放加密组件
		HAWK_RELEASE(m_pISecurity);
		HAWK_RELEASE(m_pOSecurity);
		
		//情况数据缓冲区
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
			//输出加密
			HawkOctetsStream xOS(pData, iSize);
			if(m_pOSecurity)
				m_pOSecurity->Update(xOS);

			//输出缓冲区满, 先写出
			if (m_pOutputBuf->EmptyCap() < xOS.Size())
				FlushOutput();

			//错误检查
			HawkAssert(m_pOutputBuf->EmptyCap() >= xOS.Size());

			//添加到发送缓冲区
			m_pOutputBuf->Insert(m_pOutputBuf->End(), xOS.Begin(), xOS.End());

			//实时发送
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
			//协议打包为数据流
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
				//发送数据
				Size_t iSendSize = (Size_t)m_pOutputBuf->Size();
				if (!m_sSocket.Send(m_pOutputBuf->Begin(), iSendSize))
				{
					m_sSocket.Close();
					return false;
				}

				//移除已发送的数据
				m_pOutputBuf->Erase(m_pOutputBuf->Begin(), (UInt32)iSendSize);
			}
			return true;
		}
		return false;
	}

	Bool HawkSession::Tick(UInt32 iPeriod)
	{
		//非活动会话
		if (!IsValid())
			return false;

		//清理通用缓冲用来接收原始数据
		m_pCommonBuf->Clear();
		Size_t iRecv = m_pCommonBuf->Capacity();

		//接收数据
		if (!m_sSocket.Receive(m_pCommonBuf->Begin(), iRecv))
			return false;

		//解密并拷贝到输入缓冲区
		if (iRecv > 0)
		{
			//设置通用缓冲区
			m_pCommonBuf->Resize((UInt32)iRecv);

			//解密
			if (m_pISecurity)
				m_pISecurity->Update(*m_pCommonBuf);

			//添加到输入缓冲区
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
			//异常退出
			HawkPrint(rhsExcep.GetMsg());

			//释放协议
			P_ProtocolManager->ReleaseProto(pProto);

			return false;
		}

		//移除输入缓冲的前段空白
		m_pInputBuf->RemoveBlank();	
		return true;
	}	
}
