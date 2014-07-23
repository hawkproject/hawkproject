#ifndef HAWK_CALLBACK_H
#define HAWK_CALLBACK_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 回调对象的基类,处理回调对象(函数)									*/
	/************************************************************************/
	class UTIL_API HawkCallback : public HawkRefCounter
	{
	public:
		//Invoke执行时可用参数
		DEFINE_PROPERTY(PVoid, m_pParams, Params);

	public:
		//回调对象构造
		HawkCallback() : m_pParams(0) 
		{
		};

		//回调对象析构
		virtual ~HawkCallback() 
		{
		};

	public:
		//回调对象触发
		virtual Int32 Invoke(void* pArgs = 0) 
		{
			return 0; 
		}
	};
}
#endif
