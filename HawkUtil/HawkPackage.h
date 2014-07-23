#ifndef HAWK_PACKAGE_H
#define HAWK_PACKAGE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* �ļ����ݰ�,�������ݰ���·���ָ���Ϊ"/"                               */
	/************************************************************************/
	class UTIL_API HawkPackage : public HawkRefCounter
	{
	public:
		//���ļ���
		HawkPackage(const AString& sPackFile = "",Bool bOnlyRead = true);

		//���ڴ��ļ���
		HawkPackage(void* pData,Int32 iSize);

		//���ļ�����
		virtual ~HawkPackage();

	public:
		#pragma pack(push,1)
		//���ݰ��ļ�ͷ��Ϣ
		struct PkgHeader
		{
			UInt32 PackMagic;
			UInt8  PackVersion;
			UInt32 FileCount[2];

			PkgHeader() { memset(this, 0, sizeof(*this)); }
		};

		//���ݰ��ļ���Ϣ
		struct PkgFile
		{
			UInt32 DataCrc;
			UInt64 DataOffset;
			UInt32 DataSize;
			UInt32 OrigSize;
			UInt16 NameSize;
			Bool   Valid;
			
			PkgFile()
			{
				memset(this, 0, sizeof(*this));
			}
		};
		#pragma pack(pop)

		//�ļ���Ϣ��
		typedef map<AString, PkgFile> PkgFileMap;

		//�汾�Ŷ���
		enum { PACKAGE_VERSION = 1};

	public:
		//���ļ���(bOnlyReadΪtrueʱ�ļ��������,��д��ʱ��,����ļ������ھʹ���
		virtual Bool     OpenPackage(const AString& sDiskFile, Bool bOnlyRead = true);

		virtual Bool     OpenPackage(const void* pData, UInt32 iSize);

		//������ݰ�ͷ��Ϣ
		const PkgHeader* GetPkgHeader() const;

		//����ļ���Ϣ
		const PkgFile*   GetFileInfo(const AString& sPackFile);

		//�ж��Ƿ�����ļ�
		virtual Bool     ExistFile(const AString& sPackFile);

		//�ж�Ŀ¼�Ƿ����
		virtual Bool     ExistFolder(const AString& sPackFolder);

		//���ļ�,���������ֽڴ�С����iSizeΪ׼
		virtual Bool     Read(void*  pData, UInt32& iSize, const AString& sPackFile);

		//д�ļ�
		virtual Bool     Write(void* pData, UInt32 iSize, const AString& sPackFile);

		//д�ļ�
		virtual Bool     Write(const AString& sDiskFile, const AString& sPackFile = "");

		//�Ƴ�һ���ļ�
		virtual Bool     RemoveFile(const AString& sPackFile);

		//�Ƴ�һ��Ŀ¼
		virtual Bool     RemoveFolder(const AString& sPackFolder);

		//�ر��ļ���
		virtual Bool     Close();

		//ѹ��һ���ļ�������Ŀ¼��
		virtual Bool     UnpackFile(const AString& sPackFile, const AString& sDiskFile = "");
		
		//��ѹ����ָ��Ŀ¼, Ĭ�Ͽ��ַ�����ʾ���������ļ���ѹ����ǰĿ¼("" ,"")
		virtual Bool     Unpack(const AString& sPackFolder = "", const AString& sDiskFolder = "");
	
		//�������ݰ�,ɾ����Ч�ļ�,����ɾ�����ļ���Ŀ
		virtual Bool     Marshal();

		//��ӡ�������ļ���Ϣ
		virtual void     PrintAll();

	protected:
		//�����ļ�������
		virtual Bool     Analyze();

	protected:
		HawkFile*	m_pPackFile;
		PkgHeader   m_sPkgHeader;
		PkgFileMap  m_mFileInfo;
		Bool        m_bReadOnly;
	};
}

#endif
