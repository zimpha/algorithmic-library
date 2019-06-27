/*
 * 调用init()初始化, 传入变量个数, 节点标号必须从0开始
 * 调用add_edge()添加边, 调用add_var()添加变量的初始值
 * 调用solve()求解2sat 如果有解存在sol中
 * 对于变量x, x*2表示x, x*2+1表示~x, sol[x*2]表示x的值
 *
 * 使用tarjan算法求出强连通分量并染色，如果有一个布尔变量的原变量和反变量在一个scc中，那么原问题无解，否则一定存在解。
 *
 * 若求任意解，则对染色后的图重新建图，但是把边反向，统计入度，做拓扑排序，同时求出每个点反变量的颜色。
 * 之后按照拓扑排序的顺序，依次将对应scc标记为1以表示选中，其反变量对应的scc标记为2以表示不选中（也表示其反变量选中）。
 * 最后遍历一遍布尔变量的原变量和反变量：若变量所在的scc被标记为1，则表明选中变量；否则表示其另外一个变量被选中。
 *
 * 若求字典序最小的解，也使用标记法。对所有变量进行遍历，若 x_i 其原变量和反变量都没进行标记过，则dfs进去优先标记 x_i 的原变量，
 * 并根据他们连过的边，继续dfs标记。如果标记的过程中，某个变量的原变量和反变量同时被标记，则表示选中x_i的原变量无解，
 * 进而选中x_i的反变量继续标记。若在遍历过程中，发现无论标记某个遍历的原变量和反变量都无解，表示整个2-SAT无解。否则标记的就是字典序最小的解。
 */
#include <vector>
#include <algorithm>

struct TwoSAT {
  static const int N = 200000 + 10;
  std::vector<int> edges[N];
  std::vector<int> scc[N];
  int low[N], dfn[N], col[N];
  int stk[N], top, cnt, sz;
  int sol[N];
  void init(int n) {
    this->n = n;
    for (int i = 0; i < n * 2; ++i) {
      edges[i].clear();
    }
  }
  void dfs(int x) {
    low[x] = dfn[x] = ++sz;
    stk[++top] = x;
    for (auto &y: edges[x]) {
      if (!dfn[y]) {
        dfs(y);
        low[x] = std::min(low[x], low[y]);
      }
      else if (col[y] == -1) {
        low[x] = std::min(low[x], dfn[y]);
      }
    }
    if (dfn[x] == low[x]) {
      SCC[cnt++].clear();
      do {
        SCC[cnt - 1].push_back(stk[top]);
        col[stk[top]] = cnt - 1;
      } while (stk[top--] != x);
    }
  }
  bool solve() {
    sz = top = cnt = 0;
    memset(dfn,  0, sizeof(*dfn) * n * 2);
    memset(col, -1, sizeof(*col) * n * 2);
    for (int i = 0; i < n * 2; ++i) {
      if (!dfn[i]) dfs(i);
    }
    for (int i = 0; i < n * 2; i += 2) {
      if (col[i] == col[i ^ 1]) return false;
      sol[i] = -1;
    }
    for (int i = 0; i < cnt; ++i) {
      int val = 1;
      for (auto &&x: scc[i]) {
        if (sol[x] == 0) val = 0;
        for (auto &&y: edges[x]) {
          if (sol[y] == 0) val = 0;
        }
        if (val == 0) break;
      }
      for (auto &&x: scc[i]) {
        sol[x] = val;
        sol[x ^ 1] = !val;
      }
    }
    return true;
  }
  void add_clause(int x, int xv, int y, int yv) {//x=xv or y=yv
    x = x << 1 | xv, y = y << 1 | yv;
    add_edge(x ^ 1, y);
    add_edge(y ^ 1, x);
  }
  void add_var(int x, int xv) { // x = xv
    x = x << 1 | xv;
    edges[x ^ 1].push_back(x);
  }
  void add_edge(int x, int y) {
    edges[x].push_back(y);
  }
};
