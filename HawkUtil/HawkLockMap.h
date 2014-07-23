#ifndef HAWK_LOCKMAP_H
#define HAWK_LOCKMAP_H

#include "HawkMutex.h"

namespace Hawk
{
	template<class TKey, class TVal> class HawkLockMap : protected std::map<TKey,TVal>
	{
	public:
		HawkLockMap() : std::map<TKey,TVal>()
		{
			m_pLock = new HawkMutex();
		}

		~HawkLockMap()
		{
			HAWK_RELEASE(m_pLock);
		}

	public:
		typedef typename std::map<TKey,TVal>::iterator		 iterator;
		typedef typename std::map<TKey,TVal>::const_iterator const_iterator;

	public:
		//大小
		Size_t Size() const
		{
			return this->size();
		}

		//锁定
		Bool Lock()
		{
			if (m_pLock)
			{
				m_pLock->Lock(__FILE__, __LINE__);
				return true;
			}
			return false;
		}

		//解锁
		Bool Unlock()
		{
			if (m_pLock)
			{
				m_pLock->Unlock();
				return true;
			}
			return false;
		}

		//开始
		iterator Begin()
		{
			return this->begin();
		}

		//开始
		const_iterator Begin() const
		{
			return this->begin();
		}

		//结束
		iterator End()
		{
			return this->end();
		}

		//结束
		const_iterator End() const
		{
			return this->end();
		}

		//查找
		Bool Find(const TKey& tKey, TVal* pVal = 0)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->find(tKey);
			if (it != this->end())
			{
				if(pVal)
				{
					*pVal = it->second;	
				}
				return true;
			}

			return false;
		}

		//删除
		Bool Erase(const TKey& tKey,TVal* pVal = 0)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->find(tKey);
			if (it != this->end())
			{
				if(pVal)
				{
					*pVal = it->second;
				}
				this->erase(it);
				return true;
			}

			return false;
		}

		//添加
		void Set(const TKey& tKey,const TVal& tVal)
		{
			HawkAutoMutex(lock, m_pLock);

			(*this)[tKey] = tVal;
		}

		void Clear()
		{
			HawkAutoMutex(lock, m_pLock);

			this->clear();
		}		

	public:
		//数据收集拷贝操作
		void CollectKey(vector<TKey>& vKey)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->begin();
			for (;it != this->end();it++)
			{
				vKey.push_back(it->first);
			}
		}

		void CollectValue(vector<TVal>& vVal)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->begin();
			for (;it != this->end();it++)
			{
				vVal.push_back(it->second);
			}
		}

		void CollectKeyValue(vector<TKey>& vKey,vector<TVal>& vVal)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->begin();
			for (;it != this->end();it++)
			{
				vKey.push_back(it->first);
				vVal.push_back(it->second);
			}
		}

		void Copy(map<TKey,TVal>& mCpy)
		{
			HawkAutoMutex(lock, m_pLock);

			iterator it = this->begin();
			for (;it != this->end();it++)
			{
				mCpy[it->first] = it->second;
			}
		}

	protected:
		HawkMutex*    m_pLock;
	};
}
#endif
