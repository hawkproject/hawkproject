#ifndef HAWK_PROTOCOLMANAGER_H
#define HAWK_PROTOCOLMANAGER_H

#include "HawkManagerBase.h"
#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* Э���������װ                                                       */
	/************************************************************************/
	class UTIL_API HawkProtocolManager : public HawkManagerBase
	{
	protected:
		//����
		HawkProtocolManager();

		//����
		virtual ~HawkProtocolManager();

		//��������������
		HAKW_SINGLETON_DECL(ProtocolManager);

	public:
		//Э��ע���
		typedef map<ProtoType, Protocol*>  ProtocolMap;

		//�Զ��ͷŷ�װ
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkProtocol** ppProto = 0);

			virtual ~Scope();

		protected:
			HawkProtocol**  m_ppProto;
		};

	public:
		//����Э�����
		virtual HawkProtocol*  CreateProtocol(ProtoType iType);

		//����Э������
		virtual HawkProtocol*  Decode(HawkOctetsStream& rhsOS);

		//ע��Э������
		virtual Bool		   Register(ProtoType iType, HawkProtocol* pProto);		

		//�ж�Э���Ƿ�ע��
		virtual Bool		   CheckProtocolLegal(ProtoType iType);

		//�ͷ�Э�����
		virtual Bool		   ReleaseProto(HawkProtocol*& pProto);

		//���������ļ�����Э�����
		virtual Bool		   GenProtocols(const AString& sCfgFile, const AString& sArgs = "");

		//ֱ�ӽ���Э������(��������DecodeStream����)
		virtual Bool		   IsAutoDecode() const;

		//�����Զ��������
		virtual void		   SetAutoDecode(Bool bAuto = true);

	public:
		//����Э���ͷ
		virtual Bool		   AnalyseProtocolHeader(const HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc);

		//�Ƿ��пɶ�Э���ͷ
		virtual Bool		   CheckDecodeProtocol(const HawkOctetsStream& xOS, UInt32* pChunkSize = 0);

		//��ȡЭ��ͷ�ֽڴ�С
		virtual UInt32		   GetProtoHeaderSize() const;		

		//��ȡע��Э��ID�б�
		virtual UInt32		   GetRegProtoIds(vector<ProtoType>& vProtoIds, vector<UInt8>* pProtoDrive = 0);

		//ע��ϵͳ�ڲ�Э��
		virtual Bool		   RegSysProtocol();

		//����Э���ֽ�������(Ĭ��0Ϊ����)
		virtual Bool		   SetSizeLimit(UInt32 iSize);

		//���Э���ֽ����Ƿ�Ϸ�
		virtual Bool		   CheckSizeLimit(UInt32 iSize) const;

	public:
		//дЭ���ͷ
		virtual Bool		   ReadProtocolHeader(HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc);

		//��Э���ͷ
		virtual Bool		   WriteProtocolHeader(HawkOctetsStream& xOS,ProtoType iType, ProtoSize iSize, ProtoCrc iCrc);

	protected:		
		//ע���Э���
		ProtocolMap		  m_mRegister;
		//�ͷ��Զ�Э�����
		Bool			  m_bAutoDecode;
		//Э���ͷ�ֽڴ�С
		UInt32			  m_iProtoSize;
		//Э����ֽ�������
		UInt32			  m_iSizeLimit;
	};

	#define P_ProtocolManager  HawkProtocolManager::GetInstance()

	//����ϵͳ����֧��Э���
	#define REGISTER_PROTO(protocol_class)\
	{\
		protocol_class* _ptr_ = new protocol_class;\
		P_ProtocolManager->Register(_ptr_->GetType(),_ptr_);\
		HAWK_RELEASE(_ptr_);\
	}
}
#endif
