#include "basic.hpp"
#include <vector>

// 传入y = f(x)上的n个点, 拟合出对应的一元n-1次方程, 返回各项系数
// 类型需支持加, 减, 乘, 除, 取反, 加等于这六种操作
template<typename T>
std::vector<T> interpolation(const T x[], const T y[], int n){
  std::vector<T> u(y, y + n), ret(n), sum(n);
  ret[0] = u[0], sum[0] = 1;
  for (int i = 1; i < n; ++i) {
    for (int j = n - 1; j >= i; --j) {
      u[j] = (u[j] - u[j - 1]) / (x[j] - x[j - i]);
    }
    for (int j = i; j; --j) {
      sum[j] = -sum[j] * x[i - 1] + sum[j - 1];
      ret[j] += sum[j] * u[i];
    }
    sum[0] = -sum[0] * x[i - 1];
    ret[0] += sum[0] * u[i];
  }
  return ret;
}

// f(x) is degree m - 1, given f(0), f(1), ..., f(m - 1), 
// return f(n) in linear time
int64 evaluate(const std::vector<int64> &f, int64 n, int64 mod) {
  int64 m = f.size(), nn = n % mod;
  if (n < m) return f[n];
  std::vector<int64> inv(m + 1), fact(m + 1), ifact(m + 1);
  inv[1] = fact[0] = ifact[0] = fact[1] = ifact[1] = 1;
  for (int i = 2; i <= m; ++i) {
    inv[i] = int64(mod - mod / i) * inv[mod % i] % mod;
    fact[i] = (int64)i * fact[i - 1] % mod;
    ifact[i] = (int64)inv[i] * ifact[i - 1] % mod;
  }
  int64 ret = 0, v = 1;
  std::vector<int64> s(m + 1);
  s[m] = 1;
  for (int i = m - 1; i >= 0; --i) {
    v = (int64)v * (nn - i + mod) % mod;
    s[i] = (int64)s[i + 1] * (nn - i + mod) % mod;
    if (i == nn) s[i] = 1;
  }
  v = pow_mod(v, mod - 2, mod);
  for (int i = 0; i < m; ++i) {
    int64 inv2 = (int64)v * s[i + 1] % mod;
    v = (int64)v * (nn - i + mod) % mod;
    int64 mul = (int64)ifact[i] * ifact[m - 1 - i] % mod * inv2 % mod;
    if ((m - 1 - i) & 1) mul = mod - mul;
    if (i != nn) {
      ret += f[i] * mul % mod;
    }
    if (ret >= mod) ret -= mod;
  }
  for (int i = 0; i < m; ++i) {
    ret = (int64)ret * (nn - i + mod) % mod;
  }
  if (nn <= m - 1) {
    int64 extra = f[nn] * ifact[nn] % mod * ifact[m - 1 - nn] % mod;
    if ((m - 1 - nn) & 1) extra = mod - extra;
    for (int i = 0; i < m; ++i) {
      if (i != nn) extra = extra * (nn - i + mod) % mod;
    }
    ret = (ret + extra) % mod;
  }
  return ret;
}
