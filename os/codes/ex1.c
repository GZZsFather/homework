#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("process id is %d\n", getpid());
  printf("process group is %d\n", getpgrp());
  printf("calling process's real user id is %d\n", getuid());
  sleep(5);
  int child;
  child = fork();
  if (child == 0) {
    execlp("echo", "echo", "what the f**k\n", (char *)0);
    perror("execlp error!");
    exit(1);
  }
  int i;
  i = wait(5);
  printf("Do you want to stop the process?\n");
  char x;
  scanf("%c", &x);
  if (x == "y")
    exit(0);
  return 0;
}
