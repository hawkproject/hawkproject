#include "HawkUtil.h"

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	HawkUtil::Stop();
	HawkUtil::Release();
	return 0;
}

