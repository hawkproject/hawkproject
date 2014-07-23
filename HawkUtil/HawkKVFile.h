#ifndef HAWK_KVFILE_H
#define HAWK_KVFILE_H

#include "HawkFile.h"

namespace Hawk
{
	/************************************************************************/
	/* ��ֵ�������ļ�������                                                 */
	/************************************************************************/
	class UTIL_API HawkKVFile : public HawkRefCounter
	{
	public:
		//Ĭ�Ϲ��캯��
		HawkKVFile();

		//�����ļ���Դ
		virtual ~HawkKVFile();

	public:		
		//�򿪴����ļ�(eType֧��OPEN_READ & OPEN_WRITE)
		virtual Bool Open(const AString& sFile, HawkFile::OpenType eType = HawkFile::OPEN_READ);		

		//���ļ������
		virtual Bool Open(HawkFile* pFile);

		//���浽�ļ�
		virtual Bool Save(const AString& sFile);

	public:
		//��ѯָ��Key�Ƿ����
		virtual Bool IsKeyExist(const AString& sKey);

		//��ȡָ��Key����Ӧ��ֵ
		virtual Bool QueryValue(const AString& sKey, AString& sVal);

		//���kv��ֵ
		virtual Bool SetKV(const AString& sKey, const AString& sVal);

		//ɾ��ָ����keyֵ����
		virtual Bool DelKey(const AString& sKey);

		//��ȡ����kv������
		const AStringMap& GetKVTbl() const;

	protected:
		AStringMap m_mKVTbl;
	};
}
#endif
