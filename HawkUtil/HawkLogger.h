#ifndef HAWK_LOGGER_H
#define HAWK_LOGGER_H

#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* �̰߳�ȫ��־��,����������־��¼                                      */
	/************************************************************************/
	class UTIL_API HawkLogger : public HawkRefCounter
	{
	protected:
		//����
		HawkLogger(const AString& sFile, Bool bAppend = false);

		//����
		virtual ~HawkLogger();
	
		//��������Ա����
		friend class HawkLoggerManager;

	public:
		//��־�ȼ�
		enum LogLevel
		{
			//��Ϣ
			LEVEL_INFO,
			//����
			LEVEL_WARNING,
			//����
			LEVEL_ERROR
		};
		
	public:
		//��¼��Ϣ
		virtual void    LogMsg(const AString& sMsg, LogLevel eLevel = LEVEL_INFO);

		//��¼��ʽ����Ϣ
		virtual void	LogFmtMsg(const Char* pFmt, ...);

		//��¼��ʽ������
		virtual void	LogFmtErr(const Char* pFmt, ...);

	protected:
		//��¼ʱ����Ϣ
		virtual AString GetLogTimeInfo(LogLevel eLevel) const;

		//��ȡ��־���������
		virtual AString GetLogLevelDesc(LogLevel eLevel) const;

	private:
		//��־����Ӧ���ļ����
		FILE*	        m_pFile;
		//��־�ļ���
		AString			m_sFileName;
		//�Ƿ�Ϊ׷��ģʽ
		Bool			m_bAppend;
		//��־�̰߳�ȫ����
		HawkMutex*		m_pMutex;
	};
}
#endif
