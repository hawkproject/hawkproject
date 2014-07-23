#!/bin/sh

# ������Ϣ
db_name="hawk"
db_user="root"
db_pwd="123456"
host_ip="localhost"
host_port="3306"
bin_dir="/usr/bin"
bak_dir="/data/home/app100661088/pml/dbdump"
fix_name=`date +%F-%T`

if [ ! -d "$bak_dir" ]; then 
mkdir "$bak_dir" 
fi

# ��������
$db_name="pml_svr2"
$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db_name > $bak_dir/$db_name"_"$fix_name".sql"

$db_name="pml_svr3"
$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db_name > $bak_dir/$db_name"_"$fix_name".sql"

$db_name="pml_svr4"
$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db_name > $bak_dir/$db_name"_"$fix_name".sql"

#���뵽Linux�Զ�����,���˽ű��ӵ� /etc/crontab ��ʱ������
#��1�з���0~59
#��2��Сʱ0~23(0��ʾ��ҹ)
#��3����1~31
#��4����1~12
#��5������0~6(0��ʾ������)
#��6��ִ��������û�
#��7��Ҫ���е�����
#0 0 * * * root /etc/cmd.sh
