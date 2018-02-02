import socket,select,threading

host = socket.gethostname()
port = 7070
server_addr = (host,port)

# waitable的read list, 表示异步通信中可读socket对象的列表
inputs = []
# 连接进入server的client的名称
fd_name = {}

# 创建并初始化server socket
def serverInit():
    ss = socket.socket()
    ss.bind(server_addr)
    ss.listen(10)
    return ss

# 创建一个新的socket连接
def newConnection(ss):
    client_conn,client_addr = ss.accept()
    try:
        client_conn.send(("welcome to chatroom,pls set up your nick name!").encode())
        client_name = client_conn.recv(1024).decode()
        inputs.append(client_conn)
        fd_name[client_conn] = client_name
        client_conn.send(("current members in chatroom are: %s" % fd_name.values()).encode())
        for other in fd_name.keys():
            if other != client_conn and other != ss:
                other.send((fd_name[client_conn]+" joined the chatroom!").encode())
    except Exception as e:
        print(e)

def closeConnection():
    pass

def run():
    ss = serverInit()
    inputs.append(ss)
    print ("server is running...")
    while True:
        rlist,wlist,elist = select.select(inputs, [], [])
        # 当没有可读fd时, 表示server错误,退出服务器
        if not rlist:
            print("timeout...")
            ss.close()
            break
        for r in rlist:
            if r is ss:  # server socket, 表示有新的client连接请求
                newConnection(ss)
            else:          # 表示一个client连接上有数据到达服务器
                try:
                    data = r.recv(1024).decode()
                    disconnect = not data
                    data = fd_name[r] + " : "+ data
                except socket.error:
                    disconnect = True
                else:
                    pass
                if disconnect:
                    inputs.remove(r)
                    # data = fd_name[r] + " : "+ data
                    data = fd_name[r] + " leaved the room"
                    print (data)
                    for other in inputs:
                        if other != ss and other != r:
                            try:
                                other.send(data.encode())
                            except Exception as e:
                                print(e)
                            else:
                                pass
                    # 除名
                    del fd_name[r]
                else:
                    print(data)  
                    for other in inputs:
                        # if other != ss and other != r:
                        if other != ss:
                            try:
                                other.send(data.encode())
                            except Exception as e:
                                print(e)
if __name__ == "__main__":
    run()
