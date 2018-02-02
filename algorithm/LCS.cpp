#include <iostream>
#define max(a, b) a > b ? a : b
using namespace std;

char X[51];
char Y[51];
int C[51][51];
int b[51][51];
stack<string> ss;

void Init() {
  for (int i = 0; i < 52; i++) {
    for (int j = 0; j < 52; j++) {
      C[i][j] = 0;
      b[i][j] = 0;
    }
  }
  for (int i = 0; i < 51; i++) {
    X[i] = 0;
    Y[i] = 0;
  }
}

void LCS(int i, int j, char *x) {
  if (i == 0 || j == 0)
    return;
  if (b[i][j] == 1) {
    LCS(i - 1, j - 1, x);
    cout << x[i];
    // s += x[i];
  } else if (b[i][j] == 2)
    LCS(i - 1, j, x);
  else
    LCS(i, i - 1, x);
}

int main(int argc, char const *argv[]) {
  int T = 0;
  cin >> T;
  int k = 1;
  int m = 0;
  int n = 0;

  while (T--) {
    Init();
    cin >> m >> n;
    for (int i = 1; i <= m; i++) {
      cin >> X[i];
    }
    for (int i = 1; i <= n; i++) {
      cin >> Y[i];
    }

    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (X[i] == Y[j]) {
          C[i][j] = C[i - 1][j - 1] + 1;
          b[i][j] = 1;
        } else if (C[i - 1][j] >= C[i][j - 1]) {
          C[i][j] = C[i - 1][j];
          b[i][j] = 2;
        } else {
          C[i][j] = C[i][j - 1];
          b[i][j] = 3;
        }
      }
    }

    // int **sb = b;
    int max = C[m][n];
    cout << "Case " << k++ << endl;
    cout << "LCS < ";
    LCS(m, n, X);

    cout << " >" << endl;
    cout << "max = " << max << endl;
    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {
        cout << C[i][j] << " ";
      }
      cout << endl;
    }
    cout << endl;
    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {
        cout << b[i][j] << " ";
      }
      cout << endl;
    }
  }
  return 0;
}
