#include "basic.hpp"
#include <functional>

// f(0) = 0, f(1) = 1, f(n) = f(n - 1) + f(n - 2)
std::pair<int64, int64> fib(int64 n, int64 mod) {
  if (n == 0) return {0, 1};
  int64 x, y;
  if (n & 1) {
    std::tie(y, x) = fib(n - 1, mod);
    return {x, (y + x) % mod};
  } else {
    std::tie(x, y) = fib(n >> 1, mod);
    return {(x * y + x * (y - x + mod)) % mod, (x * x + y * y) % mod};
  }
}
