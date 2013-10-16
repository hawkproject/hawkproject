#ifndef HAWK_RWEFDS_H
#define HAWK_RWEFDS_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 针对socket句柄的事件集合封装                                         */
	/************************************************************************/
	class UTIL_API HawkRWEfds : public HawkRefCounter
	{
	public:
		//构造
		HawkRWEfds();

		//析构
		virtual ~HawkRWEfds();

	public:
		//设置监听事件
		virtual void    EnableEvent(SOCKET hSocket, UInt32 iEvent);		

		//等待事件触发
		virtual Int32   SelectEvent(Int32 iTimeout = 0);

		//判断特定描述符是否有事件
		virtual Bool    IsSetEvent(SOCKET hSocket, UInt32 iEvent);

		//获取文件描述符个数
		virtual Int32   GetFdNum();

		//集合重置
		virtual void    Reset();

	protected:
		//读事件描述符集
		fd_set  m_sRfds;
		//写事件描述符集
		fd_set	m_sWfds;
		//错误事件描述符集
		fd_set	m_sEfds;
		//文件描述符个数
		Int32	m_iFdNum;
		//最大文件描述符
		SOCKET	m_iMaxFd;
	};
}
#endif
