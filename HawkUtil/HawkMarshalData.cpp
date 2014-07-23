#include "HawkMarshalData.h"

namespace Hawk
{
	HawkMarshalData* HawkMarshalData::Clone() const
	{
		T_Exception("HawkMarshalData::Clone Must Implement.");
		return 0;
	}

	HawkMarshalData& HawkMarshalData::operator = (const HawkMarshalData& rhs)
	{		
		T_Exception("HawkMarshalData::operator =  Must Implement.");
		return *this;
	}	
}
