#ifndef HAWK_UTIL_H
#define HAWK_UTIL_H

#if !defined(UTIL_EXPORT)
#	include "HawkAllocator.h"
#	include "HawkAnyType.h"
#	include "HawkApp.h"
#	include "HawkAppObj.h"
#	include "HawkAtomic.h"
#	include "HawkBase64.h"
#	include "HawkBinCfgFile.h"
#	include "HawkByteOrder.h"
#	include "HawkCallback.h"
#	include "HawkConfig.h"
#	include "HawkCounter.h"
#	include "HawkDatabase.h"
#	include "HawkDBManager.h"
#	include "HawkDemutexTable.h"
#	include "HawkDiskFile.h"
#	include "HawkException.h"
#	include "HawkFile.h"
#	include "HawkFileAcceptor.h"
#	include "HawkFileSender.h"
#	include "HawkFolder.h"
#	include "HawkHeapArray.h"
#	include "HawkHMACSHA1Security.h"
#	include "HawkIPAddr.h"
#	include "HawkJson.h"
#	include "HawkKVFile.h"
#	include "HawkLockMap.h"
#	include "HawkLogger.h"
#	include "HawkLoggerManager.h"
#	include "HawkMalloc.h"
#	include "HawkManagerBase.h"
#	include "HawkMarshal.h"
#	include "HawkMarshalData.h"
#	include "HawkMath.h"
#	include "HawkMd5.h"
#	include "HawkMemoryFile.h"
#	include "HawkMsg.h"
#	include "HawkMsgListener.h"
#	include "HawkMsgManager.h"
#	include "HawkMsgPump.h"
#	include "HawkMutex.h"
#	include "HawkMysql.h"
#	include "HawkObjBase.h"
#	include "HawkObjManager.h"
#	include "HawkOctets.h"
#	include "HawkOctetsStream.h"
#	include "HawkOSOperator.h"
#	include "HawkPackage.h"
#	include "HawkParamVector.h"
#	include "HawkProtocol.h"
#	include "HawkProtocolListener.h"
#	include "HawkProtocolManager.h"
#	include "HawkProtocolPump.h"
#	include "HawkRand.h"
#	include "HawkRange.h"
#	include "HawkRC4Security.h"
#	include "HawkRefCounter.h"
#	include "HawkRingBuffer.h"
#	include "HawkRWEFds.h"
#	include "HawkRWLock.h"
#	include "HawkScope.h"
#	include "HawkScriptState.h"
#	include "HawkScriptValue.h"
#	include "HawkSecurity.h"
#	include "HawkSession.h"
#	include "HawkSHA1Security.h"
#	include "HawkSignal.h"
#	include "HawkSize.h"
#	include "HawkSmartPtr.h"
#	include "HawkSocket.h"
#	include "HawkSocketAddr.h"
#	include "HawkSocketPair.h"
#	include "HawkSpinLock.h"
#	include "HawkSqlite.h"
#	include "HawkStdHeader.h"
#	include "HawkStreamCompress.h"
#	include "HawkStringUtil.h"
#	include "HawkSysErrorCode.h"
#	include "HawkSysProtocol.h"
#	include "HawkSysProtocolId.h"
#	include "HawkTask.h"
#	include "HawkThread.h"
#	include "HawkThreadPool.h"
#	include "HawkTimerManager.h"
#	include "HawkUtil.h"
#	include "HawkValueHolder.h"
#	include "HawkWebSocket.h"
#	include "HawkXID.h"
#	include "HawkXmlAttribute.h"
#	include "HawkXmlDocument.h"
#	include "HawkXmlElement.h"
#	include "HawkXmlFile.h"
#	include "HawkXorSecurity.h"
#	include "HawkZipReader.h"
#	include "HawkZlib.h"
#	include "HawkZmq.h"
#	include "HawkZmqManager.h"
	using namespace Hawk;
#else
#	include "HawkStdHeader.h"
#endif

namespace Hawk
{
	/************************************************************************/
	/* 应用底层初始化,更新,停止,释放的统一接口封装                          */
	/************************************************************************/
	class UTIL_API HawkUtil
	{
	public:
		//初始化
		static void Init();

		//周期更新
		static void Tick(UInt32 iPeriod);

		//停止运行
		static void Stop();

		//释放资源
		static void Release();
	};
};
#endif
