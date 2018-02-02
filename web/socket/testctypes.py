import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll("/Users/karl/Desktop/socket/libpycall.so")
a1 = ctypes.c_int(1)
a2 = ctypes.c_int(3)
a = lib.foo(1, 3)
print(a)
words = "hello"
lib.print(words.encode())
lib.run()
#print('***finish***')
