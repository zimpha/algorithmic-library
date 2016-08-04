//sum_{i=1}^{n} a[i] * x[i] = b, x[i] >= 0
//given a[i] and b, check solution existing
typedef std::pair<LL, int> PLI;
int get_vals(int n, int a[]) {
  std::priority_queue<PLI, std::vector<PLI>, std::greater<PLI>> Q;
  int X = *std::min_element(a, a + n);
  std::fill(dis, dis + X, inf);
  dis[0] = 0; Q.push(PLI(0, 0));
  while (!Q.empty()) {
    PLI nw = Q.top(); Q.pop();
    int u = nw.second;
    LL val = nw.first;
    if (dis[u] != val) continue;
    for (int i = 0; i < n; ++ i) {
      LL tmp = val + a[i];
      int v = tmp % X;
      if (dis[v] > tmp) {
        dis[v] = tmp;
        Q.push(PLI(tmp, v));
      }
    }
  }
  return X;
}
bool ok(int n, int a[], int b) {
  int X = get_vals(n, a);
  return dis[b % X] <= b;
}
