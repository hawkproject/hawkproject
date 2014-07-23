#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	AString sSendFile = "";
	if (argc >= 2) sSendFile = argv[1];

	AString sAddr = "127.0.0.1:8080";	
	if (argc >= 3) sAddr = argv[2];

	AString sFileName = HawkOSOperator::SplitFileName(sSendFile);
	if (sSendFile.size() && sAddr.size())
	{
		HawkFileSender sSender;
		if (sSender.PostFile(sAddr, sFileName, sSendFile))
		{
			HawkFmtLog("PostFile Successful, File: %s", sSendFile.c_str());
		}
		else
		{
			HawkFmtLog("PostFile Failed, File: %s", sSendFile.c_str());
		}
	}
	
	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

