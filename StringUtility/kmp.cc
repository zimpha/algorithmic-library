/**
 * fail[i] = max{j | s[0..j] is a proper suffix of s[0..i]}
 * fail[i] = -1, if j does not exist
 * 
 * time complexity: O(n)
 **/
std::vector<int> kmp(char *s, int n) {
  std::vector<int> fail(n, -1);
  for (int i = 1, j = -1; i < n; ++i) {
    while (j >= 0 && s[j + 1] != s[i]) j = fail[j];
    fail[i] = (s[j + 1] == s[i]) ? ++j : j;
  }
  return fail;
}
