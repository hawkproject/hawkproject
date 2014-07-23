#include "HawkMemLeakDetect.h"

#ifdef _DEBUG

#include <imagehlp.h>
#include "mld/mldutil.h"

#define _CRTBLD     
#	include "mld/dbgint.h"
#ifdef _MT
#	include "mld/mtdll.h"
#endif
#undef _CRTBLD

#if defined(_M_IX86)
#	define SIZEOFPTR			4
#	define AXREG				eax
#	define BPREG				ebp
#	define X86X64ARCHITECTURE	IMAGE_FILE_MACHINE_I386
#elif defined(_M_X64)
#	define SIZEOFPTR			8
#	define AXREG				rax
#	define BPREG				rbp
#	define X86X64ARCHITECTURE	IMAGE_FILE_MACHINE_AMD64
#endif

#define SYMBOLBUFFERSIZE		(sizeof(PIMAGEHLP_SYMBOL) + MAX_PATH)
#define MAX_STACK_TRACE			CALLSTACKCHUNKSIZE

DWORD GetProgramCounter()
{
	DWORD dwCounter = 0;
	{
		__asm mov AXREG, [BPREG + SIZEOFPTR] 
		__asm mov [dwCounter], AXREG 
	}
	return dwCounter;
}

//È«¾ÖÎ¨Ò»ÄÚ´æÐ¹Â¶¼ì²â¶ÔÏó
__declspec(dllexport) Hawk::HawkMemLeakDetect hawk_MemLeakDetect;

namespace Hawk
{
	HawkMemLeakDetect::HawkMemLeakDetect() : m_pMallocMap(0),m_pOldHook(0),m_hProcess(0),m_pLogFile(0)
	{
		m_pMallocMap = new BlockMap;
		m_pOldHook	 = _CrtSetAllocHook(AllocHook);
		m_hProcess	 = GetCurrentProcess();		

		time_t lTime;
		time(&lTime);
		tm xTM = *localtime(&lTime);

		Char sHeader[128] = {0};
		sprintf(sHeader,"====================HawkLog(CreateTime : %04d-%02d-%02d %02d-%02d-%02d)====================\r\n\r\nMemLeakDetect Installed.\r\n",
			xTM.tm_year + 1900,
			xTM.tm_mon  + 1,
			xTM.tm_mday,
			xTM.tm_hour,
			xTM.tm_min,
			xTM.tm_sec);

		Report(sHeader);

		OutputDebugStringA("MemLeakDetect: ¿ªÆôÄÚ´æÐ¹Â¶¼ì²âÄ£¿é.\r\n");
	}

	HawkMemLeakDetect::~HawkMemLeakDetect ()
	{
		_CRT_ALLOC_HOOK pPrevHook = _CrtSetAllocHook(m_pOldHook);
		if (pPrevHook != AllocHook) 
		{
			_CrtSetAllocHook(pPrevHook);
		}
		ReportLeaks();				

		BlockMap* pMallocMap = (BlockMap*)m_pMallocMap;
		HAWK_DELETE(pMallocMap);

		if (m_pLogFile)
			fclose(m_pLogFile);

		OutputDebugStringA("MemLeakDetect: ¹Ø±ÕÄÚ´æÐ¹Â¶¼ì²âÄ£¿é.\r\n");
	}

	Int32 HawkMemLeakDetect::AllocHook(Int32 iType, void* pData, Size_t iSize, Int32 iUse, Long iRequest, const UChar* szFile, Int32 iLine)
	{
		static Bool bInallocHook = false;

		if (bInallocHook || (iUse == _CRT_BLOCK)) 
		{
			if (hawk_MemLeakDetect.m_pOldHook)
				return hawk_MemLeakDetect.m_pOldHook(iType, pData, iSize, iUse, iRequest, szFile, iLine);

			return 0;
		}

		bInallocHook = true;

		switch (iType)
		{
		case _HOOK_ALLOC:
			hawk_MemLeakDetect.HookMalloc(iRequest);
			break;

		case _HOOK_FREE:
			hawk_MemLeakDetect.HookFree(pData);
			break;

		case _HOOK_REALLOC:
			hawk_MemLeakDetect.HookRealloc(pData, iRequest);
			break;

		default:
			break;
		}

		Int32 iRet = 0;
		if (hawk_MemLeakDetect.m_pOldHook)
			iRet = hawk_MemLeakDetect.m_pOldHook(iType, pData, iSize, iUse, iRequest, szFile, iLine);

		bInallocHook = false;
		return iRet;
	}

	void HawkMemLeakDetect::Report(const Char* pFormat, ...)
	{
		Char sMsg[512] = {0};

		va_list args;
		va_start(args, pFormat);
		_vsnprintf(sMsg, 512, pFormat, args);
		va_end(args);

		if(!m_pLogFile)
		{
			_mkdir("Logs");
			m_pLogFile = fopen("Logs/HawkMemLeak.log","wb");
		}

		if(m_pLogFile)
		{
			fwrite(sMsg,1,strlen(sMsg),m_pLogFile);
			fflush(m_pLogFile);
		}

		OutputDebugStringA(sMsg);
	}

	Char* HawkMemLeakDetect::BuildSymbolPath()
	{
		Char*	pEnv	= 0;
		UInt32  iIndex	= 0;
		UInt32  iLength = 0;
		HMODULE hModule = 0;		
		UInt32  iPos	= 0;
		Char*	pPath	= new Char[MAX_PATH];
		memset(pPath,0,MAX_PATH);

		hModule = GetModuleHandle(NULL);
		GetModuleFileNameA(hModule, pPath, MAX_PATH);
		PathRemoveFileSpecA(pPath);

		strapp(&pPath, ";.\\");

		pEnv = getenv("SYSTEMROOT");
		if (pEnv)
		{
			strapp(&pPath, ";");
			strapp(&pPath, pEnv);
			strapp(&pPath, ";");
			strapp(&pPath, pEnv);
			strapp(&pPath, "\\system32");
		}

		pEnv = getenv("_NT_SYMBOL_PATH");
		if (pEnv) 
		{
			strapp(&pPath, ";");
			strapp(&pPath, pEnv);
		}

		pEnv = getenv("_NT_ALT_SYMBOL_PATH");
		if (pEnv) 
		{
			strapp(&pPath, ";");
			strapp(&pPath, pEnv);
		}

		iPos    = 0;
		iLength = strlen(pPath);
		while (iPos < iLength)
		{
			if (pPath[iPos] == '\"')
			{
				for (iIndex = iPos; iIndex < iLength; iIndex++)
				{
					pPath[iIndex] = pPath[iIndex + 1];
				}
			}
			iPos++;
		}
		return pPath;
	}

	void HawkMemLeakDetect::GetStackTrace(void* pCallStack)
	{
		CallStack* pStack = (CallStack*)pCallStack;

		STACKFRAME sFrame;		
		CONTEXT    sContext;
		BOOL       bResult = FALSE;

		DWORD      dwArchitecture	= 0;
		DWORD      dwFramePointer	= 0;
		DWORD      dwProgramCounter = 0;

		dwArchitecture	 = X86X64ARCHITECTURE;
		dwProgramCounter = GetProgramCounter();
		{
			__asm mov [dwFramePointer], BPREG
		}		

		memset(&sContext, 0, sizeof(sContext));
		memset(&sFrame, 0, sizeof(sFrame));

		sFrame.AddrPC.Offset    = dwProgramCounter;
		sFrame.AddrPC.Mode      = AddrModeFlat;
		sFrame.AddrFrame.Offset = dwFramePointer;
		sFrame.AddrFrame.Mode   = AddrModeFlat;

		for(DWORD index = 0; index<MAX_STACK_TRACE; index++) 
		{
			bResult = StackWalk(dwArchitecture,m_hProcess,GetCurrentThread(),&sFrame,
				&sContext, 0,SymFunctionTableAccess,SymGetModuleBase,0);

			if(!bResult || sFrame.AddrFrame.Offset == 0) 
				break;

			pStack->push_back(sFrame.AddrPC.Offset);
		}
	}

	void HawkMemLeakDetect::HookFree(const void* pData)
	{
		BlockMap* pMallocMap = (BlockMap*)m_pMallocMap;
		Int32 iRequest = pHdr(pData)->lRequest;
		pMallocMap->erase(iRequest);
	}

	void HawkMemLeakDetect::HookMalloc (Int32 iRequest)
	{
		BlockMap* pMallocMap = (BlockMap*)m_pMallocMap;
		CallStack* pStack = pMallocMap->insert(iRequest);
		GetStackTrace(pStack);
	}

	void HawkMemLeakDetect::HookRealloc(const void* pData, Int32 iRequest)
	{
		HookFree(pData);
		HookMalloc(iRequest);
	}	

	void HawkMemLeakDetect::ReportLeaks ()
	{
		CallStack*	pStack			= 0;
		DWORD		dwDisplacement	= 0;
		BOOL		bFoundLine		= FALSE;
		UInt32		iFrame			= 0;
		Char*		pFunctionName	= 0;
		UInt32		iLeaksFound		= 0;
		UInt32		iLeaksBytes		= 0;
		Char*		pHeap			= 0;
		_CrtMemBlockHeader*	pHeader = 0;
		IMAGEHLP_LINE	 sSourceInfo;
		PIMAGEHLP_SYMBOL pFunctionInfo = 0;
		UChar			 sSymbolBuffer[SYMBOLBUFFERSIZE] = {0};

		pFunctionInfo = (PIMAGEHLP_SYMBOL)sSymbolBuffer;
		memset(pFunctionInfo, 0x0, SYMBOLBUFFERSIZE);

		pFunctionInfo->SizeOfStruct  = SYMBOLBUFFERSIZE;
		pFunctionInfo->MaxNameLength = SYMBOLBUFFERSIZE - sizeof(IMAGEHLP_SYMBOL);

		memset(&sSourceInfo, 0x0, sizeof(IMAGEHLP_LINE));
		sSourceInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD symOptions = SymGetOptions();
		symOptions |= SYMOPT_LOAD_LINES; 
		symOptions |= SYMOPT_DEFERRED_LOADS;
		symOptions |= SYMOPT_UNDNAME;
		SymSetOptions( symOptions );

		Char* szSymbolPath = BuildSymbolPath();
		SymInitialize(m_hProcess, szSymbolPath, TRUE);
		HAWK_DELETE_ARRAY(szSymbolPath);

		Bool bHeaderReport = false;

#ifdef _MT
		_mlock(_HEAP_LOCK);
#endif

		pHeap	= new Char;
		pHeader = pHdr(pHeap)->pBlockHeaderNext;
		HAWK_DELETE(pHeap);

		BlockMap* pMallocMap = (BlockMap*)m_pMallocMap;

		while (pHeader) 
		{
			if (_BLOCK_TYPE(pHeader->nBlockUse) == _CRT_BLOCK)
			{
				pHeader = pHeader->pBlockHeaderNext;
				continue;
			}
			pStack = pMallocMap->find(pHeader->lRequest);

			Char   pStackDesc[64][512] = {0};
			Bool   bReport = true;

			if (pStack) 
			{
				Int32 iLevel  = 0;
				bReport = true;
				memset(pStackDesc,0,64*512);

				for (iFrame = 0; iFrame < pStack->size(); iFrame++)
				{
					if ((bFoundLine = SymGetLineFromAddr(m_hProcess, (*pStack)[iFrame], &dwDisplacement, &sSourceInfo)) == TRUE)
					{
						if (strstr(sSourceInfo.FileName, "memleakdetect") ||
							strstr(sSourceInfo.FileName, "hawkmalloc")	  ||
							strstr(sSourceInfo.FileName, "crt\\src"))
						{
							continue;
						}
					}

					if (SymGetSymFromAddr(m_hProcess, (*pStack)[iFrame], &dwDisplacement, pFunctionInfo)) 
					{
						pFunctionName = pFunctionInfo->Name;
					}
					else 
					{
						pFunctionName = "(Function Unknown)";
					}

					if (bFoundLine)
					{					
						sprintf(pStackDesc[iLevel++],"    %s (%d): %s\r\n", sSourceInfo.FileName, sSourceInfo.LineNumber, pFunctionName);
					}
					else 
					{
						sprintf(pStackDesc[iLevel++],"    "ADDRESSFORMAT" (File And Line Unknown): %s\r\n", (*pStack)[iFrame],pFunctionName);
					}				
				}

				if(bReport)
				{
					if (!bHeaderReport)
					{
						Report("WARNING: MemLeakDetect Memory Leaks: \r\n");
						bHeaderReport = true;
					}

					iLeaksFound++;
					Report("---------- Block %ld At "ADDRESSFORMAT": %u Bytes ----------\r\n", pHeader->lRequest, pbData(pHeader), pHeader->nDataSize);
					iLeaksBytes += (UInt32)pHeader->nDataSize;
					Report("  Call Stack:\r\n");

					for(Int32 i=0;i<iLevel;i++)
					{
						Report(pStackDesc[i]);
					}

					Report("\r\n");
				}            
			}
			pHeader = pHeader->pBlockHeaderNext;
		}

#ifdef _MT
		_munlock(_HEAP_LOCK);
#endif 

		if (!iLeaksFound)
		{
			Report("MemLeakDetect Exit: No Memory Leak.\r\n");
		}
		else 
		{
			Report("MemLeakDetect Exit: %u Memory Leak(Total %u Bytes).\r\n", iLeaksFound,iLeaksBytes);
		}

		SymCleanup(m_hProcess);
	}
}
#endif
