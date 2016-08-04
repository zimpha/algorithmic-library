struct Hashing {
  typedef unsigned long long ULL;
  ULL seed;
  std::vector<ULL> hs, pw;
  Hashing(ULL _seed = 9875321): seed(_seed) {}
  void build(int n, char *s) {
    hs.assign(n + 1, 0);
    pw.assign(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
      hs[i] = seed * hs[i - 1];
    }
    for (int i = n - 1; i >= 0; --i) {
      hs[i] = seed * hs[i - 1] + s[i];
    }
  }
  ULL get_hash(int l, int r) {// [l, r)
    return hs[l] - hs[r] * pw[r - l];
  }
};
