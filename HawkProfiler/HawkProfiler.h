#ifndef HAWK_PROFILER_H
#define HAWK_PROFILER_H

#include "HawkUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* 进程性能监控器                                                       */
	/************************************************************************/
	class PRO_API HawkProfiler : public HawkRefCounter
	{
	public:
		//构造
		HawkProfiler(const AString& sAddr = "");

		//析构
		virtual ~HawkProfiler();

	public:
		//开始服务监控
		virtual Bool   Start(const AString& sAddr = "");

		//停止监控
		virtual Bool   Stop();

	public:
		//登记接收协议
		virtual Bool   RegRecvProto(ProtoType iType, ProtoSize iSize);
		
		//登记发送协议
		virtual Bool   RegSendProto(ProtoType iType, ProtoSize iSize);
		
		//登记连接
		virtual Bool   RegConnect(Bool bConn);

		//获取CPU数量
		virtual UInt32 GetProcessorNumber();

		//获取物理内存量
		virtual UInt64 GetTotalMem();

		//获取CPU使用率
		virtual UInt32 GetCpuUsage();

		//获取占用内存大小
		virtual UInt64 GetMemUsage();

	public:
		//执行服务线程循环
		virtual Bool   RunLoop();

	protected:
		//是否允许中
		volatile Bool m_bRunning;
		//服务地址
		AString		  m_sAddr;
		//服务套接字(UDP模式)
		HawkSocket	  m_sSocket;
		//服务线程
		HawkThread*	  m_pThread;
		//CPU数量
		UInt32		  m_iCpuCount;
		//总物理内存
		UInt64		  m_iTotalMem;
		//CPU使用率
		UInt32		  m_iCpuUsage;
		//内存使用率
		UInt64		  m_iMemUsage;
		//有效连接数
		UInt32		  m_iConnect;
		//协议接收个数
		UInt64		  m_iRecvProto;
		//协议接收字节
		UInt64		  m_iRecvSize;
		//协议发送个数
		UInt64		  m_iSendProto;
		//协议发送字节
		UInt64		  m_iSendSize;
		//UDP数据接收Buffer
		OctetsStream* m_pRecvBuf;
	};
}
#endif
