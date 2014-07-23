#ifndef HAWK_REFCOUNTER_H
#define HAWK_REFCOUNTER_H

#include "HawkException.h"

namespace Hawk
{
	/************************************************************************/
	/* 引用计数器                                                           */
	/************************************************************************/
	class UTIL_API HawkRefCounter
	{
	public:
		//对象构造
		HawkRefCounter();

		//计数对象析构
		virtual ~HawkRefCounter();

	public:
		//增加引用
		virtual Int32  AddRef();

		//减少引用
		virtual Int32  DecRef();

		//获取引用
		virtual Int32  GetRef() const;

		//释放(引用计数<=0时真正删除)
		virtual void   Release();

	public:
		//对象创建内存开辟操作符重载
		void* operator new(Size_t iSize);

		//对象删除操作符重载
		void  operator delete(void* pData, Size_t iSize);

	protected:
		//引用计数值
		volatile Long  m_iRef;
	};
}
#endif
