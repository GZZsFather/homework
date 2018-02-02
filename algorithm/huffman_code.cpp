#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

class node {
public:
  node(int type, int w, node *left, node *right, string code) {
    type = type;
    weight = w;
    leftchild = left;
    rightchild = right;
    code = code;
    frequency = weight;
  }
  int type; // 0表示真nodes，1表示合并的nodes
  int weight;
  node *leftchild;
  node *rightchild;
  string code;
  int frequency;
};

void insertion_sort(node **array, int low, int high) {

  for (int i = low + 1; i < high; i++) {
    node *tem = array[i];
    int j = i - 1;
    while (array[j]->weight > tem->weight && j >= low) {
      array[j + 1] = array[j];
      j--;
      // cout << j << endl;
    }

    array[j + 1] = tem;
  }
}

void quicksort(int left, int right, node **a) {
  int i, j;
  // node *t, temp;
  // cout << "I'm in!!" << endl;
  if (left > right)
    return;

  node *temp = a[left];
  i = left;
  j = right;
  while (i != j) {
    while (a[j]->weight >= temp->weight && i < j)
      j--;
    while (a[i]->weight <= temp->weight && i < j)
      i++;
    if (i < j) {
      node *t = a[i];
      a[i] = a[j];
      a[j] = t;
    }
  }
  // cout << "one is finished!" << endl;
  a[left] = a[i];
  a[i] = temp;

  quicksort(left, i - 1, a);
  quicksort(i + 1, right, a);
}

void create_huffman_tree(int *w, int n, node **array) {

  // for (int i = 0; i < n; i++) {
  //   array[i] = new node(0, w[i], NULL, NULL, "");
  // }

  // insertion_sort(array, 0, n);
  quicksort(0, n - 1, array);

  int p = 0;
  while (p != n - 1) {
    node *min_1 = array[p];
    node *min_2 = array[p + 1];
    node *new_node =
        new node(1, array[p]->weight + array[p + 1]->weight, min_1, min_2, "");
    array[p + 1] = new_node;
    p++;
    insertion_sort(array, p, n);
  }
}

void create_huffman_code(node *p) {
  queue<node *> nq;
  nq.push(p); //进队列
  while (nq.size() != 0) {
    node *cur = nq.front();
    nq.pop(); //出队列
    node *l = cur->leftchild;
    node *r = cur->rightchild;
    if (r != NULL) {
      r->code = cur->code + "0";
      nq.push(r);
    }
    if (l != NULL) {
      l->code = cur->code + "1";
      nq.push(l);
    }

    // if (l == NULL && r == NULL) {
    //   cout << cur->frequency << " " << cur->code << endl;
    // }
  }
}

int main(void) {
  node *array[50];
  node *tmp[50];
  int w[50] = {0};
  int T = 0;
  cin >> T;
  int k = 0;
  while (T--) {
    int n = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> w[i];
      array[i] = new node(0, w[i], NULL, NULL, "");
      tmp[i] = array[i];
    }
    if (n == 1) {
      cout << "Case " << ++k << endl;
      cout << "1 0" << endl;
      cout << endl;
    } else {
      sort(w, w + n);
      create_huffman_tree(w, n, array);
      cout << "Case " << ++k << endl;
      create_huffman_code(array[n - 1]);
      for (int i = 0; i < n; i++) {
        cout << tmp[i]->weight << " " << tmp[i]->code << endl;
      }

      cout << endl;
    }
  }

  return 0;
}
