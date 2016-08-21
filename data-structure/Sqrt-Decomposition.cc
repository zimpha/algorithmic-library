#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>

namespace Normal {
  const int MAXN = 100000 + 10, P = 333;
  struct Q {
    int l, r, id;
    bool operator < (const Q &rhs) const {
      return l / P == rhs.l / P ? r < rhs.r : l < rhs.l;
    }
  } seq[MAXN];
  int A[MAXN], ans[MAXN], n, m;
  namespace Block {
    int u[MAXN], now;
    void init() {}
    void add(int x) {}
    void del(int x) {}
  }
  void run() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) scanf("%d", A + i);
    for (int i = 0; i < m; ++i) {
      scanf("%d%d", &seq[i].l, &seq[i].r);
      seq[i].id = i;
    }
    std::sort(seq, seq + m);
    Block::init();
    for (int i = 0, l = 0, r = -1; i < m; ++i) {
      int L = seq[i].l, R = seq[i].r;
      while (r < R) Block::add(A[++r]);
      while (r > R) Block::del(A[r--]);
      while (l < L) Block::del(A[l++]);
      while (l > L) Block::add(A[--l]);
      ans[seq[i].id] = Block::now;
    }
  }
}

namespace Tree {
  typedef long long LL;
  const int MAXN = 100000 + 10, K = 17, M = 1e9 + 7;
  std::vector<int> G[MAXN];
  int dep[MAXN], f[MAXN][K + 1];
  int st[MAXN], ed[MAXN], loc[MAXN << 1];
  int n, m, P, dfn;
  void dfs(int x, int par = -1) {
    loc[st[x] = dfn++] = x;
    for (int i = 1; i <= K; ++i) f[x][i] = f[f[x][i - 1]][i - 1];
    for (auto &y: G[x]) if (y != par) {
      dep[y] = dep[f[y][0] = x] + 1;
      dfs(y, x);
    }
    loc[ed[x] = dfn++] = x;
  }
  int lca(int x, int y) {
    if (x == y) return x;
    if (dep[x] < dep[y]) std::swap(x, y);
    for (int i = K; ~i; --i) {
      if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    }
    if (x == y) return x;
    for (int i = K; ~i; --i) {
      if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    }
    return f[x][0];
  }
  int val[MAXN], cnt[MAXN], vis[MAXN], sum;
  void deal(int x) {//这部分维护序列
    int c = val[x]; vis[x] ^= 1;
    sum -= !!cnt[c];
    if (!vis[x]) cnt[c]--;
    else cnt[c]++;
    sum += !!cnt[c];
  }
  struct Node {
    int l, r, z, id;
    bool operator < (const Node &rhs) {
      return l / P == rhs.l / P ? r < rhs.r : l / P < rhs.l / P;
    }
  } Q[MAXN];
  int ans[MAXN];
  void run() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", val + i);
    for (int i = 1; i < n; ++i) {
      int u, v; scanf("%d%d", &u, &v);
      G[u].push_back(v);
      G[v].push_back(u);
    }
    dfs(dep[1] = 1);
    P = sqrt(n * 2);
    scanf("%d", &m);
    for (int i = 0; i < m; ++i) {
      int x, y; scanf("%d%d", &x, &y);
      if (st[x] > st[y]) std::swap(x, y);
      int z = lca(x, y); Q[i].id = i;
      if (z == x) Q[i].l = st[x], Q[i].r = st[y];
      else Q[i].l = ed[x], Q[i].r = st[y], Q[i].z = z;
    }
    std::sort(Q, Q + m);
    for (int i = 0, l = 0, r = -1; i < m; ++i) {
      if (r < Q[i].r) {
        for (++r; r <= Q[i].r; ++r) deal(loc[r]);
        --r;
      }
      for (; r > Q[i].r; --r) deal(loc[r]);
      for (; l < Q[i].l; ++l) deal(loc[l]);
      if (l > Q[i].l) {
        for (--l; l >= Q[i].l; --l) deal(loc[l]);
        ++l;
      }
      if (Q[i].z) deal(Q[i].z);
      ans[Q[i].id] = sum;
      if (Q[i].z) deal(Q[i].z);
    }
    for (int i = 0; i < m; ++ i) printf("%d\n", ans[i]);
  }
}

namespace Modified {
}
