#include "HawkLoggerManager.h"
#include "HawkStringUtil.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	//单例类实现
	HAKW_SINGLETON_IMPL(LoggerManager);

	HawkLoggerManager::HawkLoggerManager()
	{
		m_bConsole  = true;
		m_pPrintLog = 0;
		m_sLogDir   = "Logs/";
		m_mLoggerMap.clear();		
	}

	HawkLoggerManager::~HawkLoggerManager()
	{		
		LoggerMap::iterator it = m_mLoggerMap.begin();
		while (it!=m_mLoggerMap.end())
		{
			HawkLogger* pLogger = it->second;
			HAWK_RELEASE(pLogger);
			it++;
		}
		m_mLoggerMap.clear();
	}

	Bool HawkLoggerManager::SetupLogs(const AString& sDir)
	{
		m_sLogDir = sDir;
		HawkStringUtil::Replace<AString>(m_sLogDir, "\\","/");
		if (m_sLogDir.size() && m_sLogDir.at(m_sLogDir.size()-1) != '/')
			m_sLogDir += "/";

		//创建系统默认Log
		CreateLogger("HawkUtil.log",  "Util");
		CreateLogger("HawkError.log", "Error");
		CreateLogger("HawkPrint.log", "Print");

		//取出控制台日志
		m_pPrintLog = GetLogger("Print");

		return true;
	}

	HawkLogger* HawkLoggerManager::CreateLogger(const AString& sFile, const AString& sName, Bool bAppend)
	{
		HawkLogger* pLogger = GetLogger(sName);
		if(!pLogger)
		{
			AString sLog = m_sLogDir + sFile;
			HawkOSOperator::MakeSureFileName(sLog);
			pLogger = new HawkLogger(sLog, bAppend);
			m_mLoggerMap[sName] = pLogger;
		}
		return pLogger;
	}
	
	HawkLogger* HawkLoggerManager::GetLogger(const AString& sName)
	{
		LoggerMap::iterator it = m_mLoggerMap.find(sName);
		if(it != m_mLoggerMap.end())
			return it->second;
		
		return 0;
	}

	void  HawkLoggerManager::EnableConsole(Bool bEnable)
	{
		m_bConsole = bEnable;
	}

	void  HawkLoggerManager::EnablePrintLog(Bool bEnable)
	{
		if (!bEnable)
			m_pPrintLog = 0;
		else
			m_pPrintLog = GetLogger("Print");
	}

	void  HawkLoggerManager::Print(const AString& sMsg)
	{
		if (sMsg.size())
		{
			if (m_bConsole)
			{
				printf("%s", sMsg.c_str());
			}
			
			if (m_pPrintLog)
			{
				m_pPrintLog->LogMsg(sMsg);
			}

#if defined(PLATFORM_WINDOWS) && defined(_DEBUG)
			OutputDebugStringA(sMsg.c_str());
#endif			
		}		
	}

	//////////////////////////////////////////////////////////////////////////
	void  HawkPrint(const AString& sMsg, Bool bNewLine)
	{
		if (bNewLine)
			P_LoggerManager->Print(sMsg + "\r\n");	
		else
			P_LoggerManager->Print(sMsg);	
	}

	void HawkFmtPrint(const Char* sFmt, ...)
	{
		va_list args;
		Char sMsg[LOG_DEFAULT_SIZE + 1] = {0};
		va_start(args, sFmt);
		_vsnprintf(sMsg, LOG_DEFAULT_SIZE, sFmt, args);
		va_end(args);

		HawkPrint(sMsg);
	}

	void  HawkLog(const AString& sMsg, HawkLogger::LogLevel eLevel, const AString& sName)
	{
		HawkLogger* pLogger = P_LoggerManager->GetLogger(sName);
		if (pLogger)
		{
			pLogger->LogMsg(sMsg + "\r\n", eLevel);
		}

#ifdef _DEBUG
		HawkPrint(sMsg, true);
#endif
	}

	void  HawkFmtLog(const Char* sFmt,  ...)
	{
		va_list args;
		Char sMsg[LOG_DEFAULT_SIZE + 1] = {0};
		va_start(args, sFmt);
		_vsnprintf(sMsg, LOG_DEFAULT_SIZE, sFmt, args);
		va_end(args);

		HawkLog(sMsg);
	}

	void  HawkError(const AString& sMsg, const AString& sName)
	{
		HawkLogger* pLogger = P_LoggerManager->GetLogger(sName);
		if (pLogger)
		{
			pLogger->LogMsg(sMsg + "\r\n", HawkLogger::LEVEL_ERROR);
		}

#ifdef _DEBUG
		HawkPrint(sMsg, true);
#endif
	}

	void  HawkFmtError(const Char* sFmt,  ...)
	{
		va_list args;
		Char sMsg[LOG_DEFAULT_SIZE + 1] = {0};
		va_start(args, sFmt);
		_vsnprintf(sMsg, LOG_DEFAULT_SIZE, sFmt, args);
		va_end(args);

		HawkError(sMsg);
	}
}
