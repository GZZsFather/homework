#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define M 10

int in = 0;  // where producers put products
int out = 0; // where customers fetch products

int buf[M] = {0}; // initialized as 0

sem_t empty_sem;
sem_t full_sem;

pthread_mutex_t mutex;

int product_id;
int customer_id;

void Signal_Handle(int signal) {
  printf("%d exited!\n", signal);
  exit(0);
}

void print() {
  // init();
  printf("Product list is: ");
  for (int i = 0; i < M; i++) {
    printf("%d ", buf[i]);
  }
  printf("\n");
}

void *producer() {
  int id = ++product_id;
  while (1) {
    sleep(2);
    sem_wait(&empty_sem);
    pthread_mutex_lock(&mutex);

    in = in % M;
    printf("Producer %d has placed product %d in the list!\n", id, in);
    buf[in] = 1;
    print();
    in++;
    pthread_mutex_unlock(&mutex);
    sem_post(&full_sem);
  }
}

void *customer() {
  int id = customer_id++;
  while (1) {
    sleep(5);

    sem_wait(&full_sem);
    pthread_mutex_lock(&mutex);

    out = out % M;
    printf("Customer %d has fetched product %d from the list!\n", id, out);

    buf[out] = 0;
    print();
    out++;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty_sem);
  }
}

int main() {

  pthread_t id1[N];
  pthread_t id2[N];
  int i = 0;
  int ret[N];
  //按ctrl+C产生SIGINT信号
  if (signal(SIGINT, Signal_Handle) == SIG_ERR) {
    printf("信号安装出错\n");
  } // 初始化
  int ini1 = sem_init(&empty_sem, 0, M);
  int ini2 = sem_init(&full_sem, 0, M);
  int ini3 = pthread_mutex_init(&mutex, NULL);

  for (i = 0; i < N; i++) {
    ret[i] = pthread_create(&id1[i], NULL, producer, (void *)(&i));
    if (ret[i] != 0) {
      printf("Error!\n");
      exit(1);
    }
  }

  for (i = 0; i < N; i++) {
    ret[i] = pthread_create(&id2[i], NULL, customer, NULL);
    if (ret[i] != 0) {
      printf("Error!\n");
      exit(1);
    }
  }

  for (i = 0; i < N; i++) {
    pthread_join(id1[i], NULL);
    pthread_join(id2[i], NULL);
  }

  exit(0);
}
