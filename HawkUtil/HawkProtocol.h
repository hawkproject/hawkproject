#ifndef HAWK_PROTOCOL_H
#define HAWK_PROTOCOL_H

#include "HawkMarshalData.h"
#include "HawkSocket.h"

namespace Hawk
{
	/************************************************************************/
	/* 协议格式定义操作封装                                                 */
	/************************************************************************/
	class UTIL_API HawkProtocol : public HawkMarshal
	{
	public:
		//驱动类型
		enum DriveType
		{
			DRIVE_EVENT = 0,	//事件(每次发送)
			DRIVE_STATE,		//状态(改变发送)
		};

		typedef map<Int32, Int32> ProtoStates;

	public:
		//协议构造
		HawkProtocol(ProtoType iType = 0, UInt8 iDrive = DRIVE_EVENT, const AString& sStates = "");

		//协议析构
		virtual ~HawkProtocol();				

	public:
		//克隆协议(实际协议必须实现)
		virtual HawkProtocol* Clone() const = 0;

	public:		
		//协议打包: Type + Size + Crc + Data
		virtual Bool  Encode(HawkOctetsStream& rhsOS);

		//协议解包
		virtual Bool  Decode(HawkOctetsStream& rhsOS);

		//获取自解Buffer
		virtual Bool  GetDecodeOS(OctetsStream*& pOS);
		
	public:
		//获取协议类型
		ProtoType	  GetType() const;

		//获取字节数
		ProtoSize     GetSize() const;

		//获取协议数据校验CRC
		ProtoCrc	  GetCrc() const;		

		//获取驱动模式
		UInt8		  GetDrive() const;

		//获取适应场景
		Bool		  IsStateSuitable(Int32 iState) const;

	protected:
		//自解析,默认情况下会自解析
		virtual Bool  DecodeSelf();

		//设置协议适合场景列表
		virtual void  SetProtoStates(const AString& sStates);

	protected:
		//类型
		ProtoType	 m_iType;
		//字节数
		ProtoSize	 m_iSize;
		//校验码
		ProtoCrc	 m_iCrc;
		//驱动
		UInt8		 m_iDrive;
		//数据存储器
		OctetsStream m_sDecode;
		//使用状态
		ProtoStates  m_mStates;
	};

	//协议类型简便定义
	typedef HawkProtocol Protocol;
}
#endif
