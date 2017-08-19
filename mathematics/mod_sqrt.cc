#include "basic.hpp"
#include <algorithm>

/**
 * 求解方程: x^2 = n mod p
 * 无解返回-1, 否则返回其中一个解r, 另一个解是p - r
 * 用欧拉准则进行有解判定
 *
 * time complexity: O(log p)
 **/
int64 Tonelli_Shanks(int64 n, int64 p) {
  if (p == 2) return (n & 1) ? 1 : -1;
  if (pow_mod(n, p >> 1, p) != 1) return -1;
  if (p & 2) return pow_mod(n, p + 1 >> 2, p);
  int s = __builtin_ctzll(p ^ 1);
  int64 q = p >> s, z = 2;
  for (; pow_mod(z, p >> 1, p) == 1; ++z);
  int64 c = pow_mod(z, q, p);
  int64 r = pow_mod(n, q + 1 >> 1, p);
  int64 t = pow_mod(n, q, p), tmp;
  for (int m = s, i; t != 1;) {
    for (i = 0, tmp = t; tmp != 1; ++i) {
      tmp = mul_mod(tmp, tmp, p);
    }
    for (; i < --m;) c = mul_mod(c, c, p);
    r = mul_mod(r, c, p);
    c = mul_mod(c, c, p);
    t = mul_mod(t, c, p);
  }
  return std::min(r, p - r);
}

int64 Cipolla(int64 n, int64 p) {
  if (p == 2) return (n & 1) ? 1 : -1;
  if (pow_mod(n, p >> 1, p) != 1) return -1;
  if (p & 2) return pow_mod(n, p + 1 >> 2, p);
  int64 a = 0;
  for (; pow_mod((a * a + p - n) % p, p >> 1, p) == 1; ++a);
	int64 I = (a * a + p - n) % p;
  int64 rx = 1, ry = 0, tx = a, ty = 1;
	auto mult = [=](int64 &a, int64 &b, int64 c, int64 d) {
		int64 tmp = (mul_mod(mul_mod(b, d, p), I, p) + mul_mod(a, c, p)) % p;
		b =(mul_mod(a, d, p) + mul_mod(b, c, p)) % p;
    a = tmp;
	};
	for (int64 c = p + 1 >> 1; c ; c >>= 1) {
		if (c & 1) mult(rx, ry, tx, ty);
    mult(tx, ty, tx, ty);
  }
	return std::min(rx, p - rx);
}
