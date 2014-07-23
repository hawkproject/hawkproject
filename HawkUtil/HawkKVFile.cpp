#include "HawkKVFile.h"
#include "HawkDiskFile.h"
#include "HawkStringUtil.h"

namespace Hawk
{
	HawkKVFile::HawkKVFile()
	{

	}

	HawkKVFile::~HawkKVFile()
	{
		m_mKVTbl.clear();
	}

	Bool HawkKVFile::Open(const AString& sFile, HawkFile::OpenType eType)
	{
		HawkDiskFile diskFile;
		if (!diskFile.Open(sFile, HawkFile::OPEN_READ))
			return false;

		return Open(&diskFile);
	}

	Bool HawkKVFile::Open(HawkFile* pFile)
	{
		if (pFile)
		{
			Char szLine[DEFAULT_SIZE] = {0};
			while (pFile->ReadLine(szLine, DEFAULT_SIZE) > 0)
			{
				AString sLine = szLine;
				memset(szLine, 0, DEFAULT_SIZE);
				HawkStringUtil::Trim<AString>(sLine);
				if (sLine.size() && sLine[0] != '#')
				{
					Size_t iPos = sLine.find('=');
					if (iPos > 0 && iPos != AString::npos)
					{
						AString sKey = sLine.substr(0, iPos);
						AString sVal = sLine.substr(iPos+1, sLine.size()-iPos-1);
						HawkStringUtil::Trim<AString>(sKey);
						HawkStringUtil::Trim<AString>(sVal);
						if (sKey.size())
							m_mKVTbl[sKey] = sVal;
					}
				}			
			}
			return true;
		}
		return false;
	}

	Bool HawkKVFile::Save(const AString& sFile)
	{
		HawkDiskFile df;
		if (!df.Open(sFile, HawkFile::OPEN_WRITE))
			return false;

		Char szLine[DEFAULT_SIZE] = {0};
		AStringMap::iterator it = m_mKVTbl.begin();
		for (;it!=m_mKVTbl.end();it++)
		{
			sprintf(szLine, "%s = %s\r\n", it->first.c_str(), it->second.c_str());
			df.Write(szLine, (Int64)strlen(szLine), true);
		}

		return true;
	}

	Bool HawkKVFile::IsKeyExist(const AString& sKey)
	{
		return m_mKVTbl.find(sKey) != m_mKVTbl.end();
	}

	Bool HawkKVFile::QueryValue(const AString& sKey, AString& sVal)
	{
		AStringMap::iterator it = m_mKVTbl.find(sKey);
		if (it != m_mKVTbl.end())
		{
			sVal = it->second;
			return true;
		}
		return false;
	}

	Bool HawkKVFile::SetKV(const AString& sKey, const AString& sVal)
	{
		if (sKey.size() && sVal.size())
		{
			m_mKVTbl[sKey] = sVal;
			return true;
		}
		return false;
	}

	Bool HawkKVFile::DelKey(const AString& sKey)
	{
		AStringMap::iterator it = m_mKVTbl.find(sKey);
		if (it != m_mKVTbl.end())
		{
			m_mKVTbl.erase(it);
			return true;
		}
		return false;
	}

	const AStringMap& HawkKVFile::GetKVTbl() const
	{
		return m_mKVTbl;
	}
}
