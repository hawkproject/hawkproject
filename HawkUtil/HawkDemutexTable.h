#ifndef HAWK_DEMUTEXTABLE_H
#define HAWK_DEMUTEXTABLE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ��̬����ʵ��, ��Ч����vector��ַ�ط������                           */
	/************************************************************************/
	template <class T, UInt32 _ITEM, UInt32 _CHUNK> class HawkDemutexTable
	{
	protected:
		//����ڵ��۵�����
		struct ARRAY_ELE
		{  
			T Item[_CHUNK];
		};  

	public:
		//���鹹��
		HawkDemutexTable()
		{
			memset(m_pArray, 0, (_ITEM)*sizeof(ARRAY_ELE*));
			m_pArray[0] = (ARRAY_ELE*)HawkMalloc(sizeof(ARRAY_ELE));
		}

		//��������
		~HawkDemutexTable()
		{
			for (UInt32 i=0;i<(_ITEM);i++)
			{
				HawkFree(m_pArray[i]);
			}
		}

	public:
		//�±����
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
