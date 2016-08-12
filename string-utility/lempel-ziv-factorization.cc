#include <cstring>
#include <vector>
#include <utility>

using PII = std::pair<int, int>;

/**
 * Implement this paper:
 *   Simpler and Faster Lempel Ziv Factorization
 *
 * s: input string
 * sa: suffix array of s
 * rk: inverse suffix array
 * return a list of (len, prevOcc)
 **/
std::vector<PII> LempelZiv(char s[], int sa[], int rk[]) {
  int n = strlen(s);
  std::vector<int> psv(n + 1), nsv(n + 1), stack;
  // computer psv and nsv from suffix array
  for (int i = 0; i < n; ++i) {
    int x = sa[i];
    while (!stack.empty() && sa[stack.back()] > x) {
      nsv[stack.back()] = x;
      stack.pop_back();
    }
    psv[i] = stack.empty() ? -1 : sa[stack.back()];
    stack.push_back(i);
  }
  while (!stack.empty()) {
    nsv[stack.back()] = -1;
    stack.pop_back();
  }
  // computer Lempel-Ziv Factorization
  std::vector<PII> lz;
  lz.push_back(PII(0, s[0]));
  for (int p = 1; p < n; ) {
    int lpf = 0, prev = psv[p];
    for (auto &j: {psv[p], nsv[p]}) if (~j) {
      int l = 0;
      while (p + l < n && s[j + l] == s[p + l]) ++l;
      if (l > lpf) lpf = l, prev = j;
    }
    if (lpf) lz.push_back(PII(lpf, prev));
    else lz.push_back(PII(1, s[p]));
    p += std::max(1, lpf);
  }
  return lz;
}
