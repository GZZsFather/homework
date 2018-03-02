#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  pid_t fpid = 0;
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if (fork() == 0)
      printf("%d pid=%d ppid=%d\n", i, getpid(), getppid());
    else {
      int j = wait(0);
      printf("%d:The child %d is finished.\n", getpid(), j);
      return 0;
    }
  }
  return 0;
}
