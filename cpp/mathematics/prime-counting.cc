#include "basic.hpp"
#include <cmath>
#include <vector>
#include <map>

/**
 * 先调用sieve筛素数, 然后调用get_pi计算答案
 * 原理参考https://en.wikipedia.org/wiki/Prime-counting_function
 **/

inline int64 sqr(int64 x) {return x * x;}
inline int64 cub(int64 x) {return x * x * x;}

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
std::map<int64, int64> cache;
int64 phi(int64 x, int64 a) {
  if (a == 1 || !x) return (x + 1) / 2;
  int64 &r = cache[(x << 10) + a]; if (r) return r;
  return r = phi(x, a - 1) - phi(x / pl[a - 1], a - 1);
}
int64 get_pi(int64 n) {
  if (n < SZ) return pi[n];
  int64 a = get_pi(pow(n, .25));
  int64 b = get_pi(sqrt(n));
  int64 c = get_pi(pow(n, 1./3));
  int64 r = phi(n, a) + (b + a - 2) * (b - a + 1) / 2;
  for (int i = a + 1; i <= b; ++i) {
    int64 w = n / pl[i - 1];
    r -= get_pi(w);
    if (i <= c) {
      int upp = get_pi(sqrt(w));
      for (int j = i; j <= upp; ++j) r += j - 1 - get_pi(w / pl[j - 1]);
    }
  }
  return r;
}

// return the sum of p^k for all p <= m, where m is in form floor(n / i)
// for m <= sqrt{n}, stored in ssum[m]; for m > sqrt{n} stored in lsum[n / m]
// note: if you need all correct value of ssum and lsum, please remove `mark` 
// and make `delta` always be 1
std::pair<std::vector<int64>, std::vector<int64>> prime_count(int64 n, int64 k, int64 mod) {
  auto pow_sum = [](int64 n, int64 k, int64 mod) {
    if (k == 0) return n;
    if (k == 1) return n * (n + 1) / 2 % mod;
  };
  const int64 v = static_cast<int64>(sqrt(n));
  std::vector<int64> ssum(v + 1), lsum(v + 1);
  std::vector<bool> mark(v + 1);
  for (int i = 1; i <= v; ++i) {
    ssum[i] = pow_sum(i, k, mod) - 1;
    lsum[i] = pow_sum(n / i, k, mod) - 1;
  }
  for (int64 p = 2; p <= v; ++p) {
    if (ssum[p] == ssum[p - 1]) continue;
    int64 psum = ssum[p - 1], q = p * p, ed = std::min(v, n / q);
    int64 pk = pow_mod(p, k, mod);
    int delta = (p & 1) + 1;
    for (int i = 1; i <= ed; i += delta) if (!mark[i]) {
      int64 d = i * p;
      if (d <= v) {
        lsum[i] = sub_mod(lsum[i], sub_mod(lsum[d], psum, mod) * pk % mod, mod);
      } else {
        lsum[i] = sub_mod(lsum[i], sub_mod(ssum[n / d], psum, mod) * pk % mod, mod);
      }
    }
    for (int64 i = q; i <= ed; i += p * delta) mark[i] = true;
    for (int64 i = v; i >= q; --i) {
      ssum[i] = sub_mod(ssum[i], sub_mod(ssum[i / p], psum, mod) * pk % mod, mod);
    }
  }
  return {std::move(ssum), std::move(lsum)};
}
