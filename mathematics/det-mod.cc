#include "basic.hpp"

/**
 * 高斯消元法行列式求模.
 * n为行列式大小, 计算|mat| % m
 *
 * time complexity: O(n^3 log n)
 **/
const int MAXN = 200;
int det_mod(int n, int m, int mat[][MAXN]) {
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      mat[i][j] %= m;
  LL ret = 1;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j)
      for (; mat[j][i]; ret = -ret) {
        LL t = mat[i][i] / mat[j][i];
        for (int k = i; k < n; ++k) {
          mat[i][k] = (mat[i][k] - mat[j][k] * t) % m;
          std::swap(mat[j][k], mat[i][k]);
        }
      }
    if (mat[i][i] == 0) return 0;
    ret = ret * mat[i][i] % m;
  }
  if (ret < 0) ret += m;
  return (int)ret;
}
