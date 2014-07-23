#!/etc/crontab
# �ļ�λ��: /home/mysql/MysqlBK.sh

# ������Ϣ
db_name="hawk"
db_user="root"
db_pwd="123456"
host_ip="localhost"
host_port="3306"
bin_dir="/usr/bin"
bak_dir="/home/HawkProject/HawkProject/tencent"
fix_name=`date +%F-%T`

if [ ! -d "$bak_dir"]; then 
mkdir "$bak_dir" 
fi

# ��������
$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db_name > $bak_dir/$db_name"_"$fix_name".sql"

#$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_usr -p$db_pwd $db_name | gzip > $bak_dir/$db_name"_"$fix_name".gz"

#���뵽Linux�Զ�����,���˽ű��ӵ� /etc/crontab ��ʱ������
#��1�з���0~59
#��2��Сʱ0~23(0��ʾ��ҹ)
#��3����1~31
#��4����1~12
#��5������0~6(0��ʾ������)
#��6��ִ��������û�
#��7��Ҫ���е�����
#0 0 * * * /home/mysql/MysqlBK.sh
