namespace NetFlow {
  const int MAXN=100000,MAXM=100000,inf=1e9;
  struct Edge {
    int v,c,f,nx;//c:capcity, f:flow
    Edge() {}
    Edge(int v,int c,int f,int nx):v(v),c(c),f(f),nx(nx) {}
  } E[MAXM];
  int G[MAXN],cur[MAXN],pre[MAXN],dis[MAXN],gap[MAXN],N,sz;
  void init(int _n) {
    N=_n,sz=0; memset(G,-1,sizeof(G[0])*N);
  }
  void link(int u,int v,int c) {
    E[sz]=Edge(v,c,0,G[u]); G[u]=sz++;
    E[sz]=Edge(u,0,0,G[v]); G[v]=sz++;
  }
  int ISAP(int S,int T) {//S -> T
    int maxflow=0,aug=inf,flag=false,u,v;
    for (int i=0;i<N;++i)cur[i]=G[i],gap[i]=dis[i]=0;
    for (gap[S]=N,u=pre[S]=S;dis[S]<N;flag=false) {
      for (int &it=cur[u];~it;it=E[it].nx) {
        if (E[it].c>E[it].f&&dis[u]==dis[v=E[it].v]+1) {
          if (aug>E[it].c-E[it].f) aug=E[it].c-E[it].f;
          pre[v]=u,u=v; flag=true;
          if (u==T) {
            for (maxflow+=aug;u!=S;) {
              E[cur[u=pre[u]]].f+=aug;
              E[cur[u]^1].f-=aug;
            }
            aug=inf;
          }
          break;
        }
      }
      if (flag) continue;
      int mx=N;
      for (int it=G[u];~it;it=E[it].nx) {
        if (E[it].c>E[it].f&&dis[E[it].v]<mx) {
          mx=dis[E[it].v]; cur[u]=it;
        }
      }
      if ((--gap[dis[u]])==0) break;
      ++gap[dis[u]=mx+1]; u=pre[u];
    }
    return maxflow;
  }
  bool bfs(int S,int T) {
    static int Q[MAXN]; memset(dis,-1,sizeof(dis[0])*N);
    dis[S]=0; Q[0]=S;
    for (int h=0,t=1,u,v,it;h<t;++h) {
      for (u=Q[h],it=G[u];~it;it=E[it].nx) {
        if (dis[v=E[it].v]==-1&&E[it].c>E[it].f) {
          dis[v]=dis[u]+1; Q[t++]=v;
        }
      }
    }
    return dis[T]!=-1;
  }
  int dfs(int u,int T,int low) {
    if (u==T) return low;
    int ret=0,tmp,v;
    for (int &it=cur[u];~it&&ret<low;it=E[it].nx) {
      if (dis[v=E[it].v]==dis[u]+1&&E[it].c>E[it].f) {
        if (tmp=dfs(v,T,min(low-ret,E[it].c-E[it].f))) {
          ret+=tmp; E[it].f+=tmp; E[it^1].f-=tmp;
        }
      }
    }
    if (!ret) dis[u]=-1; return ret;
  }
  int dinic(int S,int T) {
    int maxflow=0,tmp;
    while (bfs(S,T)) {
      memcpy(cur,G,sizeof(G[0])*N);
      while (tmp=dfs(S,T,inf)) maxflow+=tmp;
    }
    return maxflow;
  }
}
