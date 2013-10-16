#include "HawkUtil.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"
#include "HawkDBManager.h"
#include "HawkProtocolManager.h"
#include "HawkTimerManager.h"
#include "HawkMsgManager.h"
#include "HawkZmqManager.h"
#include "pthread.h"

#ifdef PLATFORM_WINDOWS
#	include "HawkWin32.h"
#	ifdef _DEBUG
#		pragma comment(lib, "HawkWin32_d.lib")
#	else
#		pragma comment(lib, "HawkWin32.lib")
#	endif
#endif

namespace Hawk
{
	void CheckTypeSize()
	{
		HawkPrint("==============================================================");
		HawkFmtPrint("TypeSize:");
		HawkFmtPrint("    Bool:       %u", sizeof(Bool));
		HawkFmtPrint("    Char:       %u", sizeof(Char));
		HawkFmtPrint("    UChar:      %u", sizeof(UChar));
		HawkFmtPrint("    Utf8:       %u", sizeof(Utf8));
		HawkFmtPrint("    Short:      %u", sizeof(Short));
		HawkFmtPrint("    UShort:     %u", sizeof(UShort));
		HawkFmtPrint("    WChar:      %u", sizeof(WChar));
		HawkFmtPrint("    Int8:       %u", sizeof(Int8));
		HawkFmtPrint("    UInt8:      %u", sizeof(UInt8));
		HawkFmtPrint("    Int16:      %u", sizeof(Int16));
		HawkFmtPrint("    UInt16:     %u", sizeof(UInt16));
		HawkFmtPrint("    Int32:      %u", sizeof(Int32));
		HawkFmtPrint("    UInt32:     %u", sizeof(UInt32));
		HawkFmtPrint("    Int64:      %u", sizeof(Int64));
		HawkFmtPrint("    UInt64:     %u", sizeof(UInt64));
		HawkFmtPrint("    Float:      %u", sizeof(Float));
		HawkFmtPrint("    Double:     %u", sizeof(Double));
		HawkFmtPrint("    Size_t:     %u", sizeof(Size_t));
		HawkFmtPrint("    Long:       %u", sizeof(Long));
		HawkFmtPrint("    ULong:      %u", sizeof(ULong));
		HawkFmtPrint("    LongLong:   %u", sizeof(LongLong));
		HawkFmtPrint("    ULongLong:  %u", sizeof(ULongLong));
		HawkFmtPrint("    PVoid:      %u", sizeof(PVoid));
		HawkFmtPrint("    SID:        %u", sizeof(SID));
		HawkFmtPrint("    ProtoType:  %u", sizeof(ProtoType));
		HawkFmtPrint("    ProtoSize:  %u", sizeof(ProtoSize));
		HawkFmtPrint("    ProtoCrc:   %u", sizeof(ProtoCrc));
		HawkPrint("==============================================================");
	}

	void HawkUtil::Init()
	{
#ifdef PLATFORM_WINDOWS
		HawkWin32::Install();

		//使用pthread的线程和进程装载		
		pthread_win32_process_attach_np();
		pthread_win32_thread_attach_np();

		//网络初始化
		WSADATA sData;
		WSAStartup(MAKEWORD(2, 2),&sData);		

		//设置CRT调试模式
		HawkOSOperator::SetCrtDebugMode();
#else
		struct sigaction sAction;  
		sAction.sa_handler = SIG_IGN;  
		sigaction(SIGPIPE, &sAction, 0); 
#endif		

		//初始化管理器
		HawkLoggerManager::InitInstance();
		HawkTimerManager::InitInstance();	
		HawkProtocolManager::InitInstance();		
		HawkMsgManager::InitInstance();
		HawkDBManager::InitInstance();
		HawkZmqManager::InitInstance();

		//建立日志目录
		P_LoggerManager->SetupLogs("Logs/");

		//注册系统协议
		P_ProtocolManager->RegSysProtocol();		

		//运行管理器
		HawkLoggerManager::GetInstance()->Start();
		HawkTimerManager::GetInstance()->Start();	
		HawkProtocolManager::GetInstance()->Start();
		HawkMsgManager::GetInstance()->Start();
		HawkDBManager::GetInstance()->Start();
		HawkZmqManager::GetInstance()->Start();

#ifdef _DEBUG
		CheckTypeSize();
#endif
	}

	void HawkUtil::Tick(UInt32 iPeriod)
	{
		//更新管理器
		HawkZmqManager::GetInstance()->Tick(iPeriod);
		HawkDBManager::GetInstance()->Tick(iPeriod);
		HawkMsgManager::GetInstance()->Tick(iPeriod);
		HawkProtocolManager::GetInstance()->Tick(iPeriod);
		HawkTimerManager::GetInstance()->Tick(iPeriod);
		HawkLoggerManager::GetInstance()->Tick(iPeriod);		
	}

	void HawkUtil::Stop()
	{
		//停止管理器
		HawkZmqManager::GetInstance()->Stop();
		HawkDBManager::GetInstance()->Stop();
		HawkMsgManager::GetInstance()->Stop();
		HawkProtocolManager::GetInstance()->Stop();	
		HawkTimerManager::GetInstance()->Stop();
		HawkLoggerManager::GetInstance()->Stop();		
	}

	void HawkUtil::Release()
	{
		//释放管理器
		HawkZmqManager::ExitInstance();
		HawkDBManager::ExitInstance();
		HawkMsgManager::ExitInstance();
		HawkProtocolManager::ExitInstance();	
		HawkTimerManager::ExitInstance();
		HawkLoggerManager::ExitInstance();
		
#ifdef PLATFORM_WINDOWS
		//网络清理
		WSACleanup();

		//使用pthread的线程和进程卸载
		pthread_win32_thread_detach_np();
		pthread_win32_process_detach_np();

		HawkWin32::Uninstall();
#endif
	}
}
