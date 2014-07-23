#ifndef HAWK_OBJMANAGER_H
#define HAWK_OBJMANAGER_H

#include "HawkManagerBase.h"
#include "HawkObjBase.h"

namespace Hawk
{
	/************************************************************************/
	/* �̰߳�ȫ�������������                                               */
	/************************************************************************/
	template<class ObjKey,class ObjType> class HawkObjManager : public HawkManagerBase
	{
	public:
		//����
		HawkObjManager()
		{
			m_pMapLock = new HawkRWLock; 
		}

		//����
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
		//�������Ͷ���
		typedef HawkObjBase<ObjKey,ObjType>	ObjBaseType;

		//��������ж���
		typedef vector<ObjKey>				ObjKeyVector;

		//����ֵ���ж���
		typedef vector<ObjType*>			ObjValueVector;

		//�����ֵӳ�����
		typedef map<ObjKey,ObjBaseType*>	ObjBaseMap;

		//������ж���
		typedef deque<ObjBaseType*>			ObjBaseQueue;

		//�����ֵ����������
		typedef Bool (*ObjFilterFunc)		(ObjType* pObj);
		
		//������Ϣ����������
		template<class T> struct ObjInfoFilter
		{ 
			typedef Bool (*Func) (ObjType* pObj, vector<T>& vVal); 
		};

	public:
		//��ȫ�������ͷ�װ
		class SafeObj
		{
		public:
			//���ݶ������ƹ������
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

			//ֱ��ʹ�ö���ָ�빹�����
			SafeObj(ObjBaseType* pObjBase)
			{
				HawkAssert(pObjBase);
				m_pObjBase = pObjBase;
				if (m_pObjBase)
					m_pObjBase->LockObj();
			};

			//��������
			virtual ~SafeObj()
			{
				if (m_pObjBase)
					m_pObjBase->UnlockObj();
			}

		public:	
			//��ȡ��������
			virtual ObjBaseType* GetObjBase()
			{
				return m_pObjBase;
			}

			//��ȡʵ�����
			virtual ObjType* GetImpl()
			{
				HawkAssert(m_pObjBase);
				if (m_pObjBase)
					return m_pObjBase->GetImpl();
				
				return 0;
			}

			//��ȡʵ�����
			template<class ImplType> ImplType* GetImpl()
			{
				HawkAssert(m_pObjBase);
				if (m_pObjBase)
					return (ImplType*)m_pObjBase->GetImpl();
				
				return 0;
			}

			//�жϻ��������Ƿ���Ч
			virtual Bool IsObjValid() const
			{
				if (m_pObjBase)
					return m_pObjBase->IsObjValid();
				
				return false;
			}

			//ֱ��ָ�����
			virtual ObjType* operator ->()
			{
				return GetImpl();
			}

		protected:
			ObjBaseType*  m_pObjBase;
		};

	public:
		//���ݼ�ֵ��ѯ����
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

		//���ٻ�������
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

		//�ͷ�ʵ�����
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

		//�ռ����������
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
	
		//�ռ�����ֵ����
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

		//�ռ����������������Ϣ�б�
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
		//�����������
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
