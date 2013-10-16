#---------------------------------------------
#添加规则
include Rules.mk

#设置SDK输出目录
HAWK_SDK = ../HawkSDK

#---------------------------------------------
#~~~~~~涉及目录设置
HAWK_DIRS   = HawkUtil \
			  HawkLog \
			  HawkGeometry \
			  HawkProfiler \
			  HawkGateway \
			  HawkAppFrame \
			  HawkRedis

TOOL_DIRS   = Tools/ProtocolGen \
			  Tools/ProcMonitor \
			  Tools/ProfilerMonitor \
			  Tools/LogServer \
			  Tools/GameAnalyst \
			  Tools/DomainSvr \
			  Tools/GateServer \
			  Tools/DataPacker \
			  Tools/FileAcceptor \
			  Tools/FileSender \
			  Tools/CmdSender \
			  Tools/NetBridge \
			  Samples/EchoServer \
			  Samples/EchoClient \
			  Samples/TestCase

all: $(HAWK_DIRS) $(TOOL_DIRS)

$(HAWK_DIRS): force
	$(MAKE) -C $@

$(TOOL_DIRS): force
	$(MAKE) -C $@

install: force	
	-mkdir -p $(HAWK_SDK)
	-mkdir -p $(OUT_DIR)/sdk
	-mkdir -p $(OUT_DIR)/sdk/lib
	-mkdir -p $(OUT_DIR)/sdk/bin	
	-mkdir -p $(OUT_DIR)/sdk/include	
	-mkdir -p $(OUT_DIR)/sdk/include/json	
	-mkdir -p $(OUT_DIR)/sdk/include/zlib
	-mkdir -p $(OUT_DIR)/sdk/include/sqlite
	-mkdir -p $(OUT_DIR)/sdk/include/rapidxml
	-cp -rf HawkUtil/*.h							$(OUT_DIR)/sdk/include
	-cp -rf HawkUtil/source/json/json/*.h			$(OUT_DIR)/sdk/include/json
	-cp -rf HawkUtil/source/zlib/z*.h				$(OUT_DIR)/sdk/include/zlib
	-cp -rf HawkUtil/source/sqlite/sqlite*.h		$(OUT_DIR)/sdk/include/sqlite
	-cp -rf HawkUtil/source/rapidxml/rapidxml*.hpp  $(OUT_DIR)/sdk/include/rapidxml
	-cp -rf HawkLog/*.h								$(OUT_DIR)/sdk/include
	-cp -rf HawkGeometry/*.h						$(OUT_DIR)/sdk/include
	-cp -rf HawkProfiler/*.h						$(OUT_DIR)/sdk/include
	-cp -rf HawkGateway/*.h							$(OUT_DIR)/sdk/include
	-cp -rf HawkRedis/*.h							$(OUT_DIR)/sdk/include
	-cp -rf HawkAppFrame/*.h						$(OUT_DIR)/sdk/include
	-cp -rf $(BIN_DIR)/libhawk*.a					$(OUT_DIR)/sdk/lib
	-cp -rf $(BIN_DIR)/protocolgen					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/procmonitor					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/echoserver					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/echoclient					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/testcase						$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/logserver					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/domainsvr					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/gateserver					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/datapacker					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/fileacceptor					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/filesender					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/cmdsender					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/netbridge					$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/profilermonitor				$(OUT_DIR)/sdk/bin
	-cp -rf $(BIN_DIR)/gameanalyst					$(OUT_DIR)/sdk/bin
	-cp -rf $(OUT_DIR)/sdk/include					$(HAWK_SDK)
	-cp -rf $(OUT_DIR)/sdk/lib						$(HAWK_SDK)
	-cp -rf $(OUT_DIR)/sdk/bin						$(HAWK_SDK)

clean: force
	$(RM) $(OUT_DIR)
	-($(foreach dir,$(HAWK_DIRS),$(MAKE) -C $(dir) clean;))
	-($(foreach dir,$(TOOL_DIRS),$(MAKE) -C $(dir) clean;))

force:
