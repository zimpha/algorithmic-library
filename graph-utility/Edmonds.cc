//多源最小树形图, edmonds算法, 邻接阵形式, 复杂度 O(n^3)                            
//返回最小生成树的长度, 构造失败返回负值
//传入图的大小n和邻接阵G[][], 不相邻点边权inf, 下标0~n-1
//可更改边权的类型, pre[]返回树的构造, 用父结点表示
//传入时pre[]数组清零, 用-1标出可能的源点
#include <cstring>

const int N = 1000 + 10, inf = 1e9;
int edmonds(int n, int G[][N], int pre[]) {
  static int vis[N][N], l[N], p[N];
  int m=n,cnt,ret=0;
  for (int i=0;i<n;++i) l[i]=i;
  do {
    memset(vis,0,sizeof(vis));
    memset(p,0xff,sizeof(p));
    cnt=m; for (int i=0;i<m;++i) vis[i][i]=1;
    for (int i=0;i<cnt;++i) if (l[i]==i&&pre[i]!=-1) {
      for (int j=0;j<m;++j) {
        if (l[j]==j&&i!=j&&G[j][i]<inf&&(p[i]==-1||G[j][i]<G[p[i]][i])) p[i]=j;
      }
      if ((pre[i]=p[i])==-1) return -1;
      if (vis[i][p[i]]) {
        for (int j=0;j<=m;++j) G[j][m]=G[m][j]=inf;
        for (int k=i;l[k]!=m;l[k]=m,k=p[k]) 
          for (int j=0;j<m;++j) if (l[j]==j) {
            if (G[j][k]-G[p[k]][k]<G[j][m]) G[j][m]=G[j][k]-G[p[k]][k];
            if (G[k][j]<G[m][j]) G[m][j]=G[k][j];
          }
        vis[m][m]=1; l[m]=m; m++;
      }
      for (int j=0;j<m;++j) if (vis[i][j]) {
        for (int k=p[i];k!=-1&&l[k]==k; k=p[k]) vis[k][j]=1;
      }
    }
  } while (cnt<m);
  for (int k;m-->n;pre[k]=pre[m]) {
    for (int i=0;i<m;++i) if (l[i]==m) {
      for (int j=0;j<m;++j)
        if (pre[j]==m&&G[i][j]==G[m][j]) pre[j]=i;
      if (G[pre[m]][m]==G[pre[m]][i]-G[pre[i]][i]) k=i;
    }
  }
  for (int i=0;i<n;++i) if (pre[i]!=-1) ret+=G[pre[i]][i];
  return ret;
}
