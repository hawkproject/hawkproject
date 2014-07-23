#include "HawkUtil.h"

Bool PkgAddFile(const AString& sPkgFile, const AString& sDiskFile)
{
	HawkPackage sPkg;
	if (sPkg.OpenPackage(sPkgFile, false))
	{
		HawkDiskFile df;
		if (!df.Open(sDiskFile))
			return false;

		AString sPackFile = HawkOSOperator::SplitFileName(sDiskFile);

		Int64 iSize = df.GetFileSize();
		Char* pData = new Char[(UInt32)iSize];
		memset(pData, 0, (UInt32)iSize);
		HawkScope::DataArrayPtr scope(pData);
		iSize = df.Read(pData, iSize);

		return sPkg.Write(pData, (UInt32)iSize, sPackFile);
	}	
	return false;
}

Bool PkgAddFolder(HawkPackage* pPkg, const AString& sDiskFolder, HawkFolder::FolderInfo* pFolder)
{
	if (!pPkg || !pFolder)
		return false;

	AStringVector asv;
	HawkStringUtil::Split<AString>(sDiskFolder, asv, "/");
	AString sBaseFolder = "";
	if (asv.size())
		sBaseFolder = asv[asv.size()-1] + "/";
	sBaseFolder += pFolder->Path;

	HawkFolder::FileList::const_iterator it_file = pFolder->Files.begin();
	for (;it_file!=pFolder->Files.end();it_file++)
	{
		if (!pPkg->Write(sDiskFolder + it_file->Path, sBaseFolder + it_file->Name))
			return false;
	}
	
	HawkFolder::FolderList::const_iterator it_folder = pFolder->Folders.begin();
	for (;it_folder!=pFolder->Folders.end();it_folder++)
	{
		HawkFolder::FolderInfo* pChild = (HawkFolder::FolderInfo*)(&(*it_folder));
		if (!PkgAddFolder(pPkg, sDiskFolder, pChild))
			return false;
	}

	return true;
}

Bool PkgAddFolder(const AString& sPkgFile, const AString& sDiskFolder)
{
	HawkFolder folder;
	if (folder.Open(sDiskFolder))
	{
		HawkFolder::FolderInfo* pInfo = folder.GetFolderPtr();
		if (pInfo)
		{
			HawkPackage sPkg;
			if (sPkg.OpenPackage(sPkgFile, false))
				return PkgAddFolder(&sPkg, sDiskFolder, pInfo);
		}
	}
	return false;
}

Bool OnPkgAdd(const AString& sPkgFile, const AStringVector& vArgs)
{
	HawkPackage sPkg;
	if (vArgs.size() && sPkg.OpenPackage(sPkgFile, false))
	{
		//添加目录
		if (vArgs[0][vArgs[0].size() - 1] == '/')
		{
			return PkgAddFolder(sPkgFile, vArgs[0]);
		}
		//添加文件
		else
		{
			return PkgAddFile(sPkgFile, vArgs[0]);
		}		
	}	
	return false;
}

Bool OnPkgDel(const AString& sPkgFile, const AStringVector& vArgs)
{
	HawkPackage sPkg;
	if (!vArgs.size() || !sPkg.OpenPackage(sPkgFile, false))
		return false;

	if (sPkg.ExistFile(vArgs[0]))
	{
		return sPkg.RemoveFile(vArgs[0]);
	}
	else if (sPkg.ExistFolder(vArgs[0]))
	{
		return sPkg.RemoveFolder(vArgs[0]);
	}
	return false;
}

Bool OnPkgTrim(const AString& sPkgFile, const AStringVector& vArgs)
{
	HawkPackage sPkg;
	if (!sPkg.OpenPackage(sPkgFile, false))
		return false;

	return sPkg.Marshal();
}

Bool OnPkgUnpack(const AString& sPkgFile, const AStringVector& vArgs)
{
	HawkPackage sPkg;
	if (!sPkg.OpenPackage(sPkgFile, true))
		return false;

	if (!vArgs.size())
	{
		//解压所有文件到当前目录
		return sPkg.Unpack("", "");
	}
	else if (vArgs.size() == 1)
	{
		AString sArg = vArgs[0];
		//是目录, 解压所有文件到此目录
		if (sArg[sArg.size() - 1] == '/')
		{
			return sPkg.Unpack(sArg);
		}
		//解压单个文件
		else
		{
			AString sDiskFile = HawkOSOperator::SplitFileName(sArg);
			return sPkg.UnpackFile(sArg, sDiskFile);
		}
	}
	else if (vArgs.size() == 2)
	{
		AString sPackFile = vArgs[0];
		AString sArg      = vArgs[1];
		if (sPackFile[sPackFile.size() - 1] != '/')
		{
			if (sArg[sArg.size() - 1] == '/')
			{
				AString sDiskFile = sArg + HawkOSOperator::SplitFileName(sPackFile);
				return sPkg.UnpackFile(sPackFile, sDiskFile);
			}
			else
			{
				AString sDiskFile = sArg;
				return sPkg.UnpackFile(sPackFile, sDiskFile);
			}
		}
		else
		{
			if (!sArg.size() || sArg[sArg.size() - 1] == '/')
			{
				AString sDiskFile = sArg + HawkOSOperator::SplitFileName(sPackFile);
				return sPkg.Unpack(sPackFile, sDiskFile);
			}
			else
			{
				AString sDiskFile = sArg;
				return sPkg.UnpackFile(sPackFile, sDiskFile);
			}
		}		
	}
	return false;
}

Int32 main(Int32 argc, Char* argv[])
{
	HawkUtil::Init();

	if (argc >= 3)
	{
		HawkPackage sPkg;
		AString sPkgFile = argv[1];
		AString sOpType  = argv[2];

		HawkStringUtil::Trim(sPkgFile);
		HawkStringUtil::Trim(sOpType);

		//后续参数存储
		AStringVector vArgs;
		for (Int32 i=3;i<argc;i++)
		{
			AString sArg = argv[i];		
			HawkStringUtil::Trim<AString>(sArg);
			HawkStringUtil::Replace<AString>(sArg, "\\", "/");
			vArgs.push_back(sArg);
		}

		//对应操作处理
		if (sOpType == "add")
		{
			OnPkgAdd(sPkgFile, vArgs);
		}
		else if (sOpType == "del")
		{
			OnPkgDel(sPkgFile, vArgs);
		}
		else if (sOpType == "trim")
		{
			OnPkgTrim(sPkgFile, vArgs);
		}
		else if (sOpType == "unpack")
		{
			OnPkgUnpack(sPkgFile, vArgs);
		}
	}
	else
	{
		HawkFmtPrint("Usage: DataPacker PkgFile OpType(add, del, trim, unpack) Args");
	}

	HawkUtil::Stop();
	HawkUtil::Release();

	return 0;
}
