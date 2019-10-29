#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

static constexpr int N = 1e7;

// simple 2-3-tree
namespace ttt {
struct Node {
  // ch[0] = 0 : leaf node
  // ch[2] = 0 : internal node with 2 children
  // ch[2] != 0: internal node with 3 children
  Node* ch[3];
  int size;
  int height;
  int val;
} pool[N], *ptr;

Node* new_node(int v) {
  ptr->size = 1;
  ptr->height = 0;
  ptr->val = v;
  ptr->ch[0] = 0;
  return ptr++;
}

Node* new_node(std::vector<Node*> nodes) {
  assert(nodes.size() == 2 || nodes.size() == 3);
  for (auto &n: nodes) assert(n->height == nodes[0]->height);
  ptr->height = nodes[0]->height + 1;
  ptr->size = ptr->val = 0;
  memset(ptr->ch, 0, sizeof(ptr->ch));
  for (size_t i = 0; i < nodes.size(); ++i) {
    ptr->ch[i] = nodes[i];
    ptr->size += nodes[i]->size;
  }
  return ptr++;
}
}

// persistent 2-3-tree
namespace pttt {
struct Node {
  // ch[0] = 0 : leaf node
  // ch[2] = 0 : internal node with 2 children
  // ch[2] != 0: internal node with 3 children
  Node* ch[3];
  int size;
  int height;
  int val;
} pool[N], *ptr;

void init() { ptr = pool; }

Node* new_node(int v) {
  ptr->size = 1;
  ptr->height = 0;
  ptr->val = v;
  memset(ptr->ch, 0, sizeof(ptr->ch));
  return ptr++;
}

Node* new_node(std::vector<Node*> nodes) {
  for (auto &n: nodes) assert(n->height == nodes[0]->height);
  ptr->height = nodes[0]->height + 1;
  ptr->size = ptr->val = 0;
  memset(ptr->ch, 0, sizeof(ptr->ch));
  for (size_t i = 0; i < nodes.size(); ++i) {
    ptr->ch[i] = nodes[i];
    ptr->size += nodes[i]->size;
  }
  return ptr++;
}

void submerge(Node* a, Node* b, std::vector<Node*> &v) {
  if (a->height == b->height) {
    v.push_back(a);
    v.push_back(b);
    return;
  }

  std::vector<Node*> nv;
  if (a->height > b->height) {
    if (a->ch[2]) nv = {a->ch[0], a->ch[1]}, a = a->ch[2];
    else nv = {a->ch[0]}, a = a->ch[1];
    nv.reserve(4);
    submerge(a, b, nv);
  } else {
    nv.reserve(4);
    submerge(a, b->ch[0], nv);
    nv.push_back(b->ch[1]);
    if (b->ch[2]) nv.push_back(b->ch[2]);
  }

  if (nv.size() <= 3) {
    v.push_back(new_node(nv));
  } else {
    v.push_back(new_node({nv[0], nv[1]}));
    v.push_back(new_node({nv[2], nv[3]}));
  }
}

Node* merge(Node* a, Node* b) {
  if (a == 0 || b == 0) return a ? a : b;
  std::vector<Node*> v;
  v.reserve(2);
  submerge(a, b, v);
  return v.size() == 1 ? v[0] : new_node(v);
}

Node* merge(std::vector<Node*> &nodes) {
  if (nodes.empty()) return nullptr;
  size_t i = 0;
  while (i + 1 < nodes.size() && nodes[i]->height <= nodes[i + 1]->height) {
    if (i + 2 < nodes.size() && nodes[i]->height == nodes[i + 1]->height && nodes[i + 1]->height == nodes[i + 2]->height) {
      nodes[i + 2] = new_node({nodes[i], nodes[i + 1], nodes[i + 2]});
      i += 2;
    } else {
      nodes[i + 1] = merge(nodes[i], nodes[i + 1]);
      i++;
    }
  }
  nodes.erase(nodes.begin(), nodes.begin() + i);
  while (nodes.size() > 1) {
    auto nr = nodes.back();
    nodes.pop_back();
    auto nl = nodes.back();
    nodes.pop_back();

    if (nodes.size() >= 1 && nodes.back()->height == nl->height && nl->height == nr->height) {
      auto na = nodes.back();
      nodes.pop_back();
      nodes.push_back(new_node({na, nl, nr}));
    } else {
      nodes.push_back(merge(nl, nr));
    }
  }
  return nodes[0];
}

// get nodes between [l, r), index starts with 0
void split_by_size(Node* o, int l, int r, std::vector<Node*> &nodes) {
  if (l <= 0 && r >= o->size) {
    nodes.push_back(o);
    return;
  }
  if (r <= 0 || l >= o->size) return;
  for (int i = 0; i < 2; i++) {
    split_by_size(o->ch[i], l, r, nodes);
    l -= o->ch[i]->size;
    r -= o->ch[i]->size;
  }
  if (o->ch[2]) split_by_size(o->ch[2], l, r, nodes);
}

// get nodes between [l, r), index starts with 0
Node* split_by_size(Node* o, int l, int r) {
  if (!o) return nullptr;
  std::vector<Node*> nodes;
  split_by_size(o, l, r, nodes);
  return merge(nodes);
}

int kth(Node* o, int k) {
  if (!o->ch[0]) return o->val;
  if (o->ch[0]->size >= k) return kth(o->ch[0], k);
  if (o->ch[0]->size + o->ch[1]->size >= k) {
    return kth(o->ch[1], k - o->ch[0]->size);
  }
  return kth(o->ch[2], k - o->ch[0]->size - o->ch[1]->size);
}
}
