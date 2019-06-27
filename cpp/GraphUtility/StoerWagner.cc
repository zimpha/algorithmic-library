#include <algorithm>
#include <cstring>

// global minimum-cut
const int N = 500 + 10, inf = 1e9;
int stoer_wagner(int n, int G[][N]) {
  int dis[N], idx[N], vis[N], ret = inf;
  for (int i = 0; i < n; ++ i) idx[i] = i;
  while (n > 1) {
    int t = 1, s = 0;
    for (int i = 1; i < n; ++ i) {
      dis[idx[i]] = G[idx[0]][idx[i]];
      if (dis[idx[i]] > dis[idx[t]]) t = i;
    }
    memset(vis, 0, sizeof(vis)); vis[0]=true;
    for (int i = 1; i < n; ++ i) {
      if (i == n - 1) {
        ret = std::min(ret, dis[idx[t]]);
        for (int j = 0; j < n; ++ j) {
          G[idx[s]][idx[j]] += G[idx[j]][idx[t]];
          G[idx[j]][idx[s]] += G[idx[j]][idx[t]];
        }
        idx[t] = idx[-- n];
      }
      vis[idx[t]] = true; s = t; t = -1;
      for (int j = 1; j < n; ++ j) 
        if (!vis[idx[j]]) {
          dis[idx[j]] += G[idx[s]][idx[j]];
          if (t == -1 || dis[idx[t]] < dis[idx[j]]) t = j;
        }
    }
  }
  return ret;
}

