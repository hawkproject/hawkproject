#include "HawkPackage.h"
#include "HawkDiskFile.h"
#include "HawkMemoryFile.h"
#include "HawkLoggerManager.h"
#include "HawkOSOperator.h"
#include "HawkStringUtil.h"
#include "HawkZlib.h"
#include "HawkScope.h"

namespace Hawk
{
	HawkPackage::HawkPackage(const AString& sPackFile, Bool bOnlyRead) : m_pPackFile(0)
	{
		if(sPackFile.size())
		{
			OpenPackage(sPackFile,bOnlyRead);
		}
	}
	
	HawkPackage::HawkPackage(void* pData,Int32 iSize)
	{
		if(pData && iSize > 0)
		{
			OpenPackage(pData,iSize);
		}
	}

	HawkPackage::~HawkPackage()
	{
		Close();
	}

	Bool  HawkPackage::OpenPackage(const AString& sDiskFile, Bool bOnlyRead)
	{		
		HAWK_RELEASE(m_pPackFile);
		if(!m_pPackFile) 
			m_pPackFile = new HawkDiskFile;

		Bool bOpenOK = false;
		m_bReadOnly  = bOnlyRead;

		if(m_bReadOnly)
		{
			if(m_pPackFile->Open(sDiskFile))
				bOpenOK = true;
		}
		else
		{
			if(HawkOSOperator::ExistFile(sDiskFile))
			{
				if(m_pPackFile->Open(sDiskFile, HawkFile::OPEN_RW))
					bOpenOK = true;
			}
			else
			{	
				m_sPkgHeader.PackVersion = PACKAGE_VERSION;
				m_sPkgHeader.PackMagic   = HAWKFOURCC('H', 'A', 'W', 'K');
				if(m_pPackFile->Open(sDiskFile,HawkFile::OPEN_WR))
				{
					m_pPackFile->Write(&m_sPkgHeader, sizeof(m_sPkgHeader), true);
					bOpenOK = true;
					return true;
				}
			}			
		}

		if(!bOpenOK)
		{
			HAWK_RELEASE(m_pPackFile);
			return false;
		}

		return Analyze();
	}

	Bool HawkPackage::OpenPackage(const void* pData, UInt32 iSize)
	{
		HAWK_RELEASE(m_pPackFile);
		HawkMemoryFile* pMemFile = new HawkMemoryFile;

		m_pPackFile = pMemFile;
		m_bReadOnly = true;

		if(!pMemFile->Open((void*)pData, (Int64)iSize, false))
		{
			HAWK_RELEASE(m_pPackFile);
			return false;
		}

		return Analyze();
	}

	Bool HawkPackage::Analyze()
	{
		if(m_pPackFile)
		{
			//��ȡ�ļ���ͷ
			if(m_pPackFile->Read(&m_sPkgHeader, sizeof(m_sPkgHeader)) != sizeof(m_sPkgHeader)) 
				return false;

			if (m_sPkgHeader.PackMagic != HAWKFOURCC('H', 'A', 'W', 'K'))
				return false;

			UInt32 iFileCount[2] = {0};
			UInt32 iTotalFileCnt = m_sPkgHeader.FileCount[0] + m_sPkgHeader.FileCount[1];

			for (UInt32 i=0;i<iTotalFileCnt;i++)
			{
				PkgFile sInfo;

				//��ȡ������Ϣ
				if (m_pPackFile->Read(&sInfo, sizeof(sInfo)) != sizeof(sInfo))
					return false;

				if (sInfo.NameSize <= 0 || sInfo.DataOffset != (UInt64)m_pPackFile->Tell() + (UInt64)sInfo.NameSize)
					return false;	
				
				Char  sCompName[DEFAULT_SIZE] = {0};
				Char  sFileName[DEFAULT_SIZE] = {0};

				//��ȡ�ļ���
				if(m_pPackFile->Read(sCompName,sInfo.NameSize) != sInfo.NameSize) 
					return false;

				//��ѹ�ļ���
				ULong iBufSize = DEFAULT_SIZE;
				if(!HawkZip::UnCompress(sFileName,iBufSize, sCompName, sInfo.NameSize)) 
					return false;

				if (sInfo.Valid)
				{
					iFileCount[0] ++;
					m_mFileInfo[sFileName] = sInfo;
				}
				else
				{
					iFileCount[1] ++;
				}			

				m_pPackFile->Seek(sInfo.DataSize, HawkFile::POS_CURRENT);
			}

			//��Ч����Ч�ļ���У��
			if (m_sPkgHeader.FileCount[0] == iFileCount[0] && m_sPkgHeader.FileCount[1] == iFileCount[1])
				 return true;
		}
		return false;
	}

	const HawkPackage::PkgHeader* HawkPackage::GetPkgHeader() const
	{
		if(m_pPackFile)
			return &m_sPkgHeader;

		return 0;
	}

	const HawkPackage::PkgFile*  HawkPackage::GetFileInfo(const AString& sPackFile)
	{
		if(m_pPackFile)
		{
			PkgFileMap::iterator it = m_mFileInfo.find(sPackFile);
			if (it != m_mFileInfo.end() && it->second.Valid)
				return &it->second;
		}
		return 0;
	}
	
	Bool HawkPackage::ExistFile(const AString& sPackFile)
	{
		return GetFileInfo(sPackFile) != 0? true : false;
	}

	Bool HawkPackage::ExistFolder(const AString& sPackFolder)
	{
		PkgFileMap::iterator it = m_mFileInfo.begin();
		for (;it!=m_mFileInfo.end();it++)
		{
			if (it->first.find(sPackFolder) == 0 && it->first != sPackFolder)
				return true;
		}
		return false;
	}

	Bool HawkPackage::Read(void* pData, UInt32& iSize, const AString& sPackFile)
	{
		const PkgFile* pInfo = GetFileInfo(sPackFile);
		if(pData && pInfo && iSize >= pInfo->OrigSize)
		{
			//׼��������
			UInt32 iComp = pInfo->DataSize;
			Char* pComp  = new Char[iComp];
			memset(pComp, 0, iComp);
			HawkScope::DataArrayPtr scope(pComp);
			
			m_pPackFile->Seek(pInfo->DataOffset);

			//��ȡѹ������
			UInt32 iRead = (UInt32)m_pPackFile->Read(pComp, iComp);
			if(iRead != iComp)
				return false;

			//CRCУ��
			UInt32 iCrc = HawkOSOperator::CalcCrc((const UChar*)pComp, iComp);
			if (iCrc != pInfo->DataCrc)
				return false;

			//��ѹ��
			ULong iOutLen = iSize;
			ULong iInLen  = iComp;
			if (HawkZip::UnCompress(pData, iOutLen, pComp, iInLen))
			{
				iSize = (UInt32)iOutLen;
				return true;
			}
		}
		return false;
	}

	Bool HawkPackage::Write(void* pData, UInt32 iSize, const AString& sPackFile)
	{
		if(m_bReadOnly || sPackFile.size() <= 0 || !pData || iSize <= 0 || !m_pPackFile) 
			return false;

		//�Ѵ�������ɾ��
		const PkgFile* pInfo = GetFileInfo(sPackFile);
		if(pInfo)
			RemoveFile(sPackFile);
		
		m_pPackFile->Seek(0, HawkFile::POS_END);
		Int64 iEndPos   = m_pPackFile->Tell();
		ULong iNameSize = DEFAULT_SIZE;
		Char  sCompName[DEFAULT_SIZE] = {0};		
		
		//ѹ���ļ���
		if(HawkZip::Compress(sCompName, iNameSize, sPackFile.c_str(), (ULong)sPackFile.size()))
		{
			ULong iComp = HawkZip::GetRequiredSize((ULong)iSize);
			Char* pComp = new Char[iComp];
			memset(pComp, 0, iComp);
			HawkScope::DataArrayPtr scope(pComp);

			//ѹ���ļ�����
			if(HawkZip::Compress(pComp, iComp, pData, (ULong)iSize))
			{
				//�����ļ���Ҫ������Ϣ
				PkgFile sInfo;
				sInfo.NameSize   = (UInt16)iNameSize;
				sInfo.DataCrc    = HawkOSOperator::CalcCrc((const UChar*)pComp, (UInt32)iComp);
				sInfo.DataOffset = iEndPos + sizeof(sInfo) + sInfo.NameSize;
				sInfo.DataSize   = (UInt32)iComp;
				sInfo.OrigSize   = iSize;								
				sInfo.Valid      = true;
								
				//д����ļ�
				m_pPackFile->Write(&sInfo, sizeof(sInfo));
				m_pPackFile->Write(sCompName, iNameSize);
				m_pPackFile->Write(pComp, iComp);

				//�޸��ļ���ͷ��Ϣ
				m_pPackFile->Seek(0);
				m_sPkgHeader.FileCount[0] ++;
				m_pPackFile->Write(&m_sPkgHeader, sizeof(m_sPkgHeader));
				m_pPackFile->Flush();

				m_mFileInfo[sPackFile] = sInfo;
				return true;
			}	
		}		
		return false;
	}

	Bool HawkPackage::Write(const AString& sDiskFile,const AString& sPackFile)
	{
		AString sPkgFile = sPackFile;
		if(sPkgFile.size() == 0) 
			sPkgFile = sDiskFile;

		if(m_bReadOnly || sPkgFile.size() <= 0 || sDiskFile.size() <= 0 || !m_pPackFile) 
			return false;

		//���ļ���ȡ���ڴ���д���ļ���
		HawkDiskFile df;
		if(df.Open(sDiskFile))
		{
			Int64 iSize = df.GetFileSize();
			Char* pData = new Char[(UInt32)iSize];
			memset(pData, 0, (UInt32)iSize);
			HawkScope::DataArrayPtr scope(pData);

			iSize = df.Read(pData, iSize);
			return Write(pData, (UInt32)iSize, sPkgFile);
		}
		return false;
	}

	Bool  HawkPackage::RemoveFile(const AString& sPackFile)
	{
		const PkgFile* pInfo = GetFileInfo(sPackFile);
		if(pInfo && !m_bReadOnly)
		{
			//ɾ���ļ����޸��ļ���Ч���
			Int64 iPos = (Int64)pInfo->DataOffset - (Int64)pInfo->NameSize - (Int64)sizeof(*pInfo);
			m_pPackFile->Seek(iPos);
			PkgFile sInfo;
			memcpy(&sInfo,pInfo,sizeof(sInfo));
			sInfo.Valid = false;
			m_pPackFile->Write(&sInfo,sizeof(sInfo));

			//�޸��ļ���ͷ��Ϣ������д��
			m_sPkgHeader.FileCount[0] --;
			m_sPkgHeader.FileCount[1] ++;
			m_pPackFile->Seek(0);
			m_pPackFile->Write(&m_sPkgHeader,sizeof(m_sPkgHeader));
			m_pPackFile->Flush();

			//�����ļ���
			PkgFileMap::iterator it = m_mFileInfo.find(sPackFile);
			if (it != m_mFileInfo.end())
				m_mFileInfo.erase(it);
			
			return true;
		}
		return false;
	}

	Bool HawkPackage::RemoveFolder(const AString& sPackFolder)
	{
		if (!m_bReadOnly && ExistFolder(sPackFolder))
		{
			PkgFileMap::iterator it = m_mFileInfo.begin();
			for (;it!=m_mFileInfo.end();)
			{
				if (it->first.find(sPackFolder) == 0)
				{
					PkgFile* pInfo = &it->second;

					//ɾ���ļ����޸��ļ���Ч���
					Int64 iPos = (Int64)pInfo->DataOffset - (Int64)pInfo->NameSize - (Int64)sizeof(*pInfo);
					m_pPackFile->Seek(iPos);
					PkgFile sInfo;
					memcpy(&sInfo,pInfo,sizeof(sInfo));
					sInfo.Valid = false;
					m_pPackFile->Write(&sInfo,sizeof(sInfo));

					//�޸��ļ���ͷ��Ϣ������д��
					m_sPkgHeader.FileCount[0] --;
					m_sPkgHeader.FileCount[1] ++;

					m_mFileInfo.erase(it ++);
					continue;
				}
				
				it ++;
			}

			//��д�ļ���ͷ��Ϣ
			m_pPackFile->Seek(0);
			m_pPackFile->Write(&m_sPkgHeader,sizeof(m_sPkgHeader));
			m_pPackFile->Flush();

			return true;
		}
		return false;
	}

	Bool  HawkPackage::Close()
	{
		if (m_pPackFile)
		{
			//�ر��ļ���
			HAWK_RELEASE(m_pPackFile);
			m_mFileInfo.clear();
			memset(&m_sPkgHeader, 0, sizeof(m_sPkgHeader));			
			return true;
		}
		return false;
	}

	Bool  HawkPackage::UnpackFile(const AString& sPackFile, const AString& sDiskFile)
	{
		if (m_pPackFile)
		{
			const PkgFile* pInfo = GetFileInfo(sPackFile);
			if(pInfo)
			{
				//��ȡ�����ļ���
				AString sDst = sDiskFile;
				if (sDiskFile.size() <= 0)
					sDst = HawkOSOperator::SplitFileName(sPackFile);
				
				UInt32 iSize = pInfo->OrigSize;
				Char* pData  = new Char[iSize];
				memset(pData, 0, iSize);
				HawkScope::DataArrayPtr scope(pData);

				//��ȡ�ļ���Ϣ
				if (Read(pData, iSize, sPackFile))
				{
					HawkOSOperator::MakeSureFileName(sDst);

					//���浽����
					HawkDiskFile df;
					if (df.Open(sDst, HawkFile::OPEN_WRITE))
					{
						df.Write(pData,iSize);
						return true;
					}
				}
			}
		}
		return false;
	}
	
	Bool  HawkPackage::Unpack(const AString& sPackFolder, const AString& sDiskFolder)
	{
		if (m_pPackFile)
		{
			AString sPath = sDiskFolder;
			HawkStringUtil::Replace<AString>(sPath, "\\", "/");
			if (sPath.size() && sPath[sPath.size() - 1] != '/')
				sPath += "/";

			PkgFileMap::iterator it = m_mFileInfo.begin();
			for (;it!=m_mFileInfo.end();it++)
			{
				AString sFileName = sPath + it->first;
				if (!sPackFolder.size() || it->first.find(sPackFolder) == 0)
				{
					if (!UnpackFile(it->first, sFileName))
						return false;
				}				
			}
			return true;
		}
		return false;
	}

	Bool HawkPackage::Marshal()
	{
		if (m_pPackFile && m_sPkgHeader.FileCount[1] && !m_bReadOnly)
		{
			AString sOrigName = ((HawkDiskFile*)m_pPackFile)->GetFileName();
			AString sFileName = sOrigName;
			sFileName += ".marshal";
			HawkDiskFile hf;
			if (hf.Open(sFileName, HawkFile::OPEN_WRITE))
			{
				PkgHeader sHeader;
				memcpy(&sHeader, &m_sPkgHeader, sizeof(sHeader));
				sHeader.FileCount[0] = 0;
				sHeader.FileCount[1] = 0;

				hf.Write(&sHeader, sizeof(sHeader));	

				m_pPackFile->Seek(sizeof(m_sPkgHeader));
				while (true)
				{
					PkgFile sInfo;
					Int64 iRead = m_pPackFile->Read(&sInfo, sizeof(sInfo));
					if (iRead != sizeof(sInfo))
						break;

					//ֻ�洢��Ч�ļ�
					Int64 iSize = sInfo.NameSize + sInfo.DataSize;
					if (sInfo.Valid)
					{
						sInfo.DataOffset = hf.Tell() + sizeof(sInfo) + sInfo.NameSize;
						hf.Write(&sInfo, sizeof(sInfo));
						
						sHeader.FileCount[0] ++;
						
						Char* pData = new Char[(UInt32)iSize];
						memset(pData, 0, (UInt32)iSize);
						HawkScope::DataArrayPtr scope(pData);

						if (m_pPackFile->Read(pData, iSize) != iSize)
							return false;

						hf.Write(pData, iSize);
					}
					else
					{
						m_pPackFile->Seek(iSize, HawkFile::POS_CURRENT);
					}
				}

				//��д�ļ���ͷ��Ϣ
				hf.Seek(0);
				hf.Write(&sHeader,sizeof(sHeader));
				hf.Close();

				Close();

				HawkOSOperator::OsDeleteFile(sOrigName);
				HawkOSOperator::RenameFile(sFileName, sOrigName);

				return OpenPackage(sOrigName, m_bReadOnly);
			}
		}
		return false;
	}

	void  HawkPackage::PrintAll()
	{
		//��ӡ�����ļ���Ϣ
		Int32 iIdx = 0;
		PkgFileMap::iterator it = m_mFileInfo.begin();		
		for (;it != m_mFileInfo.end();it++)
		{
			HawkFmtPrint("(%d): File(%s), Offset(%u), Size(%u), OrigSize(%u).",
				++iIdx, it->first.c_str(), it->second.DataOffset, it->second.DataSize, it->second.OrigSize);
		}
	}
}
