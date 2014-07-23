#添加规则
include ../Rules.mk

#引用头文件路径
INC_DIRS    += -I../HawkUtil

#生成库文件
LIB_HAWKGEOMETRY = ../$(BIN_DIR)/libhawkgeometry.a

#宏定义修改
MACROS += -DGEO_EXPORT

#编译标签
all: $(OBJ_FILES)
	$(AR) $(LIB_HAWKGEOMETRY) $(GEN_OBJS)

clean: force
	$(RM) $(OUT_DIR)

force:
