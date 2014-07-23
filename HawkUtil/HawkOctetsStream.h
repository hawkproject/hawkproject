#ifndef HAWK_OCTETSSTREAM_H
#define HAWK_OCTETSSTREAM_H

#include "HawkOctets.h"

namespace Hawk
{
	/************************************************************************/
	/* ������������װ                                                       */
	/************************************************************************/
	class HawkMarshal;
	class UTIL_API HawkOctetsStream : public HawkOctets
	{
	public:
		//������Ĭ�Ϲ��캯��
		HawkOctetsStream();

		//�趨��������������
		HawkOctetsStream(Int32 iSize);

		//�趨��������������
		HawkOctetsStream(void* pData,Int32 iSize);

		//�ֽ����������ݹ���
		HawkOctetsStream(const HawkOctets& xOctets);

		//��������
		HawkOctetsStream(const HawkOctetsStream& xOS);

		//��ֵ�����
		HawkOctetsStream& operator = (const HawkOctetsStream& xOS);

	public:
		//���������ﶨ��
		enum Transaction 
		{ 
			TransBegin, 
			TransCommit, 
			TransRollback 
		};	

	public:
		//�Ƿ����
		Bool   Eos() const;	

		//��ȡƫ��λ��
		UInt32 GetPos() const;

		//��ȡ����
		void   Pop(void* pData, UInt32 iSize);		

		//��ȡ��Ч�����ֽ���
		UInt32 AvailableSize() const;

		//�ƶ������α�
		UInt32 MoveNonius(Int32 iMark);

		//������Ч����ָ��
		void*  AvailableData() const;

	public:
		//�������
		HawkOctetsStream& Clear();

		//�Ƴ��հ���
		HawkOctetsStream& RemoveBlank();

		//д������
		HawkOctetsStream& Push(const void* pData,UInt32 iSize);

		//�滻����
		HawkOctetsStream& Replace(const void* pData,UInt32 iSize);

	public:
		//Int32д�뺯��
		HawkOctetsStream& operator << (Int32 iVal);

		//Int32д������
		HawkOctetsStream& operator >> (Int32& iVal);

		//UInt32д�뺯��
		HawkOctetsStream& operator << (UInt32 iVal);

		//UInt32д������
		HawkOctetsStream& operator >> (UInt32& iVal);

		//AStringд�뺯��
		HawkOctetsStream& operator << (AString& sVal);

		//AStringд�뺯��
		HawkOctetsStream& operator << (const AString& sVal);

		//AStringд������
		HawkOctetsStream& operator >> (AString& sVal);

		//UStringд�뺯��
		HawkOctetsStream& operator << (UString& sVal);

		//UStringд�뺯��
		HawkOctetsStream& operator << (const UString& sVal);

		//UStringд������
		HawkOctetsStream& operator >> (UString& sVal);

		//WStringд�뺯��
		HawkOctetsStream& operator << (WString& sVal);

		//WStringд�뺯��
		HawkOctetsStream& operator << (const WString& sVal);

		//WStringд������
		HawkOctetsStream& operator >> (WString& sVal);

		//HawkOctetsд�뺯��
		HawkOctetsStream& operator << (HawkOctets& xOctets);

		//HawkOctetsд�뺯��
		HawkOctetsStream& operator << (const HawkOctets& xOctets);

		//HawkOctetsд������
		HawkOctetsStream& operator >> (HawkOctets& xOctets);

		//HawkMarshalд�뺯��
		HawkOctetsStream& operator << (HawkMarshal& xMarshal);

		//HawkMarshalд������
		HawkOctetsStream& operator >> (HawkMarshal& xMarshal);

		//�����ύ����
		HawkOctetsStream& operator >> (Transaction eTrans);

	public:
		//ģ����������Pop
		template<typename T> void Pop(T& tVal)
		{
			Pop(&tVal,sizeof(T));
		}	

		//ģ����������Push
		template<typename T> HawkOctetsStream& Push(T tVal)
		{
			return Push(&tVal,sizeof(T));
		}			

	public:
		//vector����д��
		template<typename T> HawkOctetsStream& operator << (vector<T>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			for (UInt32 i=0;i<iSize;i++)
				*this << (T&)rhsVal[i];

			return *this;
		}

		//vector����д��
		template<typename T> HawkOctetsStream& operator << (const vector<T>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			for (UInt32 i=0;i<iSize;i++)
				*this << (const T&)rhsVal[i];

			return *this;
		}

		//vector����д��
		template<typename T> HawkOctetsStream& operator >> (vector<T>& rhsVal)
		{
			UInt32 iSize = 0;
			*this >> iSize;

			rhsVal.clear();
			if (iSize)
			{				
				for (UInt32 i=0;i<iSize;i++)
				{
					T tVal;
					*this >> tVal;
					rhsVal.push_back(tVal);
				}
			}

			return *this;
		}
		
		//list����д��
		template<typename T> HawkOctetsStream& operator << (list<T>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			typename list<T>::iterator it = rhsVal.begin();
			for (;it!=rhsVal.end();it++)
				*this << (T&)*it;

			return *this;
		}

		//list����д��
		template<typename T> HawkOctetsStream& operator << (const list<T>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			typename list<T>::iterator it = rhsVal.begin();
			for (;it!=rhsVal.end();it++)
				*this << (const T&)*it;

			return *this;
		}

		//list����д��
		template<typename T> HawkOctetsStream& operator >> (list<T>& rhsVal)
		{
			UInt32 iSize = 0;
			*this >> iSize;

			rhsVal.clear();
			for (UInt32 i=0;i<iSize;i++)
			{
				T tVal;
				*this >> tVal;
				rhsVal.push_back(tVal);
			}

			return *this;
		}

		//map����д��
		template<typename TKey,typename TVal> HawkOctetsStream& operator << (map<TKey,TVal>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			typename map<TKey,TVal>::iterator it = rhsVal.begin();
			for (;it!=rhsVal.end();it++)
			{
				*this << (TKey&)it->first;
				*this << (TVal&)it->second;
			}

			return *this;
		}

		//map����д��
		template<typename TKey,typename TVal> HawkOctetsStream& operator << (const map<TKey,TVal>& rhsVal)
		{
			UInt32 iSize = (UInt32)rhsVal.size();
			*this << iSize;

			typename map<TKey,TVal>::const_iterator it = rhsVal.begin();
			for (;it!=rhsVal.end();it++)
			{
				*this << (const TKey&)it->first;
				*this << (const TVal&)it->second;
			}

			return *this;
		}

		//map����д��
		template<typename TKey,typename TVal> HawkOctetsStream& operator >> (map<TKey,TVal>& rhsVal)
		{
			UInt32 iSize = 0;
			*this >> iSize;

			rhsVal.clear();
			for (UInt32 i=0;i<iSize;i++)
			{
				TKey _tKey;
				TVal _tVal;
				*this >> _tKey;
				*this >> _tVal;
				rhsVal[_tKey] = _tVal;
			}

			return *this;
		}

	public:
		//�Զ����������͵����л�����
		#define OCTETS_STREAM_SERIALIZE(Type)\
		HawkOctetsStream& operator << (Type& rhsVal)\
		{\
			Push<Type>(rhsVal);\
			return *this;\
		};\
		HawkOctetsStream& operator >> (Type& rhsVal)\
		{\
			rhsVal = 0;\
			Pop<Type>(rhsVal);\
			return *this;\
		}

		OCTETS_STREAM_SERIALIZE(Bool);
		OCTETS_STREAM_SERIALIZE(WChar);
		OCTETS_STREAM_SERIALIZE(Int8);
		OCTETS_STREAM_SERIALIZE(UInt8);
		OCTETS_STREAM_SERIALIZE(Int16);
		OCTETS_STREAM_SERIALIZE(UInt16);
		OCTETS_STREAM_SERIALIZE(Int64);
		OCTETS_STREAM_SERIALIZE(UInt64);
		OCTETS_STREAM_SERIALIZE(Float);
		OCTETS_STREAM_SERIALIZE(Double);

		#undef OCTETS_STREAM_SERIALIZE

	protected:
		//32λ�з�������ѹ��
		HawkOctetsStream& CompactSint32(Int32 iVal);

		//32λ�з���������ѹ
		HawkOctetsStream& UncompactSint32(Int32& iVal);

		//32λ�޷�������ѹ��
		HawkOctetsStream& CompactUint32(UInt32 iVal);

		//32λ�޷���������ѹ
		HawkOctetsStream& UncompactUint32(UInt32& iVal);	

	protected:
		UInt32  m_iPos;
		UInt32  m_iTransPos;
	};

	//����������
	typedef HawkOctetsStream  OctetsStream;
}
#endif
