const int MAXN = 250000 + 10;
struct Node {
  Node *ch[26], *fa;
  int val, sc;
  Node(): fa(0), val(0), sc(0) {
    memset(ch, 0, sizeof(ch));
  }
  void clear() {
    memset(ch, 0, sizeof(ch));
    fa = 0; val = sc = 0;
  }
} pool[MAXN << 1], *rt, *last;
namespace SAM {
  Node *sz = pool;
  void init() {
    if (sz != pool) {
      for (Node *p = pool; p < sz; ++ p) p->clear();
    }
    sz = pool; rt = sz ++; last = rt;
  }
  void add(int c) {
    Node *p = last, *np = sz ++;
    last = np; np->val = p->val + 1;
    for (; p && !p->ch[c]; p = p->fa) p->ch[c] = np;
    if (p) {
      Node *q = p->ch[c];
      if (p->val + 1 == q->val) np->fa = q;
      else {
        Node *nq = sz ++; *nq = *q;
        nq->val = p->val + 1;
        q->fa = nq; np->fa = nq;
        for (; p && p->ch[c] == q; p = p->fa) p->ch[c] = nq;
      }
    }
    else np->fa = rt;
  }
  void getRight(char *s, int n) {
    static Node* Q[MAXN << 1];
    static int cnt[MAXN];
    for (int i = 0; i <= n; ++ i) cnt[i] = 0;
    for (Node *p = pool; p < sz; ++ p) p->sc = 0, cnt[p->val] ++;
    for (int i = 1; i <= n; ++ i) cnt[i] += cnt[i - 1];
    for (Node *p = pool; p < sz; ++ p) Q[-- cnt[p->val]] = p;
    for (Node *p = rt; ; p = p->ch[s[p->val] - 'a']) {
      p->sc = 1; if (p->val == n) break;
    }
    for (int i = (sz - pool) - 1; i >= 0; -- i) {
      Node *p = Q[i]; if (p->fa) p->fa->sc += p->sc;
    }
  }
  void build(char *s, int n) {
    init();
    for (int i = 0; i < n; ++ i) add(s[i] - 'a');
    getRight(s, n);
  }
}
