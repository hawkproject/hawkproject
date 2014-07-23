#ifndef HAWK_FILEACCEPTOR_H
#define HAWK_FILEACCEPTOR_H

#include "HawkDiskFile.h"
#include "HawkSocket.h"
#include "HawkThread.h"

namespace Hawk
{
	/************************************************************************/
	/* ����SenderͶ�ݵ��ļ�                                                 */
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
			//��ȡ�׽���
			HawkSocket*   GetPeer();
	
			//��ȡ��ַ
			SocketAddr*   GetAddr();

			//ִ������
			virtual Bool  RunTask();

			//�����ļ�
			virtual Bool  Acceptor();

			//�Ƿ����
			virtual Bool  IsFinish() const;			

		protected:
			//�׽���
			HawkSocket  m_sPeer;
			//��ַ
			SocketAddr  m_sAddr;
			//�߳�
			HawkThread* m_pThread;
			//�Ƿ����
			Bool		m_bFinish;
			//���ش洢Ŀ¼
			AString		m_sLocalDir;
		};

		typedef map<FileTask*, FileTask*> FileTaskMap;

	public:
		//��ʼ��������
		virtual Bool  Init(const AString& sAddr, const AString& sLocalDir);

		//�������շ���
		virtual Bool  Run();

	protected:
		//�����׽���
		HawkSocket  m_sSocket;
		//������������ַ
		AString		m_sAddr;
		//���ش洢��ַ
		AString		m_sLocalDir;
		//�����б�
		FileTaskMap m_mFileTask;
	};
}
#endif
