#include <bits/stdc++.h>

struct AhoCorasick {
  static const int MAXL = 1e5 + 10, C = 26;
  struct node {
    node *go[C], *link;
    bool mark;
  } pool[MAXL], *rt;
  int sz;
  node* alloc() {
    memset(&pool[sz], 0, sizeof(pool[sz]));
    return &pool[sz++];
  }
  void init() {
    sz = 0;
    rt = alloc();
  }
  void ins(const char *s) {
    node *p = rt;
    for (int i = 0; s[i]; ++i) {
      int o = s[i] - 'a';
      if (!p->go[o]) p->go[o] = alloc();
      p = p->go[o];
    }
    p->mark = true;
  }
  void build() {
    std::queue<node*> queue;
    for (int o = 0; o < C; ++o) {
      if (rt->go[o]) {
        rt->go[o]->link = rt;
        queue.push(rt->go[o]);
      } else {
        rt->go[o] = rt;
      }
    }
    for (; !queue.empty(); queue.pop()) {
      node* u = queue.front();
      u->mark |= u->link->mark;
      for (int o = 0; o < C; ++o) {
        if (u->go[o]) {
          u->go[o]->link = u->link->go[o];
          queue.push(u->go[o]);
        } else {
          u->go[o] = u->link->go[o];
        }
      }
    }
  }
  bool find(const char *s) {
    node *p = rt;
    for (int i = 0; s[i]; ++i) {
      int o = s[i] - 'a';
      p = p->go[o];
      if (p->mark) return true;
    }
    return false;
  }
};

struct AhoCorasickMap {
  static const int MAXL = 1e5;
  struct node {
    std::map<char, node*> go;
    node *link;
    bool mark;
    node* next(char o) {
      if (go.count(o)) return go[o];
      else if (link) return link->next(o);
      else return this;
    }
  } pool[MAXL], *rt;
  int sz;
  node* alloc() {
    pool[sz].go.clear();
    pool[sz].link = 0;
    pool[sz].mark = 0;
    return &pool[sz++];
  }
  void init() {
    sz = 0;
    rt = alloc();
  }
  void ins(const char *s) {
    node *p = rt;
    for (int i = 0; s[i]; ++i) {
      char o = s[i];
      if (!p->go.count(o)) p->go[o] = alloc();
      p = p->go[o];
    }
    p->mark = true;
  }
  void build() {
    std::queue<node*> queue;
    for (auto &x: rt->go) {
      x.second->link = rt;
      queue.push(x.second);
    }
    for (; !queue.empty(); queue.pop()) {
      node* u = queue.front();
      u->mark |= u->link->mark;
      for (auto &x: u->go) {
        x.second->link = u->link->next(x.first);
        queue.push(x.second);
      }
    }
  }
  bool find(const char *s) {
    node *p = rt;
    for (int i = 0; s[i]; ++i) {
      char o = s[i];
      p = p->next(o);
      if (p->mark) return true;
    }
    return false;
  }
};
