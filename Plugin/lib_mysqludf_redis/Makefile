all:
	gcc -O3 -I. -I/usr/local/include/hiredis -I/usr/include/mysql -fPIC -shared lib_mysqludf_redis.c utils.c -L/usr/lib64/mysql -lrt -ldl -lmysqlclient -lhiredis -o lib_mysqludf_redis.so
	
clean:
	rm -f *.o
	rm -f *.so
