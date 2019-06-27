#include <vector>
#include <algorithm>

namespace centroid {
  std::vector<int> sz, mark;
  int total, mins, rt;
  void init(int n) {
    sz.resize(n);
    mark.assign(n, 0);
  }
  void get_center(int u, int f, std::vector<int> G[]) {
    int mx = 0; sz[u] = 1;
    for (auto &&v: G[u]) if (v != f && !mark[v]) {
      get_center(v, u, G);
      sz[u] += sz[v];
      mx = std::max(mx, sz[v]);
    }
    mx = std::max(mx, total - sz[u]);
    if (mx < mins) mins = mx, rt = u;
  }
  void run(int u, int tot, std::vector<int> G[]) {
    total = tot, mins = tot * 2, rt = u;
    get_center(u, -1, G);// find centroid
    mark[u = rt] = true;
    get_center(u, -1, G);// recalculate subtree size
    for (auto &v: G[u]) if (!mark[v]) {
      run(v, sz[v], G);
    }
  }
}
