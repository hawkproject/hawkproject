@echo off     

set HAWKSDK_HOME=E:\Program\HawkSDK
set PROJECT_HOME=E:\ChinaSVN\HawkProject

echo 创建HawkSDK部署文件夹
mkdir 	 %HAWKSDK_HOME%
mkdir 	 %HAWKSDK_HOME%\include
mkdir 	 %HAWKSDK_HOME%\include\json
mkdir 	 %HAWKSDK_HOME%\include\zlib
mkdir 	 %HAWKSDK_HOME%\include\sqlite
mkdir 	 %HAWKSDK_HOME%\include\rapidxml
mkdir 	 %HAWKSDK_HOME%\include\fastdelegate
mkdir 	 %HAWKSDK_HOME%\lib
mkdir 	 %HAWKSDK_HOME%\bin

echo 拷贝HawkSDK文件
cd		 %PROJECT_HOME%
xcopy /y .\HawkUtil\Hawk*.h  						%HAWKSDK_HOME%\include\
xcopy /y .\HawkUtil\Hawk*.inl  						%HAWKSDK_HOME%\include\
xcopy /y .\HawkUtil\source\json\json\*.h			%HAWKSDK_HOME%\include\json\
xcopy /y .\HawkUtil\source\zlib\z*.h				%HAWKSDK_HOME%\include\zlib\
xcopy /y .\HawkUtil\source\sqlite\sqlite*.h			%HAWKSDK_HOME%\include\sqlite\
xcopy /y .\HawkUtil\source\rapidxml\rapidxml*.hpp	%HAWKSDK_HOME%\include\rapidxml\
xcopy /y .\HawkUtil\source\fastdelegate\*.h			%HAWKSDK_HOME%\include\fastdelegate\
xcopy /y .\HawkLog\Hawk*.h  						%HAWKSDK_HOME%\include\
xcopy /y .\HawkWin32\Hawk*.h  						%HAWKSDK_HOME%\include\
xcopy /y .\HawkMemLeak\Hawk*.h  					%HAWKSDK_HOME%\include\
xcopy /y .\HawkGeometry\Hawk*.h  					%HAWKSDK_HOME%\include\
xcopy /y .\HawkProfiler\Hawk*.h     				%HAWKSDK_HOME%\include\
xcopy /y .\HawkGateway\Hawk*.h      				%HAWKSDK_HOME%\include\
xcopy /y .\HawkRedis\Hawk*.h      					%HAWKSDK_HOME%\include\
xcopy /y .\HawkAppFrame\Hawk*.h      				%HAWKSDK_HOME%\include\

xcopy /y .\Debug\HawkUtil_d.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkLog_d.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkWin32_d.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkMemLeak.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkGeometry_d.lib 				%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkProfiler_d.lib 				%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkGateway_d.lib  				%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkRedis_d.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Debug\HawkAppFrame_d.lib  				%HAWKSDK_HOME%\lib

xcopy /y .\Debug\HawkMemLeak.dll  					%HAWKSDK_HOME%\bin

xcopy /y .\Release\HawkUtil.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkLog.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkWin32.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkGeometry.lib 				%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkProfiler.lib 				%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkGateway.lib  				%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkRedis.lib  					%HAWKSDK_HOME%\lib
xcopy /y .\Release\HawkAppFrame.lib 				%HAWKSDK_HOME%\lib

xcopy /y .\HawkUtil\win_bin\*.dll  					%HAWKSDK_HOME%\bin
xcopy /y .\HawkUtil\win_bin\*.exe  					%HAWKSDK_HOME%\bin

xcopy /y .\Release\ProtocolGen.exe  				%HAWKSDK_HOME%\bin
xcopy /y .\Release\ProcMonitor.exe  				%HAWKSDK_HOME%\bin
xcopy /y .\Release\LogServer.exe    				%HAWKSDK_HOME%\bin
xcopy /y .\Release\DomainSvr.exe    				%HAWKSDK_HOME%\bin
xcopy /y .\Release\GateServer.exe   				%HAWKSDK_HOME%\bin
xcopy /y .\Release\ProfilerMonitor.exe  			%HAWKSDK_HOME%\bin
xcopy /y .\Release\DataPacker.exe  					%HAWKSDK_HOME%\bin
xcopy /y .\Release\GameAnalyst.exe  				%HAWKSDK_HOME%\bin
xcopy /y .\Release\ExcelExporter.exe  				%HAWKSDK_HOME%\bin

echo 发布HawkSDK完成    
echo. & pause  
