#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAXDATASIZE 1024
#define PORT 7070
#define IP "127.0.0.1"
#define SIZE 100
#define FILE_PORT 7000
#define MAXLINE 1024
#define FINISH_FLAG "FILE_TRANSPORT_FINISH"

typedef struct sockaddr SA;

char buf[MAXLINE];
int sockfd; //TCP
int file_sockfd;  //UDP
int fds[100];//用来存放sockfd的数组
struct sockaddr_in clie_addr;
int clie_addr_len;
FILE *fp;

int fd_init(){
  sockfd = socket(AF_INET,SOCK_STREAM,0); //TCP
  if(sockfd < 0){
    perror("socket creation failed.");
    exit(-1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT); //使用hotns讲十进制点形式的ip地址转化为二进制
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd,(SA*)&addr,sizeof(addr)) < 0){
        perror("binding failed");
        exit(-1);
  }

  if (listen(sockfd,100) < 0){
        perror("listening failed");
        exit(-1);
  }
}

void SendMsgToAll(char* msg){
  int i;
  printf("%s\n",msg);
    for (i = 0;i < SIZE;i++){
        if (fds[i] != 0){
            // printf("sent to %d\n",fds[i]);
            send(fds[i],msg,strlen(msg),0);
        }
    }
}

//用多线程实现非阻塞
void* service_thread(void* p){
    int fd = *(int*)p;
    printf("pthread = %d\n",fd);
    while(1){
        char buf[100] = {};
        if (recv(fd,buf,sizeof(buf),0) <= 0){
            int i;
            for (i = 0;i < SIZE;i++){
                if (fd == fds[i]){
                    fds[i] = 0;
                    break;
                }
            }
                printf("fd = %d offline\n",fd);
                pthread_exit((void*)i);
        }
        //把服务器接受到的信息发给所有的客户端
        SendMsgToAll(buf);
    }
}

void service(){
    printf("Server started\n");
    while(1){
        struct sockaddr_in fromaddr;
        socklen_t len = sizeof(fromaddr);
        int fd = accept(sockfd,(SA*)&fromaddr,&len);
        if (fd == -1){
            printf("connection error\n");
            continue;
        }
        int i = 0;
        for (i = 0;i < SIZE;i++){
            if (fds[i] == 0){
                //记录客户端的socket
                fds[i] = fd;
                printf("fd = %d\n",fd);

                //有客户端连接之后，启动线程给此客户服务
                pthread_t tid;
                pthread_create(&tid,0,service_thread,&fd);
                printf("client socket thread is %d\n",tid);
                break;
            }
          if (SIZE == i){
              char* str = "chatromm is full!";
              send(fd,str,strlen(str),0);
              close(fd);
          }
        }
    }
}

void run(){
  fd_init();
  service();
}

int main(int argc, char const *argv[]) {
  //file_init();
  fd_init();
  service();
  return 0;
}
