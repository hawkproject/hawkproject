#ifndef HAWK_SCRIPTVALUE_H
#define HAWK_SCRIPTVALUE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �ű���ֵ���ͷ�װ                                                     */
	/************************************************************************/
	class UTIL_API HawkScriptValue : public HawkRefCounter
	{
	public:
		//֧�ֵĽű�ֵ����
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
		//Ĭ�Ϲ��캯��
		HawkScriptValue();

		//����
		virtual ~HawkScriptValue();

		//�������͹���
		explicit HawkScriptValue(Bool bVal);
	
		//��ֵ���͹���
		explicit HawkScriptValue(Int8 iVal);
		
		//��ֵ���͹���
		explicit HawkScriptValue(UInt8 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(Int16 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(UInt16 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(Int32 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(UInt32 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(Int64 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(UInt64 iVal);

		//��ֵ���͹���
		explicit HawkScriptValue(Float fVal);

		//��ֵ���͹���
		explicit HawkScriptValue(Double dbVal);

		//�ַ������͹���
		explicit HawkScriptValue(const Char* pVal);

		//�ַ������͹���
		explicit HawkScriptValue(const AString& sVal);
		
		//Voidָ��(UserData)���͹���
		explicit HawkScriptValue(const void* pVal);

		//��������
		HawkScriptValue(const HawkScriptValue& sVal);
	
		//��ֵ������
		HawkScriptValue& operator = (const HawkScriptValue& sVal);

		//��Ȳ�����
		Bool operator == (const HawkScriptValue& sVal) const;

		//���Ȳ�����
		Bool operator != (const HawkScriptValue& sVal) const;
	
	public:		
		//����ֵ����
		virtual void	SetType(UInt32 iType);

		//��ȡֵ����
		virtual UInt32  GetType() const;

		//�Ƿ���Ч
		virtual Bool	IsValid() const;

		//�Ƿ�Ϊ��ֵ
		virtual Bool    IsNil() const;

		//�Ƿ�Ϊ��������
		virtual Bool	IsBoolean() const;

		//�Ƿ�Ϊ��ֵ����
		virtual Bool	IsNumber() const;

		//�Ƿ�Ϊ�ַ�������
		virtual Bool	IsString() const;

		//�Ƿ�Ϊ������
		virtual Bool	IsTable() const;

		//�Ƿ�Ϊ�û���������
		virtual Bool	IsUserData() const;		

	public:
		//ȡBool����ֵ
		virtual Bool    GetBoolean() const;

		//ȡInt32����ֵ
		virtual Int32   GetInt32() const;

		//ȡUInt32����ֵ
		virtual UInt32  GetUInt32() const;

		//ȡFloat����ֵ
		virtual Float   GetFloat() const;

		//ȡDouble����ֵ
		virtual Double  GetDouble() const;

		//ȡAString����ֵ
		virtual AString GetString() const;

		//ȡvoid*����ֵ
		virtual void*   GetUserData() const;

		//�ű�ֵ����
		virtual void    Reset();

		//���Table������Ŀ
		virtual Int32   TblGetItemCount() const;

		//��Table�����һ��
		virtual Int32   TblAddItem(const HawkScriptValue& sKey, const HawkScriptValue& sVal);

	public:
		//��ȡTable����ָ��������Key
		const  HawkScriptValue* TblGetKey(Int32 iIdx) const;

		//��ȡTable����ָ��������Value
		const  HawkScriptValue* TblGetValue(Int32 iIdx) const;

		//��ȡTable����ָ��Key��ֵ����
		const  HawkScriptValue* TblGetValue(const AString& sName) const;	

		//��ȡTable����ָ��Key��ֵ����
		const  HawkScriptValue* TblGetValue(const HawkScriptValue& sKey) const;

	public:
		//��ȡ��ֵģ�庯��
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
		//�ű�ֵ�����Ͷ���
		UInt32	m_iType;
		//�ַ�������
		AString	m_sString;
		//��������
		Bool	m_bBoolean;
		//��ֵ����
		Double	m_nNumber;
		//UserData����
		void*	m_pData;
		//������
		struct
		{
			vector<HawkScriptValue> Key;
			vector<HawkScriptValue> Val;
		}m_sTable;
	}; 

	//���ͼ�㶨��
	typedef HawkScriptValue ScriptValue;
}
#endif
