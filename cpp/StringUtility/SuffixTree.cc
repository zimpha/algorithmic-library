#include <cstdio>
#include <queue>
#include <cstring>
#include <iostream>
#include <algorithm>

const int N = 1e6 + 10, inf = 1e9;

// Ukkonen's Online Construction of Suffix Tree
// https://codeforces.com/blog/entry/16780
struct SuffixTree {
  struct node {
    // Following suffix link will lead to the vertex corresponding to the same substring, but without first character.
    node *go[26], *link, *fa;
    int cnt, st, len;
    node() {}
    void clr() {
      st = cnt = 0;
      len = inf;
      link = fa = NULL;
      memset(go, 0, sizeof(go));
    }
  } pool[N * 2], *rt, *ptr;
  int s[N], n;
  std::queue<node*> leaf;

  struct position {
    node *u; // active node
    int cur; // the position in the string
    int pos; // number of chars needed to insert current suffix
    int rem; // number suffixes not finished yet
    position() {}
    void go(int o) {u = u->go[o]; pos -= u->len; cur += u->len; }
    bool can(int o) {return u->go[o] && pos >= u->go[o]->len;}
  } last;

  node *new_node(node* fa, int st, int len) {
    ptr->clr();
    ptr->fa = fa;
    ptr->st = st;
    ptr->len = len;
    if (fa) fa->cnt++;
    return ptr++;
  }
  void init() {
    n = 0, ptr = pool;
    last.u = rt = new_node(NULL, 0, inf);
    last.cur = last.pos = last.rem = 0;
  }
  void walk() {
    while (last.can(s[last.cur])) {
      last.go(s[last.cur]);
    }
  }
  void follow_link() {
    if (last.u == rt) { if (last.pos) --last.pos, ++last.cur; }
    else last.u = last.u->link ? last.u->link : rt;
    --last.rem;
  }
  void add(int c) {
    s[n++] = c; ++last.rem;
    for (node* p = rt; last.rem; ) {
      if (!last.pos) last.cur = n - 1;
      walk();
      int o = s[last.cur];
      node* &v = last.u->go[o];
      int t = s[v == NULL ? 0 : v->st + last.pos];
      if (v == NULL) {
        v = new_node(last.u, n - 1, inf);
        leaf.push(v);
        p->link = last.u;
        p = rt;
      } else if (t == c) {
        p->link = last.u;
        ++last.pos;
        return;
      } else {// split edge
        node *u = new_node(last.u, v->st, last.pos);
        last.u->cnt--;
        u->go[c] = new_node(u, n - 1, inf);
        leaf.push(u->go[c]);
        u->go[t] = v; ++u->cnt;
        v->fa = u;
        v->st += last.pos;
        v->len -= last.pos;
        p->link = v = u;
        p = u;
      }
      follow_link();
    }
  }
  void del() {
    if (last.pos) walk();
    auto x = leaf.front();
    leaf.pop();
    for (; x != last.u && !x->cnt; x = x->fa) {
      auto p = x->fa;
      p->go[s[x->st]] = NULL;
      p->cnt -= 1;
    }
    if (last.rem && x == last.u) {
      if (!last.pos && !x->cnt) {
        if (x->len != inf) {
          x->st = n - x->len;
          x->len = inf;
        }
        leaf.push(x);
      } else if (last.cur < n && !x->go[s[last.cur]]) {
        auto u = new_node(x, n - last.pos, inf);
        x->go[s[last.cur]] = u;
        leaf.push(u);
      } else {
        return;
      }
      follow_link();
    }
  }
};
