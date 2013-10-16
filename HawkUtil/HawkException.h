#ifndef HAWK_EXCEPTION_H
#define HAWK_EXCEPTION_H

#include "HawkStdHeader.h"
#include "HawkMalloc.h"

namespace Hawk
{
	/************************************************************************/
	/* 异常定义类,负责系统异常的处理和记录                                  */
	/************************************************************************/
	class UTIL_API HawkException : public std::exception
	{
	public:
		//属性
		DEFINE_PROPERTY(AString, m_sMsg, Msg);

		struct ShowFileLine 
		{
			ShowFileLine(const AString& sTag, const AString& sFile, const AString& sFunc, Int32 iLine);
		};

	public:
		//构造异常信息
		HawkException(const AString& sMsg,const AString& sFile, const AString& sFunc, Int32 iLine);

		//对象析构
		virtual ~HawkException() throw();

	public:
		//异常抛弃
		virtual Bool Throw() const;
	};

	//抛出异常
	#define T_Exception(msg)	throw HawkException(msg,__FILE__,__FUNCTION__,__LINE__)
	//判断val为false时抛出异常
	#define A_Exception(cond)	if(!(cond)) throw HawkException(#cond,__FILE__,__FUNCTION__,__LINE__)	
	//抛出空指针错误
	#define NullException()		{ Char* pPtr = 0; *pPtr = 0; }
#ifdef _DEBUG
	//值非空断言
	#define HawkAssert(cond)	{ if(!(cond)) HawkException::ShowFileLine sSFL("Assert", __FILE__,__FUNCTION__,__LINE__); assert(cond);}
#else
	//值非空断言
	#define HawkAssert(cond)
#endif
}
#endif
