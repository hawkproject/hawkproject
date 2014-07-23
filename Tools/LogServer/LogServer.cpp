#include "HawkUtil.h"
#include "HawkLog.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	AString sCfg = "Config/ls.cfg";
	if (argc >= 2)
		sCfg = argv[1];

	HawkLogServer* pLogger = new HawkLogServer;

	if (!pLogger->InitCfg(sCfg))
		HawkFmtPrint("LogServer Init Error.");
	
	if (!pLogger->Run())
		HawkFmtPrint("LogServer Run Error.");

	HAWK_RELEASE(pLogger);

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
