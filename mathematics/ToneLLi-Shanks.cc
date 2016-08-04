#include "basic.hpp"
/**
 * 求解方程: x^2 = n mod p
 * 无解返回-1, 否则返回其中一个解r, 另一个解是p - r
 * 用欧拉准则进行有解判定
 *
 * time complexity: O(log p)
 **/
LL ToneLLi_Shanks(LL n, LL p) {
  if (p == 2) return (n & 1) ? 1 : -1;
  if (pow_mod(n, p >> 1, p) != 1) return -1;
  if (p & 2) return pow_mod(n, p + 1 >> 2, p);
  int s = __builtin_ctzll(p ^ 1);
  LL q = p >> s, z = 2;
  for (; pow_mod(z, p >> 1, p) == 1; ++z);
  LL c = pow_mod(z, q, p);
  LL r = pow_mod(n, q + 1 >> 1, p);
  LL t = pow_mod(n, q, p), tmp;
  for (int m = s, i; t != 1;) {
    for (i = 0, tmp = t; tmp != 1; ++i) tmp = tmp * tmp % p;
    for (; i < --m;) c = c * c % p;
    r = r * c % p;
    c = c * c % p;
    t = t * c % p;
  }
  return r;
}
