#include "HawkMsg.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	HawkMsg::HawkMsg(Int32 iMsg, const XID& sTarId, const XID& sSrcId)
	{
		SysFlag = 0;
		UsrFlag = 0;
		Msg		= iMsg;
		Target	= sTarId;
		Source	= sSrcId;		
		Time	= HawkOSOperator::GetTickCount();
	}

	HawkMsg::~HawkMsg()
	{
	}

	HawkMsg& HawkMsg::operator = (const HawkMsg& sMsg)
	{
		if (this != &sMsg)
		{
			Msg	    = sMsg.Msg;
			Time    = sMsg.Time;
			Target  = sMsg.Target;
			Source  = sMsg.Source;
			UsrFlag = sMsg.UsrFlag;
			SysFlag = sMsg.SysFlag;
			Params  = sMsg.Params;
		}
		return *this;
	}

	Int32 HawkMsg::CopyParams(const ParamVector& vParams)
	{
		Params.Copy(vParams);
		return Params.Size();
	}

	Bool HawkMsg::IsValid() const
	{
		return Msg > 0 && Target.IsValid();
	}

	Bool HawkMsg::Clear()
	{
		Time    = 0;
		UsrFlag = 0;
		SysFlag = 0;
		Target.Clear();
		Source.Clear();
		Params.Clear();
		return true;
	}
}
