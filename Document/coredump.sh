#!/bin/sh

#ʹ��ulimit -c����ɲ鿴core�ļ������ɿ���
#ʹ��ulimit -c filesize�����������core�ļ��Ĵ�С��filesize�ĵ�λΪkbyte��
#��ulimit -c unlimited�����ʾcore�ļ��Ĵ�С��������
#������ϵͳ /etc/security/limits.conf������
ulimit -c unlimited

#/proc/sys/kernel/core_uses_pid���Կ���core�ļ����ļ������Ƿ����pid��Ϊ��չ
echo "1" > /proc/sys/kernel/core_uses_pid

#proc/sys/kernel/core_pattern���Կ���core�ļ�����λ�ú��ļ�����ʽ
echo "/data/home/app100661088/pml/core/core.%e.%p.%t" > /proc/sys/kernel/core_pattern

#�����޸ķ���ֻ�Ե�ǰ�Ự��Ч,ϵͳ����������
#��ϣ��������Ч,���޸� /etc/security/limits.conf ���� /etc/profile ������