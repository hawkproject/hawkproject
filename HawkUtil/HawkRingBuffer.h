#ifndef HAWK_RINGBUFFER_H
#define HAWK_RINGBUFFER_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ζ������ݽṹ��װ                                                 */
	/************************************************************************/
	template <class T> class HawkRingBuffer
	{
	public:
		//����
		HawkRingBuffer()
		{
			m_pBuffer	= 0;
			m_iCapacity	= 0;
			m_iReadPos	= 0;
			m_iWritePos	= 0;
			m_bCover	= false;
		}

		//����
		~HawkRingBuffer()
		{
			if (m_pBuffer)
			{
				HawkFree(m_pBuffer);
				m_pBuffer = 0;
			}			
		}

	public:
		//���г�ʼ��
		Bool Init(UInt32 iCap, Bool bCover = false)
		{
			m_iCapacity	= iCap;
			m_bCover	= bCover;

			if (m_iCapacity)
				m_pBuffer = (T*)HawkMalloc(m_iCapacity * sizeof(T));
				
			return m_pBuffer != 0;
		}

		//�ж϶����Ƿ���
		Bool IsFull() const
		{
			return (m_iWritePos + 1) % m_iCapacity == m_iReadPos;
		}
 
		//�ж϶����Ƿ�Ϊ��
		Bool IsEmpty() const
		{
			return m_iWritePos == m_iReadPos; 
		}
 
		//д����
		Bool Write(const T& tVal) 
		{
			if (!IsFull() || m_bCover)
			{
				m_pBuffer[m_iWritePos] = tVal;
				m_iWritePos = (m_iWritePos + 1) % m_iCapacity;

				if (m_iWritePos == m_iReadPos)
					m_iReadPos = (m_iReadPos + 1) % m_iCapacity;
				
				return true;
			}
			return false;			
		}
 
		//������
		Bool Read(T& tVal) 
		{
			if (!IsEmpty())
			{
				tVal = m_pBuffer[m_iReadPos];
				m_iReadPos = (m_iReadPos + 1) % m_iCapacity;
				return true;
			}
			return false;			
		}

	protected:
		UInt32	m_iCapacity;
		Bool	m_bCover;
		UInt32	m_iReadPos;
		UInt32	m_iWritePos;
		T*		m_pBuffer;
	};
}
#endif
