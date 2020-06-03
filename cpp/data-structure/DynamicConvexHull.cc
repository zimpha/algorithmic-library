#include <set>
#include <chrono>
#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <functional>

using int64 = long long;

const int64 inf = int64(1) << 62;
const int64 is_query = -inf;

struct Line {// y = m * x + b
  int64 m, b;
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
  void add(int64 m, int64 b) {
    auto y = insert({m, b});
    y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
    if (bad(y)) { erase(y); return; }
    while (next(y) != end() && bad(next(y))) erase(next(y));
    while (y != begin() && bad(prev(y))) erase(prev(y));
  }
  int64 eval(int64 x) {
    auto l = lower_bound((Line){x, is_query});
    if (l == end()) return -inf;
    return l->m * x + l->b;
  }
};

// copy from https://github.com/kth-competitive-programming/kactl
// add lines of the form m * x + b, query maximum value at point x.
namespace impl_fast {
  struct Line { // y = m * x + b
    mutable int64 m, b, p;
    bool operator < (const Line& o) const { return m < o.m; }
    bool operator < (int64 x) const { return p < x; }
  };

  struct LineContainer : std::multiset<Line, std::less<>> {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    static int64 div(int64 a, int64 b) { // floored division
      return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
      if (y == end()) { x->p = inf; return false; }
      if (x->m == y->m) x->p = x->b > y->b ? inf : -inf;
      else x->p = div(y->b - x->b, x->m - y->m);
      return x->p >= y->p;
    }
    void add(int64 m, int64 b) {
      auto z = insert({m, b, 0}), y = z++, x = y;
      while (isect(y, z)) z = erase(z);
      if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
      while ((y = x) != begin() && (--x)->p >= y->p) {
        isect(x, erase(y));
      }
    }
    int64 eval(int64 x) {
      auto l = lower_bound(x);
      return l->m * x + l->b;
    }
  };
}

void correctness_test(int64 max_m, int64 max_b) {
  printf("Correctness test with m in [%lld %lld], b in [%lld %lld]\n", -max_m, max_m, -max_b, max_b);
  HuLLDynamic hull;
  impl_fast::LineContainer hull_fast;
  std::vector<Line> lines;
  auto query = [&](int64 x) {
    int64 ret = lines[0].m * x + lines[0].b;
    for (auto &l: lines) {
      ret = std::max(ret, l.m * x + l.b);
    }
    return ret;
  };

  std::mt19937 gen{0};
  std::uniform_int_distribution<int64> dis_m(-max_m, max_m);
  std::uniform_int_distribution<int64> dis_b(-max_b, max_b);
  for (int it = 0; it < 5000; ++it) {
    int64 m = dis_m(gen);
    int64 b = dis_b(gen);
    int64 x = dis_b(gen);
    lines.push_back((Line){m, b});
    hull.add(m, b);
    hull_fast.add(m, b);
    
    int64 ret = query(x);
    assert(ret == hull.eval(x));
    assert(ret == hull_fast.eval(x));
  }
}

void stress_test(int n, int q, int64 max_m, int64 max_b) {
  printf("Correctness test with %d lines and %d queries, m in [%lld %lld], b in [%lld %lld]\n", n, q, -max_m, max_m, -max_b, max_b);

  std::mt19937 gen{0};
  std::uniform_int_distribution<int64> dis_m(-max_m, max_m);
  std::uniform_int_distribution<int64> dis_b(-max_b, max_b);
  std::vector<std::pair<int64, int64>> lines(n);
  std::vector<int64> query(q);
  std::vector<int64> ans1(q), ans2(q);

  HuLLDynamic hull;
  auto st1 = std::chrono::high_resolution_clock::now();
  for (auto &l: lines) hull.add(l.first, l.second);
  for (int i = 0; i < q; ++i) ans1[i] = hull.eval(query[i]);
  auto ed1 = std::chrono::high_resolution_clock::now();

  impl_fast::LineContainer hull_fast;
  auto st2 = std::chrono::high_resolution_clock::now();
  for (auto &l: lines) hull_fast.add(l.first, l.second);
  for (int i = 0; i < q; ++i) ans1[i] = hull_fast.eval(query[i]);
  auto ed2 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < q; ++i) assert(ans1[i] == ans2[i]);

  printf("  time elapsed: %lld ms vs %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed1 - st1).count(), std::chrono::duration_cast<std::chrono::milliseconds>(ed2 - st2).count());
}

int main() {
  correctness_test(10, 10);
  correctness_test(100, 100);
  correctness_test(1000, 1000);
  correctness_test(10000, 10000);
  correctness_test(100000, 100000);
  correctness_test(1000000, 1000000);
  correctness_test(10000000, 10000000);
  correctness_test(100000000, 100000000);
  correctness_test(1000000000, 1000000000);
  correctness_test(1000000, 10000000);
  correctness_test(1000000, 100000000);
  correctness_test(1000000, 1000000000);
  correctness_test(1000000, 10000000000ll);
  correctness_test(1000000, 100000000000ll);
  correctness_test(1000000, 1000000000000ll);

  stress_test(1000000, 1000000, 1000000, 1000000000000ll);
  stress_test(10000000, 10000000, 1000000, 1000000000000ll);
  stress_test(1000000, 1000000, 1000000000, 1000000000);
  stress_test(10000000, 10000000, 1000000000, 1000000000);
  stress_test(1000000, 1000000, 100000, 100000);
  stress_test(10000000, 10000000, 100000, 100000);
  return 0;
}
