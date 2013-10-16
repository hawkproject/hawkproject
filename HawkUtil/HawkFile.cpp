#include "HawkFile.h"

namespace Hawk
{
	HawkFile::HawkFile()
	{
		m_iFileSize = 0;
		m_eOpenType = OPEN_READ;
	}

	HawkFile::~HawkFile(void)
	{
		Close();
	}

	Bool HawkFile::Open(const AString& sFile, OpenType eOpen)
	{
		Close();
		return true;
	}
	
	Int64 HawkFile::Read(void* pData, Int64 iSize)
	{
		return 0;
	}

	Int64 HawkFile::Write(const void* pData, Int64 iSize, Bool bFlush)
	{
		return 0;
	}

	Int64 HawkFile::Seek(Int64 iSize, SeekPos ePos)
	{
		return 0;
	}

	Bool HawkFile::Flush()
	{
		return true;
	}

	Bool HawkFile::Close()
	{
		return true;
	}

	Int64 HawkFile::Tell()
	{
		return 0;
	}

	HawkFile::CodeType HawkFile::GetCodeType(const void* pData)
	{
		const  UChar UTF8_TAG[]		= {0xEF,0xBB,0xBF};
		const  UChar UNICODE_TAG[]	= {0xFF,0xFE};

		UChar* pTmp  = (UChar*)pData;
		Size_t iSize = strlen((Char*)pData);

		if (iSize >= 3)
		{
			if(pTmp[0] == UTF8_TAG[0] && pTmp[1] == UTF8_TAG[1] && pTmp[2] == UTF8_TAG[2])
				return CODE_UTF8;
		}

		if (iSize >= 2)
		{
			if(pTmp[0] == UNICODE_TAG[0] && pTmp[1] == UNICODE_TAG[1])
				return CODE_UNICODE;
		}

		return CODE_ASCII;
	}

	Bool  HawkFile::Chsize(Int64 iSize)
	{
		return false;
	}

	Bool HawkFile::IsWriteAble() const
	{
		if (m_eOpenType != OPEN_WRITE &&  m_eOpenType != OPEN_RW &&  m_eOpenType != OPEN_APPEND && m_eOpenType != OPEN_WR)
		{
			return false;
		}

		return true;
	}

	Bool HawkFile::IsReadAble() const
	{
		if (m_eOpenType != OPEN_READ && m_eOpenType != OPEN_RW && m_eOpenType != OPEN_WR)
		{
			return false;
		}

		return true;
	}
}

