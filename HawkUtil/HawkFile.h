#ifndef HAWK_FILE_H
#define HAWK_FILE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* �ļ���������,�ṩ�����ļ������Ļ����ӿ�                              */
	/************************************************************************/
	class UTIL_API HawkFile : public HawkRefCounter
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkFile();

		//�����ļ���Դ
		virtual ~HawkFile();

	public:
		//�����ʽ
		enum CodeType
		{
			//ASCII�����ʽ
			CODE_ASCII,
			//UTF8�����ʽ
			CODE_UTF8,
			//UNICODE�����ʽ
			CODE_UNICODE,
		};

		//�ļ�ƫ��λ��
		enum SeekPos
		{
			//��ʼλ��
			POS_BEGIN,
			//��ǰ����λ��
			POS_CURRENT,
			//�ļ�ĩβ
			POS_END,
		};

		//�ļ�������
		enum OpenType
		{
			//ֻ��
			OPEN_READ = 0,
			//ֻд
			OPEN_WRITE,
			//�ɶ���д(�ļ��������)
			OPEN_RW,
			//�ɶ���д(�ļ��ᱻ�Զ�����,����ļ�������ô���ݱ����)
			OPEN_WR,
			//����׷��
			OPEN_APPEND,
		};	

	public:
		//�ļ���С
		DEFINE_PROPERTY(Int64,	  m_iFileSize, FileSize);

		//�򿪷�ʽ
		DEFINE_PROPERTY(OpenType, m_eOpenType, OpenType);

	public:
		//���ļ�,sFileΪ�ļ���,eCodeֻ������д�ļ���ʱ�����Ч
		virtual Bool    Open(const AString& sFile, OpenType eOpen = OPEN_READ);

		//��ȡ�ļ�����,pDataΪ�洢�Ŀռ�,iSizeΪ��ȡ�ֽ���
		virtual Int64   Read(void* pData, Int64 iSize);

		//��һ��
		virtual Int64   ReadLine(void* pData, Int64 iBufSize, UChar cSeparate = '\n');

		//д�ļ�,pDataΪд�������,iSizeΪд���ֽ���
		virtual Int64   Write(const void* pData, Int64 iSize, Bool bFlush = false);

		//ָ��ƫ��
		virtual Int64   Seek(Int64 iOffset, SeekPos ePos = POS_BEGIN);

		//���㵽�ļ�ͷƫ���ֽ�
		virtual Int64   Tell();

		//����ļ�����
		virtual Bool    Flush();

		//�ر��ļ����ͷ���վ���ڴ�
		virtual Bool    Close();

		//�ı��ļ���С(�ضϻ��������ļ���С)
		virtual Bool    Chsize(Int64 iSize);

		//�жϴ򿪷�ʽ�Ƿ��д
		virtual Bool    IsWriteAble() const;

		//�жϴ򿪷�ʽ�Ƿ�ɶ�
		virtual Bool    IsReadAble() const;

	public:
		//��ȡ����ͷ�����ʽ
		static CodeType GetCodeType(const void* pData);
	};
}
#endif
