#ifndef HAWK_EXCEPTION_H
#define HAWK_EXCEPTION_H

#include "HawkStdHeader.h"
#include "HawkMalloc.h"

namespace Hawk
{
	/************************************************************************/
	/* �쳣������,����ϵͳ�쳣�Ĵ���ͼ�¼                                  */
	/************************************************************************/
	class UTIL_API HawkException : public std::exception
	{
	public:
		//����
		DEFINE_PROPERTY(AString, m_sMsg, Msg);

		struct ShowFileLine 
		{
			ShowFileLine(const AString& sTag, const AString& sFile, const AString& sFunc, Int32 iLine);
		};

	public:
		//�����쳣��Ϣ
		HawkException(const AString& sMsg,const AString& sFile, const AString& sFunc, Int32 iLine);

		//��������
		virtual ~HawkException() throw();

	public:
		//�쳣����
		virtual Bool Throw() const;
	};

	//�׳��쳣
	#define T_Exception(msg)	throw HawkException(msg,__FILE__,__FUNCTION__,__LINE__)
	//�ж�valΪfalseʱ�׳��쳣
	#define A_Exception(cond)	if(!(cond)) throw HawkException(#cond,__FILE__,__FUNCTION__,__LINE__)	
	//�׳���ָ�����
	#define NullException()		{ Char* pPtr = 0; *pPtr = 0; }
#ifdef _DEBUG
	//ֵ�ǿն���
	#define HawkAssert(cond)	{ if(!(cond)) HawkException::ShowFileLine sSFL("Assert", __FILE__,__FUNCTION__,__LINE__); assert(cond);}
#else
	//ֵ�ǿն���
	#define HawkAssert(cond)
#endif
}
#endif
