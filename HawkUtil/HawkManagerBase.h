#ifndef HAWK_MANAGERBASE_H
#define HAWK_MANAGERBASE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 管理器对象的积累定义                                                 */
	/************************************************************************/
	class UTIL_API HawkManagerBase : public HawkRefCounter
	{
	public:
		//构造
		HawkManagerBase();

		//析构
		virtual ~HawkManagerBase();

	public:
		//开启管理器
		virtual Bool  Start();

		//更新管理器
		virtual Bool  Tick(UInt32 iPeriod);

		//判断管理器是否运行
		virtual Bool  IsRunning() const;
		
		//停止管理器运行,尽量在Stop里进行资源释放
		virtual Bool  Stop();

	protected:
		volatile Bool m_bRunning;
	};
}
#endif
