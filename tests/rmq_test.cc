#include "../cpp/RangeMinimumQuery/DirectRMQ.h"
#include "../cpp/RangeMinimumQuery/BitDirectRMQ.h"
#include "../cpp/RangeMinimumQuery/DynamicRMQ.h"
#include "../cpp/RangeMinimumQuery/OfflineRMQ.h"
#include "../cpp/RangeMinimumQuery/SchieberVishkinRMQ.h"
#include "../cpp/RangeMinimumQuery/SparseTable.h"

#include <random>
#include <vector>
#include <cassert>
#include <chrono>

inline int min(int a, int b) {
  return std::min(a, b);
}

void correctness_test(int n, int min_m, int max_m) {
  printf("Correctness test with %d numbers in range[%d, %d]\n", n, min_m, max_m);
  std::mt19937 gen{0};
  std::uniform_int_distribution<int> dis_m(-max_m, max_m);
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) a[i] = dis_m(gen);

  std::vector<std::vector<int>> ret(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      if (i == j) ret[i][j] = a[i];
      else ret[i][j] = ret[i][j - 1];
      ret[i][j] = std::min(ret[i][j], a[j]);
    }
  }

  DirectRMQ<int> di_rmq; di_rmq.build(a);
  BitDirectRMQ<int> bdi_rmq; bdi_rmq.build(a);
  SchieberVishkinRMQ<int> sv_rmq; sv_rmq.build(a);
  DynamicRMQ<int, min> dy_rmq; dy_rmq.build(a);
  SparseTable<int, min> st_rmq(min_m); st_rmq.build(a);
  std::vector<int> l, r;
  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      assert(ret[i][j] == a[di_rmq.query(a.data(), i, j)]);
      assert(ret[i][j] == a[bdi_rmq.query(a.data(), i, j)]);
      assert(ret[i][j] == a[sv_rmq.query(i, j)]);
      assert(ret[i][j] == dy_rmq.query(i, j + 1, max_m));
      assert(ret[i][j] == st_rmq.query(i, j + 1));
      l.push_back(i);
      r.push_back(j);
    }  
  }
  OfflineRMQ<int> of_rmq;
  auto of_rmq_ans = of_rmq.solve(a, l, r);
  for (size_t i = 0; i < ret.size(); ++i) {
    assert(ret[l[i]][r[i]] == of_rmq_ans[i]);
  }
}

void stress_test(int n, int q) {
  printf("Stress test with %d numbers in range[-10^9, 10^9] and %d queries\n", n, q);
  std::mt19937 gen{0};
  std::uniform_int_distribution<int> dis_m(-10^9, 10^9);
  std::uniform_int_distribution<int> dis_n(0, n - 1);
  std::vector<int> a(n), l(q), r(q);
  for (int i = 0; i < n; ++i) a[i] = dis_m(gen);

  puts("* test build time with random sequence:");
  auto st = std::chrono::high_resolution_clock::now();
  DirectRMQ<int> di_rmq;
  di_rmq.build(a);
  auto ed = std::chrono::high_resolution_clock::now();
  printf("  - build time for DirectRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

  st = std::chrono::high_resolution_clock::now();
  BitDirectRMQ<int> bdi_rmq;
  bdi_rmq.build(a);
  ed = std::chrono::high_resolution_clock::now();
  printf("  - build time for BitDirectRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());
  
  st = std::chrono::high_resolution_clock::now();
  SchieberVishkinRMQ<int> sv_rmq;
  sv_rmq.build(a);
  ed = std::chrono::high_resolution_clock::now();
  printf("  - build time for SchieberVishkinRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

  st = std::chrono::high_resolution_clock::now();
  DynamicRMQ<int, min> dy_rmq;
  dy_rmq.build(a);
  ed = std::chrono::high_resolution_clock::now();
  printf("  - build time for DynamicRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

  st = std::chrono::high_resolution_clock::now();
  SparseTable<int, min> st_rmq;
  st_rmq.build(a);
  ed = std::chrono::high_resolution_clock::now();
  printf("  - build time for SparseTableRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

  OfflineRMQ<int> of_rmq;
  std::vector<int> di_ans(q), bdi_ans(q), sv_ans(q), dy_ans(q), st_ans(q), of_ans;

  auto query = [&]() {
    st = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < q; ++i) {
      di_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
    }
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for DirectRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

    st = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < q; ++i) {
      bdi_ans[i] = a[bdi_rmq.query(a.data(), l[i], r[i])];
    }
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for BitDirectRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

    st = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < q; ++i) {
      sv_ans[i] = a[sv_rmq.query(l[i], r[i])];
    }
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for SchieberVishkinRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

    st = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < q; ++i) {
      dy_ans[i] = dy_rmq.query(l[i], r[i] + 1, INT32_MAX);
    }
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for DynamicRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

    st = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < q; ++i) {
      st_ans[i] = st_rmq.query(l[i], r[i] + 1);
    }
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for SparseTableRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());

    st = std::chrono::high_resolution_clock::now();
    of_ans = of_rmq.solve(a, l, r);
    ed = std::chrono::high_resolution_clock::now();
    printf("  - query time for OfflineRMQ: %lld ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count());
    for (int i = 0; i < q; ++i) {
      assert(of_ans[i] == st_ans[i] &&
        st_ans[i] == dy_ans[i] && 
        dy_ans[i] == sv_ans[i] && 
        sv_ans[i] == bdi_ans[i] && 
        bdi_ans[i] == di_ans[i]);
    }
  };


  // small interval
  puts("* test random small intervals:");
  for (int i = 0; i < q; ++i) {
    l[i] = dis_n(gen);
    r[i] = std::min(n - 1, l[i] + dis_n(gen) % 40);
  }

  query();

  // random interval
  puts("* test random intervals:");
  for (int i = 0; i < q; ++i) {
    l[i] = dis_n(gen);
    r[i] = dis_n(gen);
    if (l[i] > r[i]) std::swap(l[i], r[i]);
  }

  query();

  // large interval
  puts("* test random large intervals:");
  for (int i = 0; i < q; ++i) {
    l[i] = dis_n(gen) % 10000;
    r[i] = n - 1 - dis_n(gen) % 10000;
    if (l[i] > r[i]) std::swap(l[i], r[i]);
  }
  query();

}

int main() {
  for (auto n: std::vector<int>{1, 2, 4, 8, 16, 32, 127, 510, 512, 1000}) {
    for (auto m: std::vector<int>{1, 2, 4, 8, 16, 32, 64, 128, 512, 1024, 1000000000}) {
      correctness_test(n, -m, m);
    }
  }
  
  for (auto n: std::vector<int>{100000, 1000000, 10000000}) {
    for (auto q: std::vector<int>{100000, 100000, 10000000}) {
      stress_test(n, q);
    }
  }
  return 0;
}
