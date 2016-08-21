#include <algorithm>
#include <vector>
#include <queue>
#include <utility>

const int N = 1000 + 10, inf = 1e9;
std::vector<int> djikstra(int n, int src, const int graph[][N]) {
  std::vector<int> dis(n, inf);
  std::vector<bool> mark(n, false);
  dis[src] = 0;
  for (int i = 0; i < n; ++i) {
    int ms = inf, k = -1;
    for (int j = 0; j < n; ++j) {
      if (!mark[j] && dis[j] < ms) {
        ms = dis[j], k = j;
      }
    }
    mark[k] = true;
    for (int j = 0; j < n; ++j) {
      if (!mark[j]) dis[j] = std::min(dis[j], dis[k] + graph[k][j]);
    }
  }
  return dis;
}

typedef std::pair<int, int> PII;
std::vector<int> spfa(int n, int src, const std::vector<PII> edges[]) {
  std::vector<int> dis(n, inf);
  std::vector<bool> mark(n, false);
  std::queue<int> queue;
  queue.push(src);
  dis[src] = 0;
  mark[src] = true;
  while (!queue.empty()) {
    int u = queue.front();
    mark[u] = false;
    queue.pop();
    for (auto &&x: edges[u]) {
      int v = x.first, w = x.second;
      if (dis[v] > dis[u] + w) {
        dis[v] = dis[u] + w;
        if (!mark[v]) queue.push(v);
        mark[v] = true;
      }
    }
  }
  return dis;
}
