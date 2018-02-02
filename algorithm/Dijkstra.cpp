#include <iostream>
#include <string>

using namespace std;

int a[51][51];
int dist[51];
int previous[51];
bool s[51] = {false};
const int MAXINT = 32767;

void show(int dst, int dep) {
  if (previous[dst] != dep) {
    show(previous[dst], dep);
  } else {
    cout << dep;
  }
  cout << "->" << dst;
}

void dijkstra(int dep, int n) {
  for (int i = 1; i <= n; i++) {
    s[i] = false;
    dist[i] = a[dep][i];
    if (a[dep][i] == -1)
      dist[i] = MAXINT;
    if (dist[i] == -1) {
      previous[i] = -1;
    } else {
      previous[i] = dep;
    }
  }
  s[dep] = true;
  dist[dep] = 0;

  for (int i = 2; i <= n; i++) {
    int mindist = MAXINT;
    int u = dep;
    for (int j = 1; j <= n; j++) {
      if (!(s[j]) && dist[j] < mindist) {
        u = j;
        mindist = dist[j];
      }
    }
    s[u] = true;
    for (int j = 1; j <= n; j++) {
      if (!(s[j]) && a[u][j] != -1) {
        if (dist[u] + a[u][j] < dist[j]) {
          　　dist[j] = dist[u] + a[u][j];
          　　previous[j] = u;
          　　
        }
        if (dist[u] + a[u][j] == dist[j]) {
        }
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  int n = 0;
  int k = 0;
  int dep, des;
  while (cin >> n) {
    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= n; j++) {
        cin >> a[i][j];
      }
    cin >> dep >> des;
    dijkstra(dep, n);
    cout << "Case " << ++k << endl;
    cout << "The least distance from " << dep << "->" << des << " is "
         << dist[des] << endl;
    cout << "the path is ";
    show(des, dep);
    cout << endl;
  }
  return 0;
}
