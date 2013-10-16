rm -rf lib_mysqludf_json.so
g++ -shared -fPIC -I/usr/include/mysql -o lib_mysqludf_json.so lib_mysqludf_json.c