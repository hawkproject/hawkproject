#ifndef HAWK_OCTETS_H
#define HAWK_OCTETS_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 数据操作封装                                                         */
	/************************************************************************/
	class UTIL_API HawkOctets : public HawkRefCounter
	{
	public:
		//数据流构造
		HawkOctets();
		
		//初始化长度构造
		HawkOctets(UInt32 iSize);

		//初始化数据构造
		HawkOctets(const void* pData, UInt32 iSize);

		//初始化数据构造
		HawkOctets(void* pBegin, void* pEnd);

		//拷贝构造
		HawkOctets(const HawkOctets& xOctets);

		//析构
		virtual ~HawkOctets();
		
	public:
		//赋值操作符
		HawkOctets& operator = (const HawkOctets& xOctets);

		//相等比较
		Bool operator == (const HawkOctets& xOctets);

		//不等比较
		Bool operator != (const HawkOctets& xOctets);

	public:
		//数据流首地址
		void*   Begin();

		//数据流尾地址
		void*   End();

		//数据流字节大小
		UInt32  Size() const;

		//是否有效,内存已开辟?
		Bool    IsValid() const;

		//数据流容量
		UInt32  Capacity() const;

		//剩余空间字节大小
		UInt32  EmptyCap() const;

	public:
		//数据流首地址
		const void*  Begin() const;

		//数据流尾地址
		const void*  End() const;

		//清空数据
		HawkOctets&  Clear();

		//抹除数据
		HawkOctets&  Erase(void* pBegin, void* pEnd);

		//抹除数据
		HawkOctets&  Erase(void* pBegin, UInt32 iSize);

		//插入数据
		HawkOctets&  Insert(void* pPos, const void* pBegin, void* pEnd);

		//插入数据
		HawkOctets&  Insert(void* pPos, const void* pData, UInt32 iSize);

		//插入数据
		HawkOctets&  Insert(UInt32 iPos, const void* pData, UInt32 iSize);

		//挂接数据
		HawkOctets&  Append(const void* pData, UInt32 iSize);

		//设置大小
		HawkOctets&  Resize(UInt32 iSize);

		//开辟空间
		HawkOctets&  Reserve(UInt32 iSize);

		//替换数据
		HawkOctets&  Replace(const void* pData, UInt32 iSize);

		//数据流交换
		HawkOctets&  Swap(HawkOctets& xOctets);

	protected:
		//数据流首地址
		void*   m_pBase;
		//数据流尾地址
		void*   m_pHigh;
		//数据容量
		UInt32  m_iCap;
	};
}
#endif
