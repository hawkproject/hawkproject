players.sql 是测试用sql，里面有一个表建立和三个相应的触发器。

toredis.sql 用于把数据库数据同步到redis里。
命令：
mysql test --skip-column-names --raw < toredis.sql | redis-cli
