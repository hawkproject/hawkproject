#ifndef HAWK_DETOUR_H
#define HAWK_DETOUR_H

#include "HawkStdHeader.h"

#define DECLARE_HOOK_API(ReturnType,CallType,SysApi,ApiArgs)\
	static ReturnType (CallType *Sys_##SysApi)##ApiArgs = SysApi;\
	static ReturnType CallType Hawk_##SysApi##ApiArgs

#define DETOURS_ATTACH_API(SysApi) HawkDetour::DetourApi(&(PVOID&)Sys_##SysApi, Hawk_##SysApi,true)
#define DETOURS_DETACH_API(SysApi) HawkDetour::DetourApi(&(PVOID&)Sys_##SysApi, Hawk_##SysApi,false)

namespace Hawk
{
	/************************************************************************/
	/* ϵͳ�����滻������װ                                                 */
	/************************************************************************/
	class WIN32_API HawkDetour
	{
	public:
		//����
		HawkDetour();

		//����
		virtual ~HawkDetour();

	public:
		//�滻������ַ
		static Bool DetourApi(void** pSysAddr, void* pUsrAddr, Bool bDetour = true);
	};
}
#endif
