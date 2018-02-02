gcc -o lib_clib.so -shared -fPIC client.c
gcc -o file_client file_client.c

python3 cpyclient.py
