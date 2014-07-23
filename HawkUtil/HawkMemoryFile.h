#ifndef HAWK_MEMORYFILE_H
#define HAWK_MEMORYFILE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* �ڴ��ļ�����                                                         */
	/************************************************************************/
	class UTIL_API HawkMemoryFile : public HawkFile
	{	
	public:
		//�ڴ��ļ�����
		HawkMemoryFile();
		
		//�ڴ��ļ�����
		virtual ~HawkMemoryFile();

	public:
		//�ļ�λ��
		DEFINE_PROPERTY(Int64, m_iFilePos, FilePos);

		//�ļ�����
		DEFINE_PROPERTY(PVoid, m_pData,	   DataPtr);

		//�������
		DEFINE_PROPERTY(Bool,  m_bExtra,   IsExtra);

	public:
		//���ڴ��ļ�
		virtual Bool     Open(Int64 iSize, OpenType eOpen = OPEN_WRITE);

		//���ڴ���ļ�
		virtual Bool     Open(void* pData, Int64 iSize, Bool bExtra, OpenType eOpen = OPEN_READ);

		//���ļ������ڴ�
		virtual Bool     Open(const AString& sFile);

		//��ȡ�ļ�����,pDataΪ�洢�Ŀռ�,iSizeΪ��ȡ�ֽ���
		virtual Int64	 Read(void* pData, Int64 iSize);

		//��һ��
		virtual Int64    ReadLine(void* pData, Int64 iBufSize, UChar cSeparate = '\n');

		//д�ļ�,pDataΪд�������,iSizeΪд���ֽ���
		virtual Int64	 Write(const void* pData, Int64 iSize, Bool bFlush = false);

		//�ļ�ƫ�Ʋ���,iSizeΪ�ļ�ƫ���ֽ���Ŀ,ePosΪ�ļ�ƫ�Ƶ���ʼλ��
		virtual Int64	 Seek(Int64 iOffset, SeekPos ePos = POS_BEGIN);
		
		//����ļ�ƫ��
		virtual Int64    Tell();

		//����ļ�����(ֻ�ƶ�λ��, ���ͷ�)
		virtual Bool     Clear();
		
		//�����ļ�CRCУ����
		virtual UInt32   CalcCrc() const;

		//д��β��'0'
		virtual Int64    WriteZeroTerminater();

		//�ر��ļ����ͷ���վ���ڴ�
		virtual Bool     Close();

		//�ı��ļ���С(�ضϻ��������ļ���С)
		virtual Bool     Chsize(Int64 iSize);

		//���浽�����ļ�
		virtual Bool     SaveToDisk(const AString& sFile);
	};
}
#endif
