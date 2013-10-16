#include "HawkRand.h"

namespace Hawk
{
	static const Int32 A = 48271;
	static const Int32 M = 2147483647;
	static const Int32 Q = M / A;
	static const Int32 R = M % A;
	static Int32   State = -1;

	Int32 HawkRand::RandInt()
	{
		if (State <0 )
		{
			srand((UInt32)(time(0)));
			State = rand();
		}
		
		Int32 tmpState = A * (State % Q) - R * (State / Q);
		if(tmpState >= 0)
			State = tmpState;
		else
			State = tmpState + M;

		return State;
	}

	Int32 HawkRand::RandInt(Int32 iMax)
	{
		return RandInt() % (iMax + 1);
	}

	Int32 HawkRand::RandInt(Int32 iLow, Int32 iHigh)
	{
		A_Exception(iHigh >= iLow && "Random Range Error.");
		return RandInt(iHigh - iLow) + iLow;
	}
	
	Float HawkRand::RandFloat()
	{
		return (Float)RandInt() / (Float)M;
	}

	Float HawkRand::RandFloat(Float fMax)
	{
		return RandFloat() * fMax;
	}

	Float HawkRand::RandFloat(Float fLow, Float fHigh)
	{
		A_Exception(fHigh >= fLow && "Random Range Error.");
		return RandFloat(fHigh - fLow) + fLow;
	}
}
