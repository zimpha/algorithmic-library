/**
 * Directed acyclic subsequence graph 
 *   - recognize all the subsequences of a string.
 *
 * time complexity: O(n * \sigma)
 **/
#include <cstring>
#include <algorithm>

struct DASG {
  static const int SIZE = 100000 + 10, SIGMA = 26;
  struct Node{
    Node *ch[SIGMA];
    Node() {memset(ch, 0, sizeof(ch));}
  } pool[SIZE], *sz, *rt, *last[SIGMA];
  void init() {
    sz = pool; rt = sz++;
    std::fill(last, last + SIGMA, rt);
  } 
  void add(int x) {
    Node *np = sz++;
    for (Node *p = last[x]; p < np; ++p) {
      if (!p->ch[x]) p->ch[x] = np;
    }
    last[x] = np;
  }
};
