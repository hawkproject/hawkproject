#ifndef HAWK_DEMUTEXTABLE_H
#define HAWK_DEMUTEXTABLE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 动态数组实现, 有效避免vector地址重分配机制                           */
	/************************************************************************/
	template <class T, UInt32 _ITEM, UInt32 _CHUNK> class HawkDemutexTable
	{
	protected:
		//数组节点累到定义
		struct ARRAY_ELE
		{  
			T Item[_CHUNK];
		};  

	public:
		//数组构造
		HawkDemutexTable()
		{
			memset(m_pArray, 0, (_ITEM)*sizeof(ARRAY_ELE*));
			m_pArray[0] = (ARRAY_ELE*)HawkMalloc(sizeof(ARRAY_ELE));
		}

		//数组析构
		~HawkDemutexTable()
		{
			for (UInt32 i=0;i<(_ITEM);i++)
			{
				HawkFree(m_pArray[i]);
			}
		}

	public:
		//下标访问
		T& operator [] (UInt32 iIdx)
		{
			A_Exception(iIdx < (_ITEM) * (_CHUNK));

			UInt32 iMajIdx = iIdx / (_CHUNK);
			UInt32 iMorIdx = iIdx % (_CHUNK);

			if (!m_pArray[iMajIdx])
				m_pArray[iMajIdx] = (ARRAY_ELE*)HawkMalloc(sizeof(ARRAY_ELE));

			return m_pArray[iMajIdx]->Item[iMorIdx];
		}

	protected:
		ARRAY_ELE*	m_pArray[_ITEM]; 
	};
}
#endif
