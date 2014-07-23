#ifndef HAWK_LOGGERMANAGER_H
#define HAWK_LOGGERMANAGER_H

#include "HawkManagerBase.h"
#include "HawkLogger.h"

namespace Hawk
{
	/************************************************************************/
	/* 日志管理器封装                                                       */
	/************************************************************************/
	class UTIL_API HawkLoggerManager : public HawkManagerBase
	{
	protected:
		//日志管理器构造
		HawkLoggerManager();

		//日志管理器析构
		virtual ~HawkLoggerManager();

		//单例类声明
		HAKW_SINGLETON_DECL(LoggerManager);
	
	public:
		//日志表类型定义
		typedef map<AString,HawkLogger*> LoggerMap;	

	public:
		//创建一个日志记录器
		HawkLogger*   CreateLogger(const AString& sFile, const AString& sName = "", Bool bAppend = false);	

		//获取一个日志记录器
		HawkLogger*   GetLogger(const AString& sName);

	public:
		//设置日志文件路径(以'/'结尾)
		virtual Bool  SetupLogs(const AString& sDir = "Logs/");

		//Print信息
		virtual void  Print(const AString& sMsg);

		//设置控制台打印输出
		virtual void  EnableConsole(Bool bEnable);

		//设置打印信息记入日志
		virtual void  EnablePrintLog(Bool bEnable = true);

	protected:
		//日志文件路径
		AString		m_sLogDir;
		//日志映射表
		LoggerMap	m_mLoggerMap;
		//是否允许打印输出
		Bool        m_bConsole;
		//控制台输出日志
		HawkLogger*	m_pPrintLog;
	};

	//控制台打印信息
	void  UTIL_API HawkPrint(const AString& sMsg, Bool bNewLine = true);
	void  UTIL_API HawkFmtPrint(const Char* sFmt, ...);

	//系统默认日志记录
	void  UTIL_API HawkLog(const AString& sMsg, HawkLogger::LogLevel eLevel = HawkLogger::LEVEL_INFO, const AString& sName = "Util");
	void  UTIL_API HawkFmtLog(const Char* sFmt,  ...);

	//系统默认错误记录
	void  UTIL_API HawkError(const AString& sMsg, const AString& sName = "Error");
	void  UTIL_API HawkFmtError(const Char* sFmt, ...);
	
	//日志全局管理器对象
	#define P_LoggerManager  HawkLoggerManager::GetInstance()
}
#endif
