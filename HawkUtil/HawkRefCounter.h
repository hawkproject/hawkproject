#ifndef HAWK_REFCOUNTER_H
#define HAWK_REFCOUNTER_H

#include "HawkException.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ü�����                                                           */
	/************************************************************************/
	class UTIL_API HawkRefCounter
	{
	public:
		//������
		HawkRefCounter();

		//������������
		virtual ~HawkRefCounter();

	public:
		//��������
		virtual Int32  AddRef();

		//��������
		virtual Int32  DecRef();

		//��ȡ����
		virtual Int32  GetRef() const;

		//�ͷ�(���ü���<=0ʱ����ɾ��)
		virtual void   Release();

	public:
		//���󴴽��ڴ濪�ٲ���������
		void* operator new(Size_t iSize);

		//����ɾ������������
		void  operator delete(void* pData, Size_t iSize);

	protected:
		//���ü���ֵ
		volatile Long  m_iRef;
	};
}
#endif
