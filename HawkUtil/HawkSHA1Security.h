#ifndef HAWK_SHA1SECURITY_H
#define HAWK_SHA1SECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* SHA1�����㷨��װ                                                     */
	/************************************************************************/
	class UTIL_API HawkSHA1Security : public HawkSecurity
	{
	public:
		//����
		HawkSHA1Security();

		//����
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
		//����
		virtual void Reset();

		//����
		virtual void Update(UInt8* pData, UInt32 iSize);

		//���
		virtual void Final();

		//��ȡ��ʽ�����
		virtual Bool ReportHash(Char* pReport, Int8 iType = REPORT_HEX);

		//��ȡHash
		virtual void GetHash(UInt8* pHash);

	protected:
		//ת��
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
