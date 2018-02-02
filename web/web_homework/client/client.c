//gcc -o lib_clib.so -shared -fPIC client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd;//客户端socket
int file_sockfd;//文件socket
char* IP = "127.0.0.1";//服务器的IP
short PORT = 7070;//服务器服务端口
short FILE_PORT = 7000;//文件服务器端口
typedef struct sockaddr SA;
char name[30];
FILE *fp;

void init(){
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    if (connect(sockfd,(SA*)&addr,sizeof(addr)) == -1){
        perror("connection failed\n");
        exit(-1);
    }
    printf("Client started\n");
}

int file_init(){
  file_sockfd = socket(AF_INET,SOCK_DGRAM,0); //UDP
  if(file_sockfd < 0){
    perror("file_socket ceatrion failed.");
    exit(-1);
  }

  struct sockaddr_in file_addr;
  file_addr.sin_family = AF_INET;
  file_addr.sin_port = htons(FILE_PORT);
  file_addr.sin_addr.s_addr = INADDR_ANY;

  if (connect(file_sockfd,(SA*)&file_addr,sizeof(file_addr)) < 0){
        perror("file socket connection failed");
        exit(-1);
  }
  printf("file server connected!\n");

}

void speak(char * msg){
  //sprintf(msg,"%s:%s",name,buf);
  send(sockfd,msg,strlen(msg),0);
}

void send_file(char* filename){
  send(file_sockfd,filename,strlen(filename),0);
  // printf("%s\n",filename);
}

void start(){
    pthread_t id;
    void* recv_thread(void*);
    pthread_create(&id,0,recv_thread,0);
    char buf2[100] = {};
    sprintf(buf2,"%s has joined",name);
    send(sockfd,buf2,strlen(buf2),0);
    while(1){
        char buf[100] = {};
        scanf("%s",buf);
        char msg[131] = {};
        sprintf(msg,"%s:%s",name,buf);
        // send(sockfd,msg,strlen(msg),0);
        speak(msg);
        if (strcmp(buf,"bye") == 0){
            memset(buf2,0,sizeof(buf2));
            sprintf(buf2,"%s quited the room",name);
            send(sockfd,buf2,strlen(buf2),0);
            break;
        }
    }
    close(sockfd);
}

char * listening(){
  char buf[100] = {};
  if (recv(sockfd,buf,sizeof(buf),0) <= 0){
      return NULL;
  }
  else{
    return buf;
  }
}

void* recv_thread(void* p){
    while(1){
        // char buf[100] = {};
        // if (recv(sockfd,buf,sizeof(buf),0) <= 0){
        //     return NULL;
        // }
        char* buf = NULL;
        buf = listening();
        printf("%s\n",buf);

    }
}

int main(){
    init();
    printf("请输入您的名字：");
    scanf("%s",name);
    start();
    return 0;
}
