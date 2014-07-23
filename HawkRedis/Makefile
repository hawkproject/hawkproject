#添加规则
include ../Rules.mk

#引用头文件路径
INC_DIRS    += -I../HawkUtil -I../HawkUtil/lnx_inc/libevent

#生成库文件
LIB_HAWKREDIS = ../$(BIN_DIR)/libhawkredis.a

#宏定义修改
MACROS += -DREDIS_EXPORT

#编译标签
all: $(OBJ_FILES)
	$(AR) $(LIB_HAWKREDIS) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
