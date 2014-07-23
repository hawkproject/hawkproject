#ifndef HAWK_LOGSERVER_H
#define HAWK_LOGSERVER_H

#include "HawkUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* ��־��������װ, ���ݿ�ģʽ��Ĭ��дlogs��                             */
	/************************************************************************/
	class UTIL_API HawkLogServer : public HawkRefCounter
	{
	public:
		//����
		HawkLogServer();

		//����
		virtual ~HawkLogServer();

	public:
		//��ʼ���ļ���־
		virtual Bool Init(const AString& sSvrAddr, const AString& sLogFile, Int32 iCacheSize = MEGABYTE, Int32 iCacheTime = 60000, Bool bDayTable = false);

		//��ʼ�����ݿ���־
		virtual Bool Init(const AString& sSvrAddr, const HawkDBConn& sConn, Int32 iCacheSize = MEGABYTE, Int32 iCacheTime = 60000, Bool bDayTable = false);

		//�������ļ���ʼ��
		virtual Bool InitCfg(const AString& sCfg);

		//������־������
		virtual Bool Run();

		//�ر���־������
		virtual Bool Stop();

		//������־��ӡ���
		virtual Bool EnableConsole(Bool bEnable);

	protected:
		//������־
		virtual Bool CacheLogMsg(const SysProtocol::Sys_LogMsg& sLog);

		//��鵥�յ����ݿ��
		virtual Bool CheckDayTable();

		//��ȡ�����ַ���, '-'�ָ��
		AString		 GetDayTimeString();

		//��־���
		virtual Bool FlushLogs();

	protected:
		//����״̬
		volatile Bool m_bRunning;
		//��ӡ����
		Bool		  m_bConsole;
		//UDPģʽ��־����
		HawkSocket	  m_sSocket;
		//����ˢ��ʱ��
		Int32		  m_iCacheTime;
		//��־�����С
		Int32		  m_iCacheSize;
		//��־����������зֲ�
		Bool		  m_bDayTable;
		//�ϴβ����־��ʱ��(��)
		UInt32		  m_iTableDay;
		//UDP���ݽ���Buffer
		OctetsStream* m_pRcvBuf;		
		//��־��ʽ��Buffer
		Utf8*		  m_pFmtBuf;
		//��־�ļ�����Buffer
		HawkOctets*   m_pFileCache;
		//��־db����Buffer
		HawkOctets*   m_pDbCache;	
		//��־�洢�ļ�����
		HawkDiskFile* m_pLogFile;
		//��־�洢�ļ�·��
		AString		  m_sLogFile;
		//��־�洢���ݿ�
		HawkDatabase* m_pLogDB;
		//��־�洢���ݿ���Ϣ
		HawkDBConn    m_sDBConn;		
		//��־���ݿ�洢����
		AString		  m_sTable;
	};
}
#endif
