#include <vector>
#include <algorithm>

struct DSU {
  std::vector<int> data;
  void init(int n) {data.assign(n, -1);}
  bool unionSet(int x, int y) {
    x = root(x); y = root(y);
    if(x != y) {
      if (data[y] < data[x]) std::swap(x, y);
      data[x] += data[y]; data[y] = x;
    }
    return x != y;
  }
  bool findSet(int x, int y) {return root(x) == root(y);}
  int root(int x) {return data[x] < 0 ? x : data[x] = root(data[x]);}
  int size(int x) {return -data[root(x)]; }
};

//带撤销的并查集
namespace DSU2 {
  const static int MAXN = 100000 + 10;
  int fa[MAXN], ds[MAXN], rk[MAXN];
  int S[MAXN], top;
  void init(int n) {
    for (int i = 1; i <= n; ++ i) {
      fa[i] = i, rk[i] = ds[i] = 0;
    }
    top = 0;
  }
  int dis(int x) {
    int r(0);
    for (; x != fa[x]; x = fa[x]) r ^= ds[x];
    return r;
  }
  int get(int x) {
    while (x != fa[x]) x = fa[x];
    return fa[x];
  }
  void merge(int x, int y, int d) {
    x = get(x); y = get(y);
    if (x == y) return;
    if (rk[x] > rk[y]) std::swap(x, y);
    if (rk[x] == rk[y]) ++ rk[y], S[++ top] = -y;
    fa[x] = y; ds[x] = d; S[++ top] = x;
  }
  void restore(int ed) {
    for (; top > ed; -- top) {
      if (S[top] < 0) -- rk[-S[top]];
      else fa[S[top]] = S[top], ds[S[top]] = 0;
    }
  }
}
