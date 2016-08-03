/**
 * Alforithm from this paper -- A Subquadratic Algorithm for Minimum Palindromic Factorization
 *
 * pl[i][0] -- minimal even factorization
 * pl[i][1] -- minimal odd factorization
 * 1 <= i <= n
 *
 * time complexity: O(n log n)
 **/ 
const int MAXN = 300000 + 10;
int pl[MAXN][2], gpl[MAXN][2];

inline void set(int *a, int x, int y, int z) {
  a[0] = x, a[1] = y, a[2] = z;
}
inline void set(int *a, int *b) {
  a[0] = b[0], a[1] = b[1], a[2] = b[2];
}
inline void set(int pl[][2], int idx, int val) {
  if (val <= 0) return;
  pl[idx][val & 1] = val;
}
inline void upd(int pl[][2], int idx, int val) {
  if (val <= 0) return;
  int &r = pl[idx][val & 1];
  if (r == -1 || r > val) r = val;
}

void factorization(char s[]) {
  int n = strlen(s);
  for (int i = 0; i <= n; ++i) {
    gpl[i][0] = 1e9;
    gpl[i][1] = 1e9 + 1;
  }
  static int g[32][3], gp[32][3], gpp[32][3];
  int pg = 0;
  for (int j = 0; j < n; ++j) {
    // g->gp
    int pgp = 0;
    for (int u = 0; u < pg; ++u) {
      int i = g[u][0];
      if (i - 1 >= 0 && s[i - 1] == s[j]) {
        g[u][0]--;
        set(gp[pgp++], g[u]);
      }
    }
    // gp->gpp
    int pgpp = 0, r = -j - 2;
    for (int u = 0; u < pgp; ++u) {
      int i = gp[u][0], d = gp[u][1], k = gp[u][2];
      if (i - r != d) {
        set(gpp[pgpp++], i, i - r, 1);
        if (k > 1) set(gpp[pgpp++], i + d, d, k - 1);
      } else set(gpp[pgpp++], i, d, k);
      r = i + (k - 1) * d;
    }
    if (j - 1 >= 0 && s[j - 1] == s[j]) {
      set(gpp[pgpp++], j - 1, j - 1 - r, 1);
      r = j - 1;
    }
    set(gpp[pgpp++], j, j - r, 1);
    // gpp->g
    pg = 0;
    int *front = gpp[0];
    for (int u = 1; u < pgpp; ++u) {
      int *x = gpp[u];
      if (x[1] == front[1]) front[2] += x[2];
      else {
        set(g[pg++], front);
        front = x;
      }
    }
    set(g[pg++], front);
    // dp update
    if ((j + 1) % 2 == 0) {
      pl[j + 1][0] = j + 1;
      pl[j + 1][1] = 1e9 + 1;
    } else {
      pl[j + 1][0] = 1e9;
      pl[j + 1][1] = j + 1;
    }
    for (int u = 0; u < pg; ++u) {
      int i = g[u][0], d = g[u][1], k = g[u][2];
      r = i + (k - 1) * d;
      upd(pl, j + 1, pl[r][0] + 1);
      upd(pl, j + 1, pl[r][1] + 1);
      if (k > 1) {
        upd(pl, j + 1, gpl[i + 1 - d][0]);
        upd(pl, j + 1, gpl[i + 1 - d][1]);
      }
      if (i + 1 >= d) {
        if (k > 1) {
          upd(gpl, i + 1 - d, pl[r][0] + 1);
          upd(gpl, i + 1 - d, pl[r][1] + 1);
        } else {
          set(gpl, i + 1 - d, pl[r][0] + 1);
          set(gpl, i + 1 - d, pl[r][1] + 1);
        }
      }
    }
  }
}
