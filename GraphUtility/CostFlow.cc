#include <bits/stdc++.h>
// 首先调用MCMF::init()设置点的个数，源点和汇点
// 调用MCMF::addedge()加边
// 调用MCMF::solve()计算答案，答案存在MinCost和MaxFlow里面
namespace MCMF {
  typedef int flow_t;
  typedef int cost_t;
  const int MAXN = 200, MAXM = 100000;
  const flow_t inf = 1e9;
  struct Edge {
    int from, to, nx;
    flow_t cap, flow;
    cost_t cost;
    Edge() {}
    Edge(int _from, int _to, int _nx, flow_t _cap, cost_t _cost):
      from(_from), to(_to), nx(_nx), cap(_cap), flow(0), cost(_cost) {}
  } E[MAXM];
  cost_t dis[MAXN];
  int G[MAXN], pre[MAXN], vis[MAXN];
  int sz, N; //S源点, T汇点
  void init(int n) {
    memset(G, -1, sizeof(G[0]) * n);
    sz = 0; N = n;
  }
  void addedge(int u, int v, flow_t f, cost_t c) {//u -> v
    E[sz] = Edge(u, v, G[u], f, +c); G[u] = sz++;
    E[sz] = Edge(v, u, G[v], 0, -c); G[v] = sz++;
  }
  bool extand(int S, int T) {
    std::queue<int> Q;
    memset(vis, 0, sizeof(vis[0]) * N);
    memset(pre, -1, sizeof(pre[0]) * N);
    for (int i = 0; i < N; ++i) dis[i] = inf;
    Q.push(S); vis[S] = 1; dis[S] = 0;
    while (!Q.empty()) {
      int u = Q.front();
      Q.pop(); vis[u]=0;
      for (int it = G[u]; ~it; it = E[it].nx) {
        int v = E[it].to;
        if (E[it].cap > E[it].flow && dis[v] > dis[u] + E[it].cost) {
          dis[v] = dis[u] + E[it].cost, pre[v] = it;
          if (!vis[v]) Q.push(v), vis[v] = true;
        }
      }
    }
    return dis[T] < inf; // 改成dis[T] <= 0 求可行流
  }
  std::pair<flow_t, cost_t> solve(int S, int T) {
    flow_t MaxFlow = 0;
    cost_t MinCost = 0;
    while (extand(S, T)) {
      flow_t aug = inf;
      for (int it = pre[T]; ~it; it = pre[E[it].from]) {
        aug = std::min(aug, E[it].cap - E[it].flow);
      }
      MinCost += aug * dis[T], MaxFlow += aug;
      for (int it = pre[T]; ~it; it = pre[E[it].from]) {
        E[it].flow += aug, E[it ^ 1].flow -= aug;
      }
    }
    return std::make_pair(MaxFlow, MinCost);
  }
}

namespace ZKW {
  const int MAXN = 50000 + 10, MAXM = 300000, inf = 1e9;
  typedef int VAL;
  struct Edge {
    int v,f;
    VAL c;
    int nx;
    Edge() {}
    Edge(int v,int f,VAL c,int nx):v(v),f(f),c(c),nx(nx) {}
  } E[MAXM];
  int G[MAXN], Q[MAXN];
  VAL dis[MAXN],mincost;
  bool vis[MAXN],mark[MAXN];
  int N,S,T,sz;
  void init(int _n, int _s, int _t) {
    N = _n; S = _s; T = _t; sz = 0;
    memset(G, -1, sizeof(G[0]) * N);
  }
  void link(int u, int v, int f, VAL c) {
    E[sz]=Edge(v,f,+c,G[u]); G[u]=sz++;
    E[sz]=Edge(u,0,-c,G[v]); G[v]=sz++;
  }
  bool spfa() {
    for (int i=0;i<N;++i) dis[i]=inf,vis[i]=0;
    dis[T]=0; Q[0]=T; vis[T]=1;
    for (int h=0,t=1;h!=t; ) {
      int u=Q[h++],v; vis[u]=false;
      if (h==MAXN) h=0;
      for (int it=G[u];~it;it=E[it].nx) {
        if (dis[u]-E[it].c<dis[v=E[it].v]&&E[it^1].f) {
          dis[v]=dis[u]-E[it].c;
          if (!vis[v]) {
            vis[v]=true; Q[t++]=v;
            if (t==MAXN) t=0;
          }
        }
      }
    }
    return dis[S]!=inf;
  }
  int dfs(int u, int low) {
    mark[u]=true; if (u==T) return low;
    int ret=0,tmp=0;
    for (int it=G[u],v;~it&&ret<low;it=E[it].nx) {
      if (E[it].f&&!mark[v=E[it].v]&&dis[u]-E[it].c==dis[v]) {
        tmp=dfs(v,std::min(E[it].f,low-ret));
        E[it].f-=tmp; E[it^1].f+=tmp;
        mincost+=E[it].c*tmp; ret+=tmp;
      }
    }
    return ret;
  }
  VAL solve() {
    int maxflow = 0; mincost = 0;
    while (spfa()) {
      mark[T] = true;
      while (mark[T]) {
        memset(mark, 0, sizeof(mark[0]) * N);
        maxflow += dfs(S, inf);
      }
    }
    return mincost;
  }
}
