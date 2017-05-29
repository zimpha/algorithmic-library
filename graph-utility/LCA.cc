#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>

namespace LCA {
  const int N = 100000 + 10, POW = 15;
  int fa[N][POW + 1], dep[N];
  void dfs(std::vector<int> G[], int u, int p = -1) {
    memset(fa[u], -1, sizeof(fa[u]));
    fa[u][0] = p;
    dep[u] = p == -1 ? 0 : dep[p] + 1;
    for (auto &&v: G[u]) dfs(G, v, u);
  }
  void build(int n) {
    for (int i = 1; (1 << i) <= n; ++i) {
      for (int j = 0; j < n; ++j) if (~fa[j][i - 1]) {
        fa[j][i] = fa[fa[j][i - 1]][i - 1];
      }
    }
  }
  int up(int u, int d) {
    for (int i = 0; d; ++i, d >>= 1) {
      if (d & 1) u = fa[u][i];
    }
    return u;
  }
  int ask(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    u = up(u, dep[u] - dep[v]);
    for (int i = POW; i >= 0; --i) {
      if (fa[u][i] == fa[v][i]) continue;
      u = fa[u][i];
      v = fa[v][i];
    }
    if (u != v) u = fa[u][0];
    return u;
  }
}
