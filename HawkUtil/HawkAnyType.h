#ifndef HAWK_ANYTYPE_H
#define HAWK_ANYTYPE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ����ģ�����,ֻ�洢����ID�ͱ���ӿ�                                  */
	/************************************************************************/
	template<Bool typeEnable> class HawkAnyType : public HawkRefCounter
	{
	public:
		//����
		HawkAnyType(ULong iType = 0) : m_iType(iType)
		{
		}		

		//����
		virtual ~HawkAnyType()
		{
		}

		//��¡һ������(���಻�ɿ�¡)
		virtual HawkAnyType<typeEnable>* Clone() const
		{
			return 0;
		}

	public:
		//��ȡ����
		virtual ULong GetType()
		{
			return m_iType;
		}

		//��ȡֵ��ַ
		virtual void* GetValPtr()
		{
			return 0;
		}

		//���ͼ��
		virtual Bool  Check()
		{
			return typeEnable && m_iType;
		}

		//���ƶ���
		virtual Bool  Clone(HawkAnyType<typeEnable>*& pType) const
		{
			T_Exception("HawkAnyType::Clone Must Implement.");
			return false;
		}

	protected:
		//����ID
		ULong  m_iType;
	};
}
#endif
