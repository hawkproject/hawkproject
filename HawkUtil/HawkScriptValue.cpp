#include "HawkScriptValue.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	HawkScriptValue::HawkScriptValue()
	{
		Reset();
	}

	HawkScriptValue::~HawkScriptValue()
	{
	}

	void HawkScriptValue::Reset()
	{
		m_iType    = SVT_UNKNOWN;		
		m_bBoolean = false;
		m_nNumber  = 0;
		m_pData    = 0;
		m_sString.clear();
		m_sTable.Key.clear();
		m_sTable.Val.clear();
	}

	HawkScriptValue::HawkScriptValue(Bool bVal)
	{
		Reset();

		m_iType    = SVT_BOOL;
		m_bBoolean = bVal;
	}

	HawkScriptValue::HawkScriptValue(Int8 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(UInt8 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(Int16 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(UInt16 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(Int32 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(UInt32 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(Float iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)iVal;
	}

	HawkScriptValue::HawkScriptValue(Double dbVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		m_nNumber = (Double)dbVal;
	}

	HawkScriptValue::HawkScriptValue(Int64 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		memcpy(&m_nNumber, &iVal, sizeof(Double));
	}

	HawkScriptValue::HawkScriptValue(UInt64 iVal)
	{
		Reset();

		m_iType   = SVT_NUMBER;
		memcpy(&m_nNumber, &iVal, sizeof(Double));
	}

	HawkScriptValue::HawkScriptValue(const Char* pVal)
	{
		Reset();

		HawkAssert(pVal);
		m_iType   = SVT_STRING;
		m_sString = pVal;
	}

	HawkScriptValue::HawkScriptValue(const AString& sVal)
	{
		Reset();

		m_iType   = SVT_STRING;
		m_sString = sVal;
	}

	HawkScriptValue::HawkScriptValue(const void* pVal)
	{
		Reset();

		m_iType = SVT_USERDATA;
		m_pData = (void*)pVal;
	}

	HawkScriptValue::HawkScriptValue(const HawkScriptValue& sVal)
	{
		m_iType = sVal.m_iType;

		if (m_iType == SVT_BOOL)
		{
			m_bBoolean = sVal.m_bBoolean;
		}
		else if (m_iType == SVT_NUMBER)
		{
			m_nNumber = sVal.m_nNumber;
		}
		else if (m_iType == SVT_STRING)
		{
			m_sString = sVal.m_sString;
		}
		else if (m_iType == SVT_USERDATA)
		{
			m_pData = sVal.m_pData;
		}
		else if (m_iType == SVT_TABLE)
		{
			m_sTable = sVal.m_sTable;
		}
	}

	HawkScriptValue& HawkScriptValue::operator = (const HawkScriptValue& sVal)
	{
		if (this != &sVal)
		{
			m_iType = sVal.m_iType;

			if (m_iType == SVT_BOOL)
			{
				m_bBoolean = sVal.m_bBoolean;
			}
			else if (m_iType == SVT_NUMBER)
			{
				m_nNumber = sVal.m_nNumber;
			}
			else if (m_iType == SVT_STRING)
			{
				m_sString = sVal.m_sString;
			}
			else if (m_iType == SVT_USERDATA)
			{
				m_pData = sVal.m_pData;
			}
			else if (m_iType == SVT_TABLE)
			{
				m_sTable = sVal.m_sTable;
			}
		}
		return *this;
	}

	Bool HawkScriptValue::operator == (const HawkScriptValue& sVal) const
	{
		if (m_iType != sVal.m_iType)
			return false;

		if (m_iType == SVT_BOOL)
		{
			return m_bBoolean == sVal.m_bBoolean;
		}
		else if (m_iType == SVT_NUMBER)
		{
			return m_nNumber == sVal.m_nNumber;
		}
		else if (m_iType == SVT_STRING)
		{
			return m_sString == sVal.m_sString;
		}
		else if (m_iType == SVT_USERDATA)
		{
			return m_pData == sVal.m_pData;
		}
		else if (m_iType == SVT_TABLE)
		{
			if (m_sTable.Key.size() != sVal.m_sTable.Key.size() || m_sTable.Val.size() != sVal.m_sTable.Val.size())
				return false;

			for (Size_t i=0;i<m_sTable.Key.size();i++)
			{
				if (m_sTable.Key[i] != sVal.m_sTable.Key[i] || m_sTable.Val[i] != sVal.m_sTable.Val[i])
					return false;
			}
		}

		return true;
	}

	Bool HawkScriptValue::operator != (const HawkScriptValue& sVal) const
	{
		return !(*this == sVal);
	}

	void  HawkScriptValue::SetType(UInt32 iType)
	{
		m_iType = iType;
	}
	
	UInt32 HawkScriptValue::GetType() const
	{
		return m_iType;
	}

	Bool  HawkScriptValue::IsValid() const
	{
		return m_iType != SVT_UNKNOWN;
	}

	Bool HawkScriptValue::IsNil() const
	{
		return m_iType == SVT_NIL;
	}

	Bool  HawkScriptValue::IsBoolean() const
	{
		return m_iType == SVT_BOOL;
	}

	Bool  HawkScriptValue::IsNumber() const
	{
		return m_iType == SVT_NUMBER;
	}

	Bool  HawkScriptValue::IsString() const
	{
		return m_iType == SVT_STRING;
	}

	Bool  HawkScriptValue::IsTable() const
	{
		return m_iType == SVT_TABLE;
	}

	Bool  HawkScriptValue::IsUserData() const
	{
		return m_iType == SVT_USERDATA;
	}

	Bool  HawkScriptValue::GetBoolean() const
	{
		HawkAssert(m_iType == SVT_BOOL && "ScriptValue TypeError");

		return m_bBoolean;
	}

	Int32 HawkScriptValue::GetInt32() const
	{
		return GetNumber<Int32>();
	}

	UInt32 HawkScriptValue::GetUInt32() const
	{
		return GetNumber<UInt32>();
	}

	Float  HawkScriptValue::GetFloat() const
	{
		return GetNumber<Float>();
	}

	Double HawkScriptValue::GetDouble() const
	{
		return GetNumber<Double>();
	}

	AString HawkScriptValue::GetString() const
	{
		HawkAssert(m_iType == SVT_STRING && "ScriptValue TypeError");

		return m_sString;
	}

	void*  HawkScriptValue::GetUserData() const
	{
		HawkAssert(m_iType == SVT_USERDATA && "ScriptValue TypeError");

		return m_pData;
	}

	Int32  HawkScriptValue::TblAddItem(const HawkScriptValue& sKey,const HawkScriptValue& sVal)
	{
		if (IsTable() && sKey.IsValid() && sVal.IsValid())
		{
			m_sTable.Key.push_back(sKey);
			m_sTable.Val.push_back(sVal);
		}
		return (Int32)m_sTable.Key.size();
	}

	Int32 HawkScriptValue::TblGetItemCount() const
	{
		return (Int32)m_sTable.Key.size();
	}

	const HawkScriptValue* HawkScriptValue::TblGetValue(const AString& sName) const
	{
		if (IsTable())
		{
			for (Size_t i=0;i<m_sTable.Key.size();i++)
			{
				if (m_sTable.Key[i].IsString() && m_sTable.Key[i].GetString() == sName)
				{
					return &m_sTable.Val[i];
				}
			}
		}
		return 0;
	}

	const  HawkScriptValue* HawkScriptValue::TblGetKey(Int32 iIdx) const
	{
		if (IsTable() && iIdx >= 0 && iIdx < (Int32)m_sTable.Key.size())
		{
			return &m_sTable.Key[iIdx];
		}
		return 0;
	}

	const HawkScriptValue* HawkScriptValue::TblGetValue(Int32 iIdx) const
	{
		if (IsTable() && iIdx >= 0 && iIdx < (Int32)m_sTable.Val.size())
		{
			return &m_sTable.Val[iIdx];
		}
		return 0;
	}

	const HawkScriptValue* HawkScriptValue::TblGetValue(const HawkScriptValue& sKey) const
	{
		if (IsTable())
		{
			for (Size_t i=0;i<m_sTable.Key.size();i++)
			{
				if (m_sTable.Key[i] == sKey)
				{
					return &m_sTable.Val[i];
				}
			}
		}
		return 0;
	}
}
