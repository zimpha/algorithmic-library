#include "../cpp/data-structure/DynamicConvexTrick.cc"
#include <random>
#include <chrono>
#include <cassert>
#include <vector>

void correctness_test(i64 max_m, i64 max_b) {
  printf("Correctness test with m in [%lld %lld], b in [%lld %lld]\n", -max_m, max_m, -max_b, max_b);
  HuLLDynamic hull;
  impl_fast::LineContainer hull_fast;
  std::vector<Line> lines;
  auto query = [&](i64 x) {
    i64 ret = lines[0].m * x + lines[0].b;
    for (auto &l: lines) {
      ret = std::max(ret, l.m * x + l.b);
    }
    return ret;
  };

  std::mt19937 gen{0};
  std::uniform_int_distribution<i64> dis_m(-max_m, max_m);
  std::uniform_int_distribution<i64> dis_b(-max_b, max_b);
  for (int it = 0; it < 5000; ++it) {
    i64 m = dis_m(gen);
    i64 b = dis_b(gen);
    i64 x = dis_b(gen);
    lines.push_back((Line){m, b});
    hull.add(m, b);
    hull_fast.add(m, b);
    
    i64 ret = query(x);
    assert(ret == hull.eval(x));
    assert(ret == hull_fast.eval(x));
  }
}

void stress_test(int n, int q, i64 max_m, i64 max_b) {
  printf("Stress test with %d lines and %d queries, m in [%lld %lld], b in [%lld %lld]\n", n, q, -max_m, max_m, -max_b, max_b);

  std::mt19937 gen{0};
  std::uniform_int_distribution<i64> dis_m(-max_m, max_m);
  std::uniform_int_distribution<i64> dis_b(-max_b, max_b);
  std::vector<std::pair<i64, i64>> lines(n);
  std::vector<i64> query(q);
  std::vector<i64> ans1(q), ans2(q);

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