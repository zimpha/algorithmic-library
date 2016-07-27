/**
 * return the lexicographically minimal string rotation
 * using the Booth's Algorithm
 *
 * For example, the lexicographically minimal rotation
 * of "bbaaccaadd" would be "aaccaaddbb".
 *
 * lcs("bbaaccaadd", 10) will return 2 as result.
 * 
 * time complexity: O(n)
 **/

int lcs(char *s, int n) {
  for (int i = 0, j = 1, k, ci, cj; ;) {
    for (k = 0; k < n; ++k) {
      ci = s[(i + k) % n];
      cj = s[(j + k) % n];
      if (ci != cj) break;
    }
    if (k == n) return std::min(i,j);
    if (ci > cj) i += k + 1,i += i == j;
    else j += k + 1, j += i==j;
  }
  return -1;
}
