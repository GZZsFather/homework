import threading
import ctypes
import os
from tkinter import *


ll = ctypes.cdll.LoadLibrary
lib = ll("./lib_clib.so")
lib.listening.restype = ctypes.c_char_p
filename = "helloworld.c"
# name = ""

def login():
    global name
    name = entryname.get()
    # key = entryname2.get()
    ip_port = ('127.0.0.1',7070)
    file_port = ('127.0.0.1',7000)
    lib.init()
    lib.file_init()
    data = name + ":" +" joined the chatroom";

    global thread1
    thread1 = threading.Thread(target = listening)
    thread1.start()


def speak():
    data = name + ":" + entryname3.get()
    lib.speak(data.encode())

def close():
    if not name:
        data = name + " quit"
        lib.speak(data.encode())
    os._exit(0) #直接杀死这个进程

def listening():
    # lib.recv_thread()
    while True:
        replys = lib.listening()
        data = replys.decode()
        # data = ctypes.string_at(replys,-1).decode('utf-8')
        print(data)
        t.insert(END,data+'\n')
        # t.place(x=100, y=300, width=200, height=400)

def send_file():
    filename = entryname4.get()
    cmd = "/Users/karl/Desktop/web_homework/client/file_client 127.0.0.1 7000 "+filename
    tmp = os.popen(cmd)
    data = tmp.readlines()
    for item in data:
        print(item)
        t.insert(END,item)


root = Tk()
root.title("聊天室客户端")
root.geometry("400x800")
# root.resizeable(width = True, height = True)
username = StringVar(value='')
userkey = StringVar(value='')
message = StringVar(value='')
t = Text(root)
filename =''

labelName = Label(root,text='用户名:',justify = RIGHT,width = 10 )
labelName.place(x=0,y=0,width=100,height=50)


entryname = Entry(root,width = 80, textvariable=username)
entryname.place(x=100,y=10,width = 100,height = 20)
# entryname2 = Entry(root,width = 80, textvariable=filename)
# entryname2.place(x=100,y=50,width = 100,height = 20)

entryname3 = Entry(root,width = 80, textvariable=message)
entryname3.place(x=100,y=700,width = 200,height = 20)

entryname4 = Entry(root,width = 80, textvariable=filename)
entryname4.place(x=100,y=150,width = 100,height = 20)

buttonOk = Button(root,text = '登陆',command = login)
buttonOk.place(x=100,y=50,width = 50,height = 20)
buttonOk1 = Button(root,text = '发送',command = speak)
buttonOk1.place(x=300,y=700,width = 50,height = 20)
buttonOk2 = Button(root,text = '退出',command = close)
buttonOk2.place(x=150,y=50,width = 50,height = 20)
buttonOk3 = Button(root,text = '上传',command = send_file)
buttonOk3.place(x=250,y=150,width = 50,height = 20)

t.insert('2.0', "        欢迎进入聊天室\n")
t.place(x=50, y=200, width=300, height=450)

root.mainloop()
