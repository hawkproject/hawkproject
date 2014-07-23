#=========================================================================
#Makefileģ������, Author: ������
#=========================================================================

#-------------------------------------------------------------------------
#~~~~~~���ú�������@
#addprefix: ���ǰ׺
#addsuffix: ��Ӻ�׺
#basename:  ȥ��׺
#wildcard:	����
#patsubst:	ģʽƥ���滻
#subst:		ָ���滻
#@echo��	��ӡ

#-------------------------------------------------------------------------
#~~~~~~������ѡ��@
#����ģʽ
DEBUG = y
ifdef release
DEBUG = n
endif
#��̬��ģʽ
SOLIB = n
#c������
CC  = gcc
#c++������
CPP = g++
#c++����ѡ��,��Releaseģʽ����� -o(1,2,3)�Ż�ѡ��
CPPFLAGS = -Wall -ansi -fexceptions
#.o�м��ļ�����ѡ��
OBJFLAGS = -c
#��̬��(.a)�����ǩ,Ŀ���ʽ: {.o}
AR = ar rcs
#��̬��(.so)�����ǩ,Ŀ���ʽ: {.o}
PIC = -fPIC
#��̬��(.so)���ӱ�ǩ,Ŀ���ʽ: .so {.o}
SO = -shared -o
#ɾ������
RM = -rm -rf
#����ģʽ(debug/release)�Ա�ǵ��޸�
#CPPFLAGS += -march=i686
ifeq ($(DEBUG), y)
	CPPFLAGS += -g -ggdb
else
	CPPFLAGS += -O3
endif
#.o����(.a/.so)��ǩ���޸�
ifeq ($(SOLIB), y)
	OBJFLAGS += $(PIC)
endif
#���õ���������������
export DEBUG SOLIB CPP CPPFLAGS OBJFLAGS AR PIC SO RM

#-------------------------------------------------------------------------
#~~~~~~�궨������@
#����궨��
MACROS = -DLINUX -D_FILE_OFFSET_BITS=64
#����ģʽ(debug/release)�Ա�ǵ��޸�
ifeq ($(DEBUG), y)
	MACROS += -D_DEBUG
else
	MACROS += -DNDEBUG
endif
#���õ�������궨��
export MACROS

#-------------------------------------------------------------------------
#~~~~~~���Ŀ¼����@
#�����ļ����Ŀ¼
ifeq ($(DEBUG), y)
	OUT_DIR = debug
else
	OUT_DIR = release
endif
#�м��ļ�(.o)����·��
OBJ_DIR = $(OUT_DIR)/objs
#Ŀ������·��
BIN_DIR = $(OUT_DIR)/bin
#�������Ŀ¼�ṹ
$(shell mkdir -p $(OUT_DIR))
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))
#���õ��������ļ�Ŀ¼�ṹ
export OUT_DIR OBJ_DIR BIN_DIR

#-------------------------------------------------------------------------
#~~~~~~��Ŀ¼�ļ�����@
INC_FILES = $(wildcard *.h*)
SRC_FILES = $(wildcard *.c*)
OBJ_FILES = $(addsuffix .o, $(basename $(SRC_FILES)))
GEN_OBJS  = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRC_FILES))))

#-------------------------------------------------------------------------
#~~~~~~����ͷ�ļ�·��@
INC_DIRS = -I. -I/usr/include/mysql
LIB_DIRS = -L. -L/usr/lib64 -L/usr/lib64/mysql
LNK_LIBS = -lrt -ldl -lpthread -lmysqlclient

#-------------------------------------------------------------------------
#~~~~~~�м��ļ����ɹ���@
.c.o:
	$(CC)  $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cc.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cxx.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

.cpp.o:
	$(CPP) $(CPPFLAGS) $(OBJFLAGS) $(MACROS) $(INC_DIRS) $< -o $(OBJ_DIR)/$@

#-------------------------------------------------------------------------
#~~~~~~αĿ������@
.PHONY: all depend install clean

#-------------------------------------------------------------------------
#~~~~~~���������ļ�@
#~~~~~~��Ҫ�����allĿ��֮��,������ΪĬ������Ŀ��@
#depend: $(SRC_FILES)
#	$(RM)  .depend
#	$(CPP) -MM $(MACROS) $(INC_DIRS) $^ > .depend

#-------------------------------------------------------------------------
#~~~~~~�������@
ifeq (.depend,$(wildcard .depend))
include .depend
endif
