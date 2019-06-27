#include <cmath>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>

namespace Normal {
  const int N = 100000 + 10, P = 333;
  struct Q {
    int l, r, id;
    bool operator < (const Q &rhs) const {
      return l / P == rhs.l / P ? r < rhs.r : l < rhs.l;
    }
  } seq[N];
  int A[N], ans[N], n, m;
  namespace mo {
    int u[N], now;
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
    mo::init();
    for (int i = 0, l = 0, r = -1; i < m; ++i) {
      int L = seq[i].l, R = seq[i].r;
      while (r < R) mo::add(A[++r]);
      while (r > R) mo::del(A[r--]);
      while (l < L) mo::del(A[l++]);
      while (l > L) mo::add(A[--l]);
      ans[seq[i].id] = mo::now;
    }
  }
}

namespace Tree {
  using int64 = long long;
  const int N = 100000 + 10, K = 17;
  std::vector<int> G[N];
  int dep[N], f[N][K + 1];
  int st[N], ed[N], loc[N << 1];
  int val[N], n, m, P, dfn;
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
  namespace mo {
    int cnt[N], vis[N], sum;
    void deal(int x) {//这部分维护序列
      int c = val[x]; vis[x] ^= 1;
      sum -= !!cnt[c];
      if (!vis[x]) cnt[c]--;
      else cnt[c]++;
      sum += !!cnt[c];
    }
  }
  struct Node {
    int l, r, z, id;
    bool operator < (const Node &rhs) {
      return l / P == rhs.l / P ? r < rhs.r : l / P < rhs.l / P;
    }
  } Q[N];
  int ans[N];
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
      while (r < Q[i].r) mo::deal(loc[++r]);
      while (r > Q[i].r) mo::deal(loc[r--]);
      while (l < Q[i].l) mo::deal(loc[l++]);
      while (l > Q[i].l) mo::deal(loc[--l]);
      if (Q[i].z) mo::deal(Q[i].z);
      ans[Q[i].id] = mo::sum;
      if (Q[i].z) mo::deal(Q[i].z);
    }
    for (int i = 0; i < m; ++ i) printf("%d\n", ans[i]);
  }
}

namespace Modified {
  using int64 = long long;
  using pii = std::pair<int, int>;

  constexpr int N = 1e5 + 10;
  constexpr int M = 1e5 + 10;

  struct query_t {
    int l, r, t;
  } ask[N];

  std::vector<int> query[60][60];
  int a[N], value[N];
  int angry[N];
  int n, q, B;

  namespace mo {
    int cnt[M], mark[N];
    int64 ret;
    void init() {
      memset(cnt, 0, sizeof(cnt));
      memset(mark, 0, sizeof(mark));
      ret = 0;
    }
    void add(int x) {
      int v = value[x];
      if (mark[x]) ret -= (int64)v * angry[cnt[v]--];
      else ret += (int64)v * angry[++cnt[v]];
      mark[x] ^= 1;
    }
  }

  void run() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
      scanf("%d", &a[i]);
    }
    for (int i = 1; i <= n; ++i) {
      scanf("%d", &angry[i]);
    }
    for (B = 1; B * B * B < n; ++B);
    B = B * B;
    std::vector<pii> modify;
    scanf("%d", &q);
    for (int i = 0; i < q; ++i) {
      int op, x, y;
      scanf("%d%d%d", &op, &x, &y);
      if (op == 2) {
        ask[i].t = -2;
        modify.emplace_back(x - 1, y);
      } else {
        --x, --y;
        ask[i].t = modify.size() - 1;
        ask[i].l = x;
        ask[i].r = y;
        query[x / B][y / B].emplace_back(i);
      }
    }
    std::vector<int64> ret(q);
    int m = (n - 1) / B + 1;
    for (int i = 0; i < m; ++i) {
      for (int j = i; j < m; ++j) {
        if (query[i][j].empty()) continue;
        memcpy(value, a, sizeof(*a) * n);
        mo::init();
        int t = 0, l = 0, r = -1;
        for (auto &&v: query[i][j]) {
          auto qr = ask[v];
          while (r < qr.r) mo::add(++r);
          while (r > qr.r) mo::add(r--);
          while (l < qr.l) mo::add(l++);
          while (l > qr.l) mo::add(--l);
          for (int x, y; t <= qr.t; ++t) {
            std::tie(x, y) = modify[t];
            bool flag = qr.l <= x && x <= qr.r;
            if (flag) mo::add(x);
            value[x] = y;
            if (flag) mo::add(x);
          }
          ret[v] = mo::ret;
        }
      }
    }
    for (int i = 0; i < q; ++i) {
      if (ask[i].t == -2) continue;
      printf("%lld\n", ret[i]);
    }
  }
}
