#!/bin/sh

#使用ulimit -c命令可查看core文件的生成开关
#使用ulimit -c filesize命令，可以限制core文件的大小（filesize的单位为kbyte）
#若ulimit -c unlimited，则表示core文件的大小不受限制
#尽量在系统 /etc/security/limits.conf中配置
ulimit -c unlimited

#/proc/sys/kernel/core_uses_pid可以控制core文件的文件名中是否添加pid作为扩展
echo "1" > /proc/sys/kernel/core_uses_pid

#proc/sys/kernel/core_pattern可以控制core文件保存位置和文件名格式
echo "/data/home/app100661088/pml/core/core.%e.%p.%t" > /proc/sys/kernel/core_pattern

#上述修改方法只对当前会话生效,系统重启又重置
#若希望永久生效,即修改 /etc/security/limits.conf 或者 /etc/profile 中配置