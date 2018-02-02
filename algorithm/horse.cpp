#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
using namespace std;

typedef pair<int, int> pii;

int dist[10][10];
const int dir[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                       {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
int sx, sy, ex, ey;
bool check(int x, int y) {
  if (x < 0 || x >= 8 || y < 0 || y >= 8)
    return 0;
  return 1;
}
void BFS() {
  memset(dist, -1, sizeof(dist));
  dist[sx][sy] = 0;
  queue<pii> Q;
  Q.push({sx, sy});
  while (!Q.empty()) {
    pii now = Q.front();
    Q.pop();
    int x = now.first;
    int y = now.second;
    for (int i = 0; i < 8; ++i) {
      int tx = x + dir[i][0];
      int ty = y + dir[i][1];
      if (!check(tx, ty))
        continue;
      if (dist[tx][ty] == -1 || dist[tx][ty] > dist[x][y] + 1) {
        dist[tx][ty] = dist[x][y] + 1;
        Q.push({tx, ty});
      }
    }
  }
}
int main() {
  char S[5], T[5];
  while (scanf("%s%s", S, T) == 2) {
    sy = S[0] - 'a';
    sx = S[1] - '1';
    ey = T[0] - 'a';
    ex = T[1] - '1';
    BFS();
    printf("%s==>%s: %d moves\n", S, T, dist[ex][ey]);
  }
}
