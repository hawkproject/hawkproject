#ifndef HAWK_PROFILER_H
#define HAWK_PROFILER_H

#include "HawkUtil.h"

namespace Hawk
{
	/************************************************************************/
	/* �������ܼ����                                                       */
	/************************************************************************/
	class PRO_API HawkProfiler : public HawkRefCounter
	{
	public:
		//����
		HawkProfiler(const AString& sAddr = "");

		//����
		virtual ~HawkProfiler();

	public:
		//��ʼ������
		virtual Bool   Start(const AString& sAddr = "");

		//ֹͣ���
		virtual Bool   Stop();

	public:
		//�Ǽǽ���Э��
		virtual Bool   RegRecvProto(ProtoType iType, ProtoSize iSize);
		
		//�ǼǷ���Э��
		virtual Bool   RegSendProto(ProtoType iType, ProtoSize iSize);
		
		//�Ǽ�����
		virtual Bool   RegConnect(Bool bConn);

		//��ȡCPU����
		virtual UInt32 GetProcessorNumber();

		//��ȡ�����ڴ���
		virtual UInt64 GetTotalMem();

		//��ȡCPUʹ����
		virtual UInt32 GetCpuUsage();

		//��ȡռ���ڴ��С
		virtual UInt64 GetMemUsage();

		//��ȡ������
		virtual UInt32 GetPeerCount() const;

		//��ȡ����Э����
		virtual UInt64 GetRecvProto() const;

		//��ȡ����Э���ֽ�
		virtual UInt64 GetRecvSize() const;

		//��ȡ����Э����
		virtual UInt64 GetSendProto() const;

		//��ȡ����Э���ֽ�
		virtual UInt64 GetSendSize() const;

	public:
		//ִ�з����߳�ѭ��
		virtual Bool   RunLoop();

	protected:
		//�Ƿ�������
		volatile Bool m_bRunning;
		//�����ַ
		AString		  m_sAddr;
		//�����׽���(UDPģʽ)
		HawkSocket	  m_sSocket;
		//�����߳�
		HawkThread*	  m_pThread;
		//CPU����
		UInt32		  m_iCpuCount;
		//�������ڴ�
		UInt64		  m_iTotalMem;
		//CPUʹ����
		UInt32		  m_iCpuUsage;
		//�ڴ�ʹ����
		UInt64		  m_iMemUsage;
		//��Ч������
		UInt32		  m_iConnect;
		//Э����ո���
		UInt64		  m_iRecvProto;
		//Э������ֽ�
		UInt64		  m_iRecvSize;
		//Э�鷢�͸���
		UInt64		  m_iSendProto;
		//Э�鷢���ֽ�
		UInt64		  m_iSendSize;
		//UDP���ݽ���Buffer
		OctetsStream* m_pRecvBuf;
	};
}
#endif
