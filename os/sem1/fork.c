#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int i;
  if (fork()){
    i = wait();

    printf("It's parent process!\n");
    printf("The child process, ID number %d, is finished.\n", i);
  }

  else{
    printf("It's child process!\n");
    sleep(10);
    exit(0);
  }
  return 0;
}
