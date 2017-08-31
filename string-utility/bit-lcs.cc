/**
 * Bit-String Longest Common Subsequence LCS Algorithm
 *
 * http://www.csse.monash.edu.au/~lloyd/tildeStrings/Alignment/86.IPL.html
 *
 * time complexity: O(nm/w)
 **/
#include <bits/stdc++.h>
typedef long long int64;

const int MAXN = 2000 + 10, SIGMA = 26;
const int W = 62;
int M = 33;
struct Bitset {
  int64 u[34];
  void clr() {
    memset(u, 0, sizeof(u));
  }
  void set(int x) {
    u[x / W] |= 1ll << (x % W);
  }
  Bitset operator |(const Bitset &r) const {
    Bitset s;
    for (int i = 0; i < M; ++ i) {
      s.u[i] = u[i] | r.u[i];
    }
    return s;
  }
  void gao(const Bitset &r) {
    for (int i = 0; i < M; ++ i) {
      u[i] = (u[i] ^ r.u[i]) & r.u[i];
    }
  }
  void sub(const Bitset &r) {
    for (int i = 0; i < M; ++ i) u[i] = r.u[i] - u[i];
    for (int i = 0; i < M; ++ i) if (u[i] < 0) {
      u[i] += 1ll << W; u[i + 1] --;
    }
  }
  void shl() {
    for (int64 i = 0, c(1); i < M; ++ i) {
      u[i] <<= 1; u[i] |= c;
      c = u[i] >> W & 1;
      u[i] ^= c << W;
    }
  }
  int cnt() const {
    int c(0);
    for (int i = 0; i < M; ++ i) {
      c += __builtin_popcountll(u[i]);
    }
    return c;
  }
} row, as[SIGMA], x;


int main() {
  char S[MAXN], T[MAXN];
  for (int i = 0; i < SIGMA; ++i) as[i].clr();
  scanf("%s%s", S, T);
  int n = strlen(S), m = strlen(T);
  for (int i = 0; i < n; ++ i) {
    as[S[i] - 'a'].set(i);
  }
  M = n / W + (n % W != 0);
  row.clr();
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < M; ++j) {
      x.u[j] = row.u[j] | as[T[i] - 'a'].u[j];
    }
    row.shl();
    row.sub(x);
    row.gao(x);
    printf("%d\n", row.cnt());
  }
  return 0;
}
