#include <cstring>
#include <algorithm>

namespace NetFlow {
  using flow_t = int;
  const int N = 1e5 + 10, M = 1e5 + 10;
  const flow_t inf = 1e9;
  struct edge_t {
    int to, nx;
    flow_t cap, flow;
    edge_t() {}
    edge_t(int to, int nx, flow_t cap): 
      to(to), nx(nx), cap(cap), flow(0) {}
  } edges[M];
  int G[N], cur[N], pre[N], gap[N], n, sz;
  flow_t dis[N];
  void init(int _n) {
    n = _n, sz = 0;
    memset(G, -1, sizeof(*G) * n);
  }
  void link(int u, int v, flow_t c) {
    edges[sz] = edge_t(v, G[u], c); G[u] = sz++;
    edges[sz] = edge_t(u, G[v], 0); G[v] = sz++;
  }
  flow_t ISAP(int S, int T) {//S -> T
    flow_t maxflow = 0, aug = inf;
    memcpy(cur, G, sizeof(*G) * n);
    memset(gap, 0, sizeof(*gap) * n);
    memset(dis, 0, sizeof(*dis) * n);
    gap[S] = n, pre[S] = S;
    for (int u = S, flag = 0; dis[S] < n; flag = 0) {
      for (int &it = cur[u]; ~it; it = edges[it].nx) {
        int v = edges[it].to;
        if (edges[it].cap > edges[it].flow && dis[u] == dis[v] + 1) {
          aug = std::min(aug, edges[it].cap - edges[it].flow);
          pre[v] = u, u = v, flag = true;
          if (u == T) {
            for (maxflow += aug; u != S; ) {
              u = pre[u];
              edges[cur[u]].flow += aug;
              edges[cur[u] ^ 1].flow -= aug;
            }
            aug = inf;
          }
          break;
        }
      }
      if (flag) continue;
      int mx = n;
      for (int it = G[u]; ~it; it = edges[it].nx) {
        if (edges[it].cap > edges[it].flow && dis[edges[it].to] < mx) {
          mx = dis[edges[it].to];
          cur[u] = it;
        }
      }
      if (--gap[dis[u]] == 0) break;
      ++gap[dis[u] = mx + 1];
      u = pre[u];
    }
    return maxflow;
  }
  bool bfs(int S, int T) {
    static int Q[N];
    memset(dis, -1, sizeof(*dis) * n);
    dis[S] = 0, Q[0] = S;
    for (int h = 0, t = 1; h < t; ++h) {
      for (int u = Q[h], it = G[u]; ~it; it = edges[it].nx) {
        int v = edges[it].to;
        if (dis[v] == -1 && edges[it].cap > edges[it].flow) {
          dis[v] = dis[u] + 1;
          Q[t++] = v;
        }
      }
    }
    return dis[T] != -1;
  }
  flow_t dfs(int u, int T, flow_t low) {
    if (u == T) return low;
    flow_t ret = 0;
    for (int &it = cur[u]; ~it && ret < low; it = edges[it].nx) {
      int v = edges[it].to;
      if (dis[v] == dis[u] + 1 && edges[it].cap > edges[it].flow) {
        flow_t tmp = dfs(v, T, std::min(low - ret, edges[it].cap - edges[it].flow));
        if (tmp > 0) {
          ret += tmp;
          edges[it].flow += tmp;
          edges[it ^ 1].flow -= tmp;
        }
      }
    }
    if (!ret) dis[u] = -1;
    return ret;
  }
  flow_t dinic(int S, int T) {
    flow_t maxflow = 0, tmp;
    while (bfs(S, T)) {
      memcpy(cur, G, sizeof(*G) * n);
      while (tmp = dfs(S, T, inf)) {
        maxflow += tmp;
      }
    }
    return maxflow;
  }
}
