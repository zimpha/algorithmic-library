struct Tarjan {// index from 0 to n - 1
  static const int MAXN = 100000 + 10;
  std::vector<int> SCC[MAXN];
  int low[MAXN], dfn[MAXN], col[MAXN];
  int stk[MAXN], top, scc_cnt, sz;
  void dfs(int x, const std::vector<int> G[]) {
    low[x] = dfn[x] = ++sz;
    stk[++top] = x;
    for (auto &y: G[x]) {
      if (!dfn[y]) {
        dfs(y, G);
        low[x] = std::min(low[x], low[y]);
      }
      else if (col[y] == -1) {
        low[x] = std::min(low[x], dfn[y]);
      }
    }
    if (dfn[x] == low[x]) {
      SCC[scc_cnt++].clear();
      do {
        SCC[scc_cnt - 1].push_back(stk[top]);
        col[stk[top]] = scc_cnt - 1;
      } while (stk[top--] != x);
    }
  }
  void run(int n,const std::vector<int> G[]) {
    sz = top = scc_cnt = 0;
    memset(dfn,  0, sizeof(dfn));
    memset(col, -1, sizeof(col));
    for (int i = 0; i < n; ++i) {
      if (!dfn[i]) dfs(i,G);
    }
  }
};
