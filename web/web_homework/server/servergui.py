import os
import ctypes
import threading
from tkinter import *
import subprocess

chat_ll = ctypes.cdll.LoadLibrary
chat_lib = chat_ll("./server_clib.so")
# lib.run()
file_ll = ctypes.cdll.LoadLibrary
file_lib = file_ll("./file_lib.so")

filename ="default_filename"


root = Tk()
root.title("聊天室客户端")
root.geometry("400x800")

def start():
    thread1 = threading.Thread(target = chat_lib.run)
    thread1.start()
    # chat_lib.run()

def close():
    os._exit(0) #直接杀死这个进程

def file_server():
    # filename = "test.c"
    # file_lib.run(filename.encode())

    filename = entryname.get()
    cmd ="/Users/karl/Desktop/web_homework/server/file_server 7000 "+filename
    tmp = os.popen(cmd)
    data = tmp.readlines()
    for item in data:
        print(item)
        t.insert(END,item)
    # t.insert(END,tmp)

def file_start():
    thread2 = threading.Thread(target = file_server)
    thread2.start()

t = Text(root)
buttonOk = Button(root,text = '服务器启动',command = start)
buttonOk.place(x=100,y=100,width = 150,height = 20)
buttonOk1 = Button(root,text = '服务器关闭',command = close)
buttonOk1.place(x=100,y=120,width = 150,height = 20)
buttonOk2 = Button(root,text = '文件服务器',command = file_start)
buttonOk2.place(x=100,y=140,width = 150,height = 20)

entryname = Entry(root,width = 120, textvariable=filename)
entryname.place(x=250,y=140,width = 100,height = 20)

t.place(x=50, y=400, width=300, height=450)

mainloop()
