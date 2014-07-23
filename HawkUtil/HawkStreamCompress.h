#ifndef HAWK_STREAMCOMPRESS_H
#define HAWK_STREAMCOMPRESS_H

#include "HawkOctetsStream.h"

namespace Hawk
{
	/************************************************************************/
	/* 流压缩和解压操作类封装                                               */
	/************************************************************************/
	class UTIL_API HawkStreamCompress : public HawkRefCounter
	{
	public:
		//流压缩解压构造
		HawkStreamCompress();

		//流压缩解压析构
		virtual ~HawkStreamCompress();

	public:
		//流压缩接口
		virtual Bool Compress(HawkOctets& xOctets);

		//流解压接口
		virtual Bool Uncompress(HawkOctets& xOctets);

	protected:
		//流对象指针
		PVoid		  m_pStream[2];
		//当前操作总流数据存储
		OctetsStream  m_sStream;
		//当前操作流数据存储
		HawkOctets    m_sOctets;
	};
}
#endif
