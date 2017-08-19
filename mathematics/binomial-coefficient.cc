#include "basic.hpp"
#include <vector>

namespace BinomialCoefficient {
  std::vector<int> fac, inv, ifac;
  void lucas_init(int n, int mod) {
    fac.assign(n + 1, 1);
    inv.assign(n + 1, 1);
    ifac.assign(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
      fac[i] = (int64)i * fac[i - 1] % mod;
      inv[i] = int64(mod - mod / i) * inv[mod % i] % mod;
      ifac[i] = (int64)ifac[i - 1] * inv[i] % mod;
    }
  }
  inline int lucas_binom(int n, int m, int p) {
    return (int64)fac[n] * inv[fac[m]] * inv[fac[n - m]] % p;
  }
  int lucas(int64 n, int64 m, int p) {
    if (!n && !m) return 1;
    int a = n % p, b = m % p;
    if (a < b) return 0;
    return (int64)lucas(n / p, m / p, p) * lucas_binom(a, b, p) % p;
  }
}
