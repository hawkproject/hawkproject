#include "HawkUtil.h"

#ifdef PLATFORM_WINDOWS

Int32 StartMonitor(const AString& sFile)
{
	HawkOSOperator::CreateDir("Logs/");	
	HawkDiskFile monitorLog;
	if (!monitorLog.Open("Logs/HawkMonitor.log", HawkFile::OPEN_WRITE))
	{
		printf("open monitor log error.\r\n");
		return -1;
	}
	
	while(true)
	{
		STARTUPINFOA si;
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(STARTUPINFOA);

		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(pi));

		LPTSTR szCmdline = (LPTSTR)_strdup(sFile.c_str());
		BOOL bRet = CreateProcessA(0,szCmdline,0,0,0,0,0,0,&si,&pi);
		if (!bRet)
		{
			printf("fork child process error: %d", GetLastError());
			break;
		}

		if (pi.dwProcessId > 0) 
		{
			Char sLog[DEFAULT_SIZE] = {0};
			sprintf(sLog, "StartProcess, Pid: %d, Time: %s\r\n", pi.dwProcessId, HawkOSOperator::GetTimeString().c_str());
			monitorLog.Write(sLog, strlen(sLog), true);

			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}	
	return 0;
}

#else

Int32 StartMonitor(const AString& sFile)
{
	HawkOSOperator::CreateDir("Logs/");	
	HawkDiskFile monitorLog;
	if (!monitorLog.Open("Logs/HawkMonitor.log", HawkFile::OPEN_WRITE))
	{
		printf("open monitor log error.\r\n");
		return -1;
	}

	while(true)
	{
		pid_t pid = fork(); 
		if (pid == -1) 
		{
			printf("fork child process error: %d", errno);
			break;
		}

		if (pid == 0) 
		{
			if (execv(sFile.c_str(), 0) < 0)
			{
				printf("execv error: %d", errno);
				break;
			}
			exit(0);
		}

		if (pid > 0) 
		{
			Char sLog[DEFAULT_SIZE] = {0};
			sprintf(sLog, "StartProcess, Pid: %d, Time: %s\r\n", pid, HawkOSOperator::GetTimeString().c_str());
			monitorLog.Write(sLog, strlen(sLog), true);

			int status = 0;
			pid = wait(&status);
		}
	}
	return 0;
}

#endif

int main(int argc, Char* argv[])
{	
	if (argc < 2) 
	{
		printf("usage: procmonitor <exe_path>.\r\n");
		return -1;
	}
	
	return StartMonitor(argv[1]);
}

/************************************************************************
#! /bin/sh
proc_dir  = "/home/HawkProject"
proc_name = "Hawk"

proc_num()
{
	num=`ps -ef | grep $proc_name | grep -v grep | wc -l`
	return $num
}

proc_id()
{
	pid=`ps -ef | grep $proc_name | grep -v grep | awk '{print $2}'`
}

proc_num
number=$?

# 判断进程是否存在
if [$number -eq 0] 
then 
	cd  $proc_dir
	./$proc_name
fi
************************************************************************/
