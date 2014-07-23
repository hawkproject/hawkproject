#ifndef HAWK_MALLOC_H
#define HAWK_MALLOC_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/* Malloc��ز�����װ													*/
	/************************************************************************/
	//�ڴ����
	void*  UTIL_API HawkMalloc(Size_t iSize);

	//�ڴ����
	void*  UTIL_API HawkCalloc(Size_t iNum, Size_t iSize);

	//�ڴ��ط���
	void*  UTIL_API HawkRealloc(void* pMem, Size_t iSize);

	//�ڴ��ͷ�
	void   UTIL_API HawkFree(void* pMem);
}

//////////////////////////////////////////////////////////////////////////
#ifdef HAWK_MALLOC
	using namespace Hawk;

	//��������汾new�� �ڴ����ʧ�ܲ����׳��쳣���Ƿ��ؿ�ָ��
	void* operator	new(Size_t iSize, const std::nothrow_t&) throw();

	//��ͨ�汾new
	void* operator  new(Size_t iSize) throw(std::bad_alloc);
	void* operator  new[](Size_t iSize) throw(std::bad_alloc);
	void  operator  delete(void* pMem) throw();
	void  operator  delete[](void* pMem) throw();	

	//��������Ϣ�汾
	void* operator  new(Size_t iSize, const Char* szFile, Int32 iLine);
	void* operator  new[](Size_t iSize, const Char* szFile, Int32 iLine);
	void  operator  delete(void* pMem, const Char* szFile, Int32 iLine);
	void  operator  delete[](void* pMem, const Char* szFile, Int32 iLine);
#endif 
//////////////////////////////////////////////////////////////////////////

#endif
