#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int globa = 4;
int main() {
  pid_t pid;
  int vari = 5;
  printf("before fork.\n");
  if ((pid = fork()) < 0) {
    printf("fork error.\n");
    exit(0);
  }
  /*创建失败处理*/
  else if (pid == 0) {
    globa++;
    vari--;
    printf("Child %d changed the vari and globa.\n", getpid());
  } else /*父进程执行*/
  {
    wait(0);
    printf("Parent %d did not changed the vari and globa.\n", getpid());
  }
  printf("pid=%d, globa=%d, vari=%d\n", getpid(), globa, vari); /*都执行*/
  exit(0);
}
