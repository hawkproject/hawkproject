#ifndef HAWK_SCOPE_H
#define HAWK_SCOPE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 作用域对象定义, 便于自动释放                                         */
	/************************************************************************/
	class UTIL_API HawkScope
	{
	public:
		//数据删除作用域(new出来的数据)
		class UTIL_API DataPtr : public HawkRefCounter
		{
		public:
			//构造
			DataPtr(void* pData)
			{
				m_pData = (Char*)pData;
			}

			//析构
			~DataPtr()
			{
				HAWK_DELETE(m_pData);
			}

		protected:
			//数据对象
			Char*  m_pData;
		};

		//malloc内存作用域(HawkMalloc出来的内存)
		class UTIL_API MallocPtr : public HawkRefCounter
		{
		public:
			//构造
			MallocPtr(void* pData)
			{
				m_pData = pData;
			}

			//析构
			~MallocPtr()
			{
				HawkFree(m_pData);
			}

		protected:
			//数据对象
			void*  m_pData;
		};

		//数组删除作用域(new出来的数组)
		class UTIL_API DataArrayPtr : public HawkRefCounter
		{
		public:
			//构造
			DataArrayPtr(void* pDataArray)
			{
				m_pDataArray = (Char*)pDataArray;
			}

			//析构
			~DataArrayPtr()
			{
				HAWK_DELETE_ARRAY(m_pDataArray);
			}

		protected:
			//数组对象
			Char*  m_pDataArray;
		};

		//引用计数对象删除作用域(new出来的RefCounter对象)
		class UTIL_API ObjPtr : public HawkRefCounter
		{
		public:
			//构造
			ObjPtr(HawkRefCounter* pObj)
			{
				m_pObj = pObj;
			}

			//析构
			~ObjPtr()
			{
				HAWK_RELEASE(m_pObj);
			}

		protected:
			//引用计数对象
			HawkRefCounter*  m_pObj;
		};
	};
}
#endif
