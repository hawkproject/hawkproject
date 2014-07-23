#ifndef HAWK_GATESECURITY_H
#define HAWK_GATESECURITY_H

#include "HawkSecurity.h"

namespace Hawk
{
	/************************************************************************/
	/* 网关数据安全组件                                                     */
	/************************************************************************/
	class GATE_API HawkGateSecurity : public HawkSecurity
	{
	public:
		//流压缩解压构造
		HawkGateSecurity(UInt8 iType = SECURITY_NONE);

		//流压缩解压析构
		virtual ~HawkGateSecurity();

	public:
		//加密解密
		enum
		{
			SECURITY_NONE = 0,
			SECURITY_ENCODE,
			SECURITY_DECODE,
		};

	public:
		//加密更新字节流
		virtual Bool  Update(HawkOctets& xOctets);

	protected:
		//流压缩接口
		virtual Bool  Encode(HawkOctets& xOctets);

		//流解压接口
		virtual Bool  Decode(HawkOctets& xOctets);

	protected:
		//类型
		UInt8		m_iType;		
		//中间数据流参数
		HawkOctets  m_sOctets;
		//流对象指针
		PVoid		m_pZStream;
	};
}
#endif
