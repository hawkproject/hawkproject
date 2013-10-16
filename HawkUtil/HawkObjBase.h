#ifndef HAWK_OBJBASE_H
#define HAWK_OBJBASE_H

#include "HawkRefCounter.h"
#include "HawkRWLock.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* 基础对象类型定义                                                     */
	/************************************************************************/
	template<class ObjKey,class ObjType> class HawkObjBase : public HawkRefCounter
	{
	public:
		//构造
		HawkObjBase(): m_pImpl(0), m_iObjFlag(0)
		{
			m_pMutex = new HawkMutex;
		}

		//析构
		virtual ~HawkObjBase()
		{
			HAWK_RELEASE(m_pImpl);
			HAWK_RELEASE(m_pMutex);
		}

		//对象状态枚举定义
		enum
		{
			OBJ_ACTIVE	= 0x01,
			OBJ_LOCKED	= 0x02,
		};	

	public:
		//获取对象键值
		virtual ObjKey GetObjKey() const
		{
			return m_tKey;
		}

		//获取实体对象
		virtual ObjType* GetImpl()
		{
			return m_pImpl;
		}		

		//设置实体对象, 仅ObjManager可调用
		virtual void SetImpl(const ObjKey& tKey, ObjType* pObj)
		{
			m_tKey  = tKey;
			m_pImpl = pObj;
			if(m_pImpl)
			{
				SetObjFlag(OBJ_ACTIVE);
			}
		}

		//释放实体对象, 仅ObjManager可调用
		virtual void FreeObject(Bool bRelease = true)
		{
			if (bRelease)
			{
				HAWK_RELEASE(m_pImpl);
			}			
			
			ClearObjFlag(OBJ_ACTIVE);
		}

	public:
		//对象锁定
		virtual Bool LockObj()
		{
			if (m_pMutex)
			{
				m_pMutex->Lock(__FILE__, __LINE__);
				SetObjFlag(OBJ_LOCKED);
				return true;
			}
			return false;
		}

		//对象解锁
		virtual Bool UnlockObj()
		{
			if (m_pMutex)
			{
				ClearObjFlag(OBJ_LOCKED);
				m_pMutex->Unlock();
				return true;
			}			
			return false;
		}

	public:
		//设置对象指定标记
		virtual void SetObjFlag(UInt32 iFlag)
		{
			m_iObjFlag |= iFlag;
		}

		//清除对象指定标记
		virtual void ClearObjFlag(UInt32 iFlag)
		{
			m_iObjFlag &= ~iFlag; 
		}

		//查询对象指定标记
		virtual Bool HasObjFlag(UInt32 iFlag) const
		{
			return (m_iObjFlag & iFlag) != 0;
		}

		//判断对象释放激活状态
		virtual Bool IsObjActive() const
		{
			return HasObjFlag(OBJ_ACTIVE);
		}

		//判断对象释放锁定状态
		virtual Bool IsObjLocked() const
		{
			return HasObjFlag(OBJ_LOCKED);
		}

		//判断对象是否有效
		virtual Bool IsObjValid() const
		{
			if (m_pImpl)
				return IsObjActive();
			
			return false;
		}

		//判断对象是否为空
		virtual Bool IsObjEmpty() const
		{
			if (m_pImpl || HasObjFlag(OBJ_ACTIVE) || HasObjFlag(OBJ_LOCKED))
				return false;
			
			return true;
		}

	protected:
		//对象键值
		ObjKey		m_tKey;
		//对象实体
		ObjType*	m_pImpl;		
		//容器锁
		HawkMutex*	m_pMutex;
		//对象标记状态
		UInt32		m_iObjFlag;
	};
}
#endif
