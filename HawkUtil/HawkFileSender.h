#ifndef HAWK_FILESENDER_H
#define HAWK_FILESENDER_H

#include "HawkDiskFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 投递文件到Acceptor                                                   */
	/************************************************************************/
	class UTIL_API HawkFileSender : public HawkRefCounter
	{
	public:
		HawkFileSender();
		
		virtual ~HawkFileSender();

	public:
		//投递文件
		virtual Bool  PostFile(const AString& sAcceptor, const AString& sFileName, const AString& sLocalFile);

		//投递数据
		virtual Bool  PostData(const AString& sAcceptor, const AString& sFileName, const Char* pData, Size_t iSize);
	};
}
#endif
