from tkinter import *
import socket
import time
import threading


root = Tk()
root.title("客户端")
root.geometry('400x800')
username = StringVar(value='')
userkey = StringVar(value='')
message = StringVar(value='')

def login():
    name = entryname.get()
    key = entryname2.get()
    ip_port = ('127.0.0.1', 1234)
    # 生成一个句柄
    global sk
    sk = socket.socket()
    # 请求连接服务端

    sk.connect(ip_port)
    # 发送数据

    echo = sk.recv(1024)
    t.insert(END,str(echo, 'utf8')+"\n")
    t.place(x=100, y=300, width=200, height=400)

    sk.sendall(bytes(name, 'utf-8'))
    sk.sendall(bytes(key, 'utf-8'))
    answer = str(sk.recv(1024), 'utf-8')

    if answer == '1':
        t.insert(END,"登陆成功\n")
        t.place(x=100, y=300, width=200, height=400)
        threading.Thread(target=accept).start()
    else:
        t.insert(END, "用户密码错误，请重新登陆\n")
        t.place(x=100, y=300, width=200, height=400)


def send():
    content = entryname3.get()
    sk.sendall(bytes(content, 'utf-8'))
    # 接受数据


def accept():
    while True:
        server_reply1 = str(sk.recv(1024),'utf-8')
        server_reply = str(sk.recv(1024), 'utf-8')
        # 打印接受的数据
        t.insert(END,server_reply1+'\n')
        t.insert(END, server_reply + '\n')
        t.place(x=100, y=300, width=200, height=400)
        time.sleep(1)


def quit():
    sk.close()



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
buttonOk1 = Button(root,text = '发送',command = send)
buttonOk1.place(x=150,y=90,width = 50,height = 20)
buttonOk2 = Button(root,text = '退出',command = quit)
buttonOk2.place(x=200,y=90,width = 50,height = 20)

t.insert('2.0', "欢迎进入聊天室\n")
t.place(x=100, y=300, width=200, height=400)


root.mainloop()
