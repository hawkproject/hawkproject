#include "HawkWin32.h"
#include "HawkMemLeakDetect.h"
#include "HawkCrashReport.h"
#include "HawkMiniDump.h"
#include "HawkDetour.h"
#include "HawkThread.h"
#include "HawkException.h"

#pragma warning(disable: 4074)
#pragma init_seg(compiler)
Hawk::HawkMemLeakDetect hawk_MemLeakDetect;
Hawk::HawkCrashReport	hawk_CrashReport;

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////

	DECLARE_HOOK_API(LPTOP_LEVEL_EXCEPTION_FILTER, WINAPI, SetUnhandledExceptionFilter, (LPTOP_LEVEL_EXCEPTION_FILTER lpFilter))
	{
#ifdef _DEBUG
		Char sMsg[64] = {0};
		sprintf(sMsg,"¾Ü¾øÒì³£´¦ÀíÆ÷: %u\r\n",(ULong)lpFilter);
		OutputDebugStringA(sMsg);
#endif
		return 0;
	}

#ifdef _DEBUG
	DECLARE_HOOK_API(_CRT_ALLOC_HOOK, __cdecl, _CrtSetAllocHook, (_CRT_ALLOC_HOOK pHook))
	{
		Char sMsg[64] = {0};
		sprintf(sMsg,"¾Ü¾øÄÚ´æ¹³×Ó: %u\r\n",(ULong)pHook);
		OutputDebugStringA(sMsg);
		return 0;
	}
#endif

	//////////////////////////////////////////////////////////////////////////

	HawkThread* g_DumpThread = 0;
	PVoid CoreDumpRoutine(void* pArg)
	{
#ifdef PLATFORM_WINDOWS
		Char* pPtr = 0; 
		UChar uKey = (UChar)pArg;	
		if(_getch() == uKey)
		{			
			__try
			{
				*pPtr = 0;
			}
			__except(HawkHandleException(GetExceptionInformation())) 
			{
			}
		}
#endif
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////

	HawkWin32::HawkWin32() 
	{ 		
	}

	HawkWin32::~HawkWin32() 
	{ 		
	}

	Bool HawkWin32::Install()
	{
		DETOURS_ATTACH_API(SetUnhandledExceptionFilter);

#ifdef _DEBUG
		DETOURS_ATTACH_API(_CrtSetAllocHook);
#endif

		HawkMiniDump::LoadDbgHelpDll(0);

		return true;
	}

	Bool HawkWin32::Uninstall()
	{
		DETOURS_DETACH_API(SetUnhandledExceptionFilter);

#ifdef _DEBUG
		DETOURS_DETACH_API(_CrtSetAllocHook);
#endif

		HAWK_RELEASE(g_DumpThread);
		return true;
	}

	Bool HawkWin32::LaunchCoredump(UChar uKey)
	{
		if (!g_DumpThread)
		{
			g_DumpThread = new HawkThread(CoreDumpRoutine);
			g_DumpThread->Start((PVoid)uKey);
		}
		return true;
	}
}
