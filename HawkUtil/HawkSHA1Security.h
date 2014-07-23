#ifndef HAWK_SHA1SECURITY_H
#define HAWK_SHA1SECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* SHA1加密算法封装                                                     */
	/************************************************************************/
	class UTIL_API HawkSHA1Security : public HawkSecurity
	{
	public:
		//构造
		HawkSHA1Security();

		//析构
		virtual ~HawkSHA1Security();

	protected:
		typedef union
		{
			UInt8  c[64];
			UInt32 l[16];
		} SHA1_BLOCK;

		enum
		{
			REPORT_HEX   = 0,
			REPORT_DIGIT = 1
		};

	public:
		//重置
		virtual void Reset();

		//更新
		virtual void Update(UInt8* pData, UInt32 iSize);

		//完成
		virtual void Final();

		//获取格式化结果
		virtual Bool ReportHash(Char* pReport, Int8 iType = REPORT_HEX);

		//获取Hash
		virtual void GetHash(UInt8* pHash);

	protected:
		//转换
		virtual void Transform(UInt32* pState, UInt8* pBuffer);

	protected:
		UInt32		m_vState[5];
		UInt32		m_vCount[2];		
		UInt8		m_vBuffer[64];
		UInt8		m_vDigest[20];
		UInt8		m_vWorkspace[64];
		SHA1_BLOCK*	m_pBlock;
	};
}
#endif
