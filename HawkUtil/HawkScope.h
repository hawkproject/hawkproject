#ifndef HAWK_SCOPE_H
#define HAWK_SCOPE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �����������, �����Զ��ͷ�                                         */
	/************************************************************************/
	class UTIL_API HawkScope
	{
	public:
		//����ɾ��������(new����������)
		class UTIL_API DataPtr : public HawkRefCounter
		{
		public:
			//����
			DataPtr(void* pData)
			{
				m_pData = (Char*)pData;
			}

			//����
			~DataPtr()
			{
				HAWK_DELETE(m_pData);
			}

		protected:
			//���ݶ���
			Char*  m_pData;
		};

		//malloc�ڴ�������(HawkMalloc�������ڴ�)
		class UTIL_API MallocPtr : public HawkRefCounter
		{
		public:
			//����
			MallocPtr(void* pData)
			{
				m_pData = pData;
			}

			//����
			~MallocPtr()
			{
				HawkFree(m_pData);
			}

		protected:
			//���ݶ���
			void*  m_pData;
		};

		//����ɾ��������(new����������)
		class UTIL_API DataArrayPtr : public HawkRefCounter
		{
		public:
			//����
			DataArrayPtr(void* pDataArray)
			{
				m_pDataArray = (Char*)pDataArray;
			}

			//����
			~DataArrayPtr()
			{
				HAWK_DELETE_ARRAY(m_pDataArray);
			}

		protected:
			//�������
			Char*  m_pDataArray;
		};

		//���ü�������ɾ��������(new������RefCounter����)
		class UTIL_API ObjPtr : public HawkRefCounter
		{
		public:
			//����
			ObjPtr(HawkRefCounter* pObj)
			{
				m_pObj = pObj;
			}

			//����
			~ObjPtr()
			{
				HAWK_RELEASE(m_pObj);
			}

		protected:
			//���ü�������
			HawkRefCounter*  m_pObj;
		};
	};
}
#endif
