#coding = utf-8

import socket
import sys
import select
import threading
from tkinter import *

host = socket.gethostname()
client_addr = (host,7070)
# global cs


def login():
    name = entryname.get()
    key = entryname2.get()
    ip_port = ('127.0.0.1',7070)
    global cs
    cs = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    cs.connect(client_addr)

    thread1 = threading.Thread(target = listening)
    thread1.start()

    # thread2 = threading.Thread(target = speak)
    # thread2.start()

def listening():
    inputs = [cs]
    while True:
        rlist,wlist,elist = select.select(inputs,[],[])
        if cs in rlist:
            try:
                replys = cs.recv(1024).decode()
                t.insert(END,replys+'\n')
                t.place(x=100, y=300, width=200, height=400)
            except Exception as e:
                print(e)
                exit()

def speak():
    data = entryname3.get()
    # print(data)
    try:
        cs.send(data.encode())
    except Exception as e:
        print(e)
        # exit()

def close():
    # cs.shutdown(2)
    cs.close()



root = Tk()
root.title("聊天室客户端")
root.geometry("400x800")
# root.resizeable(width = True, height = True)
username = StringVar(value='')
userkey = StringVar(value='')
message = StringVar(value='')
t = Text(root)

labelName = Label(root,text='用户名:',justify = RIGHT,width = 10 )
labelName.place(x=0,y=0,width=100,height=50)
labelName2 = Label(root,text='密码:',justify = RIGHT,width = 10 )
labelName2.place(x=0,y=30,width=100,height= 70)

entryname = Entry(root,width = 80, textvariable=username)
entryname.place(x=100,y=10,width = 100,height = 20)
entryname2 = Entry(root,width = 80, textvariable=userkey)
entryname2.place(x=100,y=50,width = 100,height = 20)
entryname3 = Entry(root,width = 80, textvariable=message)
entryname3.place(x=100,y=120,width = 100,height = 20)

buttonOk = Button(root,text = '确定',command = login)
buttonOk.place(x=100,y=90,width = 50,height = 20)
buttonOk1 = Button(root,text = '发送',command = speak)
buttonOk1.place(x=150,y=700,width = 50,height = 20)
buttonOk2 = Button(root,text = '退出',command = close)
buttonOk2.place(x=150,y=90,width = 50,height = 20)

t.insert('2.0', "欢迎进入聊天室\n")
t.place(x=100, y=300, width=200, height=400)

root.mainloop()
    # cs = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    # cs.connect(client_addr)
    #
    # t = threading.Thread(target = listening, args = (cs,))
    # t.start()
    #
    # t1 = threading.Thread(target = speak, args =(cs,))
    # t1.start()


# if __name__ == "__main__":
#     main()
