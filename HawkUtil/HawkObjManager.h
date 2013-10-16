#ifndef HAWK_OBJMANAGER_H
#define HAWK_OBJMANAGER_H

#include "HawkManagerBase.h"
#include "HawkObjBase.h"

namespace Hawk
{
	/************************************************************************/
	/* 线程安全对象管理器定义                                               */
	/************************************************************************/
	template<class ObjKey,class ObjType> class HawkObjManager : public HawkManagerBase
	{
	public:
		//构造
		HawkObjManager()
		{
			m_pMapLock = new HawkRWLock; 
		}

		//析构
		virtual ~HawkObjManager()
		{		
			typename ObjBaseMap::iterator mit = m_mObjBase.begin();
			for (;mit!=m_mObjBase.end();mit++)
			{
				HAWK_RELEASE(mit->second);
			}
			m_mObjBase.clear();

			typename ObjBaseQueue::iterator qit = m_qObjBase.begin();
			for (;qit!=m_qObjBase.end();qit++)
			{
				HAWK_RELEASE(*qit);
			}
			m_qObjBase.clear();

			HAWK_RELEASE(m_pMapLock);
		}

	public:
		//对象类型定义
		typedef HawkObjBase<ObjKey,ObjType>	ObjBaseType;

		//对象键序列定义
		typedef vector<ObjKey>				ObjKeyVector;

		//对象值序列定义
		typedef vector<ObjType*>			ObjValueVector;

		//对象键值映射表定义
		typedef map<ObjKey,ObjBaseType*>	ObjBaseMap;

		//对象队列定义
		typedef deque<ObjBaseType*>			ObjBaseQueue;

		//对象键值过滤器定义
		typedef Bool (*ObjFilterFunc)		(ObjType* pObj);
		
		//对象信息过滤器定义
		template<class T> struct ObjInfoFilter
		{ 
			typedef Bool (*Func) (ObjType* pObj, vector<T>& vVal); 
		};

	public:
		//安全对象类型封装
		class SafeObj
		{
		public:
			//根据对象名称构造对象
			SafeObj(HawkObjManager* pObjManager, const ObjKey& tKey)
			{
				HawkAssert(pObjManager);
				if (pObjManager)
				{
					m_pObjBase = pObjManager->QueryObject(tKey);
					if (m_pObjBase)
						m_pObjBase->LockObj();
				}				
			};

			//直接使用对象指针构造对象
			SafeObj(ObjBaseType* pObjBase)
			{
				HawkAssert(pObjBase);
				m_pObjBase = pObjBase;
				if (m_pObjBase)
					m_pObjBase->LockObj();
			};

			//对象析构
			virtual ~SafeObj()
			{
				if (m_pObjBase)
					m_pObjBase->UnlockObj();
			}

		public:	
			//获取基础对象
			virtual ObjBaseType* GetObjBase()
			{
				return m_pObjBase;
			}

			//获取实体对象
			virtual ObjType* GetImpl()
			{
				HawkAssert(m_pObjBase);
				if (m_pObjBase)
					return m_pObjBase->GetImpl();
				
				return 0;
			}

			//获取实体对象
			template<class ImplType> ImplType* GetImpl()
			{
				HawkAssert(m_pObjBase);
				if (m_pObjBase)
					return (ImplType*)m_pObjBase->GetImpl();
				
				return 0;
			}

			//判断基础对象是否有效
			virtual Bool IsObjValid() const
			{
				if (m_pObjBase)
					return m_pObjBase->IsObjValid();
				
				return false;
			}

			//直接指针访问
			virtual ObjType* operator ->()
			{
				return GetImpl();
			}

		protected:
			ObjBaseType*  m_pObjBase;
		};

	public:
		//根据键值查询对象
		virtual ObjBaseType* QueryObject(const ObjKey& tKey)
		{			
			ObjBaseType* pObjBase   = 0;

			if (m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::READ);
				typename ObjBaseMap::iterator it = m_mObjBase.find(tKey);
				if (it != m_mObjBase.end())
				{
					pObjBase = it->second;
					if(pObjBase)
					{
						A_Exception(tKey == pObjBase->GetObjKey());
					}
				}
			}			
			return pObjBase;
		}

		//开辟基础对象
		virtual ObjBaseType* AllocObject(const ObjKey& tKey, ObjType* pObj = 0)
		{
			ObjBaseType* pObjBase   = 0;

			if (m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::WRITE);
				typename ObjBaseMap::iterator it = m_mObjBase.find(tKey);
				if (it == m_mObjBase.end())
				{
					pObjBase = MallocObjBase();
					if (pObjBase)
					{
						A_Exception(pObjBase->IsObjEmpty());

						pObjBase->LockObj();
						pObjBase->SetImpl(tKey, pObj);
						pObjBase->UnlockObj();

						m_mObjBase[tKey] = pObjBase;
					}
				}
				else
				{
					T_Exception("AllocObject Key Duplicate.");
				}
			}
			return pObjBase;
		}		

		//释放实体对象
		virtual Bool FreeObject(const ObjKey& tKey, Bool bRelease = true)
		{
			ObjBaseType* pObjBase   = 0;

			if (m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::WRITE);
				typename ObjBaseMap::iterator it = m_mObjBase.find(tKey);
				if (it != m_mObjBase.end())
				{
					pObjBase = it->second;

					A_Exception(pObjBase->IsObjActive());
					A_Exception(tKey == pObjBase->GetObjKey());	

					pObjBase->LockObj();
					pObjBase->FreeObject(bRelease);
					pObjBase->UnlockObj();

					m_mObjBase.erase(it);
					m_qObjBase.push_back(pObjBase);
				}
			}
			return pObjBase != 0;
		}

		//收集对象键序列
		virtual UInt32 CollectObjKey(ObjKeyVector& vKey, ObjFilterFunc pFilter = 0)
		{			
			if (m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::READ);
				typename ObjBaseMap::iterator it = m_mObjBase.begin();
				for (;it!=m_mObjBase.end();it++)
				{
					if(!pFilter || (*pFilter)(it->second->GetImpl()))
						vKey.push_back(it->first);
				}
			}
			return (UInt32)vKey.size();
		}
	
		//收集对象值序列
		virtual UInt32 CollectObjValue(ObjValueVector& vVal, ObjFilterFunc pFilter = 0)
		{			
			if (m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::READ);
				typename ObjBaseMap::iterator it = m_mObjBase.begin();
				for (;it!=m_mObjBase.end();it++)
				{
					if(!pFilter || (*pFilter)(it->second->GetImpl()))					
						vVal.push_back(it->second->GetImpl());
				}
			}
			return (UInt32)vVal.size();
		}

		//收集对象符合条件的信息列表
		template<class T> UInt32 CollectObjInfo(vector<T>& vVal, typename ObjInfoFilter<T>::Func pFilter)
		{			
			if (pFilter && m_pMapLock)
			{
				HawkAutoRWLock(lock,m_pMapLock,HawkRWLock::READ);
				typename ObjBaseMap::iterator it = m_mObjBase.begin();
				for (;it!=m_mObjBase.end();it++)
				{
					(*pFilter)(it->second->GetImpl(), vVal);
				}
			}
			return (UInt32)vVal.size();
		}

	protected:
		//分配基础对象
		ObjBaseType* MallocObjBase()
		{
			ObjBaseType* pObjBase = 0;
			if (m_qObjBase.size())
			{
				typename ObjBaseQueue::iterator it = m_qObjBase.begin();
				for (;it != m_qObjBase.end();it++)
				{
					if((*it)->IsObjEmpty())
					{
						pObjBase = *it;
						m_qObjBase.erase(it);
						break;
					}
				}
			}

			if(!pObjBase)
				pObjBase = new ObjBaseType;

			return pObjBase;
		}

	protected:
		ObjBaseMap	  m_mObjBase;
		ObjBaseQueue  m_qObjBase;
		HawkRWLock*	  m_pMapLock;
	};
}
#endif
