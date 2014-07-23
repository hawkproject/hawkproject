#include "HawkDetour.h"
#include "detours.h"

namespace Hawk
{
	HawkDetour::HawkDetour()
	{
	}

	HawkDetour::~HawkDetour()
	{
	}

	Bool  HawkDetour::DetourApi(void** pSysAddr, void* pUsrAddr,Bool bDetour)
	{
		if (bDetour)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			if (DetourAttach(pSysAddr, pUsrAddr) != NO_ERROR)
			{
				assert(false && "DetourAttach Failed");
				return false;
			}

			if (DetourTransactionCommit() != NO_ERROR)
			{
				assert(false && "DetourCommit Failed");
				return false;
			}

			return true;
		}
		else
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			if(DetourDetach(pSysAddr, pUsrAddr) != NO_ERROR)
			{
				assert(false && "DetourAttach Failed");
				return false;
			}

			if (DetourTransactionCommit() != NO_ERROR)
			{
				assert(false && "DetourCommit Failed");
				return false;
			}

			return true;
		}
		return false;
	}
}

/************************************************************************/
/* 使用示例(DetourFileApi)                                              */
/************************************************************************/
/*
#include "DetourFileAPI.h"

/************************************************************************/
/*声明                                                                  */
/************************************************************************/
HANDLE (WINAPI* R_CreateFileW)(LPCWSTR lpFileName, 
	DWORD dwDesiredAccess, 
	DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	DWORD dwCreationDispostion , 
	DWORD dwFlagsAndAttributes, 
	HANDLE hTemplateFile)	=	CreateFileW;

HANDLE (WINAPI* R_CreateFileA)(LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)	= CreateFileA;

DWORD (WINAPI* R_SetFilePointer)(HANDLE hFile,
	LONG lDistanceToMove,
	PLONG lpDistanceToMoveHigh,
	DWORD dwMoveMethod)	=	SetFilePointer;

BOOL (WINAPI* R_SetFilePointerEx)(HANDLE hFile,
	LARGE_INTEGER liDistanceToMove,
	PLARGE_INTEGER lpNewFilePointer,
	DWORD dwMoveMethod)	=	SetFilePointerEx;

BOOL (WINAPI* R_ReadFile)(HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)	=	ReadFile;

BOOL (WINAPI* R_ReadFileEx)(HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)	
	=	ReadFileEx;

BOOL (WINAPI* R_WriteFile)(HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped)	=	WriteFile;

BOOL (WINAPI* R_WriteFileEx)(HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)	
	=	WriteFileEx;

BOOL (WINAPI* R_CloseHandle)(HANDLE hObject)			=	CloseHandle;

/************************************************************************/
/*定向                                                                  */
/************************************************************************/
/*
HANDLE WINAPI  D_CreateFileW(LPCWSTR lpFileName, 
	DWORD dwDesiredAccess, 
	DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	DWORD dwCreationDispostion , 
	DWORD dwFlagsAndAttributes, 
	HANDLE hTemplateFile)
{
	HANDLE hResult = R_CreateFileW(lpFileName,dwDesiredAccess,dwShareMode,
		lpSecurityAttributes,dwCreationDispostion,
		dwFlagsAndAttributes,hTemplateFile);

	return hResult;
}

HANDLE WINAPI  D_CreateFileA(LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	HANDLE hResult = R_CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,
		lpSecurityAttributes,dwCreationDisposition,
		dwFlagsAndAttributes,hTemplateFile);

	return hResult;
}

DWORD WINAPI D_SetFilePointer(HANDLE hFile,
	LONG lDistanceToMove,
	PLONG lpDistanceToMoveHigh,
	DWORD dwMoveMethod)
{
	DWORD  dwResult = R_SetFilePointer(hFile,lDistanceToMove,lpDistanceToMoveHigh,dwMoveMethod);

	return dwResult;
}

BOOL WINAPI D_SetFilePointerEx(HANDLE hFile,
	LARGE_INTEGER liDistanceToMove,
	PLARGE_INTEGER lpNewFilePointer,
	DWORD dwMoveMethod)
{
	BOOL  bResult = R_SetFilePointerEx(hFile,liDistanceToMove,lpNewFilePointer,dwMoveMethod);

	return bResult;
}

BOOL WINAPI D_ReadFile(HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	BOOL bResult = R_ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,lpNumberOfBytesRead,lpOverlapped);

	return bResult;
}

BOOL WINAPI D_ReadFileEx(HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	BOOL bResult = R_ReadFileEx(hFile,lpBuffer,nNumberOfBytesToRead,lpOverlapped,lpCompletionRoutine);

	return bResult;
}

BOOL WINAPI D_WriteFile(HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped)
{
	BOOL bResult = R_WriteFile(hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,lpOverlapped)	;

	return bResult;
}

BOOL WINAPI D_WriteFileEx(HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	BOOL bResult = R_WriteFileEx(hFile,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine)	;

	return bResult;
}

BOOL WINAPI D_CloseHandle(HANDLE hObject)
{
	return R_CloseHandle(hObject);
}

bool DetourFileAPI::Attach()
{
	bool bRet = true;

	bRet &= (DetourAttach(&(PVOID&)R_CreateFileW, D_CreateFileW) == NO_ERROR);

	bRet &= (DetourAttach(&(PVOID&)R_CloseHandle, D_CloseHandle) == NO_ERROR);

	bRet &= (DetourAttach(&(PVOID&)R_SetFilePointer, D_SetFilePointer) == NO_ERROR);
	bRet &= (DetourAttach(&(PVOID&)R_SetFilePointerEx, D_SetFilePointerEx) == NO_ERROR);

	bRet &= (DetourAttach(&(PVOID&)R_ReadFile, D_ReadFile) == NO_ERROR);
	bRet &= (DetourAttach(&(PVOID&)R_ReadFileEx, D_ReadFileEx) == NO_ERROR);

	bRet &= (DetourAttach(&(PVOID&)R_WriteFile, D_WriteFile) == NO_ERROR);
	bRet &= (DetourAttach(&(PVOID&)R_WriteFileEx, D_WriteFileEx) == NO_ERROR);

	return bRet;
}

bool DetourFileAPI::Detach()
{
	bool bRet = true;

	bRet &= (DetourDetach(&(PVOID&)R_CreateFileW, D_CreateFileW) == NO_ERROR);

	bRet &= (DetourDetach(&(PVOID&)R_CloseHandle, D_CloseHandle) == NO_ERROR);

	bRet &= (DetourDetach(&(PVOID&)R_SetFilePointer, D_SetFilePointer) == NO_ERROR);
	bRet &= (DetourDetach(&(PVOID&)R_SetFilePointerEx, D_SetFilePointerEx) == NO_ERROR);

	bRet &= (DetourDetach(&(PVOID&)R_ReadFile, D_ReadFile) == NO_ERROR);
	bRet &= (DetourDetach(&(PVOID&)R_ReadFileEx, D_ReadFileEx) == NO_ERROR);

	bRet &= (DetourDetach(&(PVOID&)R_WriteFile, D_WriteFile) == NO_ERROR);
	bRet &= (DetourDetach(&(PVOID&)R_WriteFileEx, D_WriteFileEx) == NO_ERROR);

	return bRet;
}
*/
