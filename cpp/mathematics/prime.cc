#include <cmath>
#include <numeric>
#include <cassert>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace prime {

using uint128 = __uint128_t;
using uint64 = unsigned long long;
using int64 = long long;
using uint32 = unsigned int;
using pii = std::pair<uint64, uint32>;

inline uint64 sqr(uint64 x) { return x * x; }
inline uint32 isqrt(uint64 x) { return sqrtl(x); }
inline uint32 ctz(uint64 x) { return __builtin_ctzll(x); }

template <typename word>
word gcd(word a, word b) {
  while (b) { word t = a % b; a = b; b = t; }
  return a;
}

template <typename word, typename dword, typename sword>
struct Mod {
  Mod(): x(0) {}
  Mod(word _x): x(init(_x)) {}
  bool operator == (const Mod& rhs) const { return x == rhs.x; }
  bool operator != (const Mod& rhs) const { return x != rhs.x; }
  Mod& operator += (const Mod& rhs) { if ((x += rhs.x) >= mod) x -= mod; return *this; }
  Mod& operator -= (const Mod& rhs) { if (sword(x -= rhs.x) < 0) x += mod; return *this; }
  Mod& operator *= (const Mod& rhs) { x = reduce(dword(x) * rhs.x); return *this; }
  Mod operator + (const Mod &rhs) const { return Mod(*this) += rhs; }
  Mod operator - (const Mod &rhs) const { return Mod(*this) -= rhs; }
  Mod operator * (const Mod &rhs) const { return Mod(*this) *= rhs; }
  Mod operator - () const { return Mod() - *this; }
  Mod pow(uint64 e) const {
    Mod ret(1);
    for (Mod base = *this; e; e >>= 1, base *= base) {
      if (e & 1) ret *= base;
    }
    return ret;
  }
  word get() const { return reduce(x); }
  static constexpr int word_bits = sizeof(word) * 8;
  static word modulus() { return mod; }
  static word init(word w) { return reduce(dword(w) * r2); }
  static void set_mod(word m) { mod = m, inv = mul_inv(mod), r2 = -dword(mod) % mod; }
  static word reduce(dword x) {
    word y = word(x >> word_bits) - word((dword(word(x) * inv) * mod) >> word_bits);
    return sword(y) < 0 ? y + mod : y;
  }
  static word mul_inv(word n, int e = 6, word x = 1) {
    return !e ? x : mul_inv(n, e - 1, x * (2 - x * n));
  }
  static word mod, inv, r2;

  word x;
};

using Mod64 = Mod<uint64, uint128, int64>;
using Mod32 = Mod<uint32, uint64, int>;
template <> uint64 Mod64::mod = 0;
template <> uint64 Mod64::inv = 0;
template <> uint64 Mod64::r2 = 0;
template <> uint32 Mod32::mod = 0;
template <> uint32 Mod32::inv = 0;
template <> uint32 Mod32::r2 = 0;

template <class word, class mod>
bool composite(word n, const uint32* bases, int m) {
  mod::set_mod(n);
  int s = __builtin_ctzll(n - 1);
  word d = (n - 1) >> s;
  mod one{1}, minus_one{n - 1};
  for (int i = 0, j; i < m; ++i) {
    mod a = mod(bases[i]).pow(d);
    if (a == one || a == minus_one) continue;
    for (j = s - 1; j > 0; --j) {
      if ((a *= a) == minus_one) break;
    }
    if (j == 0) return true;
  }
  return false;
}

bool is_prime(uint64 n) { // reference: http://miller-rabin.appspot.com
  assert(n < (uint64(1) << 63));
  static const uint32 bases[][7] = {
    {2, 3},
    {2, 299417},
    {2, 7, 61},
    {15, 176006322, uint32(4221622697)},
    {2, 2570940, 211991001, uint32(3749873356)},
    {2, 2570940, 880937, 610386380, uint32(4130785767)},
    {2, 325, 9375, 28178, 450775, 9780504, 1795265022}
  };
  if (n <= 1) return false;
  if (!(n & 1)) return n == 2;
  if (n <= 8) return true;
  int x = 6, y = 7;
  if (n < 1373653) x = 0, y = 2;
  else if (n < 19471033) x = 1, y = 2;
  else if (n < 4759123141) x = 2, y = 3;
  else if (n < 154639673381) x = y = 3;
  else if (n < 47636622961201) x = y = 4;
  else if (n < 3770579582154547) x = y = 5;
  if (n < (uint32(1) << 31)) {
    return !composite<uint32, Mod32>(n, bases[x], y);
  } else if (n < (uint64(1) << 63)) {
    return !composite<uint64, Mod64>(n, bases[x], y);
  }
  return true;
}

struct ExactDiv {
  ExactDiv() {}
  ExactDiv(uint64 n) : n(n), i(Mod64::mul_inv(n)), t(uint64(-1) / n) {}
  friend uint64 operator / (uint64 n, ExactDiv d) { return n * d.i; };
  bool divide(uint64 n) { return n / *this <= t; }
  uint64 n, i, t;
};

std::vector<ExactDiv> primes;

void init(uint32 n) {
  uint32 sqrt_n = sqrt(n);
  std::vector<bool> is_prime(n + 1, 1);
  primes.clear();
  for (uint32 i = 2; i <= sqrt_n; ++i) if (is_prime[i]) {
    if (i != 2) primes.push_back(ExactDiv(i));
    for (uint32 j = i * i; j <= n; j += i) is_prime[j] =  0;
  }
}

template <typename word, typename mod>
word brent(word n, word c) { // n must be composite and odd.
  const uint64 s = 256;
  mod::set_mod(n);
  const mod one = mod(1), mc = mod(c);
  mod y = one;
  for (uint64 l = 1; ; l <<= 1) {
    auto x = y;
    for (int i = 0; i < (int)l; ++i) y = y * y + mc;
    mod p = one;
    for (int k = 0; k < (int)l; k += s) {
      auto sy = y;
      for (int i = 0; i < (int)std::min(s, l - k); ++i) {
        y = y * y + mc;
        p *= y - x;
      }
      word g = gcd(n, p.x);
      if (g == 1) continue;
      if (g == n) for (g = 1, y = sy; g == 1; ) {
        y = y * y + mc, g = gcd(n, (y - x).x);
      }
      return g;
    }
  }
}

uint64 brent(uint64 n, uint64 c) {
  if (n < (uint32(1) << 31)) {
    return brent<uint32, Mod32>(n, c);
  } else if (n < (uint64(1) << 63)) {
    return brent<uint64, Mod64>(n, c);
  }
  return 0;
}

std::vector<pii> factors(uint64 n) {
  assert(n < (uint64(1) << 63));
  if (n <= 1) return {};
  std::vector<pii> ret;
  uint32 v = sqrtl(n);
  if (uint64(v) * v == n) {
    ret = factors(v);
    for (auto &&e: ret) e.second *= 2;
    return ret;
  }
  v = cbrtl(n);
  if (uint64(v) * v * v == n) {
    ret = factors(v);
    for (auto &&e: ret) e.second *= 3;
    return ret;
  }
  if (!(n & 1)) {
    uint32 e = __builtin_ctzll(n);
    ret.emplace_back(2, e);
    n >>= e;
  }
  uint64 lim = sqr(primes.back().n);
  for (auto &&p: primes) {
    if (sqr(p.n) > n) break;
    if (p.divide(n)) {
      uint32 e = 1; n = n / p;
      while (p.divide(n)) n = n / p, e++;
      ret.emplace_back(p.n, e);
    }
  }

  uint32 s = ret.size();
  while (n > lim && !is_prime(n)) {
    for (uint64 c = 1; ; ++c) {
      uint64 p = brent(n, c);
      if (!is_prime(p)) continue;
      uint32 e = 1; n /= p;
      while (n % p == 0) n /= p, e += 1;
      ret.emplace_back(p, e);
      break;
    }
  }
  if (n > 1) ret.emplace_back(n, 1);
  if (ret.size() - s >= 2) sort(ret.begin() + s, ret.end());
  return ret;
}
}
