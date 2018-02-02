// 3d1-2 矩阵连乘 动态规划迭代实现
// A1 30*35 A2 35*15 A3 15*5 A4 5*10 A5 10*20 A6 20*25
// p[0-6]={30,35,15,5,10,20,25}
#include <iostream>
using namespace std;

const int L = 21;

void MatrixChain(int n, int **m, int **s, int *p);
void Traceback(int i, int j, int **s); //构造最优解
void Traceback1(int i, int j, int **s, int n, int countbr);

int p[20] = {0};

int main() {
  int n;
  int count = 0;

  while (cin >> n) {
    int countbr = 0;
    int p[20];
    for (int i = 0; i <= n; i++) {
      cin >> p[i];
    }

    int **s = new int *[L];
    int **m = new int *[L];
    for (int i = 0; i < 21; i++) {
      s[i] = new int[L];
      m[i] = new int[L];
    }

    MatrixChain(n, m, s, p);
    cout << "Case " << ++count << endl;
    cout << m[1][n] << " ";
    Traceback1(1, n, s, n, countbr);
    cout << endl;
  }

  return 0;
}

void MatrixChain(int n, int **m, int **s, int *p) {
  for (int i = 1; i <= n; i++) {
    m[i][i] = 0;
  }
  for (int r = 2; r <= n; r++) // r为当前计算的链长（子问题规模）
  {
    for (int i = 1; i <= n - r + 1; i++) // n-r+1为最后一个r链的前边界
    {
      int j = i + r - 1; //计算前边界为r，链长为r的链的后边界

      m[i][j] = m[i + 1][j] +
                p[i - 1] * p[i] * p[j]; //将链ij划分为A(i) * ( A[i+1:j] )

      s[i][j] = i;

      for (int k = i + 1; k < j; k++) {
        //将链ij划分为( A[i:k] )* (A[k+1:j])
        int t = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
        if (t < m[i][j]) {
          m[i][j] = t;
          s[i][j] = k;
        }
      }
    }
  }
  // return m[1][L-1];
}

void Traceback(int i, int j, int **s) {
  if (i == j)
    return;
  Traceback(i, s[i][j], s);
  Traceback(s[i][j] + 1, j, s);
  cout << "Multiply A" << i << "," << s[i][j];
  cout << " and A" << (s[i][j] + 1) << "," << j << endl;
}

void Traceback1(int i, int j, int **s, int n, int countbr) {
  if (i == j)
    cout << "A" << i;
  else {
    if (countbr != 0) {
      cout << "(";
    }
    countbr++;
    Traceback1(i, s[i][j], s, n, countbr);
    Traceback1(s[i][j] + 1, j, s, n, countbr);
    if (countbr != 1) {
      cout << ")";
    }
    countbr++;
  }
}
