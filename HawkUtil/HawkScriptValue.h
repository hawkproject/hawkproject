#ifndef HAWK_SCRIPTVALUE_H
#define HAWK_SCRIPTVALUE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 脚本数值类型封装                                                     */
	/************************************************************************/
	class UTIL_API HawkScriptValue : public HawkRefCounter
	{
	public:
		//支持的脚本值类型
		enum 
		{
			SVT_UNKNOWN = 0,
			SVT_NIL,
			SVT_NUMBER,
			SVT_BOOL,
			SVT_STRING,
			SVT_TABLE,
			SVT_USERDATA,
		};

	public:
		//默认构造函数
		HawkScriptValue();

		//析构
		virtual ~HawkScriptValue();

		//布尔类型构造
		explicit HawkScriptValue(Bool bVal);
	
		//数值类型构造
		explicit HawkScriptValue(Int8 iVal);
		
		//数值类型构造
		explicit HawkScriptValue(UInt8 iVal);

		//数值类型构造
		explicit HawkScriptValue(Int16 iVal);

		//数值类型构造
		explicit HawkScriptValue(UInt16 iVal);

		//数值类型构造
		explicit HawkScriptValue(Int32 iVal);

		//数值类型构造
		explicit HawkScriptValue(UInt32 iVal);

		//数值类型构造
		explicit HawkScriptValue(Int64 iVal);

		//数值类型构造
		explicit HawkScriptValue(UInt64 iVal);

		//数值类型构造
		explicit HawkScriptValue(Float fVal);

		//数值类型构造
		explicit HawkScriptValue(Double dbVal);

		//字符串类型构造
		explicit HawkScriptValue(const Char* pVal);

		//字符串类型构造
		explicit HawkScriptValue(const AString& sVal);
		
		//Void指针(UserData)类型构造
		explicit HawkScriptValue(const void* pVal);

		//拷贝构造
		HawkScriptValue(const HawkScriptValue& sVal);
	
		//赋值操作符
		HawkScriptValue& operator = (const HawkScriptValue& sVal);

		//相等操作符
		Bool operator == (const HawkScriptValue& sVal) const;

		//不等操作符
		Bool operator != (const HawkScriptValue& sVal) const;
	
	public:		
		//设置值类型
		virtual void	SetType(UInt32 iType);

		//获取值类型
		virtual UInt32  GetType() const;

		//是否有效
		virtual Bool	IsValid() const;

		//是否为空值
		virtual Bool    IsNil() const;

		//是否为布尔类型
		virtual Bool	IsBoolean() const;

		//是否为数值类型
		virtual Bool	IsNumber() const;

		//是否为字符串类型
		virtual Bool	IsString() const;

		//是否为表类型
		virtual Bool	IsTable() const;

		//是否为用户数据类型
		virtual Bool	IsUserData() const;		

	public:
		//取Bool类型值
		virtual Bool    GetBoolean() const;

		//取Int32类型值
		virtual Int32   GetInt32() const;

		//取UInt32类型值
		virtual UInt32  GetUInt32() const;

		//取Float类型值
		virtual Float   GetFloat() const;

		//取Double类型值
		virtual Double  GetDouble() const;

		//取AString类型值
		virtual AString GetString() const;

		//取void*类型值
		virtual void*   GetUserData() const;

		//脚本值重置
		virtual void    Reset();

		//获得Table的项数目
		virtual Int32   TblGetItemCount() const;

		//在Table中添加一项
		virtual Int32   TblAddItem(const HawkScriptValue& sKey, const HawkScriptValue& sVal);

	public:
		//获取Table类型指定索引的Key
		const  HawkScriptValue* TblGetKey(Int32 iIdx) const;

		//获取Table类型指定索引的Value
		const  HawkScriptValue* TblGetValue(Int32 iIdx) const;

		//获取Table类型指定Key的值数据
		const  HawkScriptValue* TblGetValue(const AString& sName) const;	

		//获取Table类型指定Key的值数据
		const  HawkScriptValue* TblGetValue(const HawkScriptValue& sKey) const;

	public:
		//获取数值模板函数
		template<class number_t> number_t GetNumber() const
		{
			number_t iVal = 0;
			HawkAssert(m_iType == SVT_NUMBER && "ScriptValue TypeError");			
			if (sizeof(number_t) != sizeof(Double))
			{
				iVal = static_cast<number_t>(m_nNumber);
			}
			else
			{
				memcpy(&iVal, &m_nNumber, sizeof(Double));
			}
			return iVal;
		};

	protected:
		//脚本值的类型定义
		UInt32	m_iType;
		//字符串类型
		AString	m_sString;
		//布尔类型
		Bool	m_bBoolean;
		//数值类型
		Double	m_nNumber;
		//UserData类型
		void*	m_pData;
		//表类型
		struct
		{
			vector<HawkScriptValue> Key;
			vector<HawkScriptValue> Val;
		}m_sTable;
	}; 

	//类型简便定义
	typedef HawkScriptValue ScriptValue;
}
#endif
