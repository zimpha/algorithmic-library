#include <cstdio>
#include <cassert>
#include <algorithm>

namespace NTT {
  using ll = long long;
  const ll P = 2013265921, g = 31;
  //2113929217,5; 1811939329,13; 2130706433,3; 786433,10;
  //1945555039024054273,5; 1231453023109121ll,3;
  inline ll mul_mod(ll a, ll b, ll mod) {
    assert(0 <= a && a < mod);
    assert(0 <= b && b < mod);
    if (mod < int(1e9)) return a * b % mod;
    ll k = (ll)((long double)a * b / mod);
    ll res = a * b - k * mod;
    res %= mod;
    if (res < 0) res += mod;
    return res;
  }
  ll pow_mod(ll a, ll n, ll m) {
    ll r = 1;
    for (; n; n >>= 1) {
      if (n & 1) r = mul_mod(r, a, m);
      a = mul_mod(a, a, m);
    }
    return r;
  }
  void trans(ll a[], int n, bool inv = false) {
    ll w = 1, d = pow_mod(g, (P - 1) / n, P), t;
    int i, j, c, s;
    if (inv) {
      for (i = 1, j = n - 1; i < j; std::swap(a[i++], a[j--]));
      for (t = pow_mod(n, P - 2, P), i = 0; i < n; ++i) {
        a[i] = mul_mod(a[i], t, P);
      }
    }
    for (s = n >> 1; s; s >>= w = 1, d = mul_mod(d, d, P)) {
      for (c = 0; c < s; ++c, w = mul_mod(w, d, P)) {
        for (i = c; i < n; i += s << 1) {
          t = a[i | s];
          a[i | s] = mul_mod((a[i] + P - t) % P, w, P);
          a[i] = (a[i] + t) % P;
        }
      }
    }
    for (i = 1; i < n; ++i) {
      for (j = 0, s = i, c = n >> 1; c; c >>= 1, s >>= 1) {
        j = j << 1 | s & 1;
      }
      if (i < j) std::swap(a[i], a[j]);
    }
  }
}
