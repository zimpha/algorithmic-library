#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <map>

struct FastIO {
  static const int S = 65536;
  char buf[S];
  int pos, len;
  bool eof;
  FILE *in;
  FastIO(FILE *_in = stdin) {
    in = _in;
    pos = len = 0;
    eof = false;
  }
  inline int nextChar() {
    if (pos == len)
      pos = 0, len = fread(buf, 1, S, in);
    if (pos == len) {eof = true; return -1;}
    return buf[pos++];
  }
  inline int nextUInt() {
    int c = nextChar(), x = 0;
    while (c <= 32) c = nextChar();
    for (;'0' <= c && c <= '9'; c = nextChar()) x = x * 10 + c - '0';
    return x;
  }
  inline int nextInt() {
    int s = 1, c = nextChar(), x = 0;
    while (c <= 32) c = nextChar();
    if (c == '-') s = -1, c = nextChar();
    for (; '0' <= c && c <= '9'; c = nextChar()) x = x * 10 + c - '0';
    return x * s;
  }
  inline void nextString(char *s) {
    int c = nextChar();
    while (c <= 32) c = nextChar();
    for(; c > 32; c = nextChar()) *s++ = c;
    *s = 0;
  }
} io;

struct ZipTree {
 public:
  int size;
  ZipTree(): size(0), root(0) {}
  void insert(int x) {
    Node* p = new Node(x, 1);
    root = insert(root, p);
    ++size;
  }

  void remove(int x) {
    root = remove(root, x);
  }

  int rank(int x) {
    return rank(root, x) + 1;
  }

  int pred(int x) {
    return pred(root, x);
  }

  int succ(int x) {
    return succ(root, x);
  }

  int kth(int x) {
    return kth(root, x);
  }

  void print() {
    print(root);
    puts("");
  }

  static int rank() {
    int rank = 0;
    while (rand() % 2) ++rank;
    return rank;
  }

 private:
  struct Node {
    int value, cnt;
    int size;
    int rank;
    Node *ch[2];
    Node() {}
    Node(int v, int c): value(v), cnt(c), size(c) {
      ch[0] = ch[1] = NULL;
      rank = ZipTree::rank();
    }
    void update() {
      size = cnt;
      if (ch[0]) size += ch[0]->size;
      if (ch[1]) size += ch[1]->size;
    }
  } pool[100000];

  Node* new_node(int v, int c) {
    pool[size].value = v;
    pool[size].cnt = c;
    return pool + (size++);
  }
  

  void print(Node *o) {
    if (!o) return;
    print(o->ch[0]);
    printf("%d ", o->value);
    print(o->ch[1]);
  }

  // number of elements < x
  int rank(Node* o, int x) {
    if (!o) return 0;
    if (x < o->value) return rank(o->ch[0], x);
    int ls = !o->ch[0] ? 0 : o->ch[0]->size;
    if (o->value == x) return ls;
    else return ls + o->cnt + rank(o->ch[1], x);
  }

  int kth(Node* o, int k) {
    if (!o) return -1;
    int ls = !o->ch[0] ? 0 : o->ch[0]->size;
    if (k <= ls) return kth(o->ch[0], k);
    if (k <= ls + o->cnt) return o->value;
    else return kth(o->ch[1], k - ls - o->cnt);
  }

  int pred(Node* o, int x) {
    if (!o) return -1;
    if (x <= o->value) {
      return pred(o->ch[0], x);
    } else {
      int v = pred(o->ch[1], x);
      if (v == -1) v = o->value;
      return v;
    }
  }

  int succ(Node* o, int x) {
    if (!o) return -1;
    if (x >= o->value) {
      return succ(o->ch[1], x);
    } else {
      int v = succ(o->ch[0], x);
      if (v == -1) v = o->value;
      return v;
    }
  }

  Node* insert(Node* o, Node* c) {
    if (!o) return c;
    if (c->value == o->value) {
      o->cnt += c->cnt;
    } else {
      int d = c->value > o->value;
      if (insert(o->ch[d], c) == c) {
        if (c->rank < o->rank) o->ch[d] = c;
        else {
          o->ch[d] = c->ch[!d];
          c->ch[!d] = o;
          o->update();
          c->update();
          return c;
        }
      }
    }
    o->update();
    return o;
  }

  // make sure value exists
  Node* remove(Node *o, int value) {
    if (value == o->value) {
      --o->cnt;
      if (o->cnt == 0) {
        return zip(o->ch[0], o->ch[1]);
      }
    } else {
      int d = o->value < value;
      o->ch[d] = remove(o->ch[d], value);
    }
    o->update();
    return o;
  }

  Node* zip(Node* x, Node *y) {
    if (!x) return y;
    if (!y) return x;
    if (x->rank < y->rank) {
      y->ch[0] = zip(x, y->ch[0]);
      y->update();
      return y;
    } else {
      x->ch[1] = zip(x->ch[1], y);
      x->update();
      return x;
    }
  }

  Node* root;
};

int main() {
  int n;
  ZipTree zip_tree;
  n = io.nextUInt();
  for (int i = 0; i < n; ++i) {
    int op, x;
    op = io.nextUInt();
    x = io.nextInt();
    if (op == 1) {
      zip_tree.insert(x);
    } else if (op == 2) {
      zip_tree.remove(x);
    } else if (op == 3) {
      printf("%d\n", zip_tree.rank(x));
    } else if (op == 4) {
      printf("%d\n", zip_tree.kth(x));
    } else if (op == 5) {
      printf("%d\n", zip_tree.pred(x));
    } else {
      printf("%d\n", zip_tree.succ(x));
    }
  }
  return 0;
}
