#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define BUF_LEN 1024 
#define SERVER_PORT 7070
#define SERVER_NAME "4FUN"

#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\n"

//定义html页面

//404 ERROR页面信息
const static char http_error[] = "HTTP/1.1 404 Not Found\r\nContent-type:text/html\r\n\r\n";
//HTTP消息头
const static char http_html[] =
"HTTP/1.0 200 OK\n\
Content-Type: text/html\n\
Transfer-Encoding: chunked\n\
Connection: Keep-Alive\n\
Accept-Ranges:bytes\n\n";
//HTTP消息尾
#define TAIL "\n\n";
//HTML文件
const static char http_index_html[] ="<html><head><title>Congrats!</title></head><body><h1>testtesttest!</h1><p>just a test page.</body></html>";

int http_send_file(char *filename, int sockfd){
  //printf("%d",strcmp(filename,"/"));
  if(!strcmp(filename,"/")){
    //通过write函数发送http响应报文
    //报文包括HTTP响应头和http响应内容--HTML文件
    write(sockfd, http_html, strlen(http_html));
    write(sockfd, http_index_html, strlen(http_index_html));
  }
  else{
    //若文件未找到则返回404 ERROR信息
    printf("The file you request is : %s\n",filename);
    // printf("%s: file not found!\n",filename);
    write(sockfd,http_error, strlen(http_error));
  }
  return 0;
}


//HTTP请求解析
void serve(int sockfd){
  char buf[BUF_LEN];

  char temp[BUF_LEN];//用来存放接收到的HTTP请求
  memset(buf,'\0',sizeof(buf));
  memset(temp,'\0',sizeof(buf));
  read(sockfd, buf, BUF_LEN);
  strcpy(temp,buf);
  printf("%s\n",temp);//这里只是为了打出来看看
  printf("~~~~~~~~~~~~~~~~\n");//可爱的分隔符
  if(!strncmp(buf, "GET",3)){
    char *file = buf + 4 ;
    char *space =strchr(file, ' ');
    *space = '\0';
    http_send_file(file,sockfd);
    printf("\n\n");
    printf("~~~~~~~~~~~~~~~~\n");//可爱的分隔符
  }
  else{
    //不支持的HTTP请求处理

    printf("~~~~~~~~~~~~~~~~\n");
    printf("unsupported request!\n");
    return;
  }
}

int main(void){
  int sockfd,err,newfd;
  struct sockaddr_in addr;
  //建立TCP套接字
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    perror("socket creation failed!\n");
    return 0;
  }
  memset(&addr, 0, sizeof(addr));
  //sin_family表示地址家族，一般都是“AF_XXX”的形式
  addr.sin_family = AF_INET;  //AF_INET表示TCP/IP协议族
  //端口号一定要使用htons先转化为网络字节序， 否则绑定的实际端口可能和你需要的不同
  addr.sin_port = htons(SERVER_PORT);
  addr.sin_addr.s_addr =    ;

  //这段代码的功能是设置端口复用功能
  //如果不设置端口复用当停止server服务后端口仍然会处于TIME_WAIT状态
  int on=1;
  if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0){
      perror("setsockopt failed");
      return 0;
  }

  if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))){
    perror("socket binding failed!\n");
    return 0;
  }
  listen(sockfd, 128);
  int i = 0;
  for(;;){
    // 不间断接受HTTP请求并处理，这里是用单线程
    printf("----------------------%d----------------------\n",i++);
    newfd = accept(sockfd, NULL, NULL);
    serve(newfd);
    close(newfd);
    }
  return 0;
}
