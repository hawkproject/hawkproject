#include "HawkLogServer.h"

#define LOG_TABLE_CREATE_SQL "CREATE TABLE IF NOT EXISTS `%s`(\
	LogId INT UNSIGNED NOT NULL AUTO_INCREMENT,\
	SvrId INT UNSIGNED NOT NULL DEFAULT 0,\
	UsrId INT UNSIGNED NOT NULL DEFAULT 0,\
	UsrName VARCHAR(64) NOT NULL DEFAULT '0',\
	LogType INT UNSIGNED NOT NULL DEFAULT 0,\
	LogTime DATETIME NOT NULL DEFAULT '0000-00-00 00:00:00',\
	LogKey VARCHAR(4096) NOT NULL DEFAULT '0',\
	LogMsg VARCHAR(4096) NOT NULL DEFAULT '0',\
	PRIMARY KEY (`LogId`),\
	KEY `SvrIdIndex`   (`SvrId`),\
	KEY `UsrIdIndex`   (`UsrId`),\
	KEY `UsrNameIndex` (`UsrName`),\
	KEY `LogTypeIndex` (`LogType`),\
	KEY `LogKeyIndex`  (`LogKey`))\
	ENGINE = MyISAM;"

namespace Hawk
{
	HawkLogServer::HawkLogServer()
	{		
		m_pLogDB	 = 0;
		m_pLogFile	 = 0;
		m_pFileCache = 0;
		m_pDbCache   = 0;
		m_iCacheTime = 0;
		m_iCacheSize = 0;
		m_iTableDay  = 0;
		m_bConsole   = true;
		m_bDayTable  = false;		
		m_bRunning   = false;
		m_sTable	 = "logs";
		m_pRcvBuf	 = new OctetsStream(PAGE_SIZE);
		m_pFmtBuf    = new Utf8[PAGE_SIZE];
		memset(m_pFmtBuf, 0, PAGE_SIZE);
	}

	HawkLogServer::~HawkLogServer()
	{
		m_sSocket.Close();

		HAWK_DELETE_ARRAY(m_pFmtBuf);
		HAWK_RELEASE(m_pRcvBuf);
		HAWK_RELEASE(m_pFileCache);
		HAWK_RELEASE(m_pDbCache);
		HAWK_RELEASE(m_pLogFile);
	}

	Bool HawkLogServer::EnableConsole(Bool bEnable)
	{
		m_bConsole = bEnable;
		return m_bConsole;
	}

	Bool HawkLogServer::Init(const AString& sSvrAddr, const AString& sLogFile, Int32 iCacheSize, Int32 iCacheTime, Bool bDayTable)
	{
		m_bDayTable  = bDayTable;		
		m_iCacheTime = iCacheTime;
		m_iCacheSize = iCacheSize;
		HawkAssert(iCacheSize > 0);
		if(!m_sSocket.Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
		{
			HawkPrint("LogServer Init Socket Error.");
			return false;
		}

		m_sSocket.SetNoDelay(true);
		m_sSocket.SetBlocking(false);

		if(!m_sSocket.Bind(SocketAddr(sSvrAddr)))
		{
			HawkPrint("LogServer Bind Socket Error.");
			return false;
		}
		
		if (!m_pFileCache)
			m_pFileCache = new HawkOctets(m_iCacheSize);

		m_sLogFile = sLogFile;
		CheckDayTable();

		return true;
	}

	Bool HawkLogServer::Init(const AString& sSvrAddr, const HawkDBConn& sConn, Int32 iCacheSize, Int32 iCacheTime, Bool bDayTable)
	{
		m_bDayTable  = bDayTable;
		m_iCacheTime = iCacheTime;
		m_iCacheSize = iCacheSize;
		HawkAssert(iCacheSize > 0);
		if(!m_sSocket.Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
		{
			HawkPrint("LogServer Init Socket Error.");
			return false;
		}

		m_sSocket.SetNoDelay(true);
		m_sSocket.SetBlocking(false);

		if(!m_sSocket.Bind(SocketAddr(sSvrAddr)))
		{
			HawkPrint("LogServer Bind Socket Error.");
			return false;
		}

		if (!m_pDbCache)
			m_pDbCache = new HawkOctets(m_iCacheSize);

		m_sDBConn = sConn;
		CheckDayTable();

		return true;
	}

	Bool HawkLogServer::InitCfg(const AString& sCfg)
	{
		HawkKVFile cfgFile;
		if (cfgFile.Open(sCfg, HawkFile::OPEN_READ))
		{
			AString sValue = "";
			//创建UDP套接字
			if(!m_sSocket.Create(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) 
			{
				HawkPrint("LogServer Init Socket Error.");
				return false;
			}

			m_sSocket.SetNoDelay(true);
			m_sSocket.SetBlocking(false);

			//读取UDP绑定地址
			if (!cfgFile.QueryValue("SvrAddr", sValue))
			{
				HawkFmtPrint("LogServer InitCfg Error, SvrAddr: %s", sValue.c_str());
				return false;
			}

			//绑定套接字
			if(!m_sSocket.Bind(SocketAddr(sValue)))
			{
				HawkPrint("LogServer Bind Socket Error.");
				return false;
			}

			//日志缓存时间
			if (cfgFile.QueryValue("CacheTime", sValue))
			{
				m_iCacheTime = HawkStringUtil::StringToInt(sValue);
				HawkFmtPrint("LogServer InitCfg, CacheTime: %s", sValue.c_str());
			}
			else
			{
				HawkFmtPrint("LogServer InitCfg Error, CacheTime: %s", sValue.c_str());
				return false;
			}

			//日志缓存大小
			if (cfgFile.QueryValue("CacheSize", sValue))
			{
				m_iCacheSize = HawkStringUtil::StringToInt(sValue);
				HawkFmtPrint("LogServer InitCfg, CacheSize: %s", sValue.c_str());				
			}
			else
			{
				HawkFmtPrint("LogServer InitCfg Error, CacheSize: %s", sValue.c_str());
				return false;
			}

			//日志按日期存储
			if (cfgFile.QueryValue("DayTable", sValue))
			{
				m_bDayTable = HawkStringUtil::StringToBool(sValue);
				HawkFmtPrint("LogServer InitCfg, DayTable: %s", sValue.c_str());
			}

			//控制台打印
			if (cfgFile.QueryValue("LogConsole", sValue))
			{
				m_bConsole = HawkStringUtil::StringToBool(sValue);
				HawkFmtPrint("LogServer InitCfg, LogConsole: %s", sValue.c_str());
			}

			//日志文件存储
			if (cfgFile.QueryValue("FileEnable", sValue))
			{
				HawkFmtPrint("LogServer InitCfg, FileEnable: %s", sValue.c_str());
				if (HawkStringUtil::StringToBool(sValue) == true)
				{
					cfgFile.QueryValue("LogFile", m_sLogFile);
					HawkFmtPrint("LogServer InitCfg, LogFile: %s", m_sLogFile.c_str());

					if (!m_pFileCache)
						m_pFileCache = new HawkOctets(m_iCacheSize);
				}
			}

			//日志数据库存储
			if (cfgFile.QueryValue("DbEnable", sValue))
			{
				HawkFmtPrint("LogServer InitCfg, DbEnable: %s", sValue.c_str());
				if (HawkStringUtil::StringToBool(sValue) == true)
				{
					//host
					cfgFile.QueryValue("DbHost", sValue);
					m_sDBConn.SetHost(sValue);
					HawkFmtPrint("LogServer InitCfg, DbHost: %s", sValue.c_str());

					//type
					cfgFile.QueryValue("DbType", sValue);
					m_sDBConn.SetType(HawkStringUtil::StringToUInt<AString>(sValue));
					HawkFmtPrint("LogServer InitCfg, DbType: %s", sValue.c_str());

					//port
					cfgFile.QueryValue("DbPort", sValue);
					m_sDBConn.SetPort(HawkStringUtil::StringToInt(sValue));
					HawkFmtPrint("LogServer InitCfg, DbPort: %s", sValue.c_str());

					//user
					cfgFile.QueryValue("DbUser", sValue);
					m_sDBConn.SetUser(sValue);
					HawkFmtPrint("LogServer InitCfg, DbUser: %s", sValue.c_str());

					//pwd
					cfgFile.QueryValue("DbPwd", sValue);
					m_sDBConn.SetPwd(sValue);
					HawkFmtPrint("LogServer InitCfg, DbPwd: %s", sValue.c_str());

					//db
					cfgFile.QueryValue("DbName", sValue);
					m_sDBConn.SetDBName(sValue);
					HawkFmtPrint("LogServer InitCfg, DbName: %s", sValue.c_str());

					if (!m_pDbCache)
						m_pDbCache = new HawkOctets(m_iCacheSize);
				}
			}

			//创建日志存储载体
			CheckDayTable();
			return true;
		}
		return false;
	}

	Bool HawkLogServer::Run()
	{
		if (!m_pLogFile && !m_pLogDB)
			return false;

		if (m_bRunning)
			return false;

		m_bRunning = true;
		UInt32 iFlushTime = HawkOSOperator::GetTickCount();

		while(m_bRunning)
		{
			UInt32 iCurTime = HawkOSOperator::GetTickCount();
			
			//按日存储的数据库模式
			if (m_bDayTable && HawkOSOperator::GetSysClock().tm_yday != m_iTableDay)
			{
				//先输出之前日志数据
				FlushLogs();

				//再次创建
				CheckDayTable();
			}

			if (!m_sSocket.UpdateEvent(HEVENT_READ))
			{
				if (iCurTime - iFlushTime >= (UInt32)m_iCacheTime)
				{
					FlushLogs();
					iFlushTime = iCurTime;
				}
				else
				{
					HawkSleep(DEFAULT_SLEEP);
				}
				continue;
			}

			m_pRcvBuf->Clear();
			Size_t iRecv = (Size_t)m_pRcvBuf->Capacity();

			SocketAddr sAddr;
			if(!m_sSocket.ReceiveFrom(m_pRcvBuf->Begin(), iRecv, sAddr))
				continue;

			m_pRcvBuf->Resize((UInt32)iRecv);
			Protocol* pProto = 0;
			try
			{
				pProto = P_ProtocolManager->Decode(*m_pRcvBuf);
				if (!pProto) continue;
			}
			catch (HawkException& rhsExcep)
			{
#ifdef _DEBUG
				HawkFmtPrint("Exception: %s", rhsExcep.GetMsg().c_str());
#endif
				rhsExcep.Throw();
				continue;
			}

			ProtoType eType = pProto->GetType();
			if(eType == SysProtocol::SYS_LOG_MSG)
			{				
				CacheLogMsg(*(SysProtocol::Sys_LogMsg*)pProto);
			}

			P_ProtocolManager->ReleaseProto(pProto);
		}

		FlushLogs();
		return true;
	}

	AString HawkLogServer::GetDayTimeString()
	{
		struct tm xTM = HawkOSOperator::GetSysClock();
		Char sTime[128] = {0};
		sprintf(sTime,"%04d-%02d-%02d",
			xTM.tm_year + 1900,
			xTM.tm_mon + 1,
			xTM.tm_mday);

		return sTime;
	}

	Bool HawkLogServer::CacheLogMsg(const SysProtocol::Sys_LogMsg& sLog)
	{
		memset(m_pFmtBuf, 0, PAGE_SIZE);
		if (!sLog.m_sKey.size() || !sLog.m_sMsg.size())
			return false;

		if (m_pLogFile)
		{
			//格式化日志信息
			sprintf((Char*)m_pFmtBuf, "SvrId: %d, UsrId: %u, UsrName: %s, LogType: %d, LogTime: %s, LogKey: %s, LogMsg: %s\r\n",
				sLog.m_iSvrId, sLog.m_iUsrId, sLog.m_sUsrName.c_str(), sLog.m_iType, HawkOSOperator::GetTimeString().c_str(), sLog.m_sKey.c_str(), sLog.m_sMsg.c_str());

			UInt32 iSize = (UInt32)strlen((Char*)m_pFmtBuf);
			if (iSize + m_pFileCache->Size() >= m_pFileCache->Capacity() - 2)
			{
				//日志落地
				FlushLogs();
			}

			//屏幕输出
			if (m_bConsole)
				HawkPrint(HawkStringUtil::ToString(m_pFmtBuf));

			//写入缓存
			m_pFileCache->Append(m_pFmtBuf, iSize);
		}
		
		if (m_pLogDB)
		{
			//格式化日志信息
			if(m_pDbCache->Size())
			{
				sprintf((Char*)m_pFmtBuf, ",(%d, %u, '%s', %d, '%s', '%s', '%s')",
					sLog.m_iSvrId, sLog.m_iUsrId, sLog.m_sUsrName.c_str(), sLog.m_iType, HawkOSOperator::GetTimeString().c_str(), sLog.m_sKey.c_str(), sLog.m_sMsg.c_str());				
			}
			else
			{
				sprintf((Char*)m_pFmtBuf, "INSERT INTO `%s`(SvrId, UsrId, UsrName, LogType, LogTime, LogKey, LogMsg) VALUES(%d, %u, '%s', %d, '%s', '%s', '%s')",
					m_sTable.c_str(), sLog.m_iSvrId, sLog.m_iUsrId, sLog.m_sUsrName.c_str(), sLog.m_iType, HawkOSOperator::GetTimeString().c_str(), sLog.m_sKey.c_str(), sLog.m_sMsg.c_str());
			}

			UInt32 iSize = (UInt32)strlen((Char*)m_pFmtBuf);
			if (iSize + m_pDbCache->Size() >= m_pDbCache->Capacity() - 2)
			{
				//日志落地
				FlushLogs();

				//重新Format日志格式
				sprintf((Char*)m_pFmtBuf, "INSERT INTO `%s`(SvrId, UsrId, UsrName, LogType, LogTime, LogKey, LogMsg) VALUES(%d, %u, '%s', %d, '%s', '%s', '%s')",
					m_sTable.c_str(), sLog.m_iSvrId, sLog.m_iUsrId, sLog.m_sUsrName.c_str(), sLog.m_iType, HawkOSOperator::GetTimeString().c_str(), sLog.m_sKey.c_str(), sLog.m_sMsg.c_str());

				iSize = (UInt32)strlen((Char*)m_pFmtBuf);
			}

			//屏幕输出
			if (m_bConsole)
				HawkPrint(HawkStringUtil::ToString(m_pFmtBuf));

			//写入缓存
			m_pDbCache->Append(m_pFmtBuf, iSize);
		}
		return true;
	}

	Bool HawkLogServer::CheckDayTable()
	{
		if (m_sLogFile.size())
		{
			AString sLogFile = m_sLogFile;
			if (m_bDayTable)
				sLogFile = m_sLogFile + "." + GetDayTimeString();

			if (m_pLogFile)
				m_pLogFile->Close();
			else
				m_pLogFile = new HawkDiskFile;

			if (!m_pLogFile->Open(sLogFile, HawkFile::OPEN_APPEND))
			{
				HawkPrint("LogServer Open LogFile Error.");
				return false;
			}		
		}
		
		if (m_sDBConn.IsValid())
		{
			if (!m_pLogDB)
			{
				m_pLogDB = P_DBManager->OpenDatabase("LogDB", m_sDBConn);
				if (!m_pLogDB)
				{
					HawkPrint("LogServer Open LogDB Error.");
					return false;
				}				
			}

			if (m_bDayTable)
				m_sTable = "logs." + GetDayTimeString();

			Char szSql[DEFAULT_SIZE] = {0};
			sprintf(szSql, LOG_TABLE_CREATE_SQL, m_sTable.c_str());
			m_pLogDB->Execute((Utf8*)szSql);
		}

		m_iTableDay = HawkOSOperator::GetSysClock().tm_yday;

		return true;
	}

	Bool HawkLogServer::FlushLogs()
	{
		if (m_pLogFile && m_pFileCache->Size())
		{
			m_pLogFile->Write(m_pFileCache->Begin(), m_pFileCache->Size(), true);
			m_pFileCache->Clear();
		}

		if (m_pLogDB && m_pDbCache->Size())
		{
			Utf8 cEnd[] = {';', 0};
			m_pDbCache->Append(cEnd, 2);

			if(!m_pLogDB->Execute((Utf8*)m_pDbCache->Begin()))
			{
				HawkFmtPrint("FlushLogs Error, ErrId: %d, ErrMsg: %s", m_pLogDB->GetErrCode(), (Char*)m_pLogDB->GetErrMsg().c_str());
			}
			m_pDbCache->Clear();
		}

		return true;
	}

	Bool HawkLogServer::Stop()
	{
		if (m_bRunning)
		{
			m_bRunning = false;
			return true;
		}
		return false;
	}
}
