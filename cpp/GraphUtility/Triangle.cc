#include <tuple>
#include <vector>

// Given the (a[i], b[i]) of a graph with n vertices, find all triangles in the graph.
// Time Complexity: O(m sqrt m)
std::vector<std::tuple<int, int, int>> triangle(int n, int m, const std::vector<int> &a, const std::vector<int> &b) {
  std::vector<int> deg(n);
  for (int i = 0; i < m; ++i) {
    deg[a[i]]++; deg[b[i]]++;
  }
  std::vector<std::vector<int>> edges(n);
  for (int i = 0; i < m; ++i) {
    if (deg[a[i]] < deg[b[i]] || (deg[a[i]] == deg[b[i]] && a[i] < b[i])) edges[a[i]].push_back(b[i]);
    else edges[b[i]].push_back(a[i]);
  }
  std::vector<bool> mark(n);
  std::vector<std::tuple<int, int, int>> ret;
  for (int u = 0; u < n; ++u) {
    for (auto &v: edges[u]) mark[v] = 1;
    for (auto &v: edges[u]) {
      for (auto &w: edges[v]) if (mark[w]) {
        ret.emplace_back(u, v, w);
      }
    }
    for (auto &v: edges[u]) mark[v] = 0;
  }
  return ret;
}
