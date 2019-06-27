#include "basic.hpp"
#include <algorithm>

/**
 * 高斯消元法行列式求模.
 * n为行列式大小, 计算|mat| % m
 *
 * time complexity: O(n^3 log n)
 **/
const int N = 200;
int det_mod(int n, int mod, int mat[][N]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      mat[i][j] %= mod;
    }
  }
  int64 ret = 1;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j)
      for (; mat[j][i]; ret = -ret) {
        int64 t = mat[i][i] / mat[j][i];
        for (int k = i; k < n; ++k) {
          mat[i][k] = (mat[i][k] - mat[j][k] * t) % mod;
          std::swap(mat[j][k], mat[i][k]);
        }
      }
    if (mat[i][i] == 0) return 0;
    ret = ret * mat[i][i] % mod;
  }
  if (ret < 0) ret += mod;
  return (int)ret;
}
