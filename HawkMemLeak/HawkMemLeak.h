#ifndef HAWK_MEMLEAK_H
#define HAWK_MEMLEAK_H

//Debugģʽ������HawkMemLeakģ��
#ifdef _DEBUG

//���ÿ��ļ�
#pragma comment(lib, "HawkMemLeak.lib")

//ǿ�����ӷ���
#pragma comment(linker, "/include:__imp_?hawk_MemLeakDetect@@3VHawkMemLeakDetect@Hawk@@A")

#endif

#endif
