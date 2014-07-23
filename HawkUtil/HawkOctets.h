#ifndef HAWK_OCTETS_H
#define HAWK_OCTETS_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ���ݲ�����װ                                                         */
	/************************************************************************/
	class UTIL_API HawkOctets : public HawkRefCounter
	{
	public:
		//����������
		HawkOctets();
		
		//��ʼ�����ȹ���
		HawkOctets(UInt32 iSize);

		//��ʼ�����ݹ���
		HawkOctets(const void* pData, UInt32 iSize);

		//��ʼ�����ݹ���
		HawkOctets(void* pBegin, void* pEnd);

		//��������
		HawkOctets(const HawkOctets& xOctets);

		//����
		virtual ~HawkOctets();
		
	public:
		//��ֵ������
		HawkOctets& operator = (const HawkOctets& xOctets);

		//��ȱȽ�
		Bool operator == (const HawkOctets& xOctets);

		//���ȱȽ�
		Bool operator != (const HawkOctets& xOctets);

	public:
		//�������׵�ַ
		void*   Begin();

		//������β��ַ
		void*   End();

		//�������ֽڴ�С
		UInt32  Size() const;

		//�Ƿ���Ч,�ڴ��ѿ���?
		Bool    IsValid() const;

		//����������
		UInt32  Capacity() const;

		//ʣ��ռ��ֽڴ�С
		UInt32  EmptyCap() const;

	public:
		//�������׵�ַ
		const void*  Begin() const;

		//������β��ַ
		const void*  End() const;

		//�������
		HawkOctets&  Clear();

		//Ĩ������
		HawkOctets&  Erase(void* pBegin, void* pEnd);

		//Ĩ������
		HawkOctets&  Erase(void* pBegin, UInt32 iSize);

		//��������
		HawkOctets&  Insert(void* pPos, const void* pBegin, void* pEnd);

		//��������
		HawkOctets&  Insert(void* pPos, const void* pData, UInt32 iSize);

		//��������
		HawkOctets&  Insert(UInt32 iPos, const void* pData, UInt32 iSize);

		//�ҽ�����
		HawkOctets&  Append(const void* pData, UInt32 iSize);

		//���ô�С
		HawkOctets&  Resize(UInt32 iSize);

		//���ٿռ�
		HawkOctets&  Reserve(UInt32 iSize);

		//�滻����
		HawkOctets&  Replace(const void* pData, UInt32 iSize);

		//����������
		HawkOctets&  Swap(HawkOctets& xOctets);

	protected:
		//�������׵�ַ
		void*   m_pBase;
		//������β��ַ
		void*   m_pHigh;
		//��������
		UInt32  m_iCap;
	};
}
#endif
