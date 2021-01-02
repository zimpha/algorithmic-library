#include "ModInt.hpp"
#include <vector>
#include <cassert>

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

// works only when mod is a prime
template<class mod_t>
struct PolySum {
  static constexpr int N = 1e5 + 10;
  mod_t fac[N], ifac[N];

  void init(int n) {
    fac[0] = 1;
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
    ifac[n] = fac[n].inverse();
    for (int i = n; i >= 1; --i) ifac[i - 1] = ifac[i] * i;
  }

  // Given a[0], a[1], ..., a[d],
  // Find the value of a[n] in O(d).
  mod_t eval(int d, mod_t *a, i64 n) {
    if (n <= d) return a[n];
    std::vector<mod_t> m1(d + 1), m2(d + 1);
    m1[0] = m2[d] = 1;
    for (int i = 1; i <= d; ++i) {
      mod_t t1 = (n + mod_t::modulus() - i + 1) % mod_t::modulus();
      mod_t t2 = (n - d + i - 1 + mod_t::modulus()) % mod_t::modulus();
      m1[i] = m1[i - 1] * t1, m2[d - i] = m2[d - i + 1] * t2;
    }
    mod_t ret = 0;
    for (int i = 0; i <= d; ++i) {
      auto val = a[i] * m1[i] * m2[i] * ifac[i] * ifac[d - i];
      if ((d - i) & 1) ret -= val;
      else ret += val;
    }
    return ret;
  }

  // Given a[0], a[1], ..., a[d],
  // Find the value of sum(a[i], i=0..n - 1) in O(d)
  mod_t sum(int d, mod_t *a, i64 n) {
    std::vector<mod_t> sa(a, a + d + 1);
    sa.push_back(eval(d, a, d + 1));
    for (int i = 1; i <= d + 1; ++i) sa[i] += sa[i - 1];
    return eval(d + 1, sa.data(), n - 1);
  }

  // Given a[0], a[1], ..., a[d], and q
  // Find the value of sum(a[i] * q^i, i=0..n - 1) in O(d)
  // the answer is q^n * f(n) - f(0), f(x) is a degree d polynomial
  mod_t qsum(int d, mod_t *a, mod_t q, i64 n) {
    if (q.get() == 0) return a[0];
    if (q.get() == 1) return sum(d, a, n);
    auto inv_q = q.inverse();
    // f[i] = u[i] * f(0) + v[i];
    std::vector<mod_t> u(d + 2), v(d + 2), f(d + 2);
    u[0] = 1, v[0] = 0;
    // f[n + 1] = (f[n] + a[n]) / q
    for (int i = 1; i <= d + 1; ++i) {
      u[i] = u[i - 1] * inv_q;
      v[i] = (v[i - 1] + a[i - 1]) * inv_q;
    }
    // 0 = sum((-1)^i * binom(d+1,i) * f(i), i=0..d+1)
    mod_t su = 0, sv = 0;
    for (int i = 0; i <= d + 1; ++i) {
      mod_t mul = ifac[i] * ifac[d + 1 - i];
      if (i & 1) su -= mul * u[i], sv -= mul * v[i];
      else su += mul * u[i], sv += mul * v[i];
    }
    f[0] = (mod_t(0) - sv) * su.inverse();
    for (int i = 1; i <= d; ++i) {
      f[i] = u[i] * f[0] + v[i];
    }
    return eval(d, f.data(), n) * q.pow(n) - f[0];
  }
};
