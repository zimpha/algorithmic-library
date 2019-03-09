/**
 * 最近必经点(idom): 节点y的必经点集合dom(y)中dfn值最大的点x是距离y最近的必经点
 *                   称为y的最近必经点, 最近必经点是唯一的, 记x=idom(y)
 * 半必经点(semi): 在搜索树T上点y的祖先中，通过非树枝边可以到达y的深度最小的祖先x，
 *                 称为y的半必经点。半必经点也是唯一的，记x=semi(y)。
 *
 * 1. 设有向图G=(V,E)，(G,r)是一个Flow Graph，则称(G,r)的子图
 *    D=(V, { (idom(i),i) | i∈V,i≠r }, r)为(G,r)的一棵Dominator Tree。
 * 2. (G,r)的Dominator Tree是一棵有向有根树，从r出发可以到达G中的所有点，
 *    并且树上的每条边(u,v)都满足：u=idom(v)，即父节点是子节点的最近必经点。
 * 3. x=idom(y)，当且仅当有向边(x,y)是Dominator Tree中的一条树枝边。
 * 4. x dom y，当且仅当在Dominator Tree中存在一条从x到y的路径。
 * 5. x的必经点集合dom(x)就是Dominator Tree上x的所有祖先以及x自身。
 *
 * 应用:
 * 1. 求有向图的割顶: dominator tree上的非叶节点
 * 2. 有向图的必经边: 每条边上加一个点, 转化成必经点问题
 * 3. 求起点S到终点T的所有路径中最接近源的必经点: 求出必经点, 取最近的
 * 4. 求多少个(x,y)满足存在1->x的路径和1->y的路径只有1这个公共点:
 *    求出1为根的dominator tree, 算出不合法的, 总的减去即可.
 *    考虑1的每个儿子v, 同一颗子树的节点对都是非法的.
 *
 * succ是原图, pred是边反向后的图, dom是Dominator Tree
 * dom记录的是dfs序构成的树, G中节点u在dom树上的标号是dfn[u]
 * 相反dom中节点u在原图G中的标号是pt[u]
 * 调用build得到以s为根的Dominator Tree
 **/
#include <vector>

namespace DominatorTree {
  const static int N = 100000 + 10;
  typedef std::vector<int> VI;
  int dfn[N], pre[N], pt[N], sz;
  int semi[N], dsu[N], idom[N], best[N];
  int get(int x) {
    if (x == dsu[x]) return x;
    int y = get(dsu[x]);
    if (semi[best[x]] > semi[best[dsu[x]]]) best[x] = best[dsu[x]];
    return dsu[x] = y;
  }
  void dfs(int u, const VI succ[]) {
    dfn[u] = sz; pt[sz ++] = u;
    for (auto &v: succ[u]) if (!~dfn[v]) {
      dfs(v, succ); pre[dfn[v]] = dfn[u];
    }
  }
  void tarjan(const VI pred[], VI dom[]) {
    for (int j = sz - 1, u; u = pt[j], j > 0; -- j) {
      for (auto &tv: pred[u]) if (~dfn[tv]) {
        int v = dfn[tv]; get(v);
        if (semi[best[v]] < semi[j]) semi[j] = semi[best[v]];
      }
      dom[semi[j]].push_back(j);
      int x = dsu[j] = pre[j];
      for (auto &z: dom[x]) {
        get(z);
        if (semi[best[z]] < x) idom[z] = best[z];
        else idom[z] = x;
      }
      dom[x].clear();
    }
    for (int i = 1; i < sz; ++ i) {
      if (semi[i] != idom[i]) idom[i] = idom[idom[i]];
      dom[idom[i]].push_back(i);
    }
  }
  void build(int n, int s, const VI succ[], const VI pred[], VI dom[]) {
    for (int i = 0; i < n; ++ i) {
      dfn[i] = -1; dom[i].clear();
      dsu[i] = best[i] = semi[i] = i;
    }
    sz = 0; dfs(s, succ); tarjan(pred, dom);
  }
}
