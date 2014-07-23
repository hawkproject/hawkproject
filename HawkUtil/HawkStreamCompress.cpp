#include "HawkStreamCompress.h"
#include "zlib.h"

namespace Hawk
{
	HawkStreamCompress::HawkStreamCompress() : m_sStream(), m_sOctets(PAGE_SIZE)
	{
		m_pStream[0] = HawkMalloc(sizeof(z_stream));
		m_pStream[1] = HawkMalloc(sizeof(z_stream));
		memset(m_pStream[0], 0, sizeof(z_stream));
		memset(m_pStream[1], 0, sizeof(z_stream));

		Int32 iRet  = HAWK_OK;

		//压缩组件
		iRet = deflateInit((z_stream*)m_pStream[0], Z_DEFAULT_COMPRESSION);
		HawkAssert(iRet == HAWK_OK);

		//解压组件
		iRet = inflateInit((z_stream*)m_pStream[1]);
		HawkAssert(iRet == HAWK_OK);
	}

	HawkStreamCompress::~HawkStreamCompress()
	{ 
		if (m_pStream[0])
		{
			HawkFree(m_pStream[0]);
			m_pStream[0] = 0;
		}

		if (m_pStream[1])
		{
			HawkFree(m_pStream[1]);
			m_pStream[1] = 0;
		}
	}

	Bool HawkStreamCompress::Compress(HawkOctets& xOctets)
	{
		m_sOctets.Clear();		
		m_sStream.Clear();

		z_stream* p = (z_stream*)m_pStream[0];
		p->next_in  = (Bytef*)xOctets.Begin();  
		p->avail_in = xOctets.Size();

		while (p->avail_in > 0)
		{
			p->next_out  = (Bytef*)m_sOctets.Begin();  
			p->avail_out = m_sOctets.Capacity();  

			if (deflate(p, Z_SYNC_FLUSH) != HAWK_OK)
				return false;

			Int32 iSize = m_sOctets.Capacity() - p->avail_out;
			m_sOctets.Resize(iSize);

			if (p->avail_in == 0)
			{
				if (!m_sStream.Size())
				{
					xOctets.Replace(m_sOctets.Begin(), m_sOctets.Size());
				}
				else
				{
					xOctets.Replace(m_sStream.Begin(), m_sStream.Size());
					xOctets.Append(m_sOctets.Begin(),  m_sOctets.Size());
				}
				return true;
			}
			else
			{
				m_sStream.Push(m_sOctets.Begin(), m_sOctets.Size());
			}
		}
		return false;
	}

	Bool HawkStreamCompress::Uncompress(HawkOctets& xOctets)
	{
		m_sOctets.Clear();		
		m_sStream.Clear();

		z_stream* p = (z_stream*)m_pStream[1];
		p->next_in  = (Bytef*)xOctets.Begin();  
		p->avail_in = xOctets.Size(); 				

		while (p->avail_in > 0)
		{
			p->next_out  = (Bytef*)m_sOctets.Begin();  
			p->avail_out = m_sOctets.Capacity();  

			if (inflate(p, Z_SYNC_FLUSH) != HAWK_OK)
				return false;

			Int32 iSize = m_sOctets.Capacity() - p->avail_out;
			m_sOctets.Resize(iSize);

			if (p->avail_in == 0)
			{
				if (!m_sStream.Size())
				{
					xOctets.Replace(m_sOctets.Begin(), m_sOctets.Size());
				}
				else
				{
					xOctets.Replace(m_sStream.Begin(), m_sStream.Size());
					xOctets.Append(m_sOctets.Begin(),  m_sOctets.Size());
				}
				return true;
			}
			else
			{
				m_sStream.Push(m_sOctets.Begin(),m_sOctets.Size());
			}
		}
		return false;
	}
}
