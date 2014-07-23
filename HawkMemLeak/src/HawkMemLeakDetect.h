#ifndef HAWK_MEMLEAKDETECTOR_H
#define HAWK_MEMLEAKDETECTOR_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/*	程序内存泄露检测组件												*/
	/*	在使用的程序主函数所在的cpp中申明HawkMemLeakDetect对象就可以		*/
	/************************************************************************/
	class HawkMemLeakDetect
	{
#ifdef _DEBUG
	public:
		//构造
		HawkMemLeakDetect();

		//析构
		~HawkMemLeakDetect();

		//内存分配钩子类型定义
		typedef _CRT_ALLOC_HOOK CrtHook;

	public:
		//内存构造函数定义
		static Int32 AllocHook(Int32 iType, void* pData, Size_t iSize, Int32 iUse, Long iRequest, const UChar* szFile, Int32 iLine);

	protected:
		//构建符号路径
		Char*  BuildSymbolPath();

		//获取追溯栈
		void   GetStackTrace(void* pCallStack);	

		//报告泄露信息
		void   ReportLeaks();

	protected:
		//Free函数Hook处理
		void   HookFree(const void* pData);

		//Malloc函数Hook处理
		void   HookMalloc(Int32 iRequest);

		//Realloc函数Hook处理
		void   HookRealloc(const void* pData, Int32 iRequest);

	protected:
		//报告信息
		void   Report(const Char* pFormat, ...);

	protected:
		//分配信息表
		void*   m_pMallocMap;		   
		//进程句柄
		HANDLE	m_hProcess;
		//日志文件句柄
		FILE*	m_pLogFile;		
		//内存分配钩子
		CrtHook m_pOldHook; 
#endif
	};	
}
#endif
