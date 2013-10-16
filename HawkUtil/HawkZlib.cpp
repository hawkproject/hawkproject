#include "HawkZlib.h"
#include "zlib.h"

namespace Hawk
{
	ULong HawkZip::GetRequiredSize(ULong iOriginal)
	{
		return compressBound(iOriginal);
	}

	Bool HawkZip::Compress(void* pOut, ULong& iOutLen, const void* pIn, ULong iInLen)
	{
		Int32 code  = compress2((Byte*)pOut, &iOutLen,(const Byte*)pIn, iInLen, Z_DEFAULT_COMPRESSION);
		if (code == Z_OK)
			return true;

		return false;
	}

	Bool HawkZip::UnCompress(void* pOut, ULong& iOutLen, const void* pIn, ULong iInLen)
	{ 
		Int32 code  = uncompress((Bytef *)pOut, &iOutLen,(const Bytef *)pIn, iInLen);
		if (code == Z_OK)
			return true;

		return false;
	}

	Bool HawkZip::DeflateInit(void* pStream, Int32 iLvl)
	{
		Int32 code = deflateInit((z_stream*)pStream, iLvl);

		if (code == Z_OK)
			return true;

		return false;
	}

	Bool HawkZip::InflateInit(void* pStream)
	{
		Int32 code = inflateInit((z_stream*)pStream);

		if (code == Z_OK)
			return true;

		return false;
	}

	Bool HawkZip::Deflate(void* pStream, Int32 iFlush)
	{
		Int32 code = deflate((z_stream*)pStream, iFlush);
		
		if (code == Z_OK)
			return true;

		return false;
	}

	Bool HawkZip::Inflate(void* pStream, Int32 iFlush)
	{
		Int32 code = inflate((z_stream*)pStream, iFlush);

		if (code == Z_OK)
			return true;

		return false;
	}
}
