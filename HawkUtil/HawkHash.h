#ifndef HAWK_HASH_H
#define HAWK_HASH_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* Hash算法封装, 参考:http://burtleburtle.net/bob/hash/doobs.html       */
	/************************************************************************/
	class UTIL_API HawkHash
	{
	protected:
		enum
		{
			CPU_UNKNOWN_ENDIAN = 0,
			CPU_LITTLE_ENDIAN,
			CPU_BIG_ENDIAN,
		};

	public:
		//检查字节序
		static UInt32 CheckCpuEndian();

		//LittleEndian的Hash计算, Bob Jenkins
		static UInt32 HashLittleEndian(const void* pData, Size_t iSize, UInt32 iInitval);

		//BigEndian的Hash计算, Bob Jenkins
		static UInt32 HashBigEndian(const void* pData, Size_t iSize, UInt32 iInitval);

		//以当前CPU架构计算Hash值
		static UInt32 CalcHash(const void* pData, Size_t iSize, UInt32 iInitval = 0);

	protected:
		static UInt32  m_iCpuEndian;
	};
}
#endif
