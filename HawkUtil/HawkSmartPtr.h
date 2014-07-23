#ifndef HAWK_SMARTPTR_H
#define HAWK_SMARTPTR_H

#include "HawkException.h"

namespace Hawk
{
	/************************************************************************/
	/* ÖÇÄÜÖ¸Õë·â×°                                                         */
	/************************************************************************/
	template<typename T> class HawkSmartPtr
	{
	public:
		HawkSmartPtr(T* pPtr) : m_pPtr(pPtr)
		{
		}

		HawkSmartPtr(Bool bCreate = false) : m_pPtr(0)
		{
			if(bCreate)
				m_pPtr = new T;
		}

		HawkSmartPtr<T>& operator = (T* pPtr)
		{
			m_pPtr = pPtr;
			return *this;
		}

		~HawkSmartPtr()
		{
			HAWK_RELEASE(m_pPtr);
		}

	public:
		T&  operator *() const
		{
			HawkAssert(m_pPtr);
			return *m_pPtr;
		}

		T*  operator ->() const
		{
			HawkAssert(m_pPtr);
			return m_pPtr;
		}

		T** operator &() const
		{
			HawkAssert(m_pPtr);
			return &m_pPtr; 
		}

		Bool operator == (const T* pPtr) const
		{
			return m_pPtr == pPtr;
		}

		Bool operator !() const
		{
			return !m_pPtr;
		}  

		operator Bool() const
		{
			return m_pPtr?true:false;
		}

		T*  GetPtr() const
		{
			return m_pPtr;
		}		

	protected:
		T*  m_pPtr;
	};
}
#endif
