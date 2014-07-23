#include "HawkDiskFile.h"

namespace Hawk
{
	HawkDiskFile::HawkDiskFile()
	{
		m_pFile = 0;
	}

	HawkDiskFile::~HawkDiskFile()
	{
		Close();
	}

	Bool HawkDiskFile::Open(const AString& sFile, OpenType eOpen)
	{
		Close();

		m_eOpenType   = eOpen;
		m_sFileName   = sFile;

		AString sMode = "rb";
		if (m_eOpenType == OPEN_READ)
		{
			sMode = "rb";
		}
		else if (m_eOpenType == OPEN_WRITE)
		{
			sMode = "wb";
		}
		else if (m_eOpenType == OPEN_RW)
		{
			sMode = "r+b";
		}
		else if (m_eOpenType == OPEN_WR)
		{
			sMode = "w+b";
		}
		else if (m_eOpenType == OPEN_APPEND)
		{
			sMode = "a+b";
		}

		if (sFile.size())
		{
			m_pFile = fopen(m_sFileName.c_str(), sMode.c_str());

			if (!m_pFile) 
				return false;

			Seek(0,POS_END);
			m_iFileSize = Tell();
			Seek(0,POS_BEGIN);

			return true;
		}
		return false;
	}

	Bool HawkDiskFile::WriteCodeType(CodeType eType)
	{
		const  UChar UTF8_TAG[]		= {0xEF,0xBB,0xBF};
		const  UChar UNICODE_TAG[]	= {0xFF,0xFE};

		if (eType == CODE_UTF8)
		{
			return Write(UTF8_TAG, 3) > 0;
		} 
		else if (eType == CODE_UNICODE)
		{
			return Write(UNICODE_TAG, 2) > 0;
		}
		return true;
	}

	Int64 HawkDiskFile::Read(void* pData, Int64 iSize)
	{
		if (pData && m_pFile && iSize)
		{
			return fread(pData, 1, (Size_t)iSize, m_pFile);
		}
		return 0;
	}

	Int64 HawkDiskFile::ReadLine(void* pData, Int64 iBufSize, UChar cSeparate)
	{
		Int64 iRead = 0;
		UChar cRead = 0;

		while (pData && iRead <= iBufSize && cRead != cSeparate)
		{
			UChar* pTmp = (UChar*)pData + iRead;
			Int64  iTmp = Read(&cRead,1);
			if(iTmp == 1)
			{
				*pTmp = cRead;
				iRead++;
			}
			else
			{
				break;
			}
		}
		return iRead;
	}

	Bool HawkDiskFile::IsEndPos()
	{
		return Tell() == m_iFileSize;
	}

	Int64 HawkDiskFile::Write(const void* pData, Int64 iSize, Bool bFlush)
	{
		if (pData && m_pFile && iSize)
		{
			Size_t iWrite = fwrite(pData, 1, (Size_t)iSize, m_pFile);
			if(bFlush) 
				Flush();

			//记录写入文件大小
			m_iFileSize += (Int64)iWrite;

			return (Int64)iWrite;
		}
		return 0;
	}	

	Bool  HawkDiskFile::Chsize(Int64 iSize)
	{
		if(!m_pFile) return false;

		if(m_eOpenType == OPEN_WRITE || m_eOpenType == OPEN_RW || m_eOpenType == OPEN_APPEND)
		{
			Seek(0);

			return _chsize(_fileno(m_pFile), (long)iSize) == 0? true:false;
		}
		return false;
	}

	Bool HawkDiskFile::Flush()
	{
		if (m_pFile)
		{
			fflush(m_pFile);
			return true;
		}
		return false;
	}

	Bool HawkDiskFile::Close()
	{
		if (m_pFile) 
		{
			fclose(m_pFile);
			m_pFile		= 0;
			m_iFileSize	= 0;
		}
		return true;
	}

	Int64 HawkDiskFile::Seek(Int64 iOffset, SeekPos ePos)
	{
		if (m_pFile)
		{
			return _fseek64(m_pFile, iOffset, ePos);
		}
		return 0;
	}

	Int64 HawkDiskFile::Tell()
	{
		if (m_pFile)
		{
			Int64 iPos = _ftell64(m_pFile);
			return iPos;
		}
		return 0;
	}

	FILE* HawkDiskFile::GetFileHandle()
	{
		return m_pFile;
	}
}
