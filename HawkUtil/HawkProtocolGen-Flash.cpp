#include "HawkProtocolManager.h"
#include "HawkProtocolGenDef.h"

namespace Hawk
{
	extern ProtocolSchema* g_ProtocolSchema;

	Bool GenProtocols_Flash(ProtocolSchema& sSchema)
	{
		HawkOSOperator::CreateDir("flash/");
		return false;
	}
}
