#include "HawkUtil.h"

struct ProfInfo 
{
	UInt32	CpuCount;
	UInt32	Cpu_Usage;
	UInt64  Mem_Total;
	UInt64	Mem_Usage;
	UInt32	PeerCount;
	UInt64	RecvProto;
	UInt64	SendProto;
	UInt64	RecvSize;
	UInt64	SendSize;
};

struct FrameInfo
{
	UInt64	RecvProto;
	UInt64	SendProto;
	UInt64	RecvSize;
	UInt64	SendSize;

	UInt32  RecvProtoSpeed;
	UInt32  SendProtoSpeed;
	UInt32	RecvSizeSpeed;
	UInt32	SendSizeSpeed;
};

void PrintProf(const ProfInfo& sProf, const FrameInfo& sStub)
{
	//�����Ļ
	HawkOSOperator::ClearConsole();
	HawkOSOperator::Gotoxy(0,0);

	//��ӡ��ʶͷ
	HawkPrint("==============================================================");
	HawkPrint("==============ProfilerMonitor, Author: Daijunhua==============");
	HawkPrint("==============================================================");

	//CPU
	HawkFmtPrint("Processor:      %d", sProf.CpuCount);
	HawkFmtPrint("Cpu-Usage:      %d", sProf.Cpu_Usage);

	//�ڴ�
	HawkFmtPrint("PhyMemory:      %lld(MB)", sProf.Mem_Total / 1048576);
	HawkFmtPrint("Mem-Usage:      %lld(KB)", sProf.Mem_Usage / 1024);

	//������
	HawkFmtPrint("Connections:    %d", sProf.PeerCount);

	//Э����
	HawkFmtPrint("RecvProtocol:   %d / %lld", sStub.RecvProtoSpeed,  sProf.RecvProto);
	HawkFmtPrint("SendProtocol:   %d / %lld", sStub.SendProtoSpeed,  sProf.SendProto);

	//����
	HawkFmtPrint("RecvBytes(KB):  %d / %lld", sStub.RecvSizeSpeed / 1024, sProf.RecvSize / 1024);
	HawkFmtPrint("SendBytes(KB):  %d / %lld", sStub.SendSizeSpeed / 1024, sProf.SendSize / 1024);

	HawkOSOperator::HideCursor();
}

void StartMonitor(const AString& sSvrAddr)
{
	HawkSocket   sSocket;
	ProfInfo     sProf;
	FrameInfo	 sStub;
	OctetsStream sOctets(PAGE_SIZE);
	memset(&sProf, 0, sizeof(sProf));
	memset(&sStub, 0, sizeof(sStub));
	sSocket.InitUdpClient(sSvrAddr, true);

	PrintProf(sProf, sStub);

	UInt32 iMonitor = 0;
	do
	{
		//���Ͳ�ѯ����
		sOctets.Clear();
		SysProtocol::Sys_ProfReq sReq;
		sReq.Encode(sOctets);		
		Size_t iSize = sOctets.Size();
		SocketAddr sAddr(sSvrAddr);
		sSocket.SendTo(sOctets.Begin(), iSize, sAddr);

		//������Ϣ
		sOctets.Clear();
		iSize = sOctets.Capacity();
		if (sSocket.ReceiveFrom(sOctets.Begin(), iSize, sAddr))
		{
			//����Э��
			sOctets.Resize(iSize);
			SysProtocol::Sys_ProfInfo sRep;
			if (sRep.Decode(sOctets))
			{
				sProf.CpuCount  = sRep.m_iCpuCount;
				sProf.Cpu_Usage = sRep.m_iCpuUsage;
				sProf.Mem_Total = sRep.m_iTotalMem;
				sProf.Mem_Usage = sRep.m_iMemUsage;
				sProf.PeerCount = sRep.m_iConnect;
				sProf.RecvProto = sRep.m_iRecvProto;
				sProf.SendProto = sRep.m_iSendProto;
				sProf.RecvSize  = sRep.m_iRecvSize;
				sProf.SendSize  = sRep.m_iSendSize;

				if (iMonitor)
				{
					sStub.RecvProtoSpeed = (UInt32)(sRep.m_iRecvProto - sStub.RecvProto);
					sStub.SendProtoSpeed = (UInt32)(sRep.m_iSendProto - sStub.SendProto);
					sStub.RecvSizeSpeed  = (UInt32)(sRep.m_iRecvSize  - sStub.RecvSize);
					sStub.SendSizeSpeed  = (UInt32)(sRep.m_iSendSize  - sStub.SendSize);
				}

				sStub.RecvProto = sRep.m_iRecvProto;
				sStub.SendProto = sRep.m_iSendProto;
				sStub.RecvSize  = sRep.m_iRecvSize;
				sStub.SendSize  = sRep.m_iSendSize;

				PrintProf(sProf, sStub);
			}
		}

		HawkSleep(1000);
	}while (++iMonitor);
}

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	if (argc < 2) 
	{
		printf("usage: profilermonitor <ip:port>.\r\n");
		return -1;
	}

	//������
	HawkOSOperator::ClearConsole();

	//����ģ����
	StartMonitor(argv[1]);

	HawkUtil::Stop();
	HawkUtil::Release();
}
