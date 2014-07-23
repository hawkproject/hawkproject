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
		//����
		HawkWin32();

		//����
		~HawkWin32();

	public:
		//װ��Win32ģ��
		static Bool Install();

		//ж��Win32 ģ��
		static Bool Uninstall();

	public:
		//����Coredump�����߳�
		static Bool LaunchCoredump(UChar uKey = 'd');
	};

	//windows�쳣�������������
	extern Bool UTIL_API HawkHandleException(struct _EXCEPTION_POINTERS *pExceptionInfo);
}
#endif
