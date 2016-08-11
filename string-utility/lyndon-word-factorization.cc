#include <vector>
#include <string>
#include <cstring>

/**
 * Duval's Lyndon Factorization Algorithm.
 *
 * the input string s should end with a zero symbol
 * return a list of begins of prime strings
 **/
std::vector<int> duval(char s[]){
  std::vector<int> ret;
  int n = strlen(s) + 1; // zero used here
  int start = 0, mid = 1, cur = 0;
  ret.push_back(0);
  for (int i = 0; i < n; ++i){
    if (s[i] == s[cur]){
      if (++cur == mid) cur = start;
    } else if (s[i] > s[cur]){
      mid = i + 1;
      cur = start;
    } else if (s[i] < s[cur]){
      int temp = mid - start;
      while (start + temp <= i){
        start += temp;
        ret.push_back(start);
      }
      i = cur = start;
      mid = start + 1;
    }
  }
  return ret;
}

/**
 * Duval's Algorithm for Generating Lyndon Words
 *
 * generate the Lyndon words of length at most n with a given alphabet size m
 * in lexicographic order.
 **/
void lyndon_generate(int n, int m) {
  char z = 'a' + m - 1, s[1000];
  s[0] = 'a' - 1;
  for (int i = 1, x = 1; ; ++i) {
    s[x - 1]++; s[x] = 0;
    puts(s);
    for (int j = x; j < n; ++j) s[j] = s[j - x];
    for (x = n; s[x - 1] == z; --x);
  }
}
