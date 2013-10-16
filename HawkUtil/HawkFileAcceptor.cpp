#include "HawkFileAcceptor.h"
#include "HawkStringUtil.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	void* AcceptorRoutine(void* pArgs)
	{
		HawkFileAcceptor::FileTask* pTask = (HawkFileAcceptor::FileTask*)pArgs;
		if (pTask)
			pTask->Acceptor();

		return 0;
	}

	HawkFileAcceptor::FileTask::FileTask(const AString& sLocalDir)
	{
		m_sLocalDir = sLocalDir;
		m_pThread   = 0;
		m_bFinish   = true;
	}

	HawkFileAcceptor::FileTask::~FileTask()
	{
		m_sPeer.Close();
		HAWK_RELEASE(m_pThread);
	}

	HawkSocket* HawkFileAcceptor::FileTask::GetPeer()
	{
		return &m_sPeer;
	}

	SocketAddr* HawkFileAcceptor::FileTask::GetAddr()
	{
		return &m_sAddr;
	}

	Bool HawkFileAcceptor::FileTask::RunTask()
	{
		if (m_sPeer.IsValid())
		{
			HAWK_RELEASE(m_pThread);			
			m_pThread = new HawkThread(AcceptorRoutine);
			m_bFinish = false;
			m_pThread->Start(this);
		}		
		return true;
	}

	Bool HawkFileAcceptor::FileTask::IsFinish() const
	{
		return m_bFinish;
	}

	Bool HawkFileAcceptor::FileTask::Acceptor()
	{
		if (!m_sPeer.IsValid())
		{
			m_bFinish = true;
			return false;
		}
		
		//5秒超时
		if (!m_sPeer.UpdateEvent(HEVENT_READ, DEFAULT_TIMEOUT))
		{
			m_sPeer.Close();
			return false;
		}

		Char szHeader[DEFAULT_SIZE] = {0};
		Size_t iSize = sizeof(UInt32);
		if (!m_sPeer.Receive(szHeader, iSize) || iSize != sizeof(UInt32))
		{
			m_sPeer.Close();
			return false;
		}

		iSize = (Size_t)(*((UInt32*)szHeader));
		if (!iSize || !m_sPeer.Receive(szHeader+sizeof(UInt32), iSize) || iSize != (Size_t)(*((UInt32*)szHeader)))
		{
			m_sPeer.Close();
			return false;
		}

		HawkFmtPrint("Acceptor, %s", szHeader+sizeof(UInt32));

		UInt32 iFileSize = 0;
		Char szFileName[DEFAULT_SIZE] = {0};
		sscanf(szHeader+sizeof(UInt32), "File: %s  Size: %u", szFileName, &iFileSize);		
		AString sFileName = szFileName;
		HawkStringUtil::Trim<AString>(sFileName);
		if (!iFileSize || !sFileName.size())
		{
			m_sPeer.Close();
			return false;
		}

		HawkOSOperator::MakeSureFileName(m_sLocalDir + sFileName);

		//打开文件准备写入
		HawkDiskFile sDiskFile;
		if (!sDiskFile.Open(m_sLocalDir + sFileName, HawkFile::OPEN_WRITE))
		{
			m_sPeer.Close();
			return false;
		}

		Size_t iRecvSize = 0;
		UChar  szRecvBuf[DEFAULT_SIZE] = {0};
		while (iRecvSize < iFileSize)
		{
			Size_t iRecive = DEFAULT_SIZE;
			if (iFileSize - iRecvSize < DEFAULT_SIZE)
				iRecive = iFileSize - iRecvSize;

			//从网络读取数据
			if (!m_sPeer.Receive(szRecvBuf, iRecive))
			{
				m_sPeer.Close();
				HawkOSOperator::OsDeleteFile(m_sLocalDir + sFileName);
				return false;
			}

			//把数据写入文件
			sDiskFile.Write(szRecvBuf, iRecive, true);
			iRecvSize += iRecive;
		}
		
		//关闭连接
		m_sPeer.Close();

		//关闭文件
		sDiskFile.Close();

		//文件接收数据大小不对, 删除文件
		if (iRecvSize != iFileSize)
		{
			HawkOSOperator::OsDeleteFile(m_sLocalDir + sFileName);
		}

		m_bFinish = true;
		return iRecvSize == iFileSize;
	}

	//////////////////////////////////////////////////////////////////////////

	HawkFileAcceptor::HawkFileAcceptor()
	{
	}


	HawkFileAcceptor::~HawkFileAcceptor()
	{
		FileTaskMap::iterator it = m_mFileTask.begin();
		for (;it != m_mFileTask.end(); it++)
		{
			HAWK_RELEASE(it->second);
		}
		m_mFileTask.clear();
	}

	Bool  HawkFileAcceptor::Init(const AString& sAddr, const AString& sLocalDir)
	{
		m_sAddr     = sAddr;
		m_sLocalDir = sLocalDir;
		HawkStringUtil::Replace<AString>(m_sLocalDir, "\\", "/");
		if (m_sLocalDir.size() && m_sLocalDir.at(m_sLocalDir.size() - 1) != '/')
			m_sLocalDir += "/";

		m_sSocket.Close();
		if (!m_sSocket.InitTcpServer(sAddr))
			return false;

		return true;
	}

	Bool  HawkFileAcceptor::Run()
	{
		if (m_sSocket.IsValid())
		{
			while (true)
			{				
				if (m_sSocket.UpdateEvent(HEVENT_READ, DEFAULT_SLEEP))
				{
					FileTask* pTask = new FileTask(m_sLocalDir);
					if (!m_sSocket.Accept(*pTask->GetPeer(), *pTask->GetAddr()))
					{
						HAWK_RELEASE(pTask);
						continue;
					}

					m_mFileTask[pTask] = pTask;
					pTask->RunTask();
				}
				else
				{
					FileTaskMap::iterator it = m_mFileTask.begin();
					for (;it != m_mFileTask.end();)
					{
						if (!it->second->IsFinish())
						{
							it ++;
							continue;
						}
						
						HAWK_RELEASE(it->second);
						m_mFileTask.erase(it++);
					}
				}
			}
			return true;
		}
		return false;		
	}
}
