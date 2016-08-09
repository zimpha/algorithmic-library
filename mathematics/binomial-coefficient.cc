#include "basic.hpp"

namespace BinomialCoefficient {
  std::vector<int> fac, inv;
  void lucas_init(int m) {
    fac.assign(m, 1);
    inv.assign(m, 1);
    for (int i = 2; i < m; ++i) {
      fac[i] = 1ll * i * fac[i - 1] % m;
      inv[i] = 1ll * (m - m / i) * inv[m % i] % m;
    }
  }
  inline int lucas_binom(int n, int m, int p) {
    return 1ll * fac[n] * inv[fac[m]] * inv[fac[n - m]] % p;
  }
  int lucas(int n, int m, int p) {
    if (!n && !m) return 1;
    int a = n % p, b = m % p;
    if (a < b) return 0;
    return 1ll * lucas(n / p, m / p, p) * lucas_binom(a, b, p) % p;
  }
}
