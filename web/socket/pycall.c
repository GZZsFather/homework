/***gcc -o libpycall.so -shared -fPIC pycall.c*/
#include <stdio.h>
#include <stdlib.h>
int foo(int a, int b)
{
  printf("you input %d and %d\n", a, b);
  return a+b;
}

void print(char* msg){
  printf("%s\n",msg);
}

int run(){
  printf("%d",foo(1,3));
}
