#ifndef HAWK_PARAMVECTOR_H
#define HAWK_PARAMVECTOR_H

#include "HawkValueHolder.h"

namespace Hawk
{
	/************************************************************************/
	/* ��������з�װ                                                       */
	/************************************************************************/
	class UTIL_API HawkParamVector : public HawkRefCounter
	{
	public:
		//��������й���
		HawkParamVector();

		//�������������
		virtual ~HawkParamVector();		

		//�������и�ֵ������(������ʹ�ø�ֵ�Ϳ�������)
		HawkParamVector& operator = (const HawkParamVector& sParam);

		//������������
		typedef HawkAnyType<true>	 BaseHolder;

		//����������������Ͷ���
		typedef vector<BaseHolder*>  HolderVector;

	public:
		//��Ч������
		virtual UInt32 Size() const;

		//����(���ͷ�)����
		virtual void   Clear();
		
		//�������п���
		virtual Int32  Copy(const HawkParamVector& sParam);

	protected:
		//���(���ͷ�)��������
		virtual void   ClearParams();		

	public:
		//������
		template <class T> void Push(const T& tVal)
		{
			if (m_vParam.size() <= m_iSize)
			{
				HawkValueHolder<T>* pHolder = new HawkValueHolder<T>(tVal);
				UInt32 iType = 0;
				iType = pHolder->GetType();
				m_vParam.push_back(pHolder);
			}
			else 
			{
				BaseHolder* pHolder = (BaseHolder*)m_vParam[m_iSize];
				UInt32 iType = (UInt32)pHolder->GetType();
				if (!pHolder || iType != HawkValueHolder<T>::GetHolderId())
				{
					HAWK_RELEASE(pHolder);
					m_vParam[m_iSize] = 0;

					pHolder = new HawkValueHolder<T>(tVal);
					m_vParam[m_iSize] = pHolder;
				}
				else
				{
					((HawkValueHolder<T>*)pHolder)->SetValue(tVal);
				}				
			}		
			m_iSize ++;
		}		

		//��ȡ����
		template <class T> const T& Get(UInt32 iIdx) const
		{
			HawkAssert(iIdx < m_iSize);
			BaseHolder* pHolder = (BaseHolder*)m_vParam[iIdx];

			UInt32 iType = 0;
			iType = (UInt32)pHolder->GetType();
			HawkAssert(pHolder && iType == HawkValueHolder<T>::GetHolderId());
			
			return ((HawkValueHolder<T>*)pHolder)->GetValue();
		}		

	protected:		
		//������Ŀ
		UInt32		 m_iSize;
		//��������
		HolderVector m_vParam;
	};
}
#endif
