#include "basic.hpp"
#include <ctime>
#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>

using int64 = long long;
using uint32 = unsigned int;
using uint64 = unsigned long long;
using uint128 = __uint128_t;

namespace ntt {
// if mod is not close to 2^(word_bits-1), it's faster to use comment lines
template <class word, class dword, class sword, word mod, word root>
class Mod {
public:
  static constexpr word mul_inv(word n, int e = 6, word x = 1) {
    return e == 0 ? x : mul_inv(n, e - 1, x * (2 - x * n));
  }

  static constexpr word inv = mul_inv(mod);
  static constexpr word r2 = -dword(mod) % mod;
  static constexpr int word_bits = sizeof(word) * 8;
  static constexpr int level = __builtin_ctzll(mod - 1);

  static word modulus() {
    return mod;
  }
  static word init(const word& w) {
    return reduce(dword(w) * r2);
  }
  static word reduce(const dword& w) {
    word y = word(w >> word_bits) - word((dword(word(w) * inv) * mod) >> word_bits);
    return sword(y) < 0 ? y + mod : y;
    //return word(w >> word_bits) + mod - word((dword(word(w) * inv) * mod) >> word_bits);
  }
  static Mod omega() {
    return Mod(root).pow((mod - 1) >> level);
  }

  Mod() = default;
  Mod(const word& n): x(init(n)) {};
  Mod& operator += (const Mod& rhs) {
    //this->x += rhs.x;
    if ((x += rhs.x) >= mod) x -= mod;
    return *this;
  }
  Mod& operator -= (const Mod& rhs) {
    //this->x += mod * 3 - rhs.x;
    if (sword(x -= rhs.x) < 0) x += mod;
    return *this;
  }
  Mod& operator *= (const Mod& rhs) {
    this->x = reduce(dword(this->x) * rhs.x);
    return *this;
  }
  Mod operator + (const Mod& rhs) const {
    return Mod(*this) += rhs;
  }
  Mod operator - (const Mod& rhs) const {
    return Mod(*this) -= rhs;
  }
  Mod operator * (const Mod& rhs) const {
    return Mod(*this) *= rhs;
  }
  word get() const {
    return reduce(this->x) % mod;
  }
  Mod inverse() const {
    return pow(mod - 2);
  }
  Mod pow(word e) const {
    Mod ret(1);
    for (Mod a = *this; e; e >>= 1) {
      if (e & 1) ret *= a;
      a *= a;
    }
    return ret;
  }
  word x;
};

template <class T>
inline void sum_diff(T& x, T &y) {
  auto a = x, b = y;
  x = a + b, y = a - b;
}

// Matters Computational. 26.2.3.1
template <class mod_t>
void ntt_dit4(mod_t A[], int n, int sgn, mod_t roots[], int *rev) {
  for (int i = 0; i < n; ++i) {
    if (i < rev[i]) std::swap(A[i], A[rev[i]]);
  }
  int logn = __builtin_ctz(n);
  if (logn & 1) for (int i = 0; i < n; i += 2) {
    auto a = A[i], b = A[i + 1];
    A[i] = a + b, A[i + 1] = a - b;
    //sum_diff(A[i], A[i + 1]);
  }
  auto im = roots[mod_t::level - 2], one = mod_t(1);
  if (sgn < 0) im = im.inverse();
  for (int e = 2 + (logn & 1); e <= logn; e += 2) {
    const int m = 1 << e, m4 = m >> 2;
    auto dw = roots[mod_t::level - e];
    if (sgn < 0) dw = dw.inverse();
    const int block_size = std::min(n, std::max(m, (1 << 15) / int(sizeof(A[0]))));
    for (int k = 0; k < n; k += block_size) {
      auto w = one, w2 = one, w3 = one;
      for (int j = 0; j < m4; ++j) {
        for (int i = k + j; i < k + block_size; i += m) {
          auto a0 = A[i + m4 * 0] * one, a2 = A[i + m4 * 1] * w2;
          auto a1 = A[i + m4 * 2] * w,   a3 = A[i + m4 * 3] * w3;
          auto t02 = a0 + a2, t13 = a1 + a3;
          A[i + m4 * 0] = t02 + t13; A[i + m4 * 2] = t02 - t13;
          t02 = a0 - a2, t13 = (a1 - a3) * im;
          A[i + m4 * 1] = t02 + t13; A[i + m4 * 3] = t02 - t13;
        }
        w *= dw; w2 = w * w; w3 = w2 * w;
      }
    }
  }
}

// Matters Computational. 26.2.3.2
template <class mod_t>
void ntt_dif4(mod_t A[], int n, int sgn, mod_t roots[], int *rev) {
  int logn = __builtin_ctz(n);
  auto im = roots[mod_t::level - 2], one = mod_t(1);
  if (sgn < 0) im = im.inverse();
  for (int e = logn; e >= 2; e -= 2) {
    const int m = 1 << e, m4 = m >> 2;
    auto dw = roots[mod_t::level - e];
    if (sgn < 0) dw = dw.inverse();
    const int block_size = std::min(n, std::max(m, (1 << 15) / int(sizeof(A[0]))));
    for (int k = 0; k < n; k += block_size) {
      auto w = one, w2 = one, w3 = one;
      for (int j = 0; j < m4; ++j) {
        for (int i = k + j; i < k + block_size; i += m) {
          auto a0 = A[i + m4 * 0], a2 = A[i + m4 * 1];
          auto a1 = A[i + m4 * 2], a3 = A[i + m4 * 3];
          auto t02 = a0 + a2, t13 = a1 + a3;
          A[i + m4 * 0] = (t02 + t13) * one; A[i + m4 * 2] = (t02 - t13) * w2;
          t02 = a0 - a2, t13 = (a1 - a3) * im;
          A[i + m4 * 1] = (t02 + t13) * w;   A[i + m4 * 3] = (t02 - t13) * w3;
        }
        w *= dw; w2 = w * w; w3 = w2 * w;
      }
    }
  }
  if (logn & 1) for (int i = 0; i < n; i += 2) {
    sum_diff(A[i], A[i + 1]);
  }
  for (int i = 0; i < n; ++i) {
    if (i < rev[i]) std::swap(A[i], A[rev[i]]);
  }
}

template <class mod_t>
void convolute(mod_t A[], int n, mod_t B[], int m, bool cyclic = false) {
  int s = (cyclic ? std::max(n, m) : n + m - 1), size = 1, logn = 0;
  while (size < s) size <<= 1, ++logn;
  mod_t roots[mod_t::level] = {mod_t::omega()};
  for (int i = 1; i < mod_t::level; ++i) {
    roots[i] = roots[i - 1] * roots[i - 1];
  }
  std::vector<int> rev(size);
  for (int i = 0; i < size; ++i) {
    rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (logn - 1));
  }
  std::fill(A + n, A + size, 0);
  ntt_dit4(A, size, 1, roots, &rev[0]);
  if (A == B && n == m) {
    for (int i = 0; i < size; ++i) A[i] *= A[i];
  } else {
    std::fill(B + m, B + size, 0);
    ntt_dit4(B, size, 1, roots, &rev[0]);
    for (int i = 0; i < size; ++i) A[i] *= B[i];
  }
  ntt_dit4(A, size, -1, roots, &rev[0]);
  mod_t inv = mod_t(size).inverse();
  if (!cyclic) size = s;
  for (int i = 0; i < size; ++i) A[i] *= inv;
}
}

// transform with dif, itransform with dft, no need to use bit_rev
namespace ntt_fast {
template <typename mod_t>
void transform(mod_t* A, int n, const mod_t* roots, const mod_t* iroots) {
  const int logn = __builtin_ctz(n), nh = n >> 1, lv = mod_t::level;
  auto one = mod_t(1), im = roots[lv - 2];
  mod_t dw[lv - 1]; dw[0] = roots[lv - 3];
  for (int i = 1; i < lv - 2; ++i) {
    dw[i] = dw[i - 1] * iroots[lv - 1 - i] * roots[lv - 3 - i];
  }
  dw[lv - 2] = dw[lv - 3] * iroots[1];
  if (logn & 1) for (int i = 0; i < nh; ++i) {
    ntt::sum_diff(A[i], A[i + nh]);
  }
  for (int e = logn & ~1; e >= 2; e -= 2) {
    const int m = 1 << e, m4 = m >> 2;
    auto w2 = one;
    for (int i = 0; i < n; i += m) {
      auto w1 = w2 * w2, w3 = w1 * w2;
      for (int j = i; j < i + m4; ++j) {
        auto a0 = A[j + m4 * 0] * one, a1 = A[j + m4 * 1] * w2;
        auto a2 = A[j + m4 * 2] * w1,  a3 = A[j + m4 * 3] * w3;
        auto t02p = a0 + a2, t13p = a1 + a3;
        auto t02m = a0 - a2, t13m = (a1 - a3) * im;
        A[j + m4 * 0] = t02p + t13p; A[j + m4 * 1] = t02p - t13p;
        A[j + m4 * 2] = t02m + t13m; A[j + m4 * 3] = t02m - t13m;
      }
      w2 *= dw[__builtin_ctz(~(i >> e))];
    }
  }
}

template <typename mod_t>
void itransform(mod_t* A, int n, const mod_t* roots, const mod_t* iroots) {
  const int logn = __builtin_ctz(n), nh = n >> 1, lv = mod_t::level;
  const auto one = mod_t(1), im = iroots[lv - 2];
  mod_t dw[lv - 1]; dw[0] = iroots[lv - 3];
  for (int i = 1; i < lv - 2; ++i) {
    dw[i] = dw[i - 1] * roots[lv - 1 - i] * iroots[lv - 3 - i];
  }
  dw[lv - 2] = dw[lv - 3] * roots[1];
  for (int e = 2; e <= logn; e += 2) {
    const int m = 1 << e, m4 = m >> 2;
    auto w2 = one;
    for (int i = 0; i < n; i += m) {
      const auto w1 = w2 * w2, w3 = w1 * w2;
      for (int j = i; j < i + m4; ++j) {
        auto a0 = A[j + m4 * 0], a1 = A[j + m4 * 1];
        auto a2 = A[j + m4 * 2], a3 = A[j + m4 * 3];
        auto t01p = a0 + a1, t23p = a2 + a3;
        auto t01m = a0 - a1, t23m = (a2 - a3) * im;
        A[j + m4 * 0] = (t01p + t23p) * one; A[j + m4 * 2] = (t01p - t23p) * w1;
        A[j + m4 * 1] = (t01m + t23m) * w2;  A[j + m4 * 3] = (t01m - t23m) * w3;
      }
      w2 *= dw[__builtin_ctz(~(i >> e))];
    }
  }
  if (logn & 1) for (int i = 0; i < nh; ++i) {
    ntt::sum_diff(A[i], A[i + nh]);
  }
}

template <typename mod_t>
void convolute(mod_t* A, int n, mod_t* B, int m, bool cyclic=false) {
  const int s = cyclic ? std::max(n, m) : n + m - 1;
  const int size = 1 << (31 - __builtin_clz(2 * s - 1));
  mod_t roots[mod_t::level], iroots[mod_t::level];
  roots[0] = mod_t::omega();
  for (int i = 1; i < mod_t::level; ++i) {
    roots[i] = roots[i - 1] * roots[i - 1];
  }
  iroots[0] = roots[0].inverse();
  for (int i = 1; i < mod_t::level; ++i) {
    iroots[i] = iroots[i - 1] * iroots[i - 1];
  }
  std::fill(A + n, A + size, 0); transform(A, size, roots, iroots);
  const auto inv = mod_t(size).inverse();
  if (A == B && n == m) {
    for (int i = 0; i < size; ++i) A[i] *= A[i] * inv;
  } else {
    std::fill(B + m, B + size, 0); transform(B, size, roots, iroots);
    for (int i = 0; i < size; ++i) A[i] *= B[i] * inv;
  }
  itransform(A, size, roots, iroots);
}
}

// 4405523190172876801, 19
// 4481719345977753601, 11
// 4601552919265804289, 3
using mod_1 = ntt::Mod<uint64, uint128, int64, 709143768229478401, 31>;
using mod_2 = ntt::Mod<uint64, uint128, int64, 711416664922521601, 19>;
using mod_3 = ntt::Mod<uint64, uint128, int64, 1945555039024054273, 5>;
using mod_4 = ntt::Mod<uint32, uint64, int, 2013265921, 31>;
using mod_5 = ntt::Mod<uint32, uint64, int, 2113929217, 5>;
