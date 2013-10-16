#ifndef _ECHOPROTO_H_
#define _ECHOPROTO_H_

#include "HawkUtil.h"

namespace Hawk
{
	enum
	{
		PROTO_A = 1000,
		PROTO_B,
		PROTO_C,
		PROTO_CNT
	};

	class ProtoA : public HawkProtocol
	{
	public:
		ProtoA() : HawkProtocol(PROTO_A)
		{
			m_sMsg = HawkStringUtil::RandomString<AString>(HawkRand::RandInt(32,256));
		}

	public:
		virtual HawkProtocol* Clone() const
		{
			return new ProtoA;
		}

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS << m_sMsg;
		}
		
		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS >> m_sMsg;
		}

	public:
		AString m_sMsg;
	};

	class ProtoB : public HawkProtocol
	{
	public:
		ProtoB() : HawkProtocol(PROTO_B)
		{
			m_iSize = HawkRand::RandInt(8,32);
			m_iTime = HawkOSOperator::GetTickCount();
		}

	public:
		virtual HawkProtocol* Clone() const
		{
			return new ProtoB;
		}

		virtual void Print()
		{
			HawkFmtPrint("Size = %d, Time = %d",m_iSize,m_iTime);
		}

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS << m_iSize << m_iTime;
		}

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS >> m_iSize >> m_iTime;
		}

	public:
		Int32  m_iSize;
		Int32  m_iTime;
	};

	class ProtoC : public HawkProtocol
	{
	public:
		ProtoC() : HawkProtocol(PROTO_C)
		{
			m_fX = HawkRand::RandFloat(0,1);
			m_fY = HawkRand::RandFloat(0,1);
			m_fZ = HawkRand::RandFloat(0,1);
		}

	public:
		virtual HawkProtocol* Clone() const
		{
			return new ProtoC;
		}

		virtual HawkOctetsStream& Marshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS << m_fX << m_fY << m_fZ;
		}

		virtual HawkOctetsStream& Unmarshal(HawkOctetsStream& rhsOS)
		{
			return rhsOS >> m_fX >> m_fY >> m_fZ;
		}

	public:
		Float m_fX;
		Float m_fY;
		Float m_fZ;
	};
}
#endif 
