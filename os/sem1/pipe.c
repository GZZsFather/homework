/*程序建立一个管道fd * /
/*父进程创建两个子进程P1、P2 */ /*子进程P1、P2 分别向管道写入信息*/
/*父进程等待子进程结束，并读出管道中的信息*/

#include <stdio.h>
int main() {
  int i, r, j, k, l, p1, p2, fd[2];
  char buf[50], s[50];
  pipe(fd);
  while ((p1 = fork()) == -1)
    ;
  if (p1 == 0) {
    lockf(fd[1], 1, 0); /*子进程1 执行*/ /*管道写入端加锁*/
    sprintf(buf, "Child process P1 is sending messages! \n");
    printf("Child process P1! \n");
    write(fd[1], buf, 50);
    lockf(fd[1], 0, 0); /*信息写入管道*/ /*管道写入端解锁*/
    sleep(5);
    j = getpid();
    k = getppid();
    printf("P1 %d is weakup. My parent process ID is %d.\n", j, k);
    exit(0);
  } else {
    while ((p2 = fork()) == -1)
      ;
    if (p2 == 0) {
      lockf(fd[1], 1, 0); /*创建子进程2*/ /*子进程2 执行*/ /*管道写入端加锁*/
      sprintf(buf, "Child process P2 is sending messages! \n");
      printf("Child process P2! \n");
      write(fd[1], buf, 50);
      lockf(fd[1], 0, 0); /*信息写入管道*/ /*管道写入端解锁*/
      sleep(5);
      j = getpid();
      k = getppid();
      printf("P2 %d is weakup. My parent process ID is %d.\n", j, k);
      exit(0);
    } else {
      l = getpid();
      wait(0);
      if (r = read(fd[0], s, 50) == -1)
        printf("Can't read pipe. \n");
      else
        printf("Parent %d: %s \n", l, s);
      wait(0);
      if (r = read(fd[0], s, 50) == -1)
        printf("Can't read pipe. \n");
      else /* 等待被唤醒*/ /* 等待被唤醒*/
        printf("Parent %d: %s \n", l, s);
      exit(0);
    }
  }
}
