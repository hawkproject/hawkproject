#include "HawkFolder.h"
#include "HawkDiskFile.h"
#include "HawkStringUtil.h"

namespace Hawk
{
	struct Tmp_ListDir
	{
		AString     Main;
		AString     Sub;
		HawkFolder::FolderInfo* Folder;

		Tmp_ListDir()
		{
			Main   = "";
			Sub    = "";
			Folder = 0;
		}

		Tmp_ListDir(const AString& sMain, const AString& sSub, HawkFolder::FolderInfo* pFolder)
		{
			Main   = sMain;
			Sub    = sSub;
			Folder = pFolder;
		}
	};

	void ListDirFiles(AString& sStaticDir, const AString& sMain, const AString& sSub, HawkFolder::FolderInfo* pFolder,
		map<Size_t,Tmp_ListDir>& mDirInfo, const AString& sFilter = "*.*", Bool bHideFile = false)
	{
		if(!pFolder) 
			return;

		AString sMainTmp = sMain;
		if (sMainTmp.size() >0 && sMainTmp[sMainTmp.size()-1] != '/' && sSub.size() > 0)
			sMainTmp += "/";

		AString sFolder = sMainTmp  + sSub;
		if (sFolder.size() >0 && sFolder[sFolder.size()-1] != '/')
			sFolder += "/";

		AString sFull = sFolder;
		Size_t iPos   = sFolder.find(sStaticDir);
		pFolder->Path = iPos == 0? sFolder.substr(sStaticDir.size(), sFolder.size()-sStaticDir.size()) : sFolder;
		pFolder->Name = sSub;
		sFolder       += sFilter;

#ifdef PLATFORM_WINDOWS
		struct _finddata_t fd;
		intptr_t hf = _findfirst(sFolder.c_str(), &fd);
		if (hf <= 0) return;
		do
		{
			if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) 
			{
				if((fd.attrib & _A_SUBDIR) > 0)
				{
					HawkFolder::FolderInfo oInfo;
					oInfo.Name = fd.name;
					oInfo.Path = pFolder->Path + oInfo.Name;
					oInfo.Attr = fd.attrib;
					oInfo.Time = (UInt32)fd.time_create;

					if (oInfo.Path.size() >0 && oInfo.Path[oInfo.Path.size()-1] != '/')
						oInfo.Path += "/";

					if (!bHideFile && (oInfo.Attr & _A_HIDDEN))
						continue;

					pFolder->Folders.push_back(oInfo);

					//保存,避免递归,广度优先
					HawkFolder::FolderList::iterator it = pFolder->Folders.end();
					it --;
					
					mDirInfo.insert(std::make_pair(mDirInfo.size(), Tmp_ListDir(sFull, fd.name, &(*it))));
				}
				else
				{
					HawkFolder::FileInfo oInfo;
					oInfo.Name = fd.name;
					oInfo.Path = pFolder->Path + oInfo.Name;
					oInfo.Size = (Int32)fd.size;
					oInfo.Attr = fd.attrib;
					oInfo.Time = (UInt32)fd.time_create;

					if (!bHideFile && (oInfo.Attr & _A_HIDDEN))
						continue;

					pFolder->Files.push_back(oInfo);
				}
			}
		}while(_findnext(hf,&fd) == HAWK_OK);

		_findclose(hf);
#else
		DIR* pDir = opendir(sFolder.c_str());
		if (!pDir) return;

		struct dirent* pEntry = 0;		
		while((pEntry = readdir(pDir)) != 0) 
		{
			struct stat sStat;
			lstat(pEntry->d_name, &sStat);
			if (strcmp(pEntry->d_name, ".") != 0 && strcmp(pEntry->d_name, "..") != 0) 
			{
				if(S_ISDIR(sStat.st_mode)) 
				{
					HawkFolder::FolderInfo oInfo;
					oInfo.Name = pEntry->d_name;
					oInfo.Path = pFolder->Path + oInfo.Name;
					oInfo.Attr = sStat.st_mode;
					oInfo.Time = (UInt32)sStat.st_ctime;

					if (oInfo.Path.size() >0 && oInfo.Path[oInfo.Path.size()-1] != '/')
						oInfo.Path += "/";

					pFolder->Folders.push_back(oInfo);

					//保存,避免递归,广度优先
					HawkFolder::FolderList::iterator it = pFolder->Folders.end();
					it --;

					mDirInfo.insert(std::make_pair(mDirInfo.size(), Tmp_ListDir(sFull, pEntry->d_name, &(*it))));
				}
				else
				{
					HawkFolder::FileInfo oInfo;
					oInfo.Name = pEntry->d_name;
					oInfo.Path = pFolder->Path + oInfo.Name;
					oInfo.Size = (Int32)sStat.st_size;
					oInfo.Attr = sStat.st_mode;
					oInfo.Time = (UInt32)sStat.st_ctime;

					pFolder->Files.push_back(oInfo);
				}
			}			
		}
		
		closedir(pDir);
#endif
	}

	//构造和析构
	HawkFolder::HawkFolder()
	{
		m_sInfo.Reset();
	}

	HawkFolder::~HawkFolder()
	{
		m_sInfo.Reset();
	}

	//打开文件夹
	Bool  HawkFolder::Open(const AString& sFolder, Bool bRecursion, const AString& sFilter, Bool bHideFile)
	{
		m_sFolder = sFolder;
		HawkStringUtil::Trim<AString>(m_sFolder);
		HawkStringUtil::Replace<AString>(m_sFolder, "\\", "/");

		if (m_sFolder.size() >0 && m_sFolder[m_sFolder.size()-1] != '/')
			m_sFolder += "/";
		
		map<Size_t, Tmp_ListDir> mDirInfo;
		Size_t iIdx = 0;

		mDirInfo.insert(std::make_pair(iIdx, Tmp_ListDir(m_sFolder, "", &m_sInfo)));
		
		while(true)
		{
			if(iIdx >= mDirInfo.size()) 
				break;

			//避免递归,广度优先
			ListDirFiles(m_sFolder,mDirInfo[iIdx].Main,mDirInfo[iIdx].Sub,mDirInfo[iIdx].Folder,mDirInfo,sFilter,bHideFile);

			iIdx ++;

			if (!bRecursion)
				break;
		}
		
		return true;
	}

	//关闭文件夹
	Bool  HawkFolder::Close()
	{
		m_sInfo.Reset();
		return true;
	}

	HawkFolder::FolderInfo* HawkFolder::GetFolderPtr()
	{
		return &m_sInfo;
	}

	//写入Xml
	Bool  HawkFolder::WriteXml(AXmlDocument* pDoc)
	{
		if(pDoc)
		{
			pDoc->Clear();
			pDoc->GetRoot()->SetTag("Resource");
			pDoc->GetRoot()->AddAttribute(HawkXmlAttribute<AString>("Path", m_sFolder));
			ParseFolder(pDoc->GetRoot(), &m_sInfo);
			return true;
		}
		return false;
	}

	void HawkFolder::ParseFolder(AXmlElement* pElement, FolderInfo* pFolder)
	{
		if (pFolder &&pElement)
		{
			HawkFolder::FileList::iterator it1 = pFolder->Files.begin();
			for (;it1 != pFolder->Files.end();it1++)
			{
				AXmlElement* pChild = pElement->AddChildren("File");
				pChild->AddAttribute(HawkXmlAttribute<AString>("Name", it1->Name));
				pChild->AddAttribute(HawkXmlAttribute<AString>("Path", it1->Path));
				pChild->AddAttribute(HawkXmlAttribute<AString>("Size", HawkStringUtil::IntToString<AString>(it1->Size)));
			}

			HawkFolder::FolderList::iterator it2 = pFolder->Folders.begin();
			for (;it2 != pFolder->Folders.end();it2++)
			{
				AXmlElement* pChild = pElement->AddChildren("Folder");
				pChild->AddAttribute(HawkXmlAttribute<AString>("Name",it2->Name));
				pChild->AddAttribute(HawkXmlAttribute<AString>("Path",it2->Path));

				ParseFolder(pElement->GetChildren(pElement->GetChildrenNum()-1),&(*it2));
			}
		}
	}
}
