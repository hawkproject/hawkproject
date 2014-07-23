#ifndef HAWK_HEAPARRAY_H
#define HAWK_HEAPARRAY_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �����ݽṹ������װ(�������������ȶ���			                    */
	/* i�ĸ���㣺Parent(i) = (i-1)/2										*/
	/* i����Ҷ�ӣ�Left(i)   = i*2 + 1										*/
	/* i����Ҷ�ӣ�Right(i)  = i*2 + 2										*/
	/************************************************************************/
	template < class T, class Cmp = std::less<T> > class HawkHeapArray
	{
	public:
		//����
		HawkHeapArray()
		{
			m_vArray.clear();
		}

		//����
		~HawkHeapArray()
		{
			m_vArray.clear();
		}

	public:
		//������Push����
		void Push(const T& tVal)
		{
			m_vArray.push_back(tVal);
			ShiftUp(m_vArray.size()-1);
		}

		//�����Ѳ�����
		Bool Pop(T& tVal)
		{
			if (m_vArray.size())
			{
				tVal = m_vArray[0];
				std::swap(m_vArray[0], m_vArray.back());
				m_vArray.pop_back();
				ShiftDown(0);
				return true;
			}
			return false;
		}

		//ȡ�Ѷ�������
		const T& Top() const
		{
			return m_vArray.front();
		}

		//ȡ�Ѷ�������
		T& Top()
		{
			return m_vArray.front();
		}

		//ȡԪ�ظ���
		UInt32 Size() const
		{
			return m_vArray.size();
		}

	protected:
		//�ڵ��ϸ�
		void ShiftUp(UInt32 iIdx)
		{
			UInt32 iParentIdx = (iIdx-1)/2;
			if (iIdx && Cmp()(m_vArray[iIdx], m_vArray[iParentIdx])) 
			{
				std::swap(m_vArray[iIdx], m_vArray[iParentIdx]);
				ShiftUp(iParentIdx);
			}
		}

		//�ڵ��³�
		void ShiftDown(UInt32 iIdx)
		{
			UInt32 lChildIdx = iIdx*2 + 1;
			UInt32 rChildIdx = iIdx*2 + 2;
			if (iIdx >= m_vArray.size() || lChildIdx >= m_vArray.size())
				return;

			UInt32 iMinChildIdx = 0;
			if (rChildIdx >= m_vArray.size())
			{
				iMinChildIdx = lChildIdx;
			} 
			else 
			{
				iMinChildIdx = Cmp()(m_vArray[lChildIdx], m_vArray[rChildIdx])? lChildIdx : rChildIdx;
			}

			if (Cmp()(m_vArray[iMinChildIdx], m_vArray[iIdx])) 
			{
				std::swap(m_vArray[iMinChildIdx], m_vArray[iIdx]);
				ShiftDown(iMinChildIdx);
			}
		}

	protected:
		vector<T> m_vArray;
	};
}
#endif
