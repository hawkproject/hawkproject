#include "HawkLogger.h"
#include "HawkOSOperator.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"

namespace Hawk
{	
	HawkLogger::HawkLogger(const AString& sFile, Bool bAppend)
	{
		m_pFile		= 0;		
		m_sFileName = sFile;
		m_bAppend	= bAppend;
		m_pMutex	= new HawkMutex;
		HawkOSOperator::MakeSureFileName(m_sFileName);
	}

	HawkLogger::~HawkLogger()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = 0;
		}

		HAWK_RELEASE(m_pMutex);
	}

	AString HawkLogger::GetLogTimeInfo(LogLevel eLevel) const
	{
		if(m_pFile)
		{
			tm xTM = HawkOSOperator::GetSysClock();

			Char sTmp[128] = {0};
			sprintf(sTmp,"[%04d-%02d-%02d %02d-%02d-%02d](%s) : ",
				xTM.tm_year + 1900,xTM.tm_mon + 1,xTM.tm_mday,xTM.tm_hour,xTM.tm_min,xTM.tm_sec,GetLogLevelDesc(eLevel).c_str());

			return sTmp;
		}
		return "";
	}

	AString HawkLogger::GetLogLevelDesc(LogLevel eLevel) const
	{
		static AString sLvlDesc[] = {"Info","Warn","Error"};
		return sLvlDesc[eLevel];
	}

	void HawkLogger::LogFmtMsg(const Char* pFmt, ...)
	{
		va_list args;
		Char sMsg[LOG_DEFAULT_SIZE + 1] = {0};
		va_start(args, pFmt);
		_vsnprintf(sMsg, LOG_DEFAULT_SIZE, pFmt, args);
		va_end(args);

		LogMsg(sMsg,LEVEL_INFO);
	}

	void  HawkLogger::LogFmtErr(const Char* pFmt, ...)
	{
		va_list args;
		Char sErr[LOG_DEFAULT_SIZE + 1] = {0};
		va_start(args, pFmt);
		_vsnprintf(sErr, LOG_DEFAULT_SIZE, pFmt, args);
		va_end(args);

		LogMsg(sErr,LEVEL_ERROR);
	}

	void  HawkLogger::LogMsg(const AString& sMsg, LogLevel eLevel)
	{
		HawkAutoMutex(lock,m_pMutex);

		if (!m_pFile && m_sFileName.size())
		{
			if (HawkOSOperator::ExistFile(m_sFileName) && m_bAppend)
			{
				Int64 iSize = HawkOSOperator::GetFileSize(m_sFileName);
				m_pFile = fopen(m_sFileName.c_str(), "a+b");
				if(m_pFile && iSize > 0)
				{
					fwrite("\r\n", 1, strlen("\r\n"), m_pFile);
				}
			}
			else
			{
				m_pFile = fopen(m_sFileName.c_str(), "wb");
				if(m_pFile)
				{
					tm xTM = HawkOSOperator::GetSysClock();
					Char sTmp[128] = {0};
					sprintf(sTmp,"====================HawkLog(CreateTime : %04d-%02d-%02d %02d-%02d-%02d)====================\r\n\r\n",
						xTM.tm_year + 1900,xTM.tm_mon + 1,xTM.tm_mday,xTM.tm_hour,xTM.tm_min,xTM.tm_sec);
					fwrite(sTmp, 1, strlen(sTmp), m_pFile);
				}
			}			
		}

		AString sTmp = GetLogTimeInfo(eLevel) + sMsg;
		if(m_pFile)
		{
			fwrite(sTmp.c_str(),1,sTmp.size(),m_pFile);
			fflush(m_pFile);
		}
	}
}
