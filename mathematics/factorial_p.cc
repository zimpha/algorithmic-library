#include "basic.hpp"
#include <cstdio>
#include <cmath>
#include <vector>
#include <utility>

using poly_t = std::vector<int64>;

// n! = a * p ^ b, p <= 1e12
// v = sqrt(n)
// f(x) = \prod_{i=1}^{v} (x + i)
// n! = \prod_{i=0}^{v-1} f(i) \prod_{i=v^2+1}^{n} i
std::pair<int64, int64> fact_p(int64 n, int64 p) {

  auto shift_evaluation = [&mod = p] (const poly_t &P, int64 a) {
    // f(i) --> f(i + a), {i, 0, d}
    const int d = P.size() - 1;
  };

  int64 a = pow_mod(p - 1, n / p, p), b = n / p;
  for (int64 x = p; x <= n / p; x *= p) {
    b += n / (x * p);
  }
  if ((n %= p) == 0) return {a, b};
  const int64 v = static_cast<int64>(sqrt(n));
  for (int64 i = v * v + 1; i <= n; ++i) {
    a = mul_mod(a, i, p);
  }
  return {a, b};
}

// n! / p^{v_p(n!)} mod p^e, assume p^e < 2^63 - 1, pe < 10^6
// (n!)_p = \stirlingfirst{p}{1}^u f_{p,e}(u) \sum_{k=0}^{e-1} (up)^k \stirlingfirst{v+1}{k+1} \bmod p^e
// f_{p,e} = \prod_{i=0}^{x-1}(1 + \sum_{k=1}^{e-1}\frac{\stirlingfirst{p}{k+1}}{\stirlingfirst{p}{1}} (ip)^k)
uint64 fact_pe(uint64 n, uint64 p, uint64 e) {
  std::vector<uint64> pows(e + 1, 1);
  uint64 pe = 1, min_pe = std::min(p, e);
  for (uint64 i = 1; i <= e; ++i) {
    pows[i] = (pe *= p);
  }
  uint64 period = pe / p * 2, deg = e * 2 - 1;
  if (p == 2 && e >= 3) period >>= 1;

  // first kind stirling number: O(p * min(p, e))
  std::vector<uint64> s1(p * min_pe); s1[0] = 1;
  for (uint64 i = 1; i < p; ++i) {
    int o = i * min_pe;
    s1[o] = (uint128)s1[o - min_pe] * i % pe;
    for (uint64 j = 1; j < min_pe; ++j) {
      s1[o + j] = (s1[o + j - min_pe - 1] + (uint128)s1[o + j - min_pe] * i) % pe;
    }
  }

  // product of {up + 1, ..., up + v} mod p^e
  auto fact_range = [&] (uint64 u, uint64 v) {
    uint64 coef = (uint128)u % pe * p %pe, prod = 1, ret = 0;
    for (uint64 k = 0; k < min_pe; ++k) {
      ret = (ret + (uint128)prod * s1[v * min_pe + k]) % pe;
      prod = (uint128)prod * coef % pe;
    }
    return ret;
  };

  // f_{p,e}(0..2e-2): O(e * min(p, e) + e log(p))
  uint64 fac = fact_range(0, p - 1), ifac = mod_inv(fac, pe);
  std::vector<uint64> f_pe(deg, 1);
  for (uint64 i = 1; i < deg; ++i) {
    f_pe[i] = (uint128)f_pe[i - 1] * fact_range(i - 1, p - 1) % pe * ifac % pe;
  }

  // coprime factorials: O(e + e log(p))
  std::vector<uint64> cifac(deg, 1), cfac_vs(deg);
  uint64 prod = 1;
  for (uint64 i = 1; i < deg; ++i) {
    uint64 j = i, v = 0;
    for (; j % p == 0; j /= p, ++v);
    cfac_vs[i] = cfac_vs[i - 1] + v;
    cifac[i - 1] = j;
    prod = (uint128)prod * j % pe;
  }
  cifac[deg - 1] = mod_inv(prod, pe);
  for (int i = deg - 2; i >= 0; --i) {
    cifac[i] = (uint128)cifac[i + 1] * cifac[i] % pe;
  }

  // find the value of f_{p, e}(x): O(e log x)
  auto evaluate = [&](uint64 x) {
    if (x < (uint64)deg) return f_pe[x];
    std::vector<uint64> vs(deg), inv(deg);
    uint64 v = 0, prod = 1;
    for (uint64 i = 0; i < deg; ++i) {
      uint64 m = x - i;
      for (; m % p == 0; m /= p, ++vs[i]);
      v += vs[i];
      inv[i] = prod;
      prod = (uint128)prod * m % pe;
    }
    uint64 iprod = mod_inv(prod, pe);
    for (int i = deg - 1; i >= 0; --i) {
      inv[i] = (uint128)iprod * inv[i] % pe;
      iprod = (uint128)iprod * ((x - i) / pows[vs[i]]) % pe;
    }
    uint64 ret = 0;
    for (uint64 i = 0; i < deg; ++i) {
      uint64 j = deg - 1 - i, ex = v - vs[i] - cfac_vs[i] - cfac_vs[j];
      if (ex >= e) continue;
      uint64 add = (uint128)cifac[j] * cifac[i] % pe;
      if (j & 1) add = pe - add;
      add = (uint128)pows[ex] * prod % pe * inv[i] % pe * add % pe * f_pe[i] % pe;
      ret = (ret + add) % pe;
    }
    return ret;
  };

  // ((up+v)!)_p mod p^e: O(min(p, e))
  auto fact_p = [&](uint64 u, uint64 v) {
    return (uint128)fact_range(u, v) * evaluate(u) % pe;
  };
  
  uint64 ret = 1, ex = 0;
  while (n > 0) {
    uint64 q = n / p, v = n % p;
    uint64 u = q % period;
    ret = (uint128)ret * fact_p(u, v) % pe;
    ex += u, n = q;
  }
  for (ex %= period; ex; ex >>= 1) {
    if (ex & 1) ret = (uint128)ret * fac % pe;
    fac = (uint128)fac * fac % pe;
  }
  return ret;
}

int main() {
  uint64 p = 5, e = 3, n = 63;
  printf("%llu\n", fact_pe(n, p, e));
}
