#ifndef HAWK_FILESENDER_H
#define HAWK_FILESENDER_H

#include "HawkDiskFile.h"

namespace Hawk
{
	/************************************************************************/
	/* Ͷ���ļ���Acceptor                                                   */
	/************************************************************************/
	class UTIL_API HawkFileSender : public HawkRefCounter
	{
	public:
		HawkFileSender();
		
		virtual ~HawkFileSender();

	public:
		//Ͷ���ļ�
		virtual Bool  PostFile(const AString& sAcceptor, const AString& sFileName, const AString& sLocalFile);

		//Ͷ������
		virtual Bool  PostData(const AString& sAcceptor, const AString& sFileName, const Char* pData, Size_t iSize);
	};
}
#endif
