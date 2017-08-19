#include "basic.hpp"

void fib(int64 n, int64 &x, int64 &y) {// store in x, n-th
  if (n == 1) {
    x = y = 1;
    return;
  } else if (n & 1) {
    fib(n - 1, y, x);
    y += x;
  } else {
    int64 a, b;
    fib(n >> 1, a, b);
    y = a * a + b * b;
    x = a * b + a * (b - a);
  }
}
