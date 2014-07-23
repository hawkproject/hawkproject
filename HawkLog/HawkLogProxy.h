#ifndef HAWK_LOGPROXY_H
#define HAWK_LOGPROXY_H

#include "HawkUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* 日志记录代理端封装(日志编码为Utf8格式)                               */
	/************************************************************************/
	class UTIL_API HawkLogProxy : public HawkRefCounter
	{
	public:
		//构造
		HawkLogProxy();

		//析构
		virtual ~HawkLogProxy();

		//日志类别
		enum LogType
		{
			LT_MSG = 0,
			LT_WARN,
			LT_ERROR,
		};

	public:
		//日志代理初始化
		virtual Bool  Init(const AString& sAddr, UInt32 iSvrId = 0);		

	public:
		//普通日志(Utf8编码)
		virtual Bool  FmtMsg(const Char* pKey,const Char* pFmt, ...);

		//警告日志(Utf8编码)
		virtual Bool  FmtWarn(const Char* pKey,const Char* pFmt, ...);

		//错误日志(Utf8编码)
		virtual Bool  FmtError(const Char* pKey,const Char* pFmt, ...);

		//普通日志(Utf8编码)
		virtual Bool  FmtMsg(UInt32 iUsrId, const Char* pUsrName, const Char* pKey,const Char* pFmt, ...);
		
		//警告日志(Utf8编码)
		virtual Bool  FmtWarn(UInt32 iUsrId, const Char* pUsrName, const Char* pKey,const Char* pFmt, ...);
		
		//错误日志(Utf8编码)
		virtual Bool  FmtError(UInt32 iUsrId, const Char* pUsrName, const Char* pKey,const Char* pFmt, ...);

	protected:
		//日志发送接口(Utf8编码)
		virtual Bool  SendLog(UInt8 iType, const Char* pKey, const Char* pMsg);

		//日志发送接口(Utf8编码)
		virtual Bool  SendLog(UInt32 iUsrId, const Char* pUsrName, UInt8 iType, const Char* pKey, const Char* pMsg);

	public:
		//设置本地打印
		virtual void  EnableConsole(Bool bEnable);

		//设置打印线程信息
		virtual void  SetShowThread(Bool bShow);

	protected:
		//线程安全互斥锁
		HawkMutex*	m_pLock;
		//日志连接套接字
		HawkSocket	m_sSocket;
		//日志记录远程地址
		SocketAddr	m_sAddr;
		//日志代理ID标识
		UInt32		m_iSvrId;
		//是否本地打印
		Bool		m_bConsole;
		//自打印显示线程ID
		Bool		m_bShowThread;
	};
}
#endif
