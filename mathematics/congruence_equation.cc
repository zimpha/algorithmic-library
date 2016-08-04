#include "basic.hpp"

// ax = b (mod m)
std::vector<LL> congruence_equation(LL a, LL b, LL m) {
  std::vector<LL> ret;
  LL g = gcd(a, m), x;
  if (b % g != 0) return ret;
  a /= g, b /= g;
  x = mod_inv(a, m / g);
  for (int k = 0; k < g; ++k) {
    ret.push_back((x * b + m / g * k) % m);
  }
  return ret;
}
