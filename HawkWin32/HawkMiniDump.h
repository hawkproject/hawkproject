#ifndef HAWK_MINIDUMP_H
#define HAWK_MINIDUMP_H

#include "HawkStdHeader.h"
#include "dbghelp.h"

namespace Hawk
{
	/************************************************************************/
	/* MiniDump创建相关操作                                                 */
	/************************************************************************/
	class WIN32_API HawkMiniDump
	{
	public:
		//dump函数原型申明
		typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
			IN HANDLE hProcess,
			IN DWORD ProcessId,
			IN HANDLE hFile,
			IN MINIDUMP_TYPE DumpType,
			IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
			IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
			IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
			);

	public:
		//加载Dbghelp模块
		static HMODULE LoadDbgHelpDll(HMODULE hModule)
		{
			HMODULE hDll = NULL;
			Char szModulePath[MAX_PATH] = {0};
			if (GetModuleFileNameA(hModule, szModulePath, MAX_PATH))
			{
				Char* pSlash = strrchr(szModulePath, '\\');
				if (pSlash)
				{
					strcpy(pSlash+1, "dbghelp.dll");
					hDll = LoadLibraryA(szModulePath);
				}
			}

			if (!hDll)
			{
				hDll = LoadLibraryA("dbghelp.dll");
			}

			return hDll;
		}

		//创建MiniDump文件
		static Bool CreateMiniDump(struct _EXCEPTION_POINTERS *pExceptionInfo)
		{
			DeleteFileA("Core.dmp");

			HANDLE hToken = 0;
			if (!GetImpersonationToken(&hToken))
				return false;

			HMODULE hDbgHelp = LoadDbgHelpDll(0);
			if(!hDbgHelp)
				return false;

			MINIDUMPWRITEDUMP pWriteDumpFun = (MINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
			if(!pWriteDumpFun)
				return false;

			HANDLE hDumpFile = CreateFileA("Core.dmp", GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

			if(hDumpFile == INVALID_HANDLE_VALUE)
				return false;

			_MINIDUMP_EXCEPTION_INFORMATION sExInfo = {0};
			sExInfo.ThreadId		  = GetCurrentThreadId();
			sExInfo.ExceptionPointers = pExceptionInfo;
			sExInfo.ClientPointers	  = TRUE;

			TOKEN_PRIVILEGES sTokenPriv;
			Bool bPrivilegeEnabled = EnablePrivilege(SE_DEBUG_NAME, hToken, &sTokenPriv);

			pWriteDumpFun(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithDataSegs, &sExInfo, 0, 0);

			if (bPrivilegeEnabled)
				RestorePrivilege(hToken, &sTokenPriv);

			CloseHandle(hDumpFile);
			FreeLibrary(hDbgHelp);

			return true;
		}

		//备份MiniDump和日志文件
		static Bool BackupDumpFiles()
		{
			CreateDirectoryA("BugReport",0);

			time_t lTime;
			time(&lTime);
			struct tm xTM = *localtime(&lTime);
			Char sBackup[MAX_PATH] = {0};
			sprintf(sBackup,"BugReport/%04d-%02d-%02d-%02d-%02d-%02d",
				xTM.tm_year + 1900,
				xTM.tm_mon + 1,
				xTM.tm_mday,
				xTM.tm_hour,
				xTM.tm_min,
				xTM.tm_sec);

			CreateDirectoryA(sBackup,0);

			Char szCmd[MAX_PATH] = {0};
			sprintf(szCmd,"xcopy /R/Y/F \"Logs/*.log\" %s",sBackup);
			system(szCmd);
			sprintf(szCmd,"xcopy /R/Y/F \"Core.dmp\" %s",sBackup);
			system(szCmd);

			return true;
		}

		//开启错误报告程序
		static Bool LaunchBugReport()
		{
			STARTUPINFOA		si;
			PROCESS_INFORMATION pi;		
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));

			si.cb			= sizeof(si);
			si.dwFlags		= STARTF_USESHOWWINDOW;
			si.wShowWindow	= SW_SHOW;
			if(!CreateProcessA("BugReport.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				return false;

			return true;
		}

	protected:
		//获取线程或进程的句柄
		static Bool GetImpersonationToken(HANDLE* phToken)
		{
			*phToken = NULL;
			if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, TRUE, phToken))
			{
				if (GetLastError() == ERROR_NO_TOKEN)
				{
					if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, phToken))
						return false;
				}
				else
				{
					return false;
				}
			}

			return true;
		}

		//设置调试权限
		static Bool EnablePrivilege(LPCTSTR sPriv, HANDLE hToken, TOKEN_PRIVILEGES* pTokenPriv)
		{
			BOOL bOk = FALSE;

			TOKEN_PRIVILEGES sToken;
			sToken.PrivilegeCount = 1;
			sToken.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			bOk = LookupPrivilegeValue(0, sPriv, &sToken.Privileges[0].Luid);
			if(bOk)
			{
				DWORD dwSize = sizeof(*pTokenPriv);
				bOk = AdjustTokenPrivileges(hToken, FALSE, &sToken, dwSize, pTokenPriv, &dwSize);
			}

			return (bOk && (ERROR_NOT_ALL_ASSIGNED != GetLastError()));
		}

		//恢复调试权限
		static Bool RestorePrivilege(HANDLE hToken, TOKEN_PRIVILEGES* pTokenPriv)
		{
			BOOL bOk = AdjustTokenPrivileges(hToken, FALSE, pTokenPriv, 0, 0, 0);	
			return (bOk && (ERROR_NOT_ALL_ASSIGNED != GetLastError()));
		}
	};
}
#endif
