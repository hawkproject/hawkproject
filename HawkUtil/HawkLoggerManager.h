#ifndef HAWK_LOGGERMANAGER_H
#define HAWK_LOGGERMANAGER_H

#include "HawkManagerBase.h"
#include "HawkLogger.h"

namespace Hawk
{
	/************************************************************************/
	/* ��־��������װ                                                       */
	/************************************************************************/
	class UTIL_API HawkLoggerManager : public HawkManagerBase
	{
	protected:
		//��־����������
		HawkLoggerManager();

		//��־����������
		virtual ~HawkLoggerManager();

		//����������
		HAKW_SINGLETON_DECL(LoggerManager);
	
	public:
		//��־�����Ͷ���
		typedef map<AString,HawkLogger*> LoggerMap;	

	public:
		//����һ����־��¼��
		HawkLogger*   CreateLogger(const AString& sFile, const AString& sName = "", Bool bAppend = false);	

		//��ȡһ����־��¼��
		HawkLogger*   GetLogger(const AString& sName);

	public:
		//������־�ļ�·��(��'/'��β)
		virtual Bool  SetupLogs(const AString& sDir = "Logs/");

		//Print��Ϣ
		virtual void  Print(const AString& sMsg);

		//���ÿ���̨��ӡ���
		virtual void  EnableConsole(Bool bEnable);

		//���ô�ӡ��Ϣ������־
		virtual void  EnablePrintLog(Bool bEnable = true);

	protected:
		//��־�ļ�·��
		AString		m_sLogDir;
		//��־ӳ���
		LoggerMap	m_mLoggerMap;
		//�Ƿ������ӡ���
		Bool        m_bConsole;
		//����̨�����־
		HawkLogger*	m_pPrintLog;
	};

	//����̨��ӡ��Ϣ
	void  UTIL_API HawkPrint(const AString& sMsg, Bool bNewLine = true);
	void  UTIL_API HawkFmtPrint(const Char* sFmt, ...);

	//ϵͳĬ����־��¼
	void  UTIL_API HawkLog(const AString& sMsg, HawkLogger::LogLevel eLevel = HawkLogger::LEVEL_INFO, const AString& sName = "Util");
	void  UTIL_API HawkFmtLog(const Char* sFmt,  ...);

	//ϵͳĬ�ϴ����¼
	void  UTIL_API HawkError(const AString& sMsg, const AString& sName = "Error");
	void  UTIL_API HawkFmtError(const Char* sFmt, ...);
	
	//��־ȫ�ֹ���������
	#define P_LoggerManager  HawkLoggerManager::GetInstance()
}
#endif
