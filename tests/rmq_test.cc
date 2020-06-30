#include "../cpp/RangeMinimumQuery/DirectRMQ.h"
#include "../cpp/RangeMinimumQuery/BitDirectRMQ.h"
#include "../cpp/RangeMinimumQuery/DynamicRMQ.h"
#include "../cpp/RangeMinimumQuery/OfflineRMQ.h"
#include "../cpp/RangeMinimumQuery/SchieberVishkinRMQ.h"

inline int min(int a, int b) {
  return std::min(a, b);
}

int main() {
  clock_t st, ed;
  srand(2333);
  const int n = 10000000, q = 10000000;
  std::vector<int> a(n), l(q), r(q);
  for (int i = 0; i < n; ++i) a[i] = rand();
  printf("rmq benchmark for n = %d, q = %d\n", n, q);

  st = clock();
  DirectRMQ<int> di_rmq;
  di_rmq.build(a);
  ed = clock();
  printf("build time for DirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  BitDirectRMQ<int> bdi_rmq;
  bdi_rmq.build(a);
  ed = clock();
  printf("build time for BitDirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);
  
  st = clock();
  SchieberVishkinRMQ<int> sv_rmq;
  sv_rmq.build(a);
  ed = clock();
  printf("build time for SchieberVishkinRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  DynamicRMQ<int, min> dy_rmq;
  dy_rmq.build(a);
  ed = clock();
  printf("build time for DynamicRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  OfflineRMQ<int> of_rmq;

  std::vector<int> di_ans(q), bdi_ans(q), sv_ans(q), dy_ans(q), of_ans;

  // small interval
  puts("test small intervals:");
  for (int i = 0; i < q; ++i) {
    l[i] = rand() % n;
    r[i] = std::min(n - 1, l[i] + rand() % 40);
  }
  st = clock();
  for (int i = 0; i < q; ++i) {
    di_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for DirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    bdi_ans[i] = a[bdi_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for BitDirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    sv_ans[i] = a[sv_rmq.query(l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for SchieberVishkinRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    dy_ans[i] = dy_rmq.query(l[i], r[i] + 1, INT32_MAX);
  }
  ed = clock();
  printf(" - query time for DynamicRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  of_ans = of_rmq.solve(a, l, r);
  ed = clock();
  printf(" - query time for OfflineRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  for (int i = 0; i < q; ++i) {
    int mx = a[l[i]];
    for (int j = l[i]; j <= r[i]; ++j) {
      mx = std::min(mx, a[j]);
    }
    assert(di_ans[i] == mx);
    assert(di_ans[i] == bdi_ans[i]);
    assert(bdi_ans[i] == sv_ans[i]);
    assert(sv_ans[i] == dy_ans[i]);
    assert(dy_ans[i] == of_ans[i]);
  }

  // random interval
  puts("test random queries:");
  for (int i = 0; i < q; ++i) {
    l[i] = rand() % n;
    r[i] = rand() % n;
    if (l[i] > r[i]) std::swap(l[i], r[i]);
  }

  st = clock();
  for (int i = 0; i < q; ++i) {
    di_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for DirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    bdi_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for BitDirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    sv_ans[i] = a[sv_rmq.query(l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for SchieberVishkinRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    dy_ans[i] = dy_rmq.query(l[i], r[i] + 1, INT32_MAX);
  }
  ed = clock();
  printf(" - query time for DynamicRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  of_ans = of_rmq.solve(a, l, r);
  ed = clock();
  printf(" - query time for OfflineRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  for (int i = 0; i < q; ++i) {
    assert(di_ans[i] == bdi_ans[i] && bdi_ans[i] == sv_ans[i] && sv_ans[i] == dy_ans[i] && dy_ans[i] == of_ans[i]);
  }

  // large interval
  puts("test large intervals:");
  for (int i = 0; i < q; ++i) {
    l[i] = rand() % 1000;
    r[i] = n - 1 - rand() % 1000;
    if (l[i] > r[i]) std::swap(l[i], r[i]);
  }
  for (int i = 0; i < q; ++i) {
    l[i] = rand() % n;
    r[i] = std::min(n - 1, l[i] + rand() % 40);
  }
  st = clock();
  for (int i = 0; i < q; ++i) {
    di_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for DirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    bdi_ans[i] = a[di_rmq.query(a.data(), l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for BitDirectRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    sv_ans[i] = a[sv_rmq.query(l[i], r[i])];
  }
  ed = clock();
  printf(" - query time for SchieberVishkinRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  for (int i = 0; i < q; ++i) {
    dy_ans[i] = dy_rmq.query(l[i], r[i] + 1, INT32_MAX);
  }
  ed = clock();
  printf(" - query time for DynamicRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  st = clock();
  of_ans = of_rmq.solve(a, l, r);
  ed = clock();
  printf(" - query time for OfflineRMQ: %.3f sec\n", double(ed - st) / CLOCKS_PER_SEC);

  for (int i = 0; i < q; ++i) {
    assert(di_ans[i] == bdi_ans[i] && bdi_ans[i] == sv_ans[i] && sv_ans[i] == dy_ans[i] && dy_ans[i] == of_ans[i]);
  }
  return 0;
}
