/**
 * 求图的绝对中心, 可用在求最小直径生成树
 *
 * g[][]为邻接矩阵, 把没有的边权赋为inf
 * 返回一个pair, 表示绝对中心在这条边(s1, s2)上
 * ds1, ds2记录s1和s2距离绝对中心的距离, 按需返回
 **/
#include <algorithm>

const int N = 1000, inf = 1e9;

void floyd(int n, int g[][N], int d[][N]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      d[i][j] = g[i][j];
    }
  }
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);
      }
    }
  }
}

std::pair<int, int> KarivHakimi(int n, int g[][N]) {
  static int rk[N][N], d[N][N];
  double ds1 = 0, ds2 = 0;
  floyd(n, g, d);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) rk[i][j] = j;
    std::sort(rk[i], rk[i] + n, [&](int a, int b) {
      return d[i][a] < d[i][b];
    });
  }
  int ret = inf, s1 = -1, s2 = -1;
  for (int u = 0; u < n; ++u) {
    if (d[u][rk[u][n - 1]] * 2 < ret) {
      ret = d[u][rk[u][n - 1]] * 2;
      s1 = s2 = u;
      ds1 = ds2 = 0;
    }
    for (int v = 0; v < n; ++v) {
      if (g[u][v] == inf) continue;
      for (int k = n - 1, i = n - 2; i >= 0; --i) {
        if (d[v][rk[u][i]] > d[v][rk[u][k]]) {
          int tmp = d[u][rk[u][i]] + d[v][rk[u][k]] + g[u][v];
          if (tmp < ret) {
            ret = tmp;
            s1 = u, s2 = v;
            ds1 = 0.5 * tmp - d[u][rk[u][i]];
            ds2 = g[u][v] - ds1;
          }
          k = i;
        }
      }
    }
  }
  return std::make_pair(s1, s2);
}
