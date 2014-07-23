#ifndef HAWK_CRASHREPORT_H
#define HAWK_CRASHREPORT_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/*	�������HawkMiniDumpģ���Զ�����dump�ļ�							*/
	/*	��ʹ�õĳ������������ڵ�cpp������HawkCrashReport����Ϳ���			*/
	/************************************************************************/
	class WIN32_API HawkCrashReport 
	{
	public:
		//����
		HawkCrashReport();

		//����
		~HawkCrashReport();

	protected:
		//�쳣����ص�
		static LPTOP_LEVEL_EXCEPTION_FILTER m_pCrashFilter;
	};	
}
#endif
