/**
 * Suffix Array Construction using Prefix doubling
 *
 * time complexity: O(nlogn)
 */
struct SA {
  static const int MAXN = 100000 + 10;
  int cnt[MAXN], tr[2][MAXN], ts[MAXN];
  int sa[MAXN], rk[MAXN], ht[MAXN];
  void build(char *s, int n, int m = 256) {
    int i, k, *x = tr[0], *y = tr[1];
    memset(cnt, 0,sizeof(cnt[0]) * m);
    for (i = 0; i < n; ++i) cnt[(int)s[i]]++;
    partial_sum(cnt, cnt + m, cnt);
    for (i = 0; i < n; ++i) rk[i] = cnt[(int)s[i]] - 1;
    for (k = 1; k <= n; k <<= 1) {
      for (i = 0; i < n; ++i) {
        x[i] = rk[i];
        y[i] = i + k < n ? rk[i + k] + 1 : 0;
      }
      fill(cnt, cnt + n + 1, 0);
      for (i = 0; i < n; ++i) cnt[y[i]]++;
      partial_sum(cnt, cnt + n + 1, cnt);
      for (i = n - 1; i >= 0; --i) ts[--cnt[y[i]]] = i;
      fill(cnt, cnt + n + 1, 0);
      for (i = 0; i < n; ++i) cnt[x[i]]++;
      partial_sum(cnt, cnt + n + 1, cnt);
      for (i = n - 1; i >= 0; --i) sa[--cnt[x[ts[i]]]] = ts[i];
      for (i = rk[sa[0]] = 0; i+1 < n; ++i) {
        rk[sa[i + 1]] = rk[sa[i]] +
                        (x[sa[i]] != x[sa[i + 1]] || y[sa[i]] != y[sa[i + 1]]);
      }
    }
  }
  void get_height(char *s, int n) {
    for (int i = 0, l = 0, j; i < n; ++i) if (rk[i]) {
      j = sa[rk[i] - 1];
      while(i + l < n && j + l < n && s[i + l] == s[j + l]) l++;
      ht[rk[i]] = l; if (l) l--;
    }
  }
  inline int lcp(int a, int b) {
    a = rk[a], b = rk[b];
    if (a > b) swap(a,b);
    return rmq(a + 1,b);
  }
};
