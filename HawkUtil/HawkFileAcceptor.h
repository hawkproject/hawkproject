#ifndef HAWK_FILEACCEPTOR_H
#define HAWK_FILEACCEPTOR_H

#include "HawkDiskFile.h"
#include "HawkSocket.h"
#include "HawkThread.h"

namespace Hawk
{
	/************************************************************************/
	/* 接收Sender投递的文件                                                 */
	/************************************************************************/
	class UTIL_API HawkFileAcceptor : public HawkRefCounter
	{
	public:
		HawkFileAcceptor();

		virtual ~HawkFileAcceptor();

		class FileTask : public HawkRefCounter
		{
		public:
			FileTask(const AString& sLocalDir = "");

			virtual ~FileTask();

		public:
			//获取套接字
			HawkSocket*   GetPeer();
	
			//获取地址
			SocketAddr*   GetAddr();

			//执行任务
			virtual Bool  RunTask();

			//接收文件
			virtual Bool  Acceptor();

			//是否完成
			virtual Bool  IsFinish() const;			

		protected:
			//套接字
			HawkSocket  m_sPeer;
			//地址
			SocketAddr  m_sAddr;
			//线程
			HawkThread* m_pThread;
			//是否完成
			Bool		m_bFinish;
			//本地存储目录
			AString		m_sLocalDir;
		};

		typedef map<FileTask*, FileTask*> FileTaskMap;

	public:
		//初始化接收器
		virtual Bool  Init(const AString& sAddr, const AString& sLocalDir);

		//开启接收服务
		virtual Bool  Run();

	protected:
		//服务套接字
		HawkSocket  m_sSocket;
		//服务器监听地址
		AString		m_sAddr;
		//本地存储地址
		AString		m_sLocalDir;
		//任务列表
		FileTaskMap m_mFileTask;
	};
}
#endif
