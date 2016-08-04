#include "basic.hpp"

// 求离散对数，即 a^x % m = r的最小非负整数解x，返回-1说明无解
LL log(LL a, LL r, LL m){
  if (r >= m) return -1;
  LL i, g, x, c = 0, at = int(2 + sqrt(m));
  for (i = 0, x = 1 % m; i < 50; ++i) {
    if (x == r) return i;
    x = LL(x) * a % m;
  }
  for (g = x = 1; gcd(LL(x) * a % m, m) != g; ++c) {
    x = x * a % m;
    g = gcd(x, m);
  }
  if (r % g) return -1;
  if (x == r) return c;
  std::unordered_map<LL, LL> u;
  g = euler_phi(m / g), u[x] = 0;
  g = pow_mod(a, g - at % g, m);
  for (i = 1; i < at; ++i){ // Baby Step
    x = x * a % m;
    u.insert(std::make_pair(x, i));
    if (x == r) return c + i;
  }
  for (i = 1; i < at; ++i) { // Giant Step
    r = r * g % m;
    auto t = u.find(r);
    if (t != u.end()) return c + i * at + t->second;
  }
  return -1;
}
