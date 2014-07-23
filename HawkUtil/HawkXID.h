#ifndef HAWK_XID_H
#define HAWK_XID_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* ȫ��Ψһ��ʶ�����Ͷ���                                               */
	/************************************************************************/
	struct XID
	{
		//����
		UInt32 Type;

		//IDֵ
		UInt32 Id;

		//Ĭ�Ϲ���
		XID() : Type(0),Id(0) 
		{
		}

		//���͹���
		XID(UInt32 iType) : Type(iType),Id(0) 
		{
		}

		//ֱ���������ͺ�ID
		XID(UInt32 iType, UInt32 iId) : Type(iType),Id(iId) 
		{
		}

		//��������
		XID(const XID& sXid) : Type(sXid.Type),Id(sXid.Id) 
		{
		}

		//��ֵ������
		XID& operator = (const XID& sXid)
		{
			if(this != &sXid)
			{
				Type = sXid.Type;
				Id   = sXid.Id;
			}			
			return *this;
		}

		//�������ͺ�ID
		inline void Set(UInt32 iType, UInt32 iId)
		{
			Type = iType;
			Id   = iId;
		}

		//�������
		inline void Clear()
		{
			Type = 0;
			Id   = 0;
		}

		//��ȱȽ�
		inline Bool operator == (const XID& sXid) const
		{
			return Type == sXid.Type && Id == sXid.Id;
		}

		//���ȱȽ�
		inline Bool operator != (const XID& sXid) const
		{
			return Type != sXid.Type || Id != sXid.Id;
		}

		//С�ڱȽ�
		inline Bool operator < (const XID & sXid) const
		{
			if (Type < sXid.Type)
				return true;
			else if(Type > sXid.Type)
				return false;

			return Id < sXid.Id;
		}

		//��Ч���ж�
		inline Bool IsValid() const 
		{ 
			return Type > 0; 
		}
	};

	//ID�б���
	typedef vector<XID>	  XIDVector;
	//IDӳ�����
	typedef map<XID, XID> XIDMap;
}
#endif
