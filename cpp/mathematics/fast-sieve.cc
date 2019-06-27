#include "basic.hpp"
#include <vector>
#include <cmath>

using std::size_t;

std::vector<int> pl, phi, mp;
void fast_sieve(int n) {
  pl.clear();
  phi.assign(n, 1);
  mp.assign(n, 0);
  for (int i = 2; i < n; ++i) {
    if (!mp[i]) {
      pl.push_back(i);
      mp[i] = i;
      phi[i] = i - 1;
    }
    for (size_t j = 0; j < pl.size() && i * pl[j] < n; ++j) {
      int p = pl[j];
      mp[i * p] = p;
      phi[i * p] = phi[i] * (p - !!(i % p));
      if (i % p == 0) break;
    }
  }
}

std::vector<int64> factorize(int64 n){
  std::vector<int64> u;
  for (int i = 0, t = sqrt(n + 1); pl[i] <= t; ++i) if (n % pl[i] == 0) {
    while (n % pl[i] == 0) {
      n /= pl[i];
      u.push_back(pl[i]);
    }
    t = sqrt(n + 1);
  }
  if (n > 1) u.push_back(n);
  return u;
}

bool is_prime(int n) {
  if (n < mp.size()) return mp[n] == n;
  for (int i = 0, sq = sqrt(n + 1); pl[i] <= sq; ++i) {
    if (n % pl[i] == 0) return false;
  }
  return true;
}
