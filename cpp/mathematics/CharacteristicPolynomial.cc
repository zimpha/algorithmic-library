#include "basic.hpp"
#include <vector>

using i64 = long long;

const int N = 500 + 10;

// find the characteristic polynomial of matrix `m`.
std::vector<i64> Hessenberg(int n, i64 m[N][N], i64 mod) {
  // reduce into upper Hessenberg form while preserving the characteristic polynomial
  for (int j = 1, i; j <= n - 2; ++j) {
    for (i = j + 1; i < n && !m[i][j - 1]; ++i);
    if (i == n) continue;
    if (m[j][j - 1] == 0) {
      for (int k = 0; k < n; ++k) std::swap(m[i][k], m[j][k]);
      for (int k = 0; k < n; ++k) std::swap(m[k][i], m[k][j]);
    }
    i64 inv = mod_inv(m[j][j - 1], mod);
    for (int k = j + 1; k < n; ++k) {
      i64 u = m[k][j - 1] * inv % mod;
      for (int i = 0; i < n; ++i) {
        m[k][i] = (m[k][i] - u * m[j][i] % mod + mod) % mod;
        m[i][j] = (m[i][j] + u * m[i][k]) % mod;
      }
    }
  }
  // find the characteristic polynomial
  std::vector<std::vector<i64>> p(n + 1); p[0] = {1};
  for (int k = 1; k <= n; ++k) {
    p[k].assign(p[k - 1].size() + 1, 0);
    for (int i = 0; i < (int)p[k - 1].size(); ++i) {
      p[k][i] += p[k - 1][i] * (mod - m[k - 1][k - 1]) % mod;
      p[k][i + 1] += p[k - 1][i];
    }
    for (auto &x: p[k]) x %= mod;
    i64 t = 1;
    for (int i = 1; i < k; ++i) {
      t = t * m[k - i][k - i - 1] % mod;
      i64 mul = (mod - t) * m[k - i - 1][k - 1] % mod;
      for (int j = 0; j < (int)p[k - i - 1].size(); ++j) {
        p[k][j] = (p[k][j] + mul * p[k - i - 1][j]) % mod;
      }
    }
  }
  return {p[n].begin() + 1, p[n].end()};
}