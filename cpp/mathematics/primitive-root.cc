#include "basic.hpp"
#include <vector>

using std::size_t;

bool test(int64 g, int64 n, int64 phi, std::vector<int64> &u) {
  for (size_t i = 0; i < u.size(); ++i) {
    if (pow_mod(g, phi / u[i], n) == 1) return false;
  }
  return true;
}
int64 primitive_root(int64 n) {
  int64 phi = euler_phi(n), m = phi;
  std::vector<int64> u;
  for (int64 i = 2; i * i <= m; ++i) if (m % i == 0) {
    u.push_back(i);
    while (m % i == 0) m /= i;
  }
  if (m > 1) u.push_back(m);
  for (int g = 1; ; ++g) {
    if (test(g, n, phi, u)) return g;
  }
  return -1;
}
