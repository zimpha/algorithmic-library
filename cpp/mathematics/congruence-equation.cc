#include "basic.hpp"
#include <vector>

// ax = b (mod m)
std::vector<int64> congruence_equation(int64 a, int64 b, int64 m) {
  std::vector<int64> ret;
  int64 g = gcd(a, m), x;
  if (b % g != 0) return ret;
  a /= g, b /= g;
  x = mod_inv(a, m / g);
  for (int k = 0; k < g; ++k) {
    ret.push_back((x * b + m / g * k) % m);
  }
  return ret;
}
