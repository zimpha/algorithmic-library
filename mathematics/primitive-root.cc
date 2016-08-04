#include "basic.hpp"

bool test(LL g, LL n, LL phi, std::vector<LL> &u) {
  for (size_t i = 0; i < u.size(); ++i) {
    if (pow_mod(g, phi / u[i], n) == 1) return false;
  }
  return true;
}
LL primitive_root(LL n) {
  LL phi = euler_phi(n), m = n;
  std::vector<LL> u;
  for (LL i = 2; i * i <= m; ++i) if (n % i == 0) {
    u.push_back(i);
    while (n % i == 0) n /= i;
  }
  if (n > 1) u.push_back(n);
  for (int g = 1; ; ++g) {
    if (test(g, m, phi, u)) return g;
  }
  return -1;
}
