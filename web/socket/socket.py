import socket, select
import time

s = socket.socket()
port = 1234
s.bind(('127.0.0.1', port)) #将套接字绑定到地址，在AF_INET下，以tuple(host, port)的方式传入，如s.bind((host, port))

s.listen(5)

inputs = [s]
user = {'huangxiao1':'123456',
        'huangxiao2': '123456',
        'huangxiao3':'123456'}
dict = {}#存放已连接用户信息的字典，并进行编号

count = 0
while True:
    rs, ws, es = select.select(inputs, [],
                               [])  # 1、select函数阻塞进程，直到inputs中的套接字被触发（在此例中，套接字接收到客户端发来的握手信号，从而变得可读，满足select函数的“可读”条件），rs返回被触发的套接字（服务器套接字）；
    # 4、select再次阻塞进程，同时监听服务器套接字和获得的客户端套接字；


    for r in rs:
        if r is s:  # 2、如果是服务器套接字被触发（监听到有客户端连接服务器）
            c, addr = s.accept() #接受TCP链接并返回（conn, address），其中conn是新的套接字对象，可以用来接收和发送数据，address是链接客户端的地址。
            c.sendall(bytes('连接成功', 'utf8')) #完整发送TCP数据，将字符串中的数据发送到链接的套接字，但在返回之前尝试发送所有数据。成功返回None，失败则抛出异常
            username = str(c.recv(1024),'utf8')
            userkey = str(c.recv(1024),'utf8')
            if user.get(username)==userkey:
                c.sendall(bytes('1', 'utf8'))
                print('用户'+username+"已登录")
                inputs.append(c)  # 3、inputs加入客户端套接字
                dict[c]=username
            else:
                c.sendall(bytes('0', 'utf8'))

        else:  # 5、当客户端发送数据时，客户端套接字被触发，rs返回客户端套接字，然后进行下一步处理。
            try:
                data = r.recv(1024)
                disconnected = not data
            except socket.error:
                disconnected = True
            if disconnected:
                print('用户'+str(dict[r])+ '已退出')
                inputs.remove(r)
            else:
                print('用户'+str(dict[r])+':'+time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) )
                print(str(data, 'utf8'))
                for r1 in inputs :
                    if r1 is not s:
                        r1.sendall(bytes('用户'+str(dict[r1])+':'+time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),'utf-8'))
                        r1.sendall(data)
