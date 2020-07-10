#include <set>
#include <algorithm>
#include <functional>

using i64 = long long;

const i64 inf = i64(1) << 62;
const i64 is_query = -inf;

struct Line {// y = m * x + b
  i64 m, b;
  mutable std::function<const Line*()> succ;
  bool operator < (const Line& rhs) const {
    if (rhs.b != is_query) return m < rhs.m;
    const Line* s = succ();
    if (!s) return 0;
    return b - s->b < (s->m - m) * rhs.m;
  }
};
// wiLL maintain lower hull for maximum
// make sure that b * m <= 1e18
struct HuLLDynamic: public std::multiset<Line> {
  bool bad(iterator y) {
    auto z = next(y);
    if (y == begin()) {
      if (z == end()) return 0;
      return y->m == z->m && y->b <= z->b;
    }
    auto x = prev(y);
    if (z == end()) return y->m == x->m && y->b <= x->b;
    return (x->b - y->b) * (z->m - y->m) >= (y->b - z->b) * (y->m - x->m);
  }
  // add a line y = m * x + b
  void add(i64 m, i64 b) {
    auto y = insert({m, b});
    y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
    if (bad(y)) { erase(y); return; }
    while (next(y) != end() && bad(next(y))) erase(next(y));
    while (y != begin() && bad(prev(y))) erase(prev(y));
  }
  // find the argmax(m_i * x + b_i)
  i64 eval(i64 x) {
    auto l = lower_bound((Line){x, is_query});
    if (l == end()) return -inf;
    return l->m * x + l->b;
  }
};

// Copy from https://github.com/kth-competitive-programming/kactl
// faster version of dynamic convex hull trick.
namespace impl_fast {
struct Line { // y = m * x + b
  mutable i64 m, b, p;
  bool operator < (const Line& o) const { return m < o.m; }
  bool operator < (i64 x) const { return p < x; }
};

struct LineContainer : std::multiset<Line, std::less<>> {
  // (for doubles, use inf = 1/.0, div(a,b) = a/b)
  static i64 div(i64 a, i64 b) { // floored division
    return a / b - ((a ^ b) < 0 && a % b);
  }
  bool isect(iterator x, iterator y) {
    if (y == end()) { x->p = inf; return false; }
    if (x->m == y->m) x->p = x->b > y->b ? inf : -inf;
    else x->p = div(y->b - x->b, x->m - y->m);
    return x->p >= y->p;
  }
  // add a line y = m * x + b
  void add(i64 m, i64 b) {
    auto z = insert({m, b, 0}), y = z++, x = y;
    while (isect(y, z)) z = erase(z);
    if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->p >= y->p) {
      isect(x, erase(y));
    }
  }
  // find the argmax(m_i * x + b_i)
  i64 eval(i64 x) {
    auto l = lower_bound(x);
    return l->m * x + l->b;
  }
};
}