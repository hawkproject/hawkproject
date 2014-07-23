#include "HawkCrashReport.h"
#include "HawkMiniDump.h"

//ȫ��ΨһCoreDump����
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
		//ϵͳֻ���ڴ�һ����
		assert(m_pCrashFilter == 0 && "CrashReport Singleton.");

		//����δ�����쳣׼��
		m_pCrashFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);

		OutputDebugStringA("CrashReport: ����δ֪�쳣����ģ��.\r\n");
	}

	HawkCrashReport::~HawkCrashReport()
	{
		if(m_pCrashFilter)
		{
			SetUnhandledExceptionFilter(m_pCrashFilter);
		}

		OutputDebugStringA("CrashReport: �ر�δ֪�쳣����ģ��.\r\n");
	}
}
