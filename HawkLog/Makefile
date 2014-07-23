#添加规则
include ../Rules.mk

#引用头文件路径
INC_DIRS    += -I../HawkUtil

#生成库文件
LIB_HAWKLOG = ../$(BIN_DIR)/libhawklog.a

#宏定义修改
MACROS += -DLOG_EXPORT

#编译标签
all: $(OBJ_FILES)
	$(AR) $(LIB_HAWKLOG) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
