#ifndef HAWK_MEMLEAKDETECTOR_H
#define HAWK_MEMLEAKDETECTOR_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/*	�����ڴ�й¶������												*/
	/*	��ʹ�õĳ������������ڵ�cpp������HawkMemLeakDetect����Ϳ���		*/
	/************************************************************************/
	class WIN32_API HawkMemLeakDetect
	{
#ifdef _DEBUG
	public:
		//����
		HawkMemLeakDetect();

		//����
		~HawkMemLeakDetect();

		//�ڴ���乳�����Ͷ���
		typedef _CRT_ALLOC_HOOK CrtHook;

	public:
		//�ڴ湹�캯������
		static Int32 AllocHook(Int32 iType, void* pData, Size_t iSize, Int32 iUse, Long iRequest, const UChar* szFile, Int32 iLine);

	protected:
		//��������·��
		Char*  BuildSymbolPath();

		//��ȡ׷��ջ
		void   GetStackTrace(void* pCallStack);	

		//����й¶��Ϣ
		void   ReportLeaks();

	protected:
		//Free����Hook����
		void   HookFree(const void* pData);

		//Malloc����Hook����
		void   HookMalloc(Int32 iRequest);

		//Realloc����Hook����
		void   HookRealloc(const void* pData, Int32 iRequest);

	protected:
		//������Ϣ
		void   Report(const Char* pFormat, ...);

	protected:
		//������Ϣ��
		void*   m_pMallocMap;		   
		//���̾��
		HANDLE	m_hProcess;
		//��־�ļ����
		FILE*	m_pLogFile;		
		//�ڴ���乳��
		CrtHook m_pOldHook; 
#endif
	};	
}
#endif
