/**
 * recognize all the subsequences of a string.
 *
 * time complexity: O(n)
 **/
struct SequenceAutomaton{
  static const int SIZE = 100000 + 10;
  struct Node{
    Node *ch[26], *fa;
    Node(): fa(0) {
      memset(ch, 0, sizeof(ch));
    }
  } pool[SIZE], *sz, *rt, *last[27];
  void init() {
    sz = pool;
    last[26] = rt = sz++;
  } 
  void add(int x) {
    Node *p, *np = sz ++;
    for (int i = 0; i <= 26; ++ i) {
      for (p = last[i]; p && !p->ch[x]; p = p->fa) {
        p->ch[x] = np;
      }
    }
    np->fa = last[x];
    last[x] = np;
  }
};
