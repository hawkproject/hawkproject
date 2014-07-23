#include "HawkMarshal.h"

namespace Hawk
{
	HawkOctetsStream& HawkMarshal::Marshal(HawkOctetsStream& rhsOS)
	{
		T_Exception("HawkMarshal::Marshal Must Implement.");
		return rhsOS;
	}

	HawkOctetsStream& HawkMarshal::Unmarshal(HawkOctetsStream& rhsOS)
	{
		T_Exception("HawkMarshal::Unmarshal Must Implement.");
		return rhsOS;
	}

	Bool HawkMarshal::Clear()
	{
		return true;
	}
}
