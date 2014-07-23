#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

#ifndef _DEBUG
	Bool bLoop = true;
#else
	Bool bLoop = false;
#endif

	Char sHash[PAGE_SIZE]   = {0};
	
	//提示输入
	if (argc >= 2)
	{
		//读取参数
		memcpy(sHash, argv[1], strlen(argv[1]));
		bLoop = false;
	}
	
	do 
	{
		if (bLoop)
		{
			memset(sHash, 0, PAGE_SIZE);
			HawkPrint("Utf8 String: ", false);
			scanf("%s", sHash);
		}

		UInt32 iHash = HawkStringUtil::HashString<UString>(_Utf8(sHash));		
		HawkFmtPrint("Hash: %u", iHash);

	} while (bLoop);

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
