#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define length 32767 //被调试的指令地址流长度

int addr_length = 256; //地址流长度
int size = 128;        //页面尺寸,大小为k
int max_page = 256;    //分页情况
int switch_times = 0;  //全局变量，置换次数
int M = 512;           //实存容量

int page_array[256] = {-1}; //页地址流
int table[32] = {-1};       //页表,大小为M
int a[256];                 //地址流
int called;                 //当前请求的页面号
int used = 0;               //当前被占用的实存页面数
bool pagedefault = false;   //页面失效标志

//链表结点
typedef struct Node {
  int location;
  struct Node *next;
} Node;

// Node *header;

//下面的三个变量是用来做测试验证算法正确性的
int order = 0;
int pre = 0;
int after = 0;

//链表初始化 ，如果没有链表则生成链表头结点，如果有就全部删除重新构造
void init_list(struct Node *header) {
  // Node *header;
  header->location = -1;
  header->next = NULL;
}

void reset_list(struct Node *header) {
  if (header->next != NULL) {
    struct Node *tmp = header;
    struct Node *tmp1 = tmp;
    while (tmp->next != NULL) {
      tmp1 = tmp->next;
      if (tmp1 != NULL) {
        free(tmp);
        tmp = tmp1;
      }
    }
  }
  header->location = -1;
  header->next = NULL;
}

//随机生成满足题目要求的地址链,这里取长度320
void adr_init() {
  int m, n;
  int count = 0;
  //控制长度
  while (count < addr_length) {
    m = rand() % length;
    a[count++] = m;
    a[count++] = (m + 1); //一次顺序执行
    order++;
    if (count == length - 1)
      break;
    n = rand() % m;
    a[count++] = n; //一次前地址
    pre++;
    a[count++] = (n + 1); //一次顺序执行
    order++;
    if (count == length - 1)
      break;
    m = rand() % (m - n) + n; //一次后地址
    after++;
  }
}

void page_init() {
  for (int i = 0; i < max_page; i++) {
    page_array[i] = a[i] / (size * 1024);
  }
  //顺手把table表清空了表示清空内存
  used = 0;
  pagedefault = false;
  for (int i = 0; i < M; i++) {
    table[i] = -1;
  }
}

void table_init() {
  for (int i = 0; i < M; i++) {
    table[i] = -1;
  }
}

void adr_show() {
  printf("THE VIRTUAL ADDRESS STREAM AS FOLLOWS:\n");
  printf(
      "|------------------------------address-------------------------------|"
      "\n|");
  int i = 0;
  for (i = 0; i < addr_length; i++) {
    printf("a[%3d] = %5d   ", i, a[i]);
    if ((i + 1) % 4 == 0)
      printf("|\n|");
  }
  printf("-----------------------------------------------------------------"
         "---|"
         "\n");
  printf("\n->order = %d pre = %d after = %d \n", order, pre, after);
}

void page_show() {
  printf("PAGE NUMBER WITH SIZE 1k FOR EACH ADDRESS IS:\n");
  printf("|------------------------------page------------------------------|"
         "\n|");
  for (int i = 0; i < max_page; i++) {
    printf("page[%3d] = %3d   ", i, page_array[i]);
    if ((i + 1) % 4 == 0)
      printf("|\n|");
  }

  printf("----------------------------------------------------------------|"
         "\n\n");
}

void table_show() {
  printf(
      "|--------------------------------table-------------------------------|"
      "\n");
  for (int i = 0; i < 32; i++) {
    printf("table[%3d] = %2d   ", i, table[i]);
    if ((i + 1) % 4 == 0)
      printf("|\n");
  }
  printf(
      "|--------------------------------------------------------------------|"
      "\n\n");
}

void list_show(struct Node *header) {
  if (header->next == NULL) {
    printf("The list is empty!\n");
    return;
  }
  struct Node *tmp = header;
  // printf("%d\n", tmp->next->location);
  printf("header");
  while (tmp->next != NULL) {
    tmp = tmp->next;
    printf("->%d", tmp->location);
  }
  printf("\n");
}

//找到链表尾的函数，用以插入
struct Node *find_tail(struct Node *header) {
  struct Node *tmp = header;
  while (tmp->next != NULL) {
    tmp = tmp->next;
    // printf("->%d\n", tmp->location);
  }
  return tmp;
}

// opt算法中找到最后使用的一个页面的下标
int reverse_search(int obj[], int loc, int M) {
  int max = 0;
  int temp = M - 1;
  for (int i = M - 1; i >= loc; i--) {
    int j = max_page - 1;
    while (j > 0) {
      if (obj[i] == page_array[j]) {
        break;
      }
      j--;
    }

    if (j == loc) {
      return i; //后面一次都没用到，直接返回
    }
    if (j > max) {
      temp = i;
      max = j;
    }
  }
  return temp;
}

//页面置换算法
// loc为插入的页表位置，pg1致敬pgone
void page_switch_in(int loc, int pg1) {
  table[loc] = pg1;
  switch_times++;
}

bool in_table(int table[], int target, int M) {
  bool result = false;
  // printf("M = %d", M);
  for (int i = 0; i < M; i++) {
    if (table[i] == target && table[i] != -1) {
      result = true;
      break;
    }
  }
  return result;
}

//判断给定的target在不在list中，在不需要置换的插入时用
bool in_list(struct Node *header, int target) {
  bool result = false;
  struct Node *tmp = header;
  while (tmp != NULL) {
    // printf("-%d tmp->location == target: %d\n", tmp->location,
    //        tmp->location == target);
    if (tmp->location == target) {
      break;
    }
    tmp = tmp->next;
  }
  if (tmp != NULL) {
    result = true;
  }
  return result;
}

//在内存页面满的情况下取出有需要置换的页面
//如果target存在在链表中，就取出这个元素，如果target不存在，就取出链表首部元素
struct Node *fetch(struct Node *header, int target) {
  // printf("In function fetch: \n");
  struct Node *tmp = header;
  struct Node *front;
  struct Node *back;
  struct Node *tail = find_tail(header);
  //找到tmp之前的那个结点

  while (tmp->next != NULL) {
    if (tmp->next->location == target) {
      break;
    }
    tmp = tmp->next;
  }

  if (tmp == tail) {
    return NULL;
  }
  front = tmp;
  if (tmp->next != tail) {
    back = front->next->next;
  } else {
    back = NULL;
  }
  tmp = front->next;
  front->next = back;
  tmp->next = NULL;
  return tmp;
}

//链表插入算法，仅考虑了需要置换的插入
//因为需要置换的情况需要计数
void insert(struct Node *header, struct Node *tmp) {
  struct Node *tmp1 = NULL;
  tmp1 = fetch(header, tmp->location);
  struct Node *tail = find_tail(header);
  struct Node *start = header->next;

  if (tmp1 == NULL) {
    header->next = start->next;
    tail->next = tmp;
    free(start);
    switch_times++;
  } else {
    tail->next = tmp1;
  }
}

void OPT(int M) {
  for (int i = 0; i < max_page; i++) {
    called = page_array[i];
    // printf("->Now the page to swith is page_array[%d] = %d\n", i, called);
    pagedefault = in_table(table, called, M);
    // pagedefault = true;
    if (pagedefault) {
      // printf(">>page_array[%d] = %d in the table, so nothing happens.\n\n",
      // i,
      //        called);
      // table_show();
      continue; //如果在table中有则不用置换继续执行
    }
    if (used >= M) {
      int loc = reverse_search(table, i, M);
      page_switch_in(loc, page_array[i]);
    } else {
      int tmp = 0;
      while (table[tmp] != -1) {
        tmp++;
      }
      int loc = tmp;
      //这里不用switch函数是因为这里是直接写入不存在缺页，而switch函数中设计了缺页次数的统计
      table[loc] = page_array[i];
      used++;
      switch_times++;
    }
    // table_show();
  }
}

void LRU(struct Node *header, int MM) {
  init_list(header);

  for (int i = 0; i < max_page; i++) {
    // printf("i = %d\n", i);
    called = page_array[i];

    struct Node *tmp = (struct Node *)malloc(sizeof(struct Node));
    tmp->location = called;
    tmp->next = NULL;
    struct Node *tail = find_tail(header);

    //内存有空闲，所以直接插入链表尾部
    if (used < MM) {
      if (in_list(header, called)) {
        struct Node *tmp1 = fetch(header, called);
        tail = find_tail(header);
        tail->next = tmp1;
        continue;
      }

      tail->next = tmp;
      used++;
      switch_times++;
    } else {
      insert(header, tmp);
    }
    // list_show(header);
  }
}

int main(int argc, char const *argv[]) {

  /* code */
  size = 1;
  max_page = addr_length / size;
  struct Node *header = (struct Node *)malloc(sizeof(struct Node));
  adr_init();
  adr_show();
  page_init();

  page_show();
  char a;
  printf("Please choose the algorithm:\n\t[1]OPT\n\t[2]LRU\n");
  printf("And your choice is : ");

  scanf("%s", &a);
  switch (a) {
  case '1':
    printf("And your choice is OPT:\n");
    int i = 4;
    int MM = 0;
    for (int j = 1; j <= 8; j = j * 2) {
      size = j;
      max_page = addr_length / size;
      adr_init();
      // adr_show();
      page_init();
      // page_show();
      // table_show();

      printf("Size = %d\n", size);
      printf("page assigned\t\t\tpages_in/total references\n");
      for (i = 4; i <= 32; i = i + 2) {
        used = 0;
        switch_times = 0;
        MM = i;
        printf("  %d\t\t\t\t\t", MM);
        table_init();
        OPT(MM);
        // table_show();
        printf("rate = %f\n", 1.0 - ((float)switch_times / (float)addr_length));
      }
    }

    break;

  case '2':
    printf("And your choice is LRU:\n");
    // i = 4;
    printf("page assigned\t\t\tpages_in/total references\n");

    i = 4;
    MM = 0;
    for (int j = 1; j <= 8; j = j * 2) {
      size = j;
      max_page = addr_length / size;

      printf("Size = %d\n", size);
      for (i = 4; i <= 32; i = i + 2) {
        adr_init();
        // adr_show();
        page_init();
        // page_show();
        used = 0;
        init_list(header);
        switch_times = 0;
        MM = i;
        printf("  %d\t\t\t\t\t", MM);
        LRU(header, MM);
        // list_show(header);
        printf("rate = %f\n", 1.0 - ((float)switch_times / (float)addr_length));
        // printf("switch_times = %d\n", switch_times);
      }
      // test(header);
    }
  }

  return 0;
}
