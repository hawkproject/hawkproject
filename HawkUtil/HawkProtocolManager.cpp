#include "HawkProtocolManager.h"
#include "HawkLoggerManager.h"
#include "HawkOSOperator.h"
#include "HawkSysProtocol.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(ProtocolManager);

	HawkProtocolManager::Scope::Scope(HawkProtocol** ppProto) : m_ppProto(ppProto) 
	{
	};

	HawkProtocolManager::Scope::~Scope() 
	{ 
		HawkProtocolManager::GetInstance()->ReleaseProto(*m_ppProto); 
	}

	HawkProtocolManager::HawkProtocolManager()
	{
		m_bAutoDecode = true;
		m_iSizeLimit  = 0;
		m_iProtoSize  = sizeof(ProtoType) + sizeof(ProtoSize) + sizeof(ProtoCrc);		
	}

	HawkProtocolManager::~HawkProtocolManager()
	{		
		ProtocolMap::iterator it = m_mRegister.begin();
		for (;it!=m_mRegister.end();++it)
		{
			HAWK_RELEASE(it->second);
		}
		m_mRegister.clear();
	}	

	Bool HawkProtocolManager::RegSysProtocol()
	{
		//注册系统内部协议
		#include "HawkSysProtocolReg.inl"

		return true;
	}

	Bool HawkProtocolManager::CheckDecodeProtocol(const HawkOctetsStream& xOS, UInt32* pChunkSize)
	{
		if (xOS.AvailableSize() >= m_iProtoSize)
		{
			ProtoSize iSize = *((ProtoSize*)((Char*)xOS.AvailableData() + sizeof(ProtoType)));
			if (xOS.AvailableSize() >= m_iProtoSize + iSize)
			{
				if (pChunkSize)
					*pChunkSize = m_iProtoSize + iSize;

				return true;
			}
		}
		return false;
	}

	Bool HawkProtocolManager::SetSizeLimit(UInt32 iSize)
	{
		m_iSizeLimit = iSize;
		return true;
	}

	Bool HawkProtocolManager::CheckSizeLimit(UInt32 iSize) const
	{
		if (!m_iSizeLimit || m_iSizeLimit >= iSize)
			return true;

		return false;
	}

	UInt32 HawkProtocolManager::GetProtoHeaderSize() const
	{
		return m_iProtoSize;
	}

	Bool HawkProtocolManager::AnalyseProtocolHeader(const HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc)
	{
		if (xOS.AvailableSize() >= m_iProtoSize)
		{
			iType = *((ProtoType*)xOS.AvailableData());
			iSize = *((ProtoSize*)((Char*)xOS.AvailableData() + sizeof(ProtoType)));
			iCrc  = *((ProtoCrc*)((Char*)xOS.AvailableData() + sizeof(ProtoType) + sizeof(ProtoSize)));
			return true;			
		}
		return false;
	}

	Bool HawkProtocolManager::ReadProtocolHeader(HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc)
	{
		xOS.Pop(iType);
		xOS.Pop(iSize);
		xOS.Pop(iCrc);
		return true;
	}

	Bool HawkProtocolManager::WriteProtocolHeader(HawkOctetsStream& xOS, ProtoType iType, ProtoSize iSize, ProtoCrc iCrc)
	{
		xOS.Push(iType);
		xOS.Push(iSize);
		xOS.Push(iCrc);
		return true;
	}

	HawkProtocol*  HawkProtocolManager::Decode(HawkOctetsStream& rhsOS)
	{
		if (!CheckDecodeProtocol(rhsOS))
			return 0;

		//创建协议,开始解析
		ProtoType iType	= *((ProtoType*)rhsOS.AvailableData());
		Protocol* pProto = CreateProtocol(iType);
		if (pProto && !pProto->Decode(rhsOS))
		{
			ReleaseProto(pProto);
			pProto = 0;
		}
		return pProto;
	}

	Bool HawkProtocolManager::Register(ProtoType iType, HawkProtocol* pProto)
	{		
		ProtocolMap::iterator it = m_mRegister.find(iType);
		HawkAssert(it == m_mRegister.end());
		if (pProto && it == m_mRegister.end())
		{
			pProto->AddRef();
			m_mRegister[iType] = pProto;	
			return true;
		}
		return false;
	}

	Bool HawkProtocolManager::CheckProtocolLegal(ProtoType iType)
	{
		return m_mRegister.find(iType) != m_mRegister.end();
	}

	UInt32 HawkProtocolManager::GetRegProtoIds(vector<ProtoType>& vProtoIds, vector<UInt8>* pProtoDrive)
	{
		vProtoIds.clear();

		ProtocolMap::iterator it = m_mRegister.begin();
		for (;it!=m_mRegister.end();it++)
		{
			vProtoIds.push_back(it->first);
			if (pProtoDrive)
				pProtoDrive->push_back(it->second->GetDrive());
		}

		return (UInt32)vProtoIds.size();
	}

	Bool  HawkProtocolManager::IsAutoDecode() const
	{
		return m_bAutoDecode;
	}

	void  HawkProtocolManager::SetAutoDecode(Bool bAuto)
	{
		m_bAutoDecode = bAuto;
	}

	HawkProtocol*  HawkProtocolManager::CreateProtocol(ProtoType iType)
	{
		ProtocolMap::iterator it = m_mRegister.find(iType);
		if (it == m_mRegister.end())
		{
			HawkFmtError("Protocol Unregister, Type: %d", iType);
			T_Exception("Protocol Unregister.");
			return 0;
		}

		HawkProtocol* pProto = m_mRegister[iType]->Clone();
		return pProto;
	}

	Bool HawkProtocolManager::ReleaseProto(HawkProtocol*& pProto)
	{
		if (pProto)
		{
			pProto->Clear();
			HAWK_RELEASE(pProto);
			return true;
		}
		return false;
	}
}
