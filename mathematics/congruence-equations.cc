#include "basic.hpp"

// 中国剩余定理， 要求m[i]互质
// x = a[i] (mod m[i]) (0 <= i < n)
LL crt(int n, LL *c, LL *m) {
  LL M = 1, ans = 0;
  for (int i = 0; i < n; ++i) M *= m[i];
  for (int i = 0; i < n; ++i) {
    LL x, y, g, tm = M / m[i];
    exgcd(tm, m[i], g, x, y);
    ans = (ans + tm * x * a[i] % M) % M;
  }
  return (ans + M) % M;
}

// 同余方程合并， m[i]可以不互质
// x = c[i] mod m[i], c[i] < m[i]
// 返回-1表示解不存在
LL solve(int n, LL *c, LL *m){
   LL ans = c[0], LCM = m[0];
   for (int i = 1; i < n; ++i) {
     LL g, x, y;
     exgcd(LCM, m[i], g, x, y);
     if ((c[i] - ans) % g) return -1;
     LL tmp = add_mod((c[i] - ans) / g * x, m[i] / g);
     ans = add_mod(ans + LCM * tmp, LCM / g * m[i]);
     LCM = LCM / g * m[i];
   }
   return add_mod(ans, LCM);
}
