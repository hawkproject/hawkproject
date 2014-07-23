#include "HawkUtil.h"
#include "HawkLog.h"
#include "HawkGeometry.h"
#include "HawkProfiler.h"
#include "HawkGateProxy.h"

int main(int argc, Char* argv[])
{	
	HawkUtil::Init();

	HawkUtil::Stop();

	HawkUtil::Release();

	return 0;
}
