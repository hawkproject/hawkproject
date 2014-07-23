#!/etc/crontab
# 文件位置: /home/mysql/MysqlBK.sh

# 配置信息
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

# 备份命令
$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db_name > $bak_dir/$db_name"_"$fix_name".sql"

#$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_usr -p$db_pwd $db_name | gzip > $bak_dir/$db_name"_"$fix_name".gz"

#加入到Linux自动运行,将此脚本加到 /etc/crontab 定时任务中
#第1列分钟0~59
#第2列小时0~23(0表示子夜)
#第3列日1~31
#第4列月1~12
#第5列星期0~6(0表示星期天)
#第6列执行命令的用户
#第7列要运行的命令
#0 0 * * * /home/mysql/MysqlBK.sh
