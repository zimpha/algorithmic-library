#pragma once
#include <cassert>
#include <algorithm>

using int64 = long long;
using uint64 = unsigned long long;
using int128 = __int128_t;
using uint128 = __uint128_t;
using float80 = long double;

// return a % b
inline uint64 mod128_64_small(uint128 a, uint64 b) {
  uint64 q, r;
  __asm__ (
    "divq\t%4"
    : "=a"(q), "=d"(r)
    : "0"(uint64(a)), "1"(uint64(a >> 64)), "rm"(b)
  );
  return r;
}

// mod should be not greater than 2^62 (about 4e18)
// return a * b % mod when mod is less than 2^31
inline uint64 mul_mod(uint64 a, uint64 b, uint64 mod) {
  assert(0 <= a && a < mod);
  assert(0 <= b && b < mod);
  if (mod < int(1e9)) return a * b % mod;
  uint64 k = (uint64)((long double)a * b / mod);
  uint64 res = a * b - k * mod;
  if ((int64)res < 0) res += mod;
  return res;
}

inline int64 add_mod(int64 x, int64 y, int64 mod) {
  return (x + y) % mod;
}

inline int64 sub_mod(int64 x, int64 y, int64 mod) {
  return (x - y + mod) % mod;
}

inline uint64 mul_add_mod(uint64 a, uint64 b, uint64 c, uint64 mod) {
  return mod128_64_small(uint128(a) * b + c, mod);
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

// return x, where ax = 1 (mod mod)
int64 mod_inv(int64 a, int64 mod) {
  if (gcd(a, mod) != 1) return -1;
  int64 b = mod, s = 1, t = 0;
  while (b) {
    int64 q = a / b;
    std::swap(a -= q * b, b);
    std::swap(s -= q * t, t);
  }
  return s < 0 ? s + mod : s;
}

uint64 crt2(uint64 r1, uint64 mod1, uint64 r2, uint64 mod2) {
  uint64 inv = mod_inv(mod1, mod2);
  return mul_mod(r2 + mod2 - r1,  inv, mod2) * mod1 + r1;
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
int64 euler_phi(int64 n) {
  int64 ret = n;
  for (int64 i = 2; i * i <= n; ++i) if (n % i == 0) {
    ret = ret / i * (i - 1);
    while (n % i == 0) n /= i;
  }
  if (n > 1) ret = ret / n * (n - 1);
  return ret;
}
