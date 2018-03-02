#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int i;
  printf("My pid is %d, my father's pid is %d\n", getpid(), getppid());
  for (int i = 0; i < 3; i++) {
    if (fork() == 0)
      printf("%d pid=%d ppid=%d\n", i, getpid(), getppid());
    else {
      int j = wait(0);
      printf("%d:The child %d is finished.\n", getpid(), j);
    }
  }
  return 0;
}
