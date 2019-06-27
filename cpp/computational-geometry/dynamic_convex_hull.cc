#include <cmath>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

using flt = long double;
using ll = long long;

const flt error = 1.3, eps = 1e-10, pi = acos(-1.0);
const int N = 100000 + 10;

int sgn(flt x) {return x < -eps ? -1 : x > eps;}
int sgn(ll x) {return x < 0 ? -1 : x > 0;}
flt fix(flt x) {return x < eps ? x + pi * 2 : x;}

struct point {
  ll x, y;
  point(ll x = 0, ll y = 0): x(x), y(y) {}
  point(const point &rhs): x(rhs.x), y(rhs.y) {}
  bool operator < (const point &rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }
  bool operator <= (const point &rhs) const {
    return x < rhs.x || (x == rhs.x && y <= rhs.y);
  }
  bool operator == (const point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  point operator + (const point &rhs) const {
    return point(x + rhs.x, y + rhs.y);
  }
  point operator - (const point &rhs) const {
    return point(x - rhs.x, y - rhs.y);
  }
  ll det(const point &rhs) const {
    return x * rhs.y - y * rhs.x;
  }
  ll dot(const point &rhs) const {
    return x * rhs.x + y * rhs.y;
  }
  flt norm() const {
    return hypot(x, y);
  }
};

struct node {
  int sz;
  point p, ls, rs;
  node *l, *r;

  node() = default;
  node(const point &x): sz(1), p(x), ls(x), rs(x), l(0), r(0) {}
  node* update() {
    sz = 1, ls = rs = p;
    if (r) sz += r->sz, rs = r->rs;
    if (l) sz += l->sz, ls = l->ls;
    return this;
  }
};

bool random(int a, int b) {
  return rand() % (a + b) < a;
}

ll ccw(const point &a, const point &b, const point &c) {
  return (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x);
}

template<class cmp_t>
struct convex_hull {
  cmp_t cmp;
  node *root;
  std::vector<std::pair<point, node*>> roll;
  void init() {
    root = 0;
    roll.clear();
  }
  int get_size(node *o) {
    if (o) return o->sz;
    else return 0;
  }
  node *kth(node *o, int k) {
    int cnt = get_size(o->l);
    if (k == cnt + 1) return o;
    if (k <= cnt) return kth(o->l, k);
    else return kth(o->r, k - cnt - 1);
  }
  template<typename T>
  std::pair<node*, node*> split(node *o, const point &p, T compare) {
    node *l = 0, *r = 0;
    if (!o) return {l, r};
    if (compare(o->p, p)) {
      std::tie(o->r, r) = split(o->r, p, compare);
      l = o;
    } else {
      std::tie(l, o->l) = split(o->l, p, compare);
      r = o;
    }
    o->update();
    return {l, r};
  }
  node* merge(node *l, node *r) {
    if (!l || !r) return l ? l : r;
    if (random(l->sz, r->sz)) {
      l->r = merge(l->r, r);
      return l->update();
    } else {
      r->l = merge(l, r->l);
      return r->update();
    }
  }
  bool has(node *o, const point &p) {
    if (!o) return false;
    if (o->p == p) return true;
    if (o->p < p) return has(o->r, p);
    else return has(o->l, p);
  }
  node *find_left(node *o, const point &p) {
    if (o->l) {
      if (!cmp(ccw(o->l->rs, o->p, p), 0)) {
        return find_left(o->l, p);
      }
    }
    if (o->r) {
      if (cmp(ccw(o->p, o->r->ls, p), 0)) {
        return find_left(o->r, p);
      }
    }
    return o;
  }
  node *find_right(node *o, const point &p) {
    if (o->r) {
      if (!cmp(ccw(p, o->p, o->r->ls), 0)) {
        return find_right(o->r, p);
      }
    }
    if (o->l) {
      if (cmp(ccw(p, o->l->rs, o->p), 0)) {
        return find_right(o->l, p);
      }
    }
    return o;
  }
  void add_point(const point &p) {
    if (has(root, p)) return; 
    if (!root) {
      roll.emplace_back(p, root);
      root = new node(p);
      return;
    }
    if (root->ls < p && p < root->rs) {
      node *left_hull, *right_hull;
      std::tie(left_hull, right_hull) = split(root, p, std::less<point>());
      const point &l = left_hull->rs, &r = right_hull->ls;
      if (cmp(0, ccw(l, r, p))) {
        node *sl = find_left(left_hull, p), *sr = find_right(right_hull, p);
        node *a, *b;
        std::tie(left_hull, a) = split(left_hull, sl->p, std::less_equal<point>());
        std::tie(b, right_hull) = split(right_hull, sr->p, std::less<point>());
        root = merge(left_hull, merge(new node(p), right_hull));
        roll.emplace_back(p, merge(a, b));
        return;
      }
      root = merge(left_hull, right_hull);
      return;
    }
    if (p < root->ls) {
      node *sr = find_right(root, p), *b;
      std::tie(b, root) = split(root, sr->p, std::less<point>());
      roll.emplace_back(p, b);
      root = merge(new node(p), root);
    } else {
      node *sl = find_left(root, p), *a;
      std::tie(root, a) = split(root, sl->p, std::less_equal<point>());
      roll.emplace_back(p, a);
      root = merge(root, new node(p));
    }
  }
  void roll_back(size_t check_point) {
    while (roll.size() > check_point) {
      const point &p = roll.back().first;
      node *tree = roll.back().second;
      node *a, *b;
      std::tie(root, a) = split(root, p, std::less<point>());
      std::tie(a, b) = split(a, p, std::less_equal<point>());
      root = merge(root, merge(tree, b));
      roll.pop_back();
    }
  }
  node *kth(int k) {
    return kth(root, k + 1);
  }
  int size() {
    return get_size(root);
  }
};

convex_hull<std::greater<ll>> upp_hull;
convex_hull<std::less<ll>> low_hull;

struct query_t {
  char type;
  point p;
  int a, b, c;
} queries[N];

inline node* get_point(int i) {
  int n = low_hull.size() + upp_hull.size() - 2;
  i %= n;
  if (i < 0) i += n;
  if (i < low_hull.size()) return low_hull.kth(i);
  else return upp_hull.kth(n - i);
}

inline flt get_angle(const point &a, const point &b) {
  return fix(pi / 2 + atan2(b.y - a.y, b.x - a.x));
}

inline flt get_angle(int i) {
  const point &a = get_point(i)->p;
  const point &b = get_point(i + 1)->p;
  return get_angle(a, b);
}

int get_index(int a, int b) {
  flt angle = fix(pi / 2 + atan2(b, a));
  int left = 0, right = low_hull.size() + upp_hull.size() - 3;
  if (angle <= get_angle(0) || angle >= get_angle(right)) return 0;
  while (left < right) {
    int mid = (left + right - 1) / 2;
    if (get_angle(mid) > angle) right = mid;
    else left = mid + 1;
  }
  return right;
}

std::pair<flt, flt> find(int a, int b, int c, int st, int ed, int side) {
  int n = low_hull.size() + upp_hull.size() - 2;
  int left = 0, right = (ed - st + n) % n;
  while (left < right) {
    int mid = (left + right - 1) / 2;
    const point &p = get_point(st + mid)->p;
    if (sgn(a * p.x + b * p.y + c) * side <= 0) right = mid;
    else left = mid + 1;
  }
  const point &p = get_point(st + left)->p;
  const point &q = get_point(st + left - 1)->p;
  flt d = p.y - q.y, e = q.x - p.x, f = q.y * (p.x - q.x) - q.x * (p.y - q.y);
  return {(c * e - b * f) / (b * d - a * e), (a * f - c * d) / (b * d - a * e)};
}

flt get_intersect(int a, int b, int c, int i, int j) {
  const point &pa = get_point(i)->p;
  const point &pb = get_point(j)->p;
  int sa = sgn(a * pa.x + b * pa.y + c);
  int sb = sgn(a * pb.x + b * pb.y + c);
  if (sa == 0) {
    const point &tp1 = get_point(i + 1)->p;
    if (a * tp1.x + b * tp1.y + c == 0) return (tp1 - pa).norm();
    const point &tp2 = get_point(i - 1)->p;
    if (a * tp2.x + b * tp2.y + c == 0) return (tp2 - pa).norm();
  }
  if (sb == 0) {
    const point &tp1 = get_point(j + 1)->p;
    if (a * tp1.x + b * tp1.y + c == 0) return (tp1 - pb).norm();
    const point &tp2 = get_point(j - 1)->p;
    if (a * tp2.x + b * tp2.y + c == 0) return (tp2 - pb).norm();
  }
  if (sa * sb >= 0) return 0;
  auto p = find(a, b, c, i, j, sa);
  auto q = find(a, b, c, j, i, sb);
  return hypot(p.first - q.first, p.second - q.second);
}

void solve(int l, int r, const std::vector<int> &add, const std::vector<int> &erase_time) {
  int mid = (l + r) / 2;
  int upp_check = upp_hull.roll.size();
  int low_check = low_hull.roll.size();
  std::vector<int> rest;
  for (auto &&st: add) {
    int ed = erase_time[st];
    if (ed <= l || st >= r) continue;
    if (st <= l && ed >= r) {
      upp_hull.add_point(queries[st].p);
      low_hull.add_point(queries[st].p);
    } else rest.push_back(st);
  }
  if (l + 1 == r) {
    if (queries[l].type == '?') {
      if (low_hull.size() + upp_hull.size() <= 2) {
        printf("%.20f\n", 0.0);
      } else {
        int a = queries[l].a, b = queries[l].b, c = queries[l].c;
        int i, j;
        if (!a) i = get_index(1, 0), j = get_index(-1, 0);
        else if (!b) i = get_index(0, 1), j = get_index(0, -1);
        else i = get_index(b, -a), j = get_index(-b, a);
        printf("%.20f\n", (double)get_intersect(a, b, c, i, j));
      }
    }
  } else {
    solve(l, mid, rest, erase_time);
    solve(mid, r, rest, erase_time);
  }
  upp_hull.roll_back(upp_check);
  low_hull.roll_back(low_check);
}

int main() {
  int n;
  scanf("%d", &n);
  std::vector<int> erase_time(n, n);
  std::vector<int> add;
  for (int i = 0; i < n; ++i) {
    scanf(" %c", &queries[i].type);
    if (queries[i].type == '?') {
      scanf("%d%d%d", &queries[i].a, &queries[i].b, &queries[i].c);
    } else {
      int x, y, p;
      if (queries[i].type == '+') {
        scanf("%d%d", &x, &y);
        add.push_back(i);
        queries[i].p = {x, y};
      } else {
        scanf("%d", &p);
        erase_time[p - 1] = i;
      }
    }
  }
  upp_hull.init();
  low_hull.init();
  solve(0, n, add, erase_time);
  return 0;
}
