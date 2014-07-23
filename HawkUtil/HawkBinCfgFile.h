#ifndef HAWK_BINCFGFILE_H
#define HAWK_BINCFGFILE_H

#include "HawkMemoryFile.h"

namespace Hawk
{
	/************************************************************************/
	/* ѹ�������ļ���ȡ��                                                   */
	/************************************************************************/
	class UTIL_API HawkBinCfgFile : public HawkRefCounter
	{
	public:
		//����
		HawkBinCfgFile();

		//����
		virtual ~HawkBinCfgFile();

	public:
		//���������ļ�
		virtual Bool    LoadCfgData(const AString& sFile);

		//���������ļ�
		virtual Bool    LoadFromMem(const void* pData, UInt32 iSize);

	public:
		//��ȡ�ļ��ֽڴ�С
		virtual Int64   GetFileSize() const;

		//��ȡ�ڴ��ļ�
		HawkMemoryFile* GetMemFile();

		//��ȡ����
		virtual Int64   Read(void* pData, Int64 iSize);

		//ƫ���ֽ�
		virtual Int64   Seek(Int64 iOffset, HawkFile::SeekPos ePos = HawkFile::POS_BEGIN);

	public:
		//ѹ������
		static  Bool    CompressToFile(const void* pData, ULong iSize, const AString& sDstFile);

	protected:
		//�ļ����
		HawkMemoryFile* m_pFile;
	};
}
#endif
