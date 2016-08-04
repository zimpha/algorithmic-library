#include "basic.hpp"

struct Primality {
public:
  // 用miller rabin素数测试判断n是否为质数
  bool is_prime(LL n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (~n & 1) return false;
    const int u[] = {2,325,9375,28178,450775,9780504,1795265022,0};
    LL e = n - 1, a, c = 0; // 原理：http://miller-rabin.appspot.com/
    while (~e & 1) e >>= 1, ++c;
    for (int i = 0; u[i]; ++i) {
      if (n <= u[i]) return true;
      a = pow_mod_LL(u[i], e, n);
      if (a == 1) continue;
      for (int j = 1; a != n - 1; ++j) {
        if (j == c) return false;
        a = mul_mod(a, a, n);
      }
    }
    return true;
  }
  // 求一个小于n的因数，期望复杂度为O(n^0.25)，当n为非合数时返回n本身
  LL pollard_rho(LL n){
    if (n <= 3 || is_prime(n)) return n; // 保证n为合数时可去掉这行
    while (1) {
      int i = 1, cnt = 2;
      LL x = rand() % n, y = x, c = rand() % n;
      if (!c || c == n - 2) ++c;
      do {
        LL u = gcd(n - x + y, n);
        if (u > 1 && u < n) return u;
        if (++i == cnt) y = x, cnt <<= 1;
        x = (c + mul_mod(x, x, n)) % n;
      } while (x != y);
    }
    return n;
  }
  // 使用rho方法对n做质因数分解，建议先筛去小质因数后再用此函数
  std::vector<LL> factorize(LL n){
    std::vector<LL> u;
    if (n > 1) u.push_back(n);
    for (size_t i = 0; i < u.size(); ++i){
      LL x = pollard_rho(u[i]);
      if(x == u[i]) continue;
      u[i--] /= x;
      u.push_back(x);
    }
    std::sort(u.begin(), u.end());
    return u;
  }
};
