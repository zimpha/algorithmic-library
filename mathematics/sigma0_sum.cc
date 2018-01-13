#include <cmath>
#include <cstdio>
#include <stack>
#include <algorithm>
#include <functional>

using uint32 = unsigned int;
using uint64 = unsigned long long;
using uint128 = __uint128_t;

// compute \sum_{i=1}^{n} sigma0(i) in O(n^{1/3+}) time.
uint128 sum_sigma0(uint64 n) {
  auto out = [n] (uint64 x, uint32 y) {
    return x * y > n;
  };
  auto cut = [n] (uint64 x, uint32 dx, uint32 dy) {
    return uint128(x) * x * dy >= uint128(n) * dx;
  };
  const uint64 sn = sqrtl(n);
  const uint64 cn = pow(n, 0.34);//cbrtl(n);
  uint64 x = n / sn;
  uint32 y = n / x + 1;
  uint128 ret = 0;
  std::stack<std::pair<uint32, uint32>> stk;
  stk.emplace(1, 0);
  stk.emplace(1, 1);
  while (true) {
    uint32 lx, ly;
    std::tie(lx, ly) = stk.top();
    stk.pop();
    while (out(x + lx, y - ly)) {
      ret += x * ly + uint64(ly + 1) * (lx - 1) / 2;
      x += lx, y -= ly;
    }
    if (y <= cn) break;
    uint32 rx = lx, ry = ly;
    while (true) {
      std::tie(lx, ly) = stk.top();
      if (out(x + lx, y - ly)) break;
      rx = lx, ry = ly;
      stk.pop();
    }
    while (true) {
      uint32 mx = lx + rx, my = ly + ry;
      if (out(x + mx, y - my)) {
        stk.emplace(lx = mx, ly = my);
      } else {
        if (cut(x + mx, lx, ly)) break;
        rx = mx, ry = my;
      }
    }
  }
  for (--y; y > 0; --y) ret += n / y;
  return ret * 2 - sn * sn;
}
