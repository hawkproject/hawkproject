#include "HawkRefCounter.h"
#include "HawkAtomic.h"

namespace Hawk
{
	HawkRefCounter::HawkRefCounter() : m_iRef(1)
	{
	}

	HawkRefCounter::~HawkRefCounter()
	{
	}

	Int32 HawkRefCounter::AddRef()
	{ 
		return HawkAtomic::Inc<Long>(&m_iRef);
	}	

	Int32 HawkRefCounter::DecRef()
	{ 
		return HawkAtomic::Dec<Long>(&m_iRef);
	}

	Int32 HawkRefCounter::GetRef() const
	{ 
		return m_iRef;
	}

	void HawkRefCounter::Release()
	{
		//判断引用计数有效,避免无效对象释放
		A_Exception(m_iRef > 0 && "RefCounter Is Error.");

		//减少引用计数
		Long iRef = DecRef();

		//删除对象
		if (iRef == 0)
		{
			delete this;
		}
	}

	void* HawkRefCounter::operator new(Size_t iSize)
	{
		return HawkMalloc(iSize);
	}

	void HawkRefCounter::operator delete(void* pData, Size_t iSize)
	{
		HawkFree(pData);
	}
}
