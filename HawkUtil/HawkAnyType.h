#ifndef HAWK_ANYTYPE_H
#define HAWK_ANYTYPE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 泛型模板基类,只存储类型ID和必须接口                                  */
	/************************************************************************/
	template<Bool typeEnable> class HawkAnyType : public HawkRefCounter
	{
	public:
		//构造
		HawkAnyType(ULong iType = 0) : m_iType(iType)
		{
		}		

		//析构
		virtual ~HawkAnyType()
		{
		}

		//克隆一个对象(基类不可克隆)
		virtual HawkAnyType<typeEnable>* Clone() const
		{
			return 0;
		}

	public:
		//获取类型
		virtual ULong GetType()
		{
			return m_iType;
		}

		//获取值地址
		virtual void* GetValPtr()
		{
			return 0;
		}

		//类型检查
		virtual Bool  Check()
		{
			return typeEnable && m_iType;
		}

		//复制对象
		virtual Bool  Clone(HawkAnyType<typeEnable>*& pType) const
		{
			T_Exception("HawkAnyType::Clone Must Implement.");
			return false;
		}

	protected:
		//类型ID
		ULong  m_iType;
	};
}
#endif
