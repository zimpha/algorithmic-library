#include "fast-fourier-transform.cc"
#include "basic.hpp"
#include <cstring>

using fft::N;

namespace Poly {// aint64 n should be power of 2
  int tmp1[N], tmp2[N], tmp3[N], mod;
  int fact[N], ifact[N], iv[N];
  void init() {
    fact[0] = ifact[0] = 1;
    for (int i = 1; i < N; ++i) {
      fact[i] = (int64)fact[i - 1] * i % mod;
      ifact[i] = pow_mod(ifact[i], mod - 2, mod);
      iv[i] = pow_mod(i, mod - 2, mod);
    }
  }
  // res = 1 / poly
  void inv2(int poly[], int res[], int n) {// when use NTT::mod
    if (n == 1) {
      res[0] = pow_mod(poly[0], mod - 2, mod);
      return;
    }
    inv2(poly, res, n >> 1);
    int s = n << 1;
    memcpy(tmp1, poly, sizeof(*poly) * n);
    memset(tmp1 + n, 0, sizeof(*tmp1) * n);
    memset(res + n, 0, sizeof(*res) * n);
    // NTT::trans(tmp1, s, 1); NTT::trans(res, s, 1);
    for (int i = 0; i < s; ++i) {
      res[i] = (int64)res[i] * (2 - (int64)tmp1[i] * res[i] % mod) % mod;
      if (res[i] < 0) res[i] += mod;
    }
    // NTT::trans(res, s, -1);
    memset(res + n, 0, sizeof(*res) * n);
  }
  // res = 1 / poly
  void inv(int poly[], int res[], int n) {
    int deg = n - 1;
    std::vector<int> degs;
    while (deg > 0) {
      degs.push_back(deg);
      deg >>= 1;
    }
    std::reverse(degs.begin(), degs.end());
    res[0] = deg = 1;
    for (int t: degs) {
      fft::conv(poly, res, t + 1, deg, mod, tmp1);
      fft::conv(res, tmp1 + deg, t + 1 - deg, t + 1 - deg, mod, tmp1);
      for (int i = 0; i < t + 1 - deg; ++i) {
        res[i + deg] = mod - tmp1[i];
      }
      deg = t + 1;
    }
  }
  // res = ln(poly), poly[0] should be 1
  void log(int poly[], int res[], int n) {
    inv(poly, tmp2, n);
    for (int i = 0; i < n - 1; ++i) {
      res[i] = (int64)poly[i + 1] * (i + 1) % mod;
    }
    fft::conv(res, tmp2, n - 1, n, mod, res);
    for (int i = n - 1; i >= 1; --i) {
      res[i] = (int64)res[i - 1] * iv[i] % mod;
    }
    res[0] = 0;
  }
  // res = exp(poly), poly[0] should be 0
  void exp(int poly[], int res[], int n) {
    if (n == 1) {
      res[0] = 1;
      return;
    }
    exp(poly, res, n >> 1);
    log(res, tmp3, n);
    for (int i = 0; i < n; ++i) {
      tmp3[i] = poly[i] - tmp3[i];
      if (tmp3[i] < 0) tmp3[i] += mod;
    }
    if (++tmp3[0] == mod) tmp3[0] = 0;
    fft::conv(tmp3, res, n, n, mod, res);
    memset(res + n, 0, sizeof(*res) * n);
  }
  // res = sqrt(poly), poly[0] should be 1
  void sqrt(int poly[], int res[], int n) {
    if (n == 1) {
      res[0] = 1;
      return;
    }
    sqrt(poly, res, n >> 1);
    inv(res, tmp2, n);
    int s = n << 1;
    memcpy(tmp1, poly, sizeof(*poly) * n);
    memset(tmp1 + n, 0, sizeof(*tmp1) * n);
    memset(res + n, 0, sizeof(*res) * n);
    // NTT::trans(tmp1, s, 1); NTT::trans(res, s, 1); NTT::trans(tmp2, s, 1);
    for (int i = 0; i < s; ++i) {
      res[i] = ((int64)res[i] * res[i] + tmp1[i]) % mod * iv[2] % mod * tmp2[i] % mod;
    }
    // NTT::trans(res, s, -1)
    memset(res + n, 0, sizeof(*res) * n);
  }
}
