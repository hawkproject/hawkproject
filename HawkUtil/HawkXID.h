#ifndef HAWK_XID_H
#define HAWK_XID_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 全局唯一标识符类型定义                                               */
	/************************************************************************/
	struct XID
	{
		//类型
		UInt32 Type;

		//ID值
		UInt32 Id;

		//默认构造
		XID() : Type(0),Id(0) 
		{
		}

		//类型构造
		XID(UInt32 iType) : Type(iType),Id(0) 
		{
		}

		//直接设置类型和ID
		XID(UInt32 iType, UInt32 iId) : Type(iType),Id(iId) 
		{
		}

		//拷贝构造
		XID(const XID& sXid) : Type(sXid.Type),Id(sXid.Id) 
		{
		}

		//赋值操作符
		XID& operator = (const XID& sXid)
		{
			if(this != &sXid)
			{
				Type = sXid.Type;
				Id   = sXid.Id;
			}			
			return *this;
		}

		//设置类型和ID
		inline void Set(UInt32 iType, UInt32 iId)
		{
			Type = iType;
			Id   = iId;
		}

		//清空数据
		inline void Clear()
		{
			Type = 0;
			Id   = 0;
		}

		//相等比较
		inline Bool operator == (const XID& sXid) const
		{
			return Type == sXid.Type && Id == sXid.Id;
		}

		//不等比较
		inline Bool operator != (const XID& sXid) const
		{
			return Type != sXid.Type || Id != sXid.Id;
		}

		//小于比较
		inline Bool operator < (const XID & sXid) const
		{
			if (Type < sXid.Type)
				return true;
			else if(Type > sXid.Type)
				return false;

			return Id < sXid.Id;
		}

		//有效性判断
		inline Bool IsValid() const 
		{ 
			return Type > 0; 
		}
	};

	//ID列表定义
	typedef vector<XID>	  XIDVector;
	//ID映射表定义
	typedef map<XID, XID> XIDMap;
}
#endif
