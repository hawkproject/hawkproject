#ifndef HAWK_APPOBJ_H
#define HAWK_APPOBJ_H

#include "HawkMsgManager.h"
#include "HawkProtocolManager.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	/************************************************************************/
	/* Ӧ�ö����װ,��Ҫ������Ϣ��Э��                                      */
	/************************************************************************/
	class UTIL_API HawkAppObj : public HawkRefCounter
	{
	public:
		//����
		HawkAppObj(const XID& sXid = XID());

		//����
		virtual ~HawkAppObj();

		//Ӧ�û��������Ա����
		friend class HawkApp;

		//������Ϣ����
		enum
		{
			HOM_NONE = 0,
			HOM_MSG,
			HOM_PROTO,
		};

		//��Ϣ��¼����
		struct ObjMsg
		{
			UInt32	Type;
			UInt32	Id;
			UInt32  Time;

			ObjMsg(UInt32 iType = 0, UInt32 iId = 0) : Type(iType),Id(iId)
			{
				Time = HawkOSOperator::GetTickCount();
			}
		};
		typedef vector<ObjMsg> MsgTrack;

	public:
		//����Ψһ��ʶID
		virtual XID    GetXid() const;

		//������Ϣ��¼
		virtual Bool   EnableMsgTrack(Bool bEnable);

	protected:
		//��Ϣ��Ӧ
		virtual Bool   OnMessage(const HawkMsg& sMsg);

		//Э����Ӧ
		virtual Bool   OnProtocol(SID iSid, Protocol* pProto);

		//����
		virtual Bool   OnTick();

		//�쳣
		virtual Bool   OnException(Int32 iCode);

	protected:
		//������Ϣ׷�ټ�¼
		virtual void   TrackMsg(UInt32 iType, UInt32 iId);

	protected:
		//����ΨһID
		XID		 m_sXid;
		//�Ƿ�����Ϣ׷��
		Bool	 m_bMsgTrack;
		//������Ϣ�б�
		MsgTrack m_vObjMsg;
	};
}
#endif
