#include "HawkFileSender.h"
#include "HawkScope.h"
#include "HawkSocket.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	HawkFileSender::HawkFileSender()
	{
	}

	HawkFileSender::~HawkFileSender()
	{
	}

	Bool  HawkFileSender::PostFile(const AString& sAcceptor, const AString& sFileName, const AString& sLocalFile)
	{
		HawkDiskFile sDiskFile;
		if (sAcceptor.size() && sFileName.size() && sLocalFile.size() && sDiskFile.Open(sLocalFile))
		{
			Size_t iSize = (Size_t)sDiskFile.GetFileSize();
			
			//连接文件接收器
			HawkSocket sSocket;
			if (!sSocket.InitTcpClient(sAcceptor))
				return false;

			//构造数据头
			Char szHeader[DEFAULT_SIZE] = {0};
			sprintf(szHeader+sizeof(UInt32), "File: %s  Size: %u", sFileName.c_str(), (UInt32)iSize);
			Size_t iTagSize  = strlen(szHeader+sizeof(UInt32));
			Size_t iHeadSize = sizeof(UInt32) + iTagSize;
			*((UInt32*)szHeader) = (UInt32)iTagSize;

			//发送数据头
			if (!sSocket.Send((void*)szHeader, iHeadSize))
			{
				sSocket.Close();
				return false;
			}

			//发送数据
			Size_t iPostSize = 0;
			UChar  szSendBuf[DEFAULT_SIZE] = {0};
			while(iPostSize < iSize)
			{
				Int64 iRead  = sDiskFile.Read(szSendBuf, DEFAULT_SIZE);
				Size_t iPost = (Size_t)iRead;
				if (iRead < 0 || !sSocket.Send(szSendBuf, iPost))
				{
					sSocket.Close();
					return false;
				}

				iPostSize += iPost;
			}
			sSocket.Close();
			return true;
		}

		return false;
	}

	Bool  HawkFileSender::PostData(const AString& sAcceptor, const AString& sFileName, const Char* pData, Size_t iSize)
	{
		if (sAcceptor.size() && sFileName.size() && pData && iSize)
		{
			//连接文件接收器
			HawkSocket sSocket;
			if (!sSocket.InitTcpClient(sAcceptor))
				return false;

			//构造数据头
			Char szHeader[DEFAULT_SIZE] = {0};
			sprintf(szHeader+sizeof(UInt32), "File: %s  Size: %u", sFileName.c_str(), (UInt32)iSize);
			Size_t iTagSize  = strlen(szHeader+sizeof(UInt32));
			Size_t iHeadSize = sizeof(UInt32) + iTagSize;
			*((UInt32*)szHeader) = (UInt32)iTagSize;
			
			//发送数据头
			if (!sSocket.Send((void*)szHeader, iHeadSize))
			{
				sSocket.Close();
				return false;
			}

			//发送数据
			Size_t iPostSize = 0;
			while(iPostSize < iSize)
			{
				Size_t iPost = iSize - iPostSize;
				if (!sSocket.Send((void*)(pData + iPostSize), iPost))
				{
					sSocket.Close();
					return false;
				}

				iPostSize += iPost;
			}
			sSocket.Close();
			return true;
		}
		
		return false;
	}
}
