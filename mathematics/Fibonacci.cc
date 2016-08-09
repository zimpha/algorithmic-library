void fib(LL n, LL &x, LL &y) {// store in x, n-th
  if (n == 1) {
    x = y = 1;
    return;
  } else if (n & 1) {
    fib(n - 1, y, x);
    y += x;
  } else {
    LL a, b;
    fib(n >> 1, a, b, M);
    y = a * a + b * b;
    x = a * b + a * (b - a);
  }
}
