#ifndef HAWK_LOGSERVER_H
#define HAWK_LOGSERVER_H

#include "HawkUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* 日志服务器封装, 数据库模式下默认写logs表                             */
	/************************************************************************/
	class UTIL_API HawkLogServer : public HawkRefCounter
	{
	public:
		//构造
		HawkLogServer();

		//析构
		virtual ~HawkLogServer();

	public:
		//初始化文件日志
		virtual Bool Init(const AString& sSvrAddr, const AString& sLogFile, Int32 iCacheSize = MEGABYTE, Int32 iCacheTime = 60000, Bool bDayTable = false);

		//初始化数据库日志
		virtual Bool Init(const AString& sSvrAddr, const HawkDBConn& sConn, Int32 iCacheSize = MEGABYTE, Int32 iCacheTime = 60000, Bool bDayTable = false);

		//从配置文件初始化
		virtual Bool InitCfg(const AString& sCfg);

		//启动日志服务器
		virtual Bool Run();

		//关闭日志服务器
		virtual Bool Stop();

		//设置日志打印标记
		virtual Bool EnableConsole(Bool bEnable);

	protected:
		//缓存日志
		virtual Bool CacheLogMsg(const SysProtocol::Sys_LogMsg& sLog);

		//检查单日的数据库表
		virtual Bool CheckDayTable();

		//获取日期字符串, '-'分割符
		AString		 GetDayTimeString();

		//日志落地
		virtual Bool FlushLogs();

	protected:
		//运行状态
		volatile Bool m_bRunning;
		//打印控制
		Bool		  m_bConsole;
		//UDP模式日志接收
		HawkSocket	  m_sSocket;
		//缓存刷新时间
		Int32		  m_iCacheTime;
		//日志缓存大小
		Int32		  m_iCacheSize;
		//日志按日期天进行分拆
		Bool		  m_bDayTable;
		//上次拆分日志的时间(天)
		UInt32		  m_iTableDay;
		//UDP数据接收Buffer
		OctetsStream* m_pRcvBuf;		
		//日志格式化Buffer
		Utf8*		  m_pFmtBuf;
		//日志文件缓存Buffer
		HawkOctets*   m_pFileCache;
		//日志db缓存Buffer
		HawkOctets*   m_pDbCache;	
		//日志存储文件对象
		HawkDiskFile* m_pLogFile;
		//日志存储文件路径
		AString		  m_sLogFile;
		//日志存储数据库
		HawkDatabase* m_pLogDB;
		//日志存储数据库信息
		HawkDBConn    m_sDBConn;		
		//日志数据库存储表名
		AString		  m_sTable;
	};
}
#endif
