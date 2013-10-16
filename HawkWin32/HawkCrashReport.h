#ifndef HAWK_CRASHREPORT_H
#define HAWK_CRASHREPORT_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/*	程序调用HawkMiniDump模块自动生成dump文件							*/
	/*	在使用的程序主函数所在的cpp中申明HawkCrashReport对象就可以			*/
	/************************************************************************/
	class WIN32_API HawkCrashReport 
	{
	public:
		//构造
		HawkCrashReport();

		//析构
		~HawkCrashReport();

	protected:
		//异常捕获回调
		static LPTOP_LEVEL_EXCEPTION_FILTER m_pCrashFilter;
	};	
}
#endif
