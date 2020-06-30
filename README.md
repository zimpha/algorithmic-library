# Algorithmic Library: collections of some commonly used algorithms.

Most of the codes are tested against random tests in local. See [README](tests/README.md) in tests directory for more details.

Currently, only C++ version and link-cut-tree in rust version are available. In some day, Java, Python, Dlang version will be added.

## String Algorithms

+ [x] KMP
+ [x] Z algorithm
+ [x] Lexicographically minimal string rotation
+ [x] Manacher's Algorithm
+ [x] Rolling hash
+ [x] EERTREE (also Palindromic Tree)
+ [x] Suffix Array
  + [x] Prefix Doubling
  + [x] SA-IS
+ [x] Suffix Tree
+ [x] Suffix Automata
+ [x] Subsequence Automata
+ [x] Aho-Corasick Automata
+ [x] Palindromic Factorization
+ [x] Lyndon Word Factorization
+ [ ] Square Factorization
+ [x] Lempel-Ziv Factorization
+ [ ] Square, Cube, Runs, 
+ [x] Cyclic Longest Common Subsequence (CLCS)
+ [x] Bit-String Longest Common Subsequence LCS Algorithm
+ [ ] Symmetric Compact Directed Acyclic Word Graphs (SCDAWG)

## Graph Algorithms

+ [x] [Topological Ordering](cpp/GraphUtility/TopoSort.cc)
+ [x] Shortest Path
  + [x] [Floyd/无向图最小环](cpp/GraphUtility/Floyd.cc)
  + [x] [Dijkstra](cpp/GraphUtility/shortest-path.cc)
  + [x] [SPFA](cpp/GraphUtility/shortest-path.cc)
+ [ ] Spanning Tree
  + [ ] Prim
  + [ ] Kruskal
  + [ ] Borůvka
  + [ ] Minimum Degree-constrained Spanning Tree
  + [x] [最小树形图](cpp/GraphUtility/Edmonds.cc)
  + [ ] 次小生成树
  + [ ] Matrix Tree Theorem
+ [x] [最大流](cpp/GraphUtility/NetworkFlow.cc)
+ [x] [费用流](cpp/GraphUtility/CostFlow.cc)
+ [x] [Dominator Tree](cpp/GraphUtility/DominatorTree.cc)
+ [x] Bipartite Matching 
  + [x] [Hungarian Algorithm](cpp/GraphUtility/Hungarian.cc)
  + [x] [Hopcroft-Karp Algorithm](cpp/GraphUtility/Hopcroft.cc)
  + [x] [Kuhn-Munkres Algorithm](cpp/GraphUtility/KuhnMunkres.cc)
+ [ ] General Graph Matching
  + [x] [Maximum Cardinality Matching](cpp/GraphUtility/Blossom.cc)
  + [ ] Maximum Weight Matching
+ [x] [2-SAT](cpp/GraphUtility/TwoSat.cc)
+ [x] [Strongly Connected Components](cpp/GraphUtility/SCC.cc)
+ [x] [Articulation Points/Bridges](gcpp/GraphUtility/ArticulationPoints.cc)
+ [x] [Chordal Graph](cpp/GraphUtility/ChordalGraph.cc)
+ [x] [Absolute Center of Network](cpp/GraphUtility/KarivHakimi.cc)
+ [ ] Pseudoforest
+ [ ] Cactus Graph
+ [ ] 欧拉回路
+ [ ] 虚树
+ [ ] 最近公共祖先
+ [ ] Prüfer序列
+ [ ] 最大团
+ [x] [Stoer Wagner algorithm](cpp/GraphUtility/StoerWagner.cc)
+ [ ] Gomory-Hu tree
+ [ ] 树hash
+ [ ] 树上最长路
+ [ ] Tutte matrix

## Data Structures

+ [ ] easy segment tree
+ [x] [树状数组](data-structure/FenwickTree.cc)
+ [x] [ST表](data-structure/SparseTable.cc)
+ [x] [并查集](data-structure/Disjoint-Set.cc)
+ [ ] [莫队算法](data-structure/Sqrt-Decomposition.cc)
+ [x] [动态凸壳](data-structure/DynamicConvexHull.cc)
+ [ ] 树链剖分
+ [x] [树分治](data-structure/Centroid-Decomposition.cc)
+ [ ] 二叉堆
+ [ ] 左偏树
+ [ ] Splay
+ [ ] Treap
+ [ ] 替罪羊树
+ [ ] link-cut-tree
+ [ ] Top Tree
+ [ ] Euler Tour Tree
+ [ ] k-d Tree

## Computational Geometry

+ [ ] 基本类型 - 点, 线
+ [ ] 多边形
+ [ ] 半平面交
+ [ ] 圆
+ [ ] 三维几何
+ [ ] 球面几何
+ [ ] 平面最近点对
+ [ ] 曼哈顿距离生成树
+ [ ] 最大空凸包
+ [ ] 平面图求域

## Mathematics

+ [x] 基本数论算法
+ [x] 逆元相关
+ [x] 素数相关
+ [x] 线性筛相关
+ [x] 快速素数计数
+ [x] 原根
+ [x] 平方剩余
+ [ ] 立方剩余
+ [ ] N次剩余
+ [x] 离散对数
+ [x] 佩尔方程/二元二次不定方程
+ [x] 同余方程
+ [x] 同余方程组
+ [ ] 定积分计算
  + [ ] Romberg
  + [ ] 变步长Simpson
  + [ ] 自适应Simpson
+ [ ] 线性基
+ [ ] 康托展开
+ [x] 行列式求模
+ [ ] 快速傅里叶变换
+ [ ] 快速数论变换
+ [x] Fibonacci数
+ [x] 高斯消元
+ [x] 勾股数对
+ [x] 单纯型
+ [x] 组合数取模
+ [x] 多项式插值
+ [ ] 连分数

## Other Useful Tools

+ [x] 快速读入
+ [x] 分数类
+ [ ] 矩阵类
+ [x] ModInt
+ [ ] 大整数类
+ [ ] 日期相关
+ [ ] Fast Walsh–Hadamard transform
+ [ ] 完全单调性
+ [x] Frobenius Equations
+ [x] 最长上升子序列
+ [ ] Stern–Brocot tree
+ [ ] 直线下格点
