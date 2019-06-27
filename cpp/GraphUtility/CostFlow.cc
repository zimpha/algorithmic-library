#include <bits/stdc++.h>

template<typename flow_t, typename cost_t>
struct MCMF {
  static const int N = 200, M = 100000;
  const flow_t inf = 1e9;
  struct node {
    int from, to, nxt;
    flow_t cap, flow;
    cost_t cost;
    node() {}
    node(int from, int to, int nxt, flow_t cap, cost_t cost):
      from(from), to(to), nxt(nxt), cap(cap), flow(0), cost(cost) {}
  } E[M];
  cost_t dis[N];
  int G[N], pre[N], vis[N], n, m;
  void init(int n) {
    this->n = n;
    this->m = 0;
    std::fill(G, G + n, -1);
  }
  void link(int u, int v, flow_t f, cost_t c) {
    E[m] = node(u, v, G[u], f, +c); G[u] = m++;
    E[m] = node(v, u, G[v], 0, -c); G[v] = m++;
  }
  bool extand(int S, int T) {
    std::fill(vis, vis + n, 0);
    std::fill(dis, dis + n, inf);
    std::queue<int> queue;
    dis[S] = 0;
    queue.push(S);
    for (; !queue.empty(); queue.pop()) {
      int u = queue.front();
      vis[u] = false;
      for (int it = G[u]; ~it; it = E[it].nxt) {
        int v = E[it].to;
        if (E[it].cap > E[it].flow && dis[v] > dis[u] + E[it].cost) {
          dis[v] = dis[u] + E[it].cost;
          pre[v] = it;
          if (!vis[v]) queue.push(v);
          vis[v] = true;
        }
      }
    }
    return dis[T] < inf; // 改成dis[T] <= 0 求可行流
  }
  std::pair<flow_t, cost_t> run(int S, int T) {
    flow_t max_flow = 0;
    cost_t min_cost = 0;
    while (extand(S, T)) {
      flow_t delta = inf;
      for (int u = T; u != S; u = E[pre[u]].from) {
        delta = std::min(delta, E[pre[u]].cap - E[pre[u]].flow);
      }
      min_cost += delta * dis[T];
      max_flow += delta;
      for (int u = T; u != S; u = E[pre[u]].from) {
        E[pre[u]].flow += delta;
        E[pre[u] ^ 1].flow -= delta;
      }
    }
    return {max_flow, min_cost};
  }
};

template<typename flow_t, typename cost_t>
struct MCMF_ZKW {
  static const int N = 200, M = 3000, inf = 1e9;
  struct node {
    int to, nxt;
    flow_t cap, flow;
    cost_t cost;
    node() {}
    node(int to, int nxt, flow_t cap, cost_t cost):
      to(to), nxt(nxt), cap(cap), flow(0), cost(cost) {}
  } E[M];
  int G[N], n, m;
  cost_t min_cost, len;
  flow_t max_flow;
  bool done[N];
  void init(int n) {
    this->n = n;
    this->m = 0;
    std::fill(G, G + n, -1);
  }
  void link(int u, int v, flow_t f, cost_t c) {
    E[m] = node(v, G[u], f, +c); G[u] = m++;
    E[m] = node(u, G[v], 0, -c); G[v] = m++;
  }
  flow_t aug(int now, int T, flow_t max_cap) {
    if (now == T) {
      max_flow += max_cap;
      min_cost += max_cap * len;
      return max_cap;
    }
    done[now] = true;
    flow_t upp = max_cap;
    for (int it = G[now]; ~it && upp; it = E[it].nxt) {
      if (E[it].cap > E[it].flow && !E[it].cost && !done[E[it].to]) {
        flow_t delta = aug(E[it].to, T, std::min(upp, E[it].cap - E[it].flow));
        E[it].flow += delta;
        E[it ^ 1].flow -= delta;
        upp -= delta;
      }
    }
    return max_cap - upp;
  }
  bool label(int S, int T) {//不能用于负费用
    cost_t delta = inf;
    for (int u = 0; u < n; ++u) if (done[u]) {
      for (int it = G[u]; ~it; it = E[it].nxt) {
        if (E[it].cap > E[it].flow && !done[E[it].to]) {
          delta = std::min(delta, E[it].cost);
        }
      }
    }
    if (delta == inf) return false;
    for (int u = 0; u < n; ++u) if (done[u]) {
      for (int it = G[u]; ~it; it = E[it].nxt) {
        E[it].cost -= delta;
        E[it ^ 1].cost += delta;
      }
    }
    len += delta;
    return true;
  }
  cost_t dis[N];
  bool label_primal_dual(int S, int T) {
    for (int i = 0; i < n; ++i) dis[i] = inf;
    std::fill(done, done + n, 0);
    dis[T] = 0;
    std::queue<int> queue;
    queue.push(T);
    for (; !queue.empty(); queue.pop()) {
      int u = queue.front();
      done[u] = false;
      for (int it = G[u]; ~it; it = E[it].nxt) {
        int v = E[it].to;
        cost_t cost = dis[u] - E[it].cost;
        if (E[it ^ 1].cap > E[it ^ 1].flow && cost < dis[v]) {
          dis[v] = cost;
          if (!done[v]) queue.push(v);
          done[v] = true;
        }
      }
    }
    for (int u = 0; u < n; ++u) {
      for (int it = G[u]; ~it; it = E[it].nxt) {
        E[it].cost += dis[E[it].to] - dis[u];
      }
    }
    len += dis[S];
    return dis[S] < inf;
  }
  std::pair<flow_t, cost_t> run_primal_dual(int S, int T) {
    max_flow = min_cost = len = 0;
    while (label_primal_dual(S, T)) {
      do {
        std::fill(done, done + n, 0);
      } while (aug(S, T, inf));
    }
    return {max_flow, min_cost};
  }
  std::pair<flow_t, cost_t> run(int S, int T) {
    max_flow = min_cost = len = 0;
    do {
      do {
        std::fill(done, done + n, 0);
      } while (aug(S, T, inf));
    } while (label_primal_dual(S, T));
    return {max_flow, min_cost};
  }
};
