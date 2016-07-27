#include <bits/stdc++.h>

/**
 * u[i] is the length of the longest palindrome centered as i / 2.
 * treat string 'babbaa' as 'b.a.b.b.a.a', the result u[] is
 * {1,0,3,0,1,4,1,0,1,2,1}.
 * Also, u[i + j] is for s[i..j]
 *
 * time complexity: O(n)
 **/
std::vector<int> manacher(char *s, int n) {
  std::vector<int> u(n <<= 1, 0);
  for (int i = 0, j = 0, k; i < n; i += k, j = std::max(j - k, 0)) {
    while (i >= j && i + j + 1 < n && s[(i - j) >> 1] == s[(i + j + 1) >> 1]) ++j;
    for (u[i] = j, k = 1; i >= k && u[i] >= k && u[i - k] != u[i] - k; ++k) {
      u[i + k] = std::min(u[i - k], u[i] - k);
    }
  }
  return u;
}
