#pragma once
#include <cassert>

using int64 = long long;

// mod should be not greater than 4e18
// return a * b % mod when mod is less than 2^31
inline int64 mul_mod(int64 a, int64 b, int64 mod) {
  assert(0 <= a && a < mod);
  assert(0 <= b && b < mod);
  if (mod < int(1e9)) return a * b % mod;
  int64 k = (int64)((long double)a * b / mod);
  int64 res = a * b - k * mod;
  res %= mod;
  if (res < 0) res += mod;
  return res;
}

inline int64 add_mod(int64 x, int64 y, int64 mod) {
  return (x + y) % mod;
}

inline int64 sub_mod(int64 x, int64 y, int64 mod) {
  return (x - y + mod) % mod;
}

int64 pow_mod(int64 a, int64 n, int64 m) {
  int64 res = 1;
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
void exgcd(int64 a, int64 b, int64 &g, int64 &x, int64 &y) {
  if (!b) x = 1, y = 0, g = a;
  else {
    exgcd(b, a % b, g, y, x);
    y -= x * (a / b);
  }
}

// ax = 1 (mod m), gcd(a, m) = 1
int64 mod_inv(int64 a, int64 m) {
  int64 d, x, y;
  exgcd(a, m, d, x, y);
  return d == 1 ? (x % m + m) % m : -1;
}

//ax + by = c,x >= 0, x is minimum
//xx = x + t * b, yy = y - t * a
bool linear_equation(int64 a, int64 b, int64 c, int64 &x, int64 &y) {
  int64 g;
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
