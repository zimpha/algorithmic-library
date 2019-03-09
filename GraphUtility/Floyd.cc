#include <vector>

template<typename T>
struct Floyd {
  static const int N = 1000 + 10;
  std::vector<int> path;
  T dis[N][N], res;
  int src[N][N];
  // 传入结点个数n及权值矩阵graph[][]，返回最小环的长度res，方案记在path中
  // 对于矩阵graph[][]中不存在的边，权值设为1e9+7或0x7F7F7F7F之类的极大值
  // 若最后的返回值大于等于1e9，则不存在最小环
  T run(int n, const T graph[N][N]) {
    res = 1e9 + 7;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        src[i][j] = -1;
        dis[i][j] = graph[i][j];
      }
    }
    for (int k = 0; k < n; ++k) {
      for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
          T tmp = graph[k][i] + graph[j][k];
          if (dis[i][j] > res - tmp) continue;
          path.clear();
          get_path(i, j);
          path.push_back(k);
          path.push_back(i);
          res = tmp + dis[i][j];
        }
      }
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          T tmp = dis[i][k] + dis[k][j];
          if (tmp < dis[i][j]) {
            dis[i][j] = tmp;
            src[i][j] = k;
          }
        }
      }
    }
    return res;
  }
  void get_path(int i, int j) {
    int k = src[i][j];
    if (k != -1) {
      get_path(i, k);
      get_path(k, j);
    } else {
      path.push_back(j);
    }
  }
};
