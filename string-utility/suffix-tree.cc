#include <cstring>

/**
 * Ukkonen's Online Construction of Suffix Tree
 **/
namespace SuffixTree {
  static const int SIZE = 100000 + 10, SIGMA = 26, inf = 1e9;
  struct node {
    node *go[26], *link, *fa;
    int st, len;
    node() {}
    void clr() {
      st = 0, len = inf;
      link = fa = NULL;
      memset(go, 0, sizeof(go));
    }
  } pool[SIZE], *rt, *sz;

  struct position {
    node *u;
    int pos;
    position() {}
    position(node* u, int pos): u(u), pos(pos) {}
    void go(int o) {u = u->go[o], pos -= u->len;}
    bool can(int o) {return pos > u->go[o]->len;}
  } last;
  int s[SIZE], n;
  node *new_node(int st, int len) {
    sz->clr();
    sz->st = st, sz->len = len;
    return sz++;
  }
  void init() {
    n = 0, sz = pool;
    rt = new_node(0, inf);
    last = position(rt, 0);
  }
  void add(int c) {
    s[n++] = c;
    last.pos++;
    for (node* p = rt; last.pos; ) {
      while (last.can(s[n - last.pos])) last.go(s[n - last.pos]);
      int o = s[n - last.pos];
      node* &v = last.u->go[o];
      int t = s[v == NULL ? 0 : v->st + last.pos - 1];
      if (v == NULL) {
        v = new_node(n - last.pos, inf);
        p->link = last.u;
        p = rt;
      } else if (t == c) {
        p->link = last.u;
        return;
      } else {// split edge
        node *u = new_node(v->st, last.pos - 1);
        u->go[c] = new_node(n - 1, inf);
        u->go[t] = v;
        v->st += last.pos - 1;
        v->len -= last.pos - 1;
        p->link = v = u;
        p = u;
      }
      if (last.u == rt) --last.pos;
      else last.u = last.u->link ? last.u->link : rt;
    }
  }
}
