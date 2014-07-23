#include "HawkBinCfgFile.h"
#include "HawkStringUtil.h"
#include "HawkDiskFile.h"
#include "HawkScope.h"
#include "HawkZlib.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	HawkBinCfgFile::HawkBinCfgFile()
	{
		m_pFile = 0;
	}

	HawkBinCfgFile::~HawkBinCfgFile()
	{
		HAWK_RELEASE(m_pFile);
	}

	Bool HawkBinCfgFile::LoadCfgData(const AString& sFile)
	{
		HawkDiskFile src_file;
		if (src_file.Open(sFile))
		{
			//���Ⱥ�CRC��ȡ
			Int64 iSize	  = src_file.GetFileSize();
			Int32 iUnSize = 0;
			UInt32 iCrc	  = 0;

			if(src_file.Read(&iUnSize,sizeof(iUnSize)) != sizeof(iUnSize))
				return false;

			Int64 iComSize = iSize - (Int64)sizeof(iUnSize);
			if(src_file.Read(&iCrc,sizeof(iCrc)) != sizeof(iCrc))
				return false;

			iComSize -= (Int64)sizeof(iCrc);

			Char* pComData = new Char[(Size_t)iComSize];
			HawkScope::DataArrayPtr comdata_scope(pComData);
			memset(pComData, 0, (Size_t)iComSize);
			if (src_file.Read(pComData,iComSize) != iComSize)
				return false;

			if (iCrc != HawkOSOperator::CalcCrc((const UChar*)pComData, (UInt32)iComSize))
				return false;

			//��λ������
			for (Int64 i=0;i<iComSize;i++)
				pComData[i] = (~pComData[i]);

			Char* pSrcData = new Char[iUnSize];
			HawkScope::DataArrayPtr srcdata_scope(pSrcData);
			memset(pSrcData,0,iUnSize);

			ULong iSrcSize = (ULong)iUnSize;
			if (!HawkZip::UnCompress(pSrcData, iSrcSize, pComData, (ULong)iComSize))
				return false;

			HAWK_RELEASE(m_pFile);
			m_pFile = new HawkMemoryFile();
			if (!m_pFile->Open(pSrcData, iSrcSize, false))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkBinCfgFile::LoadFromMem(const void* pData, UInt32 iSize)
	{
		const UChar* pDataPtr = (const UChar*)pData;
		if (pData && iSize)
		{		
			//���Ⱥ�CRC��ȡ
			Int32 iUnSize  = *((Int32*)pDataPtr);
			pDataPtr	   += sizeof(Int32);
			UInt32 iCrc	   = *((UInt32*)pDataPtr);
			pDataPtr	   += sizeof(UInt32);
			Int64 iComSize = iSize - (Int64)(sizeof(iUnSize) + sizeof(iCrc));
			if (iCrc != HawkOSOperator::CalcCrc((const UChar *)pDataPtr, (UInt32)iComSize))
				return false;

			//��������
			Char* pComData  = new Char[(Size_t)iComSize];
			HawkScope::DataArrayPtr comdata_scope(pComData);
			memcpy(pComData, pDataPtr, (Size_t)iComSize);

			//��λ������
			for (Int64 i=0;i<iComSize;i++)
				pComData[i] = (~pComData[i]);

			Char* pSrcData = new Char[iUnSize];
			HawkScope::DataArrayPtr srcdata_scope(pSrcData);
			memset(pSrcData,0,iUnSize);

			ULong iSrcSize = (ULong)iUnSize;
			if (!HawkZip::UnCompress(pSrcData, iSrcSize, pComData, (ULong)iComSize))
				return false;

			HAWK_RELEASE(m_pFile);
			m_pFile = new HawkMemoryFile();
			if (!m_pFile->Open(pSrcData, iSrcSize, false))
				return false;

			return true;
		}
		return false;
	}

	Int64 HawkBinCfgFile::GetFileSize() const
	{
		if (m_pFile)
			return m_pFile->GetFileSize();

		return 0;
	}

	HawkMemoryFile* HawkBinCfgFile::GetMemFile()
	{
		return m_pFile;
	}

	Int64 HawkBinCfgFile::Read(void* pData,Int64 iSize)
	{
		if (m_pFile)
			return m_pFile->Read(pData, iSize);

		return 0;
	}

	Int64 HawkBinCfgFile::Seek(Int64 iOffset,HawkFile::SeekPos ePos)
	{
		if (m_pFile)
			return m_pFile->Seek(iOffset,ePos);

		return 0;
	}

	Bool HawkBinCfgFile::CompressToFile(const void* pData, ULong iSize, const AString& sDstFile)
	{
		//����ѹ��
		ULong lComSize  = HawkZip::GetRequiredSize(iSize);
		Char* pCompress = new Char[lComSize];
		memset(pCompress, 0, lComSize);
		HawkScope::DataArrayPtr scope(pCompress);
		if (!HawkZip::Compress(pCompress, lComSize, pData, iSize))
			return false;

		//ѹ������λ������
		for (ULong i=0;i<lComSize;i++)
			pCompress[i] = (~pCompress[i]);

		//ѹ�����CRCУ��
		UInt32 iCrc = HawkOSOperator::CalcCrc((const UChar*)pCompress, (UInt32)lComSize);

		HawkDiskFile com_file;
		if (com_file.Open(sDstFile, HawkFile::OPEN_WRITE))
		{
			com_file.Write(&iSize, sizeof(iSize));
			com_file.Write(&iCrc, sizeof(iCrc));
			com_file.Write(pCompress, lComSize);
			com_file.Close();
			return true;
		}
		
		return false;
	}
}
