#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct freememory {
  struct freememory *front;
  struct freememory *back;
  int space;
  int start; //起始地址
};

int r;
struct freememory *idx;
struct freememory *tail; //增加一个尾部结点 防止溢出同时方便排序

void printStatus() {
  struct freememory *tmp = idx;
  printf("--------------------------------\n");
  printf("| Number | Size(KB) |   start   |\n");
  for (int i = 0; i < r; ++i) {
    printf("|   %2d   |   %4d   |   %5d   |\n", i, tmp->space, tmp->start);
    tmp = tmp->back;
  }
}

void init() {
  // srand((unsigned)time(NULL));
  r = 10;
  // r = rand() % 20
  //随机种子
  idx = malloc(sizeof(struct freememory));
  tail = malloc(sizeof(struct freememory));
  tail->space = -1;
  tail->start = -1;
  idx->back = NULL;
  idx->front = NULL;
  idx->space = rand() % 1024;
  idx->start = 0;
  struct freememory *tmp = idx;
  for (int i = 1; i < r; ++i) {
    struct freememory *tmp2 = malloc(sizeof(struct freememory));

    // tmp2->back = NULL;

    tmp2->back = tail;
    tail->front = tmp2;

    tmp2->front = tmp;
    tmp->back = tmp2;
    tmp2->space = rand() % 1024;
    tmp2->start = tmp->start + tmp->space + rand() % 1024;
    tmp = tmp2;
  }
}

void delete () {
  for (int i = 0; i < r; ++i) {
    struct freememory *tmp = idx;
    idx = idx->back;
    free(tmp);
  }
}

void ff(const int *need) {
  struct freememory *tmp = idx;
  for (int i = 0; i < r; ++i) {
    if (tmp->space > *need) {
      tmp->space -= *need;
      break;
    } else if (tmp->space == *need) {
      if (i == 0) {
        idx = idx->back;
      } else {
        tmp->front->back = tmp->back;
        tmp->back->front = tmp->front;
      }
      free(tmp);
      --r;
      break;
    }
    tmp = tmp->back;
  }
}

//链表中两个结点的交换
void MemSwap(struct freememory *mem1, struct freememory *mem2) {
  int temp = mem1->space;
  mem1->space = mem2->space;
  mem2->space = temp;
  temp = mem1->start;
  mem1->start = mem2->start;
  mem2->start = temp;
}

//用来区分排序算法的两个compare函数
bool IsGreater(int a, int b) { return a > b; }
bool IsLess(int a, int b) { return a < b; }

// qsort的功能函数 找到分区 用来递归
struct freememory *GetPart(struct freememory *pBegin, struct freememory *pEnd,
                           int type) {
  bool (*compare)(int, int);
  compare = IsGreater;
  compare = (type == 1) ? IsLess : IsGreater;
  int space = pBegin->space;
  struct freememory *p = pBegin;
  struct freememory *q = p->back;

  while (q != tail) {
    if ((*compare)(q->space, space)) {
      // if (q->space < space) {
      p = p->back;
      MemSwap(p, q);
    }

    q = q->back;
  }
  MemSwap(p, pBegin);
  return p;
}

// type 为排序类型 1为升序 2为降序
void QuickSort(struct freememory *pBegin, struct freememory *pEnd, int type) {
  if (pBegin != pEnd) {
    struct freememory *part = GetPart(pBegin, pEnd, type);
    QuickSort(pBegin, part, type);
    QuickSort(part->back, pEnd, type);
  }
}

void bf(const int *need) {
  struct freememory *tmp = idx;
  for (int i = 0; i < r; ++i) {
    if (tmp->space > *need) {
      tmp->space -= *need;
      if (i == 0 || tmp->space > tmp->front->space)
        break;
      else {
        tmp->front->back = tmp->back;
        tmp->back->front = tmp->front;

        struct freememory *tmp2 = idx;
        for (int j = 0; j < i; ++j) {
          if (tmp->space <= tmp2->space) {
            if (j == 0) {
              tmp->back = tmp2;
              tmp2->front = tmp;
              tmp->front = NULL;
              idx = tmp;
              return;
            } else {
              tmp2->front->back = tmp;
              tmp->front = tmp2->front;
              tmp2->front = tmp;
              tmp->back = tmp2;
              return;
            }
          }
          tmp2 = tmp2->back; //这句没加到底是怎么跑出来的!!
        }
      }
    } else if (tmp->space == *need) {
      if (i == 0) {
        idx = idx->back;
      } else {
        tmp->front->back = tmp->back;
        tmp->back->front = tmp->front;
      }
      free(tmp);
      --r;
      return;
    }
    tmp = tmp->back;
  }
}

void wf(const int *need) {
  QuickSort(idx, tail, 2);
  struct freememory *tmp = idx;
  if (tmp->space > *need) {
    tmp->space -= *need;
    if (tmp->space >= tmp->back->space) {
      return;
    } else {
      idx = tmp->back;
      tmp->back->front = NULL;
      struct freememory *tmp2 = idx;
      for (int j = 0; j < r; ++j) {
        if (tmp->space >= tmp2->space) {
          tmp->back = tmp2;
          tmp->front = tmp2->front;
          tmp2->front->back = tmp;
          tmp2->front = tmp;
          break;
        }
        tmp2 = tmp2->back; //这里同bf
      }
    }
  } else if (tmp->space == *need) {
    idx = tmp->back;

    free(tmp);
    --r;
  }
}

//这个函数是用来在链表中找到它物理上的前一个元素和后一个元素
void locate(struct freememory **front, struct freememory **back, int start) {
  struct freememory *tmp = idx;
  tmp = idx;

  int min = idx->start;
  int max = idx->start;

  for (int i = 0; i < r; i++) {
    if (tmp->start < min) {
      min = tmp->start;
      *front = tmp;
    }
    if (tmp->start > max) {
      max = tmp->start;
      *back = tmp;
    }
    tmp = tmp->back;
  }
  tmp = idx;

  for (int i = 0; i < r; i++) {
    if (start >= tmp->start + tmp->space &&
        (start - tmp->start - tmp->space) <
            (start - (*front)->start - (*front)->space)) {
      *front = tmp;
    }

    if (start <= tmp->start && tmp->start - start < (*back)->start - start) {
      *back = tmp;
    }
    tmp = tmp->back;
  }
}
//以上这么长一个部分都是为了找到前一个元素和后一个元素 life sucks

//插入排序
void insertion_sort(struct freememory *obj, struct freememory *begin,
                    int type) {
  struct freememory *tmp = begin;
  // printf("begin with %d\nobj space = %d\n", tmp->start, obj->space);
  //找到插入点
  if (type == 2) {
    while (tmp != tail && tmp->space < obj->space) {
      tmp = tmp->back;
    }
  }
  if (type == 3) {
    while (tmp->front != NULL && tmp->space < obj->space) {
      tmp = tmp->front;
    }
  }
  printf("finally tmp = %d\n", tmp->start);
  if (tmp == idx) {
    idx->front = obj;
    obj->back = idx;
    idx = obj;
  } else {
    obj->front = tmp->front;
    obj->back = tmp;
    tmp->front->back = obj;
    tmp->front = obj;
  }
}

// recycle的返回值为bool 如果输入的start和size不合法就返回false
bool recycle(int start, int size, int type) {
  struct freememory *front = idx;
  struct freememory *back = idx;
  struct freememory *tmp;
  // struct freememory *next;
  bool flag = 1;
  locate(&front, &back, start);
  printf("front = %d back = %d\n", front->start, back->start);
  //越界判断 若不合法直接返回false
  if (start + size > back->start || start < front->start + front->space) {
    return false;
  }
  if (back->start == start + size && back && back != NULL) {
    //和 back 合并
    back->start -= size;
    back->space += size;
    size = back->space;
    flag = 0;
    tmp = back;
  }
  if (front->start + front->space == start && flag && front != NULL) {
    //和 front 合并
    front->space += size;
    flag = 0;
    tmp = front;
    //合并后把结点拿出去用来做插入排序
    if (tmp != idx) {
      tmp->front->back = tmp->back;
    } else {
      idx = tmp->back;
      tmp->back->front = NULL;
    }
  }

  if (front->start + front->space == back->start) {
    //合并后back和front重合 于是合并
    tmp = back;
    tmp->back->front = tmp->front;

    //直接把back结点删除
    if (tmp != idx) {
      tmp->front->back = tmp->back;
    } else {
      idx = tmp->back;
      tmp->back->front = NULL;
    }

    front->space += tmp->space;
    free(tmp);
    r--;
    tmp = front;
    //合并后把结点拿出去用来做插入排序
    if (tmp != idx) {
      tmp->front->back = tmp->back;
    } else {
      idx = tmp->back;
      tmp->back->front = NULL;
    }
  }
  if (flag) {
    //不满足任何条件就插入新的结点
    tmp = malloc(sizeof(struct freememory));
    tmp->start = start;
    tmp->space = size;
    r++;
  }

  if (type == 1) {
    tmp->front = front;
    tmp->back = back;
    front->back = tmp;
    back->front = tmp;
  }
  if (type == 2) {
    if (flag) {
      insertion_sort(tmp, idx, type);
    } else if (tmp->back == tail) {
      insertion_sort(tmp, tmp, type);
    } else {
      insertion_sort(tmp, tmp->back, type);
    }
    // pass
  }
  if (type == 3) {
    if (flag) {
      insertion_sort(tmp, idx, type);
    } else if (tmp == idx) {
      insertion_sort(tmp, tmp, type);
    } else {
      insertion_sort(tmp, tmp->front, type);
    }
  }
  return true;
}

void run_hints() {
  printf("Please choose the operation:\n");
  printf("  [1] Recycle\n");
  printf("  [2] Allocate\n");
  printf("  [3] Exit\n");
  printf("Please input the number of algorithm:");
}

void recycle_run(int choice) {
  printf("recycle running!\n");
  int start, size;
  bool result = false;
  printf("input start and size:");
  scanf("%d %d", &start, &size);

  result = recycle(start, size, choice);
  if (result) {
    printStatus();
  } else {
    printf("error\n");
  }
}

void allocate_run(int choice) {
  void (*func)(const int *);
  int *need = malloc(sizeof(int));
  switch (choice) {
  case 1:
    func = ff;
    break;
  case 2:
    func = bf;
    break;
  case 3:
    func = wf;
    break;
  }
  printf("Please input the space you need:");
  scanf("%d", need);
  func(need);
  printStatus();
}

void run(int choice) {
  switch (choice) {
  case 2:
    QuickSort(idx, tail, 1);
    printStatus();
    break;
  case 3:
    QuickSort(idx, tail, 2);
    printStatus();
    break;
  }
  run_hints();
  int choice1;
  while (scanf("%d", &choice1)) {
    switch (choice1) {
    case 1:
      recycle_run(choice);
      break;
    case 2:
      allocate_run(choice);
      break;
    case 3:
      return;
      break;
    }
    run_hints();
  }
}

int main() {

  int choice = 1;

  bool result;
  init();
  printStatus();

  printf("Please choose the algorithm:\n");
  printf("  [1] First Fit\n");
  printf("  [2] Best Fit\n");
  printf("  [3] Worst Fit\n");
  printf("Please input the number of algorithm:");

  scanf("%d", &choice);
  run(choice);
  printStatus();
  delete ();
  return 0;
}
