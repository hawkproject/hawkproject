#=========================================================================
#Makefile模板设置, Author: 戴俊华
#=========================================================================

#-------------------------------------------------------------------------
#~~~~~~常用函数介绍@
#addprefix: 添加前缀
#addsuffix: 添加后缀
#basename:  去后缀
#wildcard:	遍历
#patsubst:	模式匹配替换
#subst:		指定替换
#@echo：	打印

#-------------------------------------------------------------------------
#~~~~~~编译器选项@
#调试模式
DEBUG = y
ifdef release
DEBUG = n
endif
#动态库模式
SOLIB = n
#c编译器
CC  = gcc
#c++编译器
CPP = g++
#c++编译选项,在Release模式可添加 -o(1,2,3)优化选项
CPPFLAGS = -Wall -ansi -fexceptions
#.o中间文件生成选项
OBJFLAGS = -c
#静态库(.a)编译标签,目标格式: {.o}
AR = ar rcs
#动态库(.so)编译标签,目标格式: {.o}
PIC = -fPIC
#动态库(.so)连接标签,目标格式: .so {.o}
SO = -shared -o
#删除命令
RM = -rm -rf
#编译模式(debug/release)对标记的修改
#CPPFLAGS += -march=i686
ifeq ($(DEBUG), y)
	CPPFLAGS += -g -ggdb
else
	CPPFLAGS += -O3
endif
#.o生成(.a/.so)标签的修改
ifeq ($(SOLIB), y)
	OBJFLAGS += $(PIC)
endif
#设置导出基础变量定义
export DEBUG SOLIB CPP CPPFLAGS OBJFLAGS AR PIC SO RM

#-------------------------------------------------------------------------
#~~~~~~宏定义设置@
#编译宏定义
MACROS = -DLINUX -D_FILE_OFFSET_BITS=64
#编译模式(debug/release)对标记的修改
ifeq ($(DEBUG), y)
	MACROS += -D_DEBUG
else
	MACROS += -DNDEBUG
endif
#设置导出编译宏定义
export MACROS

#-------------------------------------------------------------------------
#~~~~~~输出目录设置@
#编译文件输出目录
ifeq ($(DEBUG), y)
	OUT_DIR = debug
else
	OUT_DIR = release
endif
#中间文件(.o)生成路径
OBJ_DIR = $(OUT_DIR)/objs
#目标生成路径
BIN_DIR = $(OUT_DIR)/bin
#创建输出目录结构
$(shell mkdir -p $(OUT_DIR))
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))
#设置导出生成文件目录结构
export OUT_DIR OBJ_DIR BIN_DIR

#-------------------------------------------------------------------------
#~~~~~~本目录文件集合@
INC_FILES = $(wildcard *.h*)
SRC_FILES = $(wildcard *.c*)
OBJ_FILES = $(addsuffix .o, $(basename $(SRC_FILES)))
GEN_OBJS  = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRC_FILES))))

#-------------------------------------------------------------------------
#~~~~~~引用头文件路径@
INC_DIRS = -I. -I/usr/include/mysql
LIB_DIRS = -L. -L/usr/lib64 -L/usr/lib64/mysql
LNK_LIBS = -lrt -ldl -lpthread -lmysqlclient

#-------------------------------------------------------------------------
#~~~~~~中间文件生成规则@
.c.o:
	$(CC)  $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cc.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cxx.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cpp.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

#-------------------------------------------------------------------------
#~~~~~~伪目标申明@
.PHONY: all depend install clean

#-------------------------------------------------------------------------
#~~~~~~生成依赖文件@
#~~~~~~需要添加在all目标之后,否则视为默认生成目标@
#depend: $(SRC_FILES)
#	$(RM)  .depend
#	$(CPP) -MM $(MACROS) $(INC_DIRS) $^ > .depend

#-------------------------------------------------------------------------
#~~~~~~添加依赖@
ifeq (.depend,$(wildcard .depend))
include .depend
endif
