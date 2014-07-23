#ifndef HAWK_VALUEHOLDER_H
#define HAWK_VALUEHOLDER_H

#include "HawkAnyType.h"

namespace Hawk
{
	/************************************************************************/
	/* ����������������,ģ��������Ҫ�ṩ��ֵ������(operator =)              */
	/************************************************************************/	
	template<class T> class HawkValueHolder : public HawkAnyType<true>
	{
	public:
		//���캯��
		HawkValueHolder(const T& tVal = 0) : m_tVal(tVal) 
		{
		}

		//��������
		virtual ~HawkValueHolder()
		{
		}

		//��ֵ������
		HawkValueHolder<T>& operator = (const HawkValueHolder<T>& sHolder)
		{
			if (this != &sHolder)
			{
				m_tVal = sHolder.m_tVal;
			}
			return *this;
		}

	public:
		//��ȡģ�����͵�ȫ��ID
		static ULong  GetHolderId()
		{
			static T* static_HolderId = 0;
			ULong iAddr = reinterpret_cast<ULong>(&static_HolderId);
			return iAddr;
		}

	public:
		//��ȡ����
		virtual ULong GetType()
		{
			if (!m_iType)
				m_iType = GetHolderId();

			return m_iType;
		}

		//��ȡֵ��ַ
		virtual void*  GetValPtr()
		{
			return &m_tVal;
		}

		//���ƶ���
		virtual Bool   Clone(HawkAnyType<true>*& pHolder) const
		{
			if (pHolder && pHolder->GetType() == m_iType)
			{
				void* pVal = pHolder->GetValPtr();
				HawkAssert(pVal != 0);
				if (pVal)
				{
					*((T*)pVal) = m_tVal;
				}
			}
			else
			{
				HAWK_RELEASE(pHolder);
				pHolder = new HawkValueHolder<T>(m_tVal);
			}
			return pHolder != 0;
		}

	public:
		//��ȡ����
		const T&  GetValue() const
		{
			return m_tVal;
		}		

		//��������
		void  SetValue(const T& tVal)
		{
			m_tVal = tVal;
		}	

	protected:
		//�������ݶ���
		T  m_tVal;
	};
}
#endif
