players.sql �ǲ�����sql��������һ��������������Ӧ�Ĵ�������

toredis.sql ���ڰ����ݿ�����ͬ����redis�
���
mysql test --skip-column-names --raw < toredis.sql | redis-cli
