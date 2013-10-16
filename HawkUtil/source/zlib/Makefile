#---------------------------------------------
#添加规则
include ../../../Rules.mk

#zlib使用32位文件操作API宏定义
MACROS += -DUSE_FILE32API

#---------------------------------------------
#~~~~~~生成的库文件
LIB_ZLIB = $(BIN_DIR)/libz.a

#---------------------------------------------
#~~~~~~编译标签设置
all: $(OBJ_FILES)
	$(AR) $(LIB_ZLIB) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
