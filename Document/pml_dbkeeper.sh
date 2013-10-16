#! /bin/sh

# db config
db_name=("pml_svr2" "pml_svr3" "pml_svr4")
db_user="root"
db_pwd="123456"
host_ip="localhost"
host_port="3306"
bin_dir="/usr/bin"
bak_dir="/data/home/app100661088/pml/dbdump"
fix_name=`date +%F-%T`
dump_hour="-1"

# create backup dir
if [ ! -d "$bak_dir" ]; then 
mkdir "$bak_dir" 
fi

# call mysqldump to backup pmldb
function pml_db_dump()
{
	fix_name=`date +%F-%T`
	
	if [ ! -d "$bak_dir/$fix_name" ]; then 
		mkdir "$bak_dir/$fix_name"
	fi
	
	for db in ${db_name[@]};do
		echo "pml_db_dump: "$db" time: "$fix_name | tee -a pml_dbkeeper.log		
		$bin_dir/mysqldump --opt -h$host_ip -P$host_port -u$db_user -p$db_pwd $db > $bak_dir/$fix_name/$db".sql"
	done
}

while [ true ];do
	hour=`date +%H`
	min=`date +%M`
	sec=`date +%S`	
	flag=$[ 10#$hour % 2 ]
	
	echo $hour":"$min":"$sec":"$flag":"$dump_hour
	
	if [ $flag -eq 0 -a $hour -ne $dump_hour ];then
		pml_db_dump
		dump_hour=$hour
	fi
	
	sleep 60
done
