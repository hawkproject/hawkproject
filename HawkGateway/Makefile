#添加规则
include ../Rules.mk

#引用头文件路径
INC_DIRS    += -I../HawkUtil -I../HawkProfiler -I../HawkUtil/source/zlib -I../HawkUtil/lnx_inc/libevent -I../HawkUtil/lnx_inc/libzmq

#生成库文件
LIB_HAWKGATEWAY= ../$(BIN_DIR)/libhawkgateway.a

#宏定义修改
MACROS += -DGATE_EXPORT

#编译标签
all: $(OBJ_FILES)
	$(AR) $(LIB_HAWKGATEWAY) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
