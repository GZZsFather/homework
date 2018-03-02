
/*The client send a message to server,and receives another message from server*/
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#define MSGKEY 75
/*定义消息结构*/
struct msgform {
  long mtype;
  char mtext[256];
};
main() {
  struct msgform msg;
  int msgqid, pid, *pint;
  msgqid = msgget(MSGKEY, 0777);
  pid = getpid();
  pint = (int *)msg.mtext;
  *pint = pid;
  msg.mtype = 1;
  msgsnd(msgqid, &msg, sizeof(int), 0);
  msgrcv(msgqid, &msg, 256, pid, 0);
  /*建立消息队列*/ /*定义消息类型*/ /*发送消息*/ /*接受从服务者发来的消息*/
  printf("Clint : receive from pid %d\n", *pint);
}
