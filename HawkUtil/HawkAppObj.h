#ifndef HAWK_APPOBJ_H
#define HAWK_APPOBJ_H

#include "HawkMsgManager.h"
#include "HawkProtocolManager.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	/************************************************************************/
	/* 应用对象封装,主要处理消息和协议                                      */
	/************************************************************************/
	class UTIL_API HawkAppObj : public HawkRefCounter
	{
	public:
		//构造
		HawkAppObj(const XID& sXid = XID());

		//析构
		virtual ~HawkAppObj();

		//应用基础类的友员访问
		friend class HawkApp;

		//对象消息类型
		enum
		{
			HOM_NONE = 0,
			HOM_MSG,
			HOM_PROTO,
		};

		//消息记录对象
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
		//对象唯一标识ID
		virtual XID    GetXid() const;

		//开启消息记录
		virtual Bool   EnableMsgTrack(Bool bEnable);

	protected:
		//消息响应
		virtual Bool   OnMessage(const HawkMsg& sMsg);

		//协议响应
		virtual Bool   OnProtocol(SID iSid, Protocol* pProto);

		//更新
		virtual Bool   OnTick();

	protected:
		//对象消息追踪记录
		virtual void   TrackMsg(UInt32 iType, UInt32 iId);

	protected:
		//对象唯一ID
		XID		 m_sXid;
		//是否开启消息追踪
		Bool	 m_bMsgTrack;
		//对象消息列表
		MsgTrack m_vObjMsg;
	};
}
#endif
