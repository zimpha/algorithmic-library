#include <vector>
#include <cmath>

using std::size_t;

// 单纯形解线性规划 By 与星独白/猛犸也钻地 @ 2014.09.06
// 给出m个这样的约束条件：sum(A[i]*X[i])<=B
// 求出X的解，在满足X[i]>=0的情况下，sum(C[i]*X[i])达到最大
static const double EPS = 1e-12;
class Simplex {
public:
  static const int SIZE = 100 + 10;   // 略大于约束条件的最大数量
  std::vector<double> A[SIZE],B;     // 约束条件：sum(A[k][i]*X[i])<=B[k]
  std::vector<double> C,X;       // 目标函数：max(sum(C[i]*X[i]))
  void init(const std::vector<double>& C){ // 初始化，传入目标函数C
    this->C=C;
    for(size_t i=0;i<=B.size();i++) A[i].clear();
    B.clear(); X.clear();
  }
  void constraint(const std::vector<double>& u, double v){ // 添加约束条件
    A[B.size()]=u;  // 约束条件的数组长度必须和目标函数的长度相同
    B.push_back(v);
  }
  bool gao(){ // 返回目标函数值，如果是nan则无解，是inf则解无穷大
    int m=B.size(),n=C.size();
    A[m].resize(n+1);
    for(int i=0;i<m;i++) A[i].push_back(B[i]);
    for(int i=0;i<n;i++) A[m][i]=-C[i];
    std::vector<int> idx(n+m);
    for(int i=0;i<n+m;i++) idx[i]=i;
    for(int k=0;k<m;k++) while(cmp(A[k][n],0)<0){
      int r=k,c;
      for(c=0;c<n;c++) if(cmp(A[k][c],0)<0) break;
      if(c==n) return false;
      for(int i=0;i<m;i++)
        if(cmp(A[i][n],0)>=0 && cmp(A[i][c], 0)>0
        && cmp(A[r][n]/A[r][c],A[i][n]/A[i][c])>0) r=i;
      pivot(idx,r,c);
    }
    for(int cnt=64;cnt--;){ //卡时用，时限充裕的话可以改大
      double tmp,low=1; int r=-1,c=-1;
      for(int j=0;j<n;++j) if(cmp(A[m][j],0)<0){
        int k=-1;
        for(int i=0;i<m;++i) if(cmp(A[i][j],0)>0)
          if(k==-1 || cmp(A[k][n]/A[k][j],A[i][n]/A[i][j])>0) k=i;
        if(k!=-1 && cmp(low,tmp=A[m][j]*A[k][n]/A[k][j])>0)
          low=tmp,r=k,c=j;
        if(r<0) c=j;
      }
      if(c<0) break;
      if(r<0) return true;
      pivot(idx,r,c);
    }
    X.resize(n); // 具体的解在数组X中
    for(int i=0;i<n+m;i++) if(idx[i]<n) X[idx[i]]=i<n?0:A[i-n][n];
    return true;
  }
private:
  double cmp(double a, double b){return (1+fabs(a))*EPS<fabs(a-b)?a-b:0;}
  void pivot(std::vector<int>& idx, int r, int c){
    int m=B.size()+1,n=A[0].size();
    double div=1/A[r][c];
    for(int i=0;i<m;i++) if(i!=r)
    for(int j=0;j<n;j++) if(j!=c)
      A[i][j]-=A[r][j]*A[i][c]*div;
    for(int i=0;i<m;i++) A[i][c]*=-div;
    for(int j=0;j<n;j++) A[r][j]*=+div;
    A[r][c]=div; std::swap(idx[c],idx[n-1+r]);
  }
};
