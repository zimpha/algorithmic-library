#include "basic.hpp"
#include <cmath>
#include <unordered_map>

// 求离散对数，即 a^x % m = r的最小非负整数解x，返回-1说明无解
int64 log(int64 a, int64 r, int64 m){
  if (r >= m) return -1;
  int64 i, g, x, c = 0, at = int(2 + sqrt(m));
  for (i = 0, x = 1 % m; i < 50; ++i) {
    if (x == r) return i;
    x = int64(x) * a % m;
  }
  for (g = x = 1; gcd(int64(x) * a % m, m) != g; ++c) {
    x = x * a % m;
    g = gcd(x, m);
  }
  if (r % g) return -1;
  if (x == r) return c;
  std::unordered_map<int64, int64> u;
  g = euler_phi(m / g), u[x] = 0;
  g = pow_mod(a, g - at % g, m);
  for (i = 1; i < at; ++i){ // Baby Step
    x = x * a % m;
    u.emplace(x, i);
    if (x == r) return c + i;
  }
  for (i = 1; i < at; ++i) { // Giant Step
    r = r * g % m;
    auto t = u.find(r);
    if (t != u.end()) return c + i * at + t->second;
  }
  return -1;
}
