#include "basic.hpp"

/**
 * 先调用sieve筛素数, 然后调用get_pi计算答案
 * 原理参考https://en.wikipedia.org/wiki/Prime-counting_function
 **/

inline LL sqr(LL x) {return x * x;}
inline LL cub(LL x) {return x * x * x;}

const int SZ = 100000000, MN = 10000;
int pi[SZ], pl[SZ], m;
void sieve() {
  m = 0; for (int i = 2; i < SZ; ++ i) pi[i] = 1;
  for (int i = 2; i < SZ; ++i) {
    if (pi[i]) pl[m++] = i;
    for (int j = 0; j < m && pl[j] * i < SZ; ++j) {
      pi[pl[j] * i] = 0;
      if (i % pl[j] == 0) break;
    }
  }
  for (int i = 2; i < SZ; ++ i) pi[i] += pi[i - 1];
}
std::map<LL, LL> cache;
LL phi(LL x, LL a) {
  if (a == 1 || !x) return (x + 1) / 2;
  LL &r = cache[(x << 10) + a]; if (r) return r;
  return r = phi(x, a - 1) - phi(x / pl[a - 1], a - 1);
}
LL get_pi(LL n) {
  if (n < SZ) return pi[n];
  LL a = get_pi(pow(n, .25));
  LL b = get_pi(sqrt(n));
  LL c = get_pi(pow(n, 1./3));
  LL r = phi(n, a) + (b + a - 2) * (b - a + 1) / 2;
  for (int i = a + 1; i <= b; ++i) {
    LL w = n / pl[i - 1];
    r -= get_pi(w);
    if (i <= c) {
      int upp = get_pi(sqrt(w));
      for (int j = i; j <= upp; ++j) r += j - 1 - get_pi(w / pl[j - 1]);
    }
  }
  return r;
}
