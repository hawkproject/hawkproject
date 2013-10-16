#ifndef HAWK_WIN32_H
#define HAWK_WIN32_H

#ifndef WIN32_EXPORT
#	include "HawkCrashReport.h"
#	include "HawkDetour.h"
#	include "HawkMemLeakDetect.h"
#	include "HawkMiniDump.h"
#else
#	include "HawkStdHeader.h"
#endif

namespace Hawk
{
	class WIN32_API HawkWin32
	{
	public:
		//构造
		HawkWin32();

		//析构
		~HawkWin32();

	public:
		//装载Win32模块
		static Bool Install();

		//卸载Win32 模块
		static Bool Uninstall();

	public:
		//开启Coredump生成线程
		static Bool LaunchCoredump(UChar uKey = 'd');
	};

	//windows异常处理函数句柄申明
	extern Bool UTIL_API HawkHandleException(struct _EXCEPTION_POINTERS *pExceptionInfo);
}
#endif
