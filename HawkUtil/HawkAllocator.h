#ifndef HAWK_ALLOCATOR_H
#define HAWK_ALLOCATOR_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �Զ����ڴ������                                                     */
	/************************************************************************/
	template <class _Ty> class HawkAllocator
	{
	public:
		//ϵͳƥ�����Ͷ���
		typedef _Ty               value_type;
		typedef value_type*       pointer;
		typedef const value_type* const_pointer;
		typedef value_type&       reference;
		typedef const value_type& const_reference;
		typedef std::size_t       size_type;
		typedef std::ptrdiff_t    difference_type;

	public:
		HawkAllocator() {}

		~HawkAllocator() {}

		HawkAllocator(const HawkAllocator&) {}

		template <class _Other> HawkAllocator(const HawkAllocator<_Other>&) {}

	public:
		//�ذ󶨻���
		template <class _Other> struct rebind 
		{ 
			typedef HawkAllocator<_Other> other; 
		};

	public:
		pointer address(reference tVal) const 
		{ 
			return &tVal; 
		}

		const_pointer address(const_reference tVal) const 
		{
			return &tVal;
		}

		pointer allocate(size_type iCount, const_pointer = 0) 
		{
			pointer pVal = (pointer)HawkMalloc(iCount * sizeof(value_type));
			if (!pVal) 
				throw std::bad_alloc();

			return pVal;
		}

		void deallocate(pointer pVal, size_type = 0) 
		{
			HawkFree(pVal); 
		}

		size_type max_size() const 
		{
			return static_cast<size_type>(-1) / sizeof(value_type);
		}

		void construct(pointer pVal, const value_type& tVal) 
		{
			new(pVal) value_type(tVal);
		}

		void destroy(pointer pVal) 
		{
			pVal->~value_type(); 
		}

		template<class _Other> HawkAllocator<_Ty>& operator = (const HawkAllocator<_Other>&)
		{
			return *this;
		}
	};

	//void�������⻯
	template<> class HawkAllocator<void>
	{
		typedef void        value_type;
		typedef void*       pointer;
		typedef const void* const_pointer;

		template<class _Other> struct rebind
		{
			typedef HawkAllocator<_Other> other;
		};
	};

	//�Ƚϲ���������(==)
	template <class _Ty> inline bool operator == (const HawkAllocator<_Ty>&, const HawkAllocator<_Ty>&) 
	{
		return true;
	}

	//�Ƚϲ���������(!=)
	template <class _Ty> inline bool operator != (const HawkAllocator<_Ty>&, const HawkAllocator<_Ty>&) 
	{
		return false;
	}
}
#endif
