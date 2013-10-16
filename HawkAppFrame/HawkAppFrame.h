#ifndef HAWK_APPFRAME_H
#define HAWK_APPFRAME_H

#include "HawkUtil.h"
#include "HawkLogProxy.h"
#include "HawkAppGateway.h"
#include "HawkAppGateProxy.h"

namespace Hawk
{
	/************************************************************************/
	/* 服务器基础框架									                    */
	/************************************************************************/
	class FRAME_API HawkAppFrame : public HawkApp
	{
	public:
		//构造
		HawkAppFrame(const XID& sXid = XID());

		//析构
		virtual ~HawkAppFrame();

		//线程使用数据库对象
		typedef map<UInt32, HawkMysql*>   DBHdlMap;
		
		//获取会话的地址属性
		typedef HawkLockMap<SID, AString> SidAddrMap;

		//友员访问
		friend class HawkAppGateway;
		friend class HawkAppGateProxy;

		//App初始化结构体
		struct AppCfg
		{
			//应用ID, 服务器ID, 线ID
			UInt32		AppId;
			//是否开启控制台打印
			Bool		Console;
			//逻辑线程数, 网络IO线程数
			UInt32		Threads;			
			//日志服务器地址
			AString		LogAddr;			
			//mysql数据库连接地址
			HawkDBConn	DBConn;

			//网关代理器配置
			struct  
			{
				//网关代理器地址, 网关: tcp://
				AString		Address;
				//连接数限制
				UInt32		ConnLimit;
			}Gateproxy;
			

			//内嵌网关模式参数集合
			struct 
			{
				//内嵌网关服务器地址(ip:port)
				AString		Address;
				//网关线程数
				UInt32		Threads;
				//网关模式
				Int32		Mode;
				//平台
				Int32		Platform;
				//网关加密模式
				Bool		Security;
				//网关会话超时时间
				Int32		Timeout;
				//网关性能监视器
				AString		Profiler;
			}Gateway;

			AppCfg()
			{
				AppId	= 0;
				Threads = 0;
				Console = false;

				Gateproxy.Address	= "";
				Gateproxy.ConnLimit = 0;

				Gateway.Address		= "";
				Gateway.Threads		= 0;
				Gateway.Mode		= 0;
				Gateway.Platform	= 0;
				Gateway.Security	= false;
				Gateway.Timeout		= 0;
				Gateway.Profiler	= "";
			}
		};

	public:
		//初始化
		virtual Bool		Init(const AppCfg& sCfg);

		//运行
		virtual Bool		Run();

		//停止
		virtual Bool		Stop();

	public:
		//获取应用ID
		virtual UInt32		GetAppId() const;

		//获取当前日期
		virtual UInt32		GetSysTime() const;
		
		//获取更新时间
		virtual UInt32		GetTickTime() const;

		//获取帧间间隔
		virtual UInt32      GetElapseTime() const;

		//获取会话的IP地址
		virtual AString		GetSidAddr(SID iSid);

		//退出循环
		virtual Bool		BreakLoop();

		//获取当前线程安全的数据库连接
		HawkMysql*			GetSafeDbHdl();

		//获取应用配置
		const AppCfg*		GetAppCfg() const;

	public:
		//发送协议
		virtual Bool		SendProtocol(SID iSid, Protocol* pProto);
		
		//关闭会话
		virtual Bool		CloseSession(SID iSid);
		
		//广播消息
		virtual Bool		BroadcastMsg(ObjMan* pObjMan, HawkMsg* pMsg);
		
		//广播协议
		virtual Bool		BroadcastProto(Protocol* pProto, const vector<SID>& vSid = vector<SID>());	

	public:
		//日志模块接口(消息)
		virtual Bool		FormatMsg(const Char* pKey, const Char* pFmt, ...);

		//日志模块接口(警告)
		virtual Bool		FormatWarn(const Char* pKey, const Char* pFmt, ...);

		//日志模块接口(错误)
		virtual Bool		FormatError(const Char* pKey, const Char* pFmt, ...);

		//日志模块接口(消息)
		virtual Bool		FormatMsg(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

		//日志模块接口(警告)
		virtual Bool		FormatWarn(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

		//日志模块接口(错误)
		virtual Bool		FormatError(UInt32 iUsrId, const Char* pUsrName, const Char* pKey, const Char* pFmt, ...);

	protected:
		//每帧更新
		virtual Bool		OnTick(UInt32 iPeriod);

		//网关连接成功(网关模式回调)
		virtual Bool		OnGateProxyConnected();

		//会话开启(网关模式回调)
		virtual Bool		OnSessionStart(SID iSid, const AString& sAddr);

		//会话协议通知(网关模式回调)
		virtual Bool		OnSessionProtocol(SID iSid, Protocol* pProto);

		//会话结束通知(网关模式回调)
		virtual Bool		OnSessionClose(SID iSid, const AString& sAddr);
		
		//网关连接关闭(网关模式回调)
		virtual Bool		OnGateProxyDisconnect();

	protected:
		//有新连接(非网关模式, 返回True即允许连接，否则直接关闭)
		virtual Bool		OnAcceptConnect(const HawkSocket& sSocket, const SocketAddr& sAddr);

	protected:
		//应用层创建对象(应用层必须实现)
		virtual HawkAppObj*	AppCreateObj(const XID& sXid);

	protected:
		//应用配置
		AppCfg				m_sAppCfg;
		//应用ID
		UInt32				m_iAppId;
		//当前系统时间秒
		UInt32				m_iSysTime;
		//当前系统时间毫秒
		UInt32				m_iTickTime;
		//逻辑帧时间
		UInt32				m_iElapseTime;
		//会话地址表
		SidAddrMap			m_mSidAddr;
		//数据库连接信息
		HawkDBConn			m_sDbConn;
		//数据库句柄映射
		DBHdlMap			m_mDbHdl;
		//通知退出标记
		volatile Bool		m_bBreakLoop;

	protected:
		//日志对象
		HawkLogProxy*		m_pLogProxy;
		//内嵌网关服务器
		HawkAppGateway*		m_pGateway;
		//网关对象
		HawkAppGateProxy*	m_pGateProxy;
	};
}
#endif
