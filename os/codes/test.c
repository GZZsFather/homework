#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STRING = "HELLO WORLD!"

int main() {
  pid_t pid;
  //创建一个进程

  //创建失败
  int pipefd[2];
  //创建描述符
  char buf[BUFSIZ];
  char s[BUFSIZ];

  if (pipe(pipefd) == -1) {
    perror("pipe() error");
    exit(1);
  }
  pid = fork();
  if (pid < 0) {
    perror("fork error:");
    exit(1);
  }
  //子进程
  if (pid == 0) {
    printf("I am the child process.\n");

    //输出进程ID和父进程ID
    printf("pid: %d\tppid:%d\n", getpid(), getppid());
    // int i = read(pipefd[1], s, BUFSIZ);
    // printf("i: %d\n", i);
    if (read(pipefd[0], s, BUFSIZ) < 0) {
      perror("write()");
      exit(1);
    }
    // printf("!!\n");
    printf("%s!\n", s);

    // bzero(buf, BUFSIZ);
    // snprintf(buf, BUFSIZ, "Message from child: My pid is: %d", getpid());
    printf("I will sleep five seconds.\n");
    //睡眠5s，保证父进程先退出
    sleep(10);
    printf("pid: %d\tppid:%d\n", getpid(), getppid());
    printf("child process is exited.\n");
    // sleep(10);
  }
  //父进程
  else {
    printf("I am father process.\n");
    printf("Parent pid is: %d\n", getpid());

    lockf(pipefd[1], 1, 0);
    snprintf(buf, BUFSIZ, "Message from parent: My pid is: %d", getpid());
    // printf("%s\n", buf);
    lockf(pipefd[1], 0, 0);
    if (write(pipefd[1], buf, strlen(buf)) < 0) {
      perror("write() error ");
      exit(1);
    }

    //父进程睡眠1s，保证子进程输出进程id
    sleep(1);

    // bzero(buf, BUFSIZ);
    // if (read(pipefd[0], buf, BUFSIZ) < 0) {
    //   perror("write()");
    //   exit(1);
    // }

    // printf("%s\n", buf);
    printf("father process is  exited.\n");
    exit(0);

    // wait(NULL);
  }

  // return 0;
}
