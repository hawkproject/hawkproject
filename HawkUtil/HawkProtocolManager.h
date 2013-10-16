#ifndef HAWK_PROTOCOLMANAGER_H
#define HAWK_PROTOCOLMANAGER_H

#include "HawkManagerBase.h"
#include "HawkProtocol.h"

namespace Hawk
{
	/************************************************************************/
	/* 协议管理器封装                                                       */
	/************************************************************************/
	class UTIL_API HawkProtocolManager : public HawkManagerBase
	{
	protected:
		//构造
		HawkProtocolManager();

		//析构
		virtual ~HawkProtocolManager();

		//管理器单例申明
		HAKW_SINGLETON_DECL(ProtocolManager);

	public:
		//协议注册表
		typedef map<ProtoType, Protocol*>  ProtocolMap;

		//自动释放封装
		class UTIL_API Scope : public HawkRefCounter
		{
		public:
			Scope(HawkProtocol** ppProto = 0);

			virtual ~Scope();

		protected:
			HawkProtocol**  m_ppProto;
		};

	public:
		//创建协议对象
		virtual HawkProtocol*  CreateProtocol(ProtoType iType);

		//解析协议数据
		virtual HawkProtocol*  Decode(HawkOctetsStream& rhsOS);

		//注册协议类型
		virtual Bool		   Register(ProtoType iType, HawkProtocol* pProto);		

		//判断协议是否注册
		virtual Bool		   CheckProtocolLegal(ProtoType iType);

		//释放协议对象
		virtual Bool		   ReleaseProto(HawkProtocol*& pProto);

		//依据配置文件生产协议代码
		virtual Bool		   GenProtocols(const AString& sCfgFile, const AString& sHawkHead = "HawkUtil.h");

		//直接解析协议内容(不保留在DecodeStream里面)
		virtual Bool		   IsAutoDecode() const;

		//设置自动解析标记
		virtual void		   SetAutoDecode(Bool bAuto = true);

	public:
		//分析协议包头
		virtual Bool		   AnalyseProtocolHeader(const HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc);

		//是否有可读协议包头
		virtual Bool		   CheckDecodeProtocol(const HawkOctetsStream& xOS, UInt32* pChunkSize = 0);

		//获取协议头字节大小
		virtual UInt32		   GetProtoHeaderSize() const;		

		//获取注册协议ID列表
		virtual UInt32		   GetRegProtoIds(vector<ProtoType>& vProtoIds, vector<UInt8>* pProtoDrive = 0);

		//注册系统内部协议
		virtual Bool		   RegSysProtocol();

		//设置协议字节数限制(默认0为不限)
		virtual Bool		   SetSizeLimit(UInt32 iSize);

		//检测协议字节数是否合法
		virtual Bool		   CheckSizeLimit(UInt32 iSize) const;

	public:
		//写协议包头
		virtual Bool		   ReadProtocolHeader(HawkOctetsStream& xOS, ProtoType& iType, ProtoSize& iSize, ProtoCrc& iCrc);

		//读协议包头
		virtual Bool		   WriteProtocolHeader(HawkOctetsStream& xOS,ProtoType iType, ProtoSize iSize, ProtoCrc iCrc);

	protected:		
		//注册的协议表
		ProtocolMap		 m_mRegister;
		//释放自动协议解析
		Bool			 m_bAutoDecode;
		//协议包头字节大小
		UInt32			 m_iProtoSize;
		//协议包字节数限制
		UInt32			 m_iSizeLimit;
	};

	#define P_ProtocolManager  HawkProtocolManager::GetInstance()

	//定义系统所能支持协议宏
	#define REGISTER_PROTO(protocol_class)\
	{\
		protocol_class* _ptr_ = new protocol_class;\
		P_ProtocolManager->Register(_ptr_->GetType(),_ptr_);\
		HAWK_RELEASE(_ptr_);\
	}
}
#endif
