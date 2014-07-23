#ifndef HAWK_FOLDER_H
#define HAWK_FOLDER_H

#include "HawkRefCounter.h"
#include "HawkXmlFile.h"

namespace Hawk
{
	/************************************************************************/
	/* ϵͳ�ļ��в���                                                       */
	/************************************************************************/
	class UTIL_API HawkFolder : public HawkRefCounter
	{
	public:
		//���ݽṹ����
		struct FileInfo
		{
			AString Name;
			AString Path;
			Int32	Size;
			UInt32  Attr;
			UInt32  Time;

			FileInfo(const AString& sName = "", Int32 iSize = 0)
			{
				Name = sName;
				Size = iSize;
				Path = "";				
				Attr = 0;
				Time = 0;
			}
		};

		struct  FolderInfo;
		typedef list<FileInfo>   FileList;
		typedef list<FolderInfo> FolderList;

		struct FolderInfo
		{
			AString     Name;
			AString		Path;
			UInt32      Time;
			UInt32		Attr;
			FileList    Files;
			FolderList  Folders;

			FolderInfo(const AString& sName = "")
			{
				Name = sName;
				Path = "";
				Time = 0;
				Attr = 0;
				Files.clear();
				Folders.clear();
			}

			void  Reset()
			{
				Files.clear();
				Folders.clear();
			}

			FolderInfo& operator = (const FolderInfo& sInfo)
			{
				Name = sInfo.Name;
				Path = sInfo.Path;
				Time = sInfo.Time;
				Attr = sInfo.Attr;

				FileList::const_iterator it_file = sInfo.Files.begin();
				for (;it_file!=sInfo.Files.end();it_file++)
					Files.push_back(*it_file);

				FolderList::const_iterator it_folder = sInfo.Folders.begin();
				for (;it_folder!=sInfo.Folders.end();it_folder++)
					Folders.push_back(*it_folder);

				return *this;
			}
		};

	public:
		//���������
		HawkFolder();

		virtual ~HawkFolder();

	public:
		//���ļ���,bRecuInto��ʾ�Ƿ�ݹ����²���Ŀ¼
		virtual Bool  Open(const AString& sFolder, Bool bRecursion = true, const AString& sFilter = "*.*", Bool bHideFile = false);

		//д��Xml�ļ�
		virtual Bool  WriteXml(AXmlDocument* pDoc);

		//�ر��ļ���
		virtual Bool  Close();

		//���FolderInfoָ��
		FolderInfo*   GetFolderPtr();

	protected:
		virtual void  ParseFolder(AXmlElement* pElement, FolderInfo* pFolder);

	protected:
		AString		m_sFolder;
		FolderInfo	m_sInfo;
	};
}
#endif


