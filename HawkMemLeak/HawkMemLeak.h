#ifndef HAWK_MEMLEAK_H
#define HAWK_MEMLEAK_H

//Debug模式下引入HawkMemLeak模块
#ifdef _DEBUG

//引用库文件
#pragma comment(lib, "HawkMemLeak.lib")

//强制链接符号
#pragma comment(linker, "/include:__imp_?hawk_MemLeakDetect@@3VHawkMemLeakDetect@Hawk@@A")

#endif

#endif
