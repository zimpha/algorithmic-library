#include <vector>
#include <algorithm>

struct Hungarian {
  std::vector<int> vc, is;// vertex corver and independent set
  std::vector<int> pos, neg;// pos为左侧点所匹配到的右侧点编号, neg反之
  std::vector<bool> vis, mark;// 分别记录左右点的访问情况
  // 左侧点数目n, 右侧点数目m, 以及左边点到右边点的边表
  // 返回最大匹配数目, 方案存在pos和neg里面
  // vc和is记录了最小点覆盖和最大独立集的方案
  int run(int n, int m, std::vector<int> edges[]) {
    neg.assign(m, -1), pos.assign(n, -1);
    mark.resize(m), vis.resize(n);
    int ret = 0;
    for (int i = 0; i < n; ++i) {
      std::fill(mark.begin(), mark.end(), false);
      std::fill(vis.begin(), vis.end(), false);
      if (aug(i, edges)) ++ret;
    }
    std::fill(mark.begin(), mark.end(), false);
    std::fill(vis.begin(), vis.end(), false);
    for (int i = 0; i < n; ++i) {
      if (pos[i] == -1) aug(i, edges);
    }
    vc.clear(), is.clear();
    for (int i = 0; i < n; ++i) {
      if (!vis[i]) vc.push_back(i);
      else is.push_back(i);
    }
    for (int i = 0; i < m; ++i) {
      if (mark[i]) vc.push_back(i);
      else is.push_back(i);
    }
    return ret;
  }
  bool aug(int u, std::vector<int> edges[]) {
    vis[u] = true;
    for (auto &&v: edges[u]) if (!mark[v]) {
      mark[v] = true;
      if (neg[v] == -1 || aug(neg[v], edges)) {
        pos[u] = v, neg[v] = u;
        return true;
      }
    }
    return false;
  }
};
