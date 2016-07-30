/* 相关定义 //
1. 子图：原图点集的子集+原图边集的子集
2. 诱导子图：原图点集的子集+这些点在原图中所连出的边集
3. 团：原图的一个子图，且是完全图
4. 极大团：此团不是其他团的子集
5. 最大团：点数最多的团 -> 团数
6. 最小染色：用最少的颜色给点染色使相邻点颜色不同 -> 色数
7. 最大独立集：原图点集的子集，任意两点在原图中没有边相连
8. 最小团覆盖：用最少个数的团覆盖所有的点
   推论 -> 团数<=色数，最大独立集数<=最小团覆盖数
9. 弦：连接环中不相邻的两个点的边
10.弦图：图中任意长度大于3的环都至少有1个弦
   推论 -> 弦图的每一个诱导子图一定是弦图
           弦图的任一个诱导子图不同构于Cn(n>3)
11.单纯点：记N(v)为点v相邻点的集合，若N(v)+{v}是一个团，则v为单纯点
   引理 -> 任何一个弦图都至少有一个单纯点
           不是完全图的弦图至少有两个不相邻的单纯
// 重点内容 //
12.完美消除序列：点的序列v1,v2,..,vn，满足vi在{vi,vi+1,..,vn}中是单纯点
   定理 -> 一个无向图是弦图，当且仅当它有一个完美消除序列
   构造算法 -> 令cnt[i]为第i个点与多少个已标记的点相邻，初值全为零
               每次选择一个cnt[i]最大的结点并打上标记
               标记顺序的逆序则为完美消除序列
   判定算法 -> 对于每个vi，其出边为vi1,vi2,..,vik
               然后判断vi1与vi2,vi3,..,vik是否都相邻
               若存在不相邻的情况，则说明不是完美消除序列
13.弦图各类算法：
   最小染色算法 -> 按照完美消除序列，从后向前，依次染上可以染的最小颜色
   最大独立集算法 -> 按照完美消除序列，从前向后，能选则选
   最小团覆盖算法 -> 求出最大独立集，记为{p1,p2,..,pk}
                     则{N(p1)+{p1},N(p2)+{p2},..,N(pk)+{pk}}即为解
16.区间图：坐标轴上的一些区间看作点，任意两个交集非空的区间之间有边
   定理 -> 区间图一定是弦图 */

typedef std::pair<int, int> PII;

std::vector<int> MCS(int n, const std::vector<int> G[]) {
  std::vector<int> rank(n, -1), label(n, 0), res(n);
  std::priority_queue<PII> Q;
  for (int i = 0; i < n; ++ i) Q.push(PII(0, i));
  for (int i = n - 1; i >= 0; -- i) {
    while (true) {
      int u = Q.top().second; Q.pop();
      if (rank[u] == -1) {
        rank[u] = i;
        for (auto &v: G[u]) if (rank[v] == -1) {
          Q.push(PII(++label[v], v));
        }
        break;
      }
    }
  }
  for (int i = 0; i < n; ++ i) res[rank[i]] = i;
  return res;
}

bool check(int n, std::vector<int> G[], std::vector<int> ord) {
  std::vector<bool> mark(n, 0);
  std::vector<int> rk(n, 0);
  for (int i = 0; i < n; ++ i) rk[ord[i]] = i;
  for (int i = 0; i < n; ++ i) {
    std::vector<PII> tmp;
    std::set<int> S;
    for (auto &u: G[ord[i]]) if (!mark[u]) {
      tmp.push_back(PII(rk[u], u));
    }
    std::sort(tmp.begin(), tmp.end());
    if (!tmp.empty()) {
      int u = tmp[0].second;
      for (auto &v: G[u]) S.insert(v);
      for (auto &v: tmp) if (u != v.second) {
        if (!S.count(v.second)) return false;
      }
    }
    mark[ord[i]] = true;
  }
  return true;
}

bool is_chordal(int n, vector<int> G[]) {
  return check(n, G, MCS(n, G));
}
