#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	AString sAddr = "*:8080";	
	if (argc >= 2) sAddr = argv[1];

	AString sLocalDir = "Files/";
	if (argc >= 3) sLocalDir = argv[2];

	HawkFileAcceptor sAcceptor;
	if (sAcceptor.Init(sAddr, sLocalDir))
	{
		HawkFmtPrint("FileAcceptor Is Running......");
		sAcceptor.Run();
	}
	else
	{
		HawkFmtError("Run FileAcceptor Failed, Address: %s", sAddr.c_str());
	}

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

