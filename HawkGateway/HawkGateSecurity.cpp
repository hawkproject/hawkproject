#include "HawkGateSecurity.h"
#include "HawkZlib.h"
#include "zlib.h"

namespace Hawk
{
	HawkGateSecurity::HawkGateSecurity(UInt8 iType) : m_iType(iType), m_sOctets(PAGE_SIZE)
	{
		m_pZStream = HawkMalloc(sizeof(z_stream));
		memset(m_pZStream, 0, sizeof(z_stream));

		if (m_iType == SECURITY_ENCODE)
		{
			if (HawkZip::DeflateInit(m_pZStream, Z_DEFAULT_COMPRESSION) != HAWK_OK)
				m_iType = SECURITY_NONE;
		}
		else if (m_iType == SECURITY_DECODE)
		{
			if (HawkZip::InflateInit(m_pZStream) != HAWK_OK)
				m_iType = SECURITY_NONE;
		}
	}

	HawkGateSecurity::~HawkGateSecurity()
	{ 
		if (m_pZStream)
		{
			HawkFree(m_pZStream);
			m_pZStream = 0;
		}
	}

	Bool HawkGateSecurity::Update(HawkOctets& xOctets)
	{
		if (m_iType == SECURITY_ENCODE)
		{
			return Encode(xOctets);
		}
		else if (m_iType == SECURITY_DECODE)
		{
			return Decode(xOctets);
		}
		return true;
	}

	Bool HawkGateSecurity::Encode(HawkOctets& xOctets)
	{
		m_sOctets = xOctets;
		xOctets.Clear();

		z_stream* p = (z_stream*)m_pZStream;
		p->next_in  = (Bytef*)m_sOctets.Begin();  
		p->avail_in = m_sOctets.Size();

		while (p->avail_in > 0)
		{
			UChar sOutBuf[PAGE_SIZE] = {0};
			p->next_out  = sOutBuf;  
			p->avail_out = PAGE_SIZE;  

			//流压缩
			if (HawkZip::Deflate(p, Z_SYNC_FLUSH) != HAWK_OK)
			{
				xOctets = m_sOctets;
				return false;
			}

			UInt32 iSize = PAGE_SIZE - p->avail_out;

			//流位反
			for (UInt32 i=0;i<iSize;i++)
				sOutBuf[i] = (~sOutBuf[i]);

			xOctets.Append(sOutBuf, iSize);

			if (p->avail_in == 0)
				return true;
		}

		xOctets = m_sOctets;
		return false;
	}

	Bool HawkGateSecurity::Decode(HawkOctets& xOctets)
	{
		m_sOctets = xOctets;
		xOctets.Clear();

		z_stream* p = (z_stream*)m_pZStream;
		p->next_in  = (Bytef*)m_sOctets.Begin();  
		p->avail_in = m_sOctets.Size(); 				

		while (p->avail_in > 0)
		{
			UChar sOutBuf[PAGE_SIZE] = {0};
			p->next_out  = sOutBuf;  
			p->avail_out = PAGE_SIZE;  

			if (HawkZip::Inflate(p, Z_SYNC_FLUSH) != HAWK_OK)
			{
				xOctets = m_sOctets;
				return false;
			}

			UInt32 iSize = PAGE_SIZE - p->avail_out;

			//流位反
			for (UInt32 i=0;i<iSize;i++)
				sOutBuf[i] = (~sOutBuf[i]);

			xOctets.Append(sOutBuf, iSize);

			if (p->avail_in == 0)
				return true;
		}

		xOctets = m_sOctets;
		return false;
	}
}
