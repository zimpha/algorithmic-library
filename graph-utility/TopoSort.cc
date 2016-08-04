/**
 * 返回一个有向图的一个拓扑序列, 如果不存在返回一个空vector
 * 如果需要字典序最大或者最小, 可以把queue替换成优先队列
 *
 * time complexity: O(n)
 **/
std::vector<int> toposort(int n, const std::vector<int> edges[]) {
  std::vector<int> deg(n, 0);
  for (int i = 0; i < n; ++i) {
    for (auto &v: edges[i]) ++deg[v];
  }
  std::vector<int> queue;
  for (int i = 0; i < n; ++i) {
    if (deg[i] == 0) queue.push_back(i);
  }
  for (size_t i = 0; i < queue.size(); ++i) {
    int u = queue[i];
    for (auto &v: edges[u]) {
      if (--deg[v] == 0) queue.push_back(v);
    }
  }
  if (queue.size() != n) queue.clear();
  return queue;
}
