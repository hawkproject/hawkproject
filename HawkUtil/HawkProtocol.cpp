#include "HawkProtocol.h"
#include "HawkOSOperator.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"
#include "HawkProtocolManager.h"

namespace Hawk
{
	HawkProtocol::HawkProtocol(ProtoType iType, UInt8 iDrive, const AString& sStates) : m_iType(iType), m_iSize(0), m_iCrc(0), m_iDrive(iDrive)
	{
		SetProtoStates(sStates);
	}

	HawkProtocol::~HawkProtocol()
	{
	}

	ProtoType HawkProtocol::GetType() const
	{
		return m_iType;
	}	

	ProtoSize HawkProtocol::GetSize() const
	{
		return m_iSize;
	}

	ProtoCrc HawkProtocol::GetCrc() const
	{
		return m_iCrc;
	}

	UInt8 HawkProtocol::GetDrive() const
	{
		return m_iDrive;
	}

	Bool HawkProtocol::IsStateSuitable(Int32 iState) const
	{
		if (m_mStates.size())
		{
			return m_mStates.find(iState) != m_mStates.end();
		}

		return true;
	}

	Bool HawkProtocol::GetDecodeOS(OctetsStream*& pOS)
	{
		pOS = &m_sDecode;
		return true;
	}

	Bool HawkProtocol::Encode(HawkOctetsStream& rhsOS)
	{
		try
		{
			HawkOctetsStream sEncode;
			sEncode << *this;

			m_iSize = sEncode.Size();

#ifdef CRC_PROTOCOL
			m_iCrc  = HawkOSOperator::CalcCrc((UChar*)sEncode.Begin(), m_iSize);
#endif

			P_ProtocolManager->WriteProtocolHeader(rhsOS, m_iType, m_iSize, m_iCrc);

			rhsOS.Push(sEncode.Begin(),m_iSize);
		}
		catch (HawkException& rhsExcep)
		{
			HawkFmtError(rhsExcep.GetMsg().c_str());
			return false;
		}

		return true;
	}

	Bool HawkProtocol::Decode(HawkOctetsStream& rhsOS)
	{
		//检测协议完整性
		if (!P_ProtocolManager->CheckDecodeProtocol(rhsOS))
			return false;

		m_iSize = 0;
		m_iCrc  = 0;
		m_sDecode.Clear();

		ProtoType iType = 0;
		ProtoCrc  iCrc  = 0;
		try
		{		
			//读协议头
			rhsOS >> HawkOctetsStream::TransBegin;

			P_ProtocolManager->ReadProtocolHeader(rhsOS, iType, m_iSize, m_iCrc);
			
			if ((ProtoSize)rhsOS.AvailableSize() >= m_iSize)
			{
				//读协议数据
				if (m_iSize)
				{
					m_sDecode.Reserve(m_iSize);
					rhsOS.Pop((void*)m_sDecode.Begin(), m_iSize);
					m_sDecode.Resize(m_iSize);

#ifdef CRC_PROTOCOL
					iCrc = HawkOSOperator::CalcCrc((UChar*)m_sDecode.Begin(),m_iSize);
#endif
				}

				//读取完全,提交
				rhsOS >> HawkOctetsStream::TransCommit;
			}
			else
			{
				//数据包不完全,回滚
				rhsOS >> HawkOctetsStream::TransRollback;
				return false;
			}
		}
		catch (HawkException& rhsExcep)
		{
			rhsOS >> HawkOctetsStream::TransRollback;
			HawkFmtError(rhsExcep.GetMsg().c_str());
			return false;
		}

		//数据校验
		if(m_iType != iType)
		{
			HawkFmtError("Protocol Type Inconformity, Type: %u, Crc: %u", iType, m_iCrc);
			T_Exception("Protocol Type Inconformity.");
			return false;
		}
#ifdef CRC_PROTOCOL
		else if(m_iCrc != iCrc)
		{
			HawkFmtError("Protocol Crc Inconformity, Type: %u, Crc: %u", iType, m_iCrc);
			T_Exception("Protocol Crc Inconformity.");
			return false;
		}
#endif

		if (P_ProtocolManager->IsAutoDecode())
		{
			return DecodeSelf();
		}

		return true;
	}

	Bool HawkProtocol::DecodeSelf()
	{
		try
		{
			m_sDecode >> *this;
		}
		catch (HawkException& rhsExcep)
		{
			HawkFmtError(rhsExcep.GetMsg().c_str());		
			return false;
		}

		return true;
	}

	void HawkProtocol::SetProtoStates(const AString& sStates)
	{
		AStringVector asv;
		HawkStringUtil::Split<AString>(sStates, asv, ",");
		for (Size_t i=0;i<asv.size();i++)
		{
			HawkStringUtil::Trim<AString>(asv[i]);
			Int32 iState = HawkStringUtil::StringToInt<AString>(asv[i]);
			m_mStates[iState] = iState;
		}
	}
}
