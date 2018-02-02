/*服务者程序*/
/*The server receives the message from client,and answer a message*/
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#define MSGKEY 75
struct msgform {
  long mtype;
  char mtext[256];
} msg;
int msgqid;
int main() {
  int i, pid, *pint;
  extern cleanup();
  for (i = 0; i < 20; i++)
    signal(i, cleanup);
  msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);
  for (;;) /*定义消息结构*/ /*设置软中断信号的处理程序*/ /*建立消息队列*/
  /*等待接受消息*/ {
    msgrcv(msgqid, &msg, 256, 1, 0); /* 接受消息*/
    pint = (int *)msg.mtext;
    pid = *pint;
    printf("Server :receive from pid %d\n", pid);
    msg.mtype = pid;
    *pint = getpid();
    msgsnd(msgqid, &msg, sizeof(int), 0);
    /*显示消息来源*/ /*加入自己的进程标识*/ /*发送消息*/
  }
}
cleanup() {
  msgctl(msgqid, IPC_RMID, 0);
  exit(0);
}
