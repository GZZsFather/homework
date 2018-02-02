#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

void func() /*处理程序*/ {
  int m;
  m = getpid();
  printf("I am Process %d: It is signal 18 processing function.\n", m);
}

main() {
  int i, j, k;
  // int func();
  signal(18, func); /*设置18 号信号的处理程序*/
  // i = fork();
  if (i = fork()) {
    j = kill(i, 18); /*创建子进程*/ /*父进程执行*/ /*向子进程发送信号*/
    printf("Parent: signal 18 has been sent to child %d,returned %d.\n", i, j);
    k = wait(0); /*父进程被唤醒*/
    printf("After wait %d,Parent %d: finished.\n", k, getpid());
  } else { /*子进程执行*/
    sleep(10);
    printf("Child %d: A signal from my parent is recived.\n", getpid());
  } /*子进程结束，向父进程发子进程结束信号*/
}
