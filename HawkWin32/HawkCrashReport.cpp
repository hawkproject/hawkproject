#include "HawkCrashReport.h"
#include "HawkMiniDump.h"

//全局唯一CoreDump对象
extern Hawk::HawkCrashReport hawk_CrashReport;

namespace Hawk
{
	LPTOP_LEVEL_EXCEPTION_FILTER HawkCrashReport::m_pCrashFilter = 0;

	Bool HawkHandleException(struct _EXCEPTION_POINTERS *pExceptionInfo)
	{
		if(!HawkMiniDump::CreateMiniDump(pExceptionInfo))
			return false;

		if(!HawkMiniDump::BackupDumpFiles())
			return false;

		HawkMiniDump::LaunchBugReport();

		return true;
	}

	LONG WINAPI UnhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
	{
		if( IsDebuggerPresent() )
			return EXCEPTION_CONTINUE_SEARCH;		

		HawkHandleException(pExceptionInfo);
	
		return EXCEPTION_EXECUTE_HANDLER;
	}

	//////////////////////////////////////////////////////////////////////////

	HawkCrashReport::HawkCrashReport()
	{
		//系统只存在此一对象
		assert(m_pCrashFilter == 0 && "CrashReport Singleton.");

		//设置未处理异常准备
		m_pCrashFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);

		OutputDebugStringA("CrashReport: 开启未知异常捕获模块.\r\n");
	}

	HawkCrashReport::~HawkCrashReport()
	{
		if(m_pCrashFilter)
		{
			SetUnhandledExceptionFilter(m_pCrashFilter);
		}

		OutputDebugStringA("CrashReport: 关闭未知异常捕获模块.\r\n");
	}
}
