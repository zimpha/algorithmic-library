#include <bits/stdc++.h>
typedef long long LL;

LL mul_mod(LL a, LL n, LL m) {
  LL res = 0;
  for (a %= m; n; n >>= 1) {
    if (n & 1) res = (res + a) % m;
    a = (a + a) % m;
  }
  return res;
}

inline LL add_mod(LL x, LL y) {
  return (x % y + y) % y;
}

LL pow_mod(LL a, LL n, LL m) {
  LL res = 1;
  for (a %= m; n; n >>= 1) {
    if (n & 1) res = res * a % m;
    a = a * a % m;
  }
  return res;
}

// 适合m超过int的时候
// 条件允许的话使用__int128会比较快
LL pow_mod_LL(LL a, LL n, LL m) {
  LL res = 1;
  for (a %= m; n; n >>= 1) {
    if (n & 1) res = mul_mod(res, a, m);
    a = mul_mod(a, a, m);
  }
  return res;
}

template<typename T>
T gcd(T a, T b) {
  return !b ? a : gcd(b, a % b);
}

// ax + by = gcd(a, b), |x| + |y| is minimum
void exgcd(LL a, LL b, LL &g, LL &x, LL &y) {
  if (!b) x = 1, y = 0, g = a;
  else {
    exgcd(b, a % b, g, y, x);
    y -= x * (a / b);
  }
}

// ax = 1 (mod m), gcd(a, m) = 1
LL mod_inv(LL a, LL m) {
  LL d, x, y;
  exgcd(a, m, d, x, y);
  return d == 1 ? (x % + m) % m : -1;
}

//ax + by = c,x >= 0, x is minimum
//xx = x + t * b, yy = y - t * a
bool linear_equation(LL a, LL b, LL c, LL &x, LL &y) {
  LL g;
  exgcd(a,b,g,x,y);
  if (c % g) return false;
  b /= g, a /= g, c /= g;
  x = (x % b * (c % b) % b + b) % b;
  y = (c - a * x) / b;
  return true;
}

// 求n的欧拉函数值，简易版
int euler_phi(int n) {
  int ret = n;
  for (int i = 2; i * i <= n; ++i) if (n % i == 0) {
    ret = ret / i * (i - 1);
    while (n % i == 0) n /= i;
  }
  if (n > 1) ret = ret / n * (n - 1);
  return ret;
}
