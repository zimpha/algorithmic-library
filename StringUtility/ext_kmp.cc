/**
 * z[i] = lcp(s[i:], s[0:])
 *
 * time complexity: O(n)
 **/ 
std::vector<int> ext_kmp(char *s, int n) {
  std::vector<int> z(n, 0);
  for (int i = 1, x = 0, y = 0; i < n; ++i) {
    if (i <= y) z[i] = std::min(y - i, z[i - x]);
    while (i + z[i] < n && s[i + z[i]] == s[z[i]]) ++z[i];
    if (y <= i + z[i]) x = i, y = i + z[i];
  }
  z[0] = n;
  return z;
}
