#include "basic.hpp"

// 中国剩余定理， 要求m[i]互质
// x = c[i] (mod m[i]) (0 <= i < n)
int64 crt(int n, int64 *c, int64 *m) {
  int64 M = 1, ans = 0;
  for (int i = 0; i < n; ++i) M *= m[i];
  for (int i = 0; i < n; ++i) {
    int64 x, y, g, tm = M / m[i];
    exgcd(tm, m[i], g, x, y);
    ans = (ans + tm * x * c[i] % M) % M;
  }
  return (ans + M) % M;
}

// 同余方程合并， m[i]可以不互质
// x = c[i] mod m[i], c[i] < m[i]
// 返回-1表示解不存在
int64 solve(int n, int64 *c, int64 *m){
  auto mod = [](int64 x, int64 y) {
    x %= y;
    if (x < 0) x += y;
    return x;
  };
  int64 ans = c[0], LCM = m[0];
  for (int i = 1; i < n; ++i) {
    int64 g, x, y;
    exgcd(LCM, m[i], g, x, y);
    if ((c[i] - ans) % g) return -1;
    int64 tmp = mod((c[i] - ans) / g * x, m[i] / g);
    ans = mod(ans + LCM * tmp, LCM / g * m[i]);
    LCM = LCM / g * m[i];
  }
  return mod(ans, LCM);
}
