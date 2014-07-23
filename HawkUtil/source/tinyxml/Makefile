#---------------------------------------------
#添加规则
include ../../../Rules.mk

#---------------------------------------------
#~~~~~~生成的库文件
LIB_TINYXML = $(BIN_DIR)/libtinyxml.a

#---------------------------------------------
#~~~~~~编译标签设置
all: $(OBJ_FILES)
	$(AR) $(LIB_TINYXML) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
