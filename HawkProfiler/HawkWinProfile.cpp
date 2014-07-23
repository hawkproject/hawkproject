#include "HawkProfiler.h"

#ifdef PLATFORM_WINDOWS

namespace Hawk
{
	UInt64 file_time_2_utc(const FILETIME* pFTime)
	{
		if (pFTime)
		{
			LARGE_INTEGER li;
			li.LowPart  = pFTime->dwLowDateTime;
			li.HighPart = pFTime->dwHighDateTime;
			return li.QuadPart;
		}
		return 0;
	}

	UInt32 get_processor_number()
	{
		static UInt32 iNumber = 0;
		if (!iNumber)
		{
			SYSTEM_INFO sInfo;
			GetSystemInfo(&sInfo);
			iNumber = (UInt32)sInfo.dwNumberOfProcessors;
		}		
		return iNumber;
	}	

	UInt64 get_phys_mem()
	{
		static UInt64 iPhysMem = 0;
		if (!iPhysMem)
		{
			MEMORYSTATUSEX sStatus;
			memset(&sStatus, 0, sizeof(sStatus));
			sStatus.dwLength = sizeof(sStatus);
			GlobalMemoryStatusEx(&sStatus);
			iPhysMem = sStatus.ullTotalPhys;
		}
		return iPhysMem;
	}

	UInt32 get_cpu_usage()
	{
		static HANDLE hProcess = 0;
		if (hProcess == 0) 
			hProcess = GetCurrentProcess();

		static UInt64 last_time = 0;
		static UInt64 last_system_time = 0;

		FILETIME now_time;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;

		UInt64	 time;
		UInt64	 time_delta;
		UInt64   system_time;
		UInt64   system_time_delta;

		GetSystemTimeAsFileTime(&now_time);

		if (!GetProcessTimes(hProcess, &creation_time, &exit_time,&kernel_time, &user_time))
			return 0;

		system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time));

		time = file_time_2_utc(&now_time);

		if (last_time == 0 || last_system_time == 0)
		{
			last_time = time;
			last_system_time = system_time;		
			return 0;
		}

		time_delta = time - last_time;
		system_time_delta = system_time - last_system_time;	

		if (time_delta == 0)
			return 0;

		last_system_time = system_time;
		last_time = time;

		return (UInt32)((system_time_delta * 100 + time_delta / 2) / time_delta);
	}

	UInt64 get_memory_usage()
	{
		static HANDLE hProcess = 0;
		if (hProcess == 0) 
			hProcess = GetCurrentProcess();

		PROCESS_MEMORY_COUNTERS sInfo;
		if(GetProcessMemoryInfo(hProcess, &sInfo, sizeof(sInfo)))
			return (UInt64)sInfo.WorkingSetSize;

		return 0;
	}
}
#endif
