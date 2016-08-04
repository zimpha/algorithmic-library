std::vector<int> lis(int n, int a[]) {
  std::vector<int> g, res(n);
  for (int i = 0; i < n; ++ i) {
    res[i] = std::lower_bound(g.begin(), g.end(), a[i]) - g.begin();
    if (res[i] == g.size()) g.push_back(a[i]);
    else g[res[i]] = a[i];
  }
  return res;
}
