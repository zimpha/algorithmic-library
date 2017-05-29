// 调用init()初始化, 传入变量个数, 节点标号必须从0开始
// 调用add_edge()添加边, 调用add_var()添加变量的初始值
// 调用solve()求解2sat 如果有解存在mark中
// 对于变量x, x*2表示x, x*2+1表示~x, mark[x*2]表示x的值
#include <vector>
#include <algorithm>

struct TwoSAT {
  static const int N = 200000 + 10;
  std::vector<int> G[N];
  int mark[N], S[N], sz, n;
  void init(int n) {
    this->n = n;
    for (int i = 0; i < n * 2; ++i) G[i].clear();
  }
  bool dfs(int x) {
    if (mark[x ^ 1]) return false;
    if (mark[x]) return true;
    mark[x] = true; S[sz++] = x;
    for (auto &y: G[x]) {
      if (!dfs(y)) return false;
    }
    return true;
  }
  bool solve() {
    std::fill(mark, mark + 2 * n, 0);
    for (int i = 0; i < n * 2; i += 2) {
      if (mark[i] || mark[i ^ 1]) continue;
      sz = 0;
      if (!dfs(i)) {
        while (sz > 0) mark[S[--sz]] = false;
        if (!dfs(i + 1)) return false;
      }
    }
    return true;
  }
  void add_clause(int x, int xv, int y, int yv) {//x=xv or y=yv
    x = x << 1 | xv, y = y << 1 | yv;
    add_edge(x ^ 1, y);
    add_edge(y ^ 1, x);
  }
  void add_var(int x, int xv) {
    x = x << 1 | xv;
    add_edge(x, x ^ 1);
  }
  void add_edge(int x, int y) {
    G[x].push_back(y);
  }
};
