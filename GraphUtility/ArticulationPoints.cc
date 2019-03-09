#include <bits/stdc++.h>
class Articulation {
public:
  typedef std::pair<int, int> PII;
  static const int N = 100005; // 最大结点个数
  std::vector<PII> keyE;               // keyE为割边集，keyV为割点集
  std::vector<int> keyV, cc[N];      // cc[p]表示结点p在哪些双连通分量中
  int cnt;  // 对于旧版编译器，将上面cc[N]改成vector的形式
  // 传入结点个数n及各结点的出边e[]，返回双连通分量的个数cnt
  int run(int n, const std::vector<int> G[]){
    memset(dfn, use = 0, sizeof(dfn[0]) * n);
    memset(low, cnt = 0, sizeof(low[0]) * n);
    keyE.clear();
    fill_n(cc, n, keyV = std::vector<int>());
    for(int i = 0; i < n; ++i) {
      if(!dfn[i]) dfs(i, 1, G);
    }
    return cnt;
  }
private:
  int dfn[N], low[N], dot[N], use;
  void dfs(int x, int dep, const std::vector<int> G[]){
    int src = 0, out = 1 < dep;
    dot[use++] = x;
    dfn[x] = low[x] = dep;
    for (auto &y: G[x]) {
      if (!dfn[y]){
        dfs(y, dep + 1, G);
        low[x] = std::min(low[x], low[y]);
        if (low[y] > dfn[x]) keyE.push_back(PII(x, y));
        if (low[y] >= dfn[x]){
          if (++out == 2) keyV.push_back(x);
          while (dot[--use] != y) cc[dot[use]].push_back(cnt);
          cc[x].push_back(cnt); 
          cc[y].push_back(cnt++);
        }
      } else if (dfn[y] != dfn[x] - 1 || src++){
        low[x] = std::min(low[x], dfn[y]);
      }
    }
  }
};
