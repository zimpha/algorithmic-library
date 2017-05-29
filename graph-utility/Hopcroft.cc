#include <vector>
#include <algorithm>

class Hopcroft {
public:
  static const int N = 100005; // 最大的单侧点个数
  int cnt,pos[N],neg[N]; // pos[]为左侧点所匹配到的右侧点编号
  // neg[]反之，没有匹配到对应的点则为-1
  // 传入左侧点个数n和左侧点至右侧点的边表e[]，返回匹配点对的数量cnt
  int gao(int n, const std::vector<int> e[]){ // 复杂度O(sqrt(n)*m)
    std::fill(pos,pos+n,-1);
    std::fill(neg,neg+n,-1);
    for(int x=cnt=0,y;x<n;x++){
      for(size_t i=0;i<e[x].size();i++){
        if(~neg[y=e[x][i]]) continue;
        pos[neg[y]=x]=y;
        cnt++; break;
      }
    }
    while(true){
      int push=0,pop=0,ok=0;
      std::fill(lx,lx+n,-1);
      std::fill(ly,ly+n,-1);
      for(int x=0;x<n;x++) if(pos[x]<0) lx[q[push++]=x]=0;
      while(push!=pop){
        int x=q[pop++],y;
        for(size_t i=0;i<e[x].size();i++){
          if(~ly[y=e[x][i]]) continue;
          ly[y]=1+lx[x];
          if(~neg[y] && ~lx[neg[y]]) continue;
          if(~neg[y]) lx[q[push++]=neg[y]]=1+ly[y];
          else ok=1;
        }
      }
      if(!ok) return cnt;
      for(int x=0;x<n;x++) if(pos[x]<0 && aug(x,e)) cnt++;
    }
  }
private:
  int lx[N],ly[N],q[N];
  bool aug(int x, const std::vector<int> e[]){
    int c=lx[x]+1,y=lx[x]=-1;
    for(size_t i=0;i<e[x].size();i++) if(ly[y=e[x][i]]==c){
      ly[y]=-1;
      if(~neg[y] && !aug(neg[y],e)) continue;
      pos[neg[y]=x]=y;
      return true;
    }
    return false;
  }
};
