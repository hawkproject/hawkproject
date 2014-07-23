#ifndef HAWK_MSG_H
#define HAWK_MSG_H

#include "HawkXID.h"
#include "HawkParamVector.h"

namespace Hawk
{
	/************************************************************************/
	/* ������Ϣ��װ����                                                     */
	/************************************************************************/
	class UTIL_API HawkMsg : public HawkRefCounter
	{
	public:
		//����
		HawkMsg(Int32 iMsg = 0, const XID& sTarId = XID(), const XID& sSrcId = XID());

		//����
		virtual ~HawkMsg();

		//��ֵ������(������ʹ�ø�ֵ�Ϳ�������,������ParamVector�Ŀ�������)
		HawkMsg& operator = (const HawkMsg& sMsg);

		//�����������Ͷ���
		typedef  HawkParamVector ParamVector;

		//�������
		enum
		{			
			SYS_CREATE = 0x01,
		};

	public:
		//�������п���
		virtual Int32 CopyParams(const ParamVector& vParams);

		//�ж���Ϣ�Ƿ���Ч
		virtual Bool  IsValid() const;

		//�������
		virtual Bool  Clear();
		
	public:
		//��Ϣ������Ϣ
		Int32		Msg;
		//��Ϣ����ʱ��
		UInt32		Time;
		//��ϢĿ��ID
		XID			Target;
		//��Ϣ��ԴID
		XID			Source;
		//��Ϣ�����б�
		ParamVector	Params;
		//��Ϣϵͳ���(Ӧ�ò��ɲ���)
		UInt32		SysFlag;
		//��Ϣ�û����
		UInt32		UsrFlag;
	};	
}
#endif
