#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct freememory {
  struct freememory *front;
  struct freememory *back;
  int space;
};

int r;
// struct freememory *start = &memory[0];
struct freememory *start;

void init_link() {
  struct freememory *head;
  start = (struct freememory *)malloc(sizeof(struct freememory));
  head = start;
  r = 10;
  start->space = rand() % 1024;
  start->front = NULL;
  int i = 0;
  while (i < r) {
    head->back = (struct freememory *)malloc(sizeof(struct freememory));
    head->back->front = head;
    head = head->back;
    head->back = NULL;
    head->space = rand() % 1024;
    i++;
  }
}
void MemSwap(struct freememory *mem1, struct freememory *mem2) {
  //下面注释掉的这段是因为我本来想调换结点，后来就偷懒只调换了结点里的space的值，如果不需要的话直接删了吧
  // struct freememory *front_temp;
  // struct freememory *back_temp;
  // front_temp = mem1->front;
  // back_temp = mem1->back;
  // mem1->front = mem2->front;
  // mem1->back = mem2->back;
  // mem2->front = front_temp;
  // mem2->back = back_temp;
  int temp = mem1->space;
  mem1->space = mem2->space;
  mem2->space = temp;
}

//寻找链表中指定偏移量的函数，begin是起始地址，index是偏移量
struct freememory *GetNode(struct freememory *begin, int index) {
  struct freememory *temp = start;
  for (int i = 0; i < index; i++) {
    if (temp->back == NULL)
      return NULL;
    else {
      temp = temp->back;
    }
  }
  return temp;
}

//比较蠢的冒泡排序
struct freememory *bubble_sort(struct freememory *L) {
  struct freememory *p, *tail, *next;
  int temp;
  if (L->back->back == NULL)
    return L;
  p = L;
  while (p->back != NULL) {
    p = p->back;
  }
  tail = p;
  while (tail != L->back->back) {
    for (p = L; p->back != tail; p = p->back) {
      next = p->back;
      if (p->space > next->space) {
        temp = p->space;
        p->space = next->space;
        next->space = temp;
      }
    }
    tail = p;
  }
  return L;
}

// qsort的partion函数
struct freememory *GetPartion(struct freememory *pBegin,
                              struct freememory *pEnd) {
  int space = pBegin->space;
  struct freememory *p = pBegin;
  struct freememory *q = p->back;

  while (q != pEnd) {
    if (q->space < space) {
      p = p->back;
      MemSwap(p, q);
    }

    q = q->back;
  }
  MemSwap(p, pBegin);
  return p;
}

//快速排序函数主体
void QuickSort(struct freememory *pBeign, struct freememory *pEnd) {
  if (pBeign != pEnd) {
    struct freememory *partion = GetPartion(pBeign, pEnd);
    QuickSort(pBeign, partion);
    QuickSort(partion->back, pEnd);
  }
}

void ff(int need) {}

void bf(const int *need) {
  // sort_asc();
  struct freememory *end = GetNode(start, r - 1);
  printf("begin = %d end = %d", start->space, end->space);
  QuickSort(start, end);
  // printf("hello\n");
  // printStatus_link();
  struct freememory *tmp = start;
  for (int i = 0; i < r; ++i) {
    if (tmp->space > *need) {
      tmp->space -= *need;
      if (i == 0 || tmp->space > tmp->front->space)
        break;
      else {
        tmp->front->back = tmp->back;
        tmp->back->front = tmp->front;
        struct freememory *tmp2 = start;
        for (int j = 0; j < i; ++j) {
          if (tmp->space <= tmp2->space) {
            tmp->back = tmp2->back;
            tmp2->back->front = tmp;
            tmp2->back = tmp;
            tmp->front = tmp2;
            break;
          }
        }
      }
    } else if (tmp->space == *need) {
      if (i == 0) {
        start = start->back;
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
  // printf("hello\n");
}

void wf(const int *need) {
  // sort_dsc();
  struct freememory *end = GetNode(start, r - 1);
  printf("begin = %d end = %d", start->space, end->space);
  QuickSort(start, end);

  struct freememory *tmp = start;
  for (int i = 0; i < r; ++i) {
    if (tmp->space > *need) {
      tmp->space -= *need;
      if (i == 0 || tmp->space < tmp->front->space)
        break;
      else {
        tmp->front->back = tmp->back;
        tmp->back->front = tmp->front;
        struct freememory *tmp2 = start;
        for (int j = 0; j < i; ++j) {
          if (tmp->space >= tmp2->space) {
            tmp->back = tmp2->back;
            tmp2->back->front = tmp;
            tmp2->back = tmp;
            tmp->front = tmp2;
            break;
          }
        }
      }
    } else if (tmp->space == *need) {
      if (i == 0) {
        start = start->back;
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

void printStatus_link() {
  printf("---------------------\n");
  printf("| Number | Size(KB) |\n");
  struct freememory *head = start;
  int i = 0;
  while (head->back != NULL) {
    printf("|    %d   |  %4d   |\n", i, head->space);
    head = head->back;
    i++;
  }
}

int main() {
  init_link();
  printf("original data: \n");
  printStatus_link();
  int algorithm = 0;
  int need = 0;
  struct freememory *end = GetNode(start, r - 1);
  struct freememory *temp;
  // printf("%d\n", end->space);
  temp = bubble_sort(start);
  // QuickSort(start, end);

  printStatus_link();
  printf("Please input the space(kB) needed:");
  scanf("%d", &need);
  printf("Please choose the algorithm:\n");
  printf("  [0]First Fit\n");
  printf("  [1]Best Fit\n");
  printf("  [2]Worst Fit\n");
  printf("Input the number:");
  scanf("%d", &algorithm);
  //

  switch (algorithm) {
  case 0:
    ff(need);
    break;
  case 1:

    bf(need);
    break;
  case 2:
    wf(need);
    break;
  default:
    ff(need);
  }
  printStatus_link();
  return 0;
}
