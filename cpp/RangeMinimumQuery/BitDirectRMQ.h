#include <memory>
#include <cstdint>
#include <cstring>
#include <vector>
#include <functional>

template <class T, class Compare = std::less<T> >
class BitDirectRMQ {
public:
  static constexpr int block_size = 32;

  void build(const std::vector<T> &a) {
    build(a.data(), a.size());
  }

  void build(const T *a, int n) {
    blocks = (n + block_size - 1) / block_size;
    buildSparseTable(a, n);
    buildInnerBlocks(a, n);
  }

  // return the index of the minimum value in [u, v] in O(1)
  int query(const T *a, int l, int r) const {
    int x = l / block_size, y = r / block_size;
    if (x == y) return query_lookup(l, r);
    int ret = l;
    if (x + 1 <= y - 1) ret = query_st(a, x + 1, y - 1);
    ret = leftMinIndex(a, query_lookup(l, x * block_size + block_size - 1), ret);
    ret = leftMinIndex(a, ret, query_lookup(y * block_size, r));
    return ret;
  }
  
  int query_st(const T *a, int x, int y) const {
    int z = log[y - x + 1];
    return leftMinIndex(a, table[x + z * blocks], table[y - (1 << z) + 1 + z * blocks]);
  }
  int query_lookup(int x, int y) const {
    return x + __builtin_ctz(lookup[y] >> (x % block_size));
  }

private:
  int minIndex(const T *a, int x, int y) const {
    return compare(a[x], a[y]) || (a[x] == a[y] && x < y) ? x : y;
  }

  int leftMinIndex(const T *a, int x, int y) const {
    return compare(a[y], a[x]) ? y : x;
  }

  void buildSparseTable(const T *a, int n) {
    int height = 0;
    while ((1 << height) < blocks) ++height;
    table.resize(blocks * (height + 1));
    log.assign(blocks + 1, -1);
    for (int i = 1; i <= blocks; ++i) {
      log[i] = log[i >> 1] + 1;
    }
    int *u = table.data();
    for (int i = 0, idx = 0; i < n; i += block_size, ++idx) {
      u[idx] = i;
      for (int j = i + 1; j < n && j < i + block_size; ++j) {
        u[idx] = leftMinIndex(a, u[idx], j);
      }
    }
    for (int t = 1; t * 2 < blocks; t *= 2) {
      memcpy(u + blocks, u, sizeof(int) * blocks);
      u += blocks;
      for (int i = 0; i < blocks - t; ++i) {
        u[i] = leftMinIndex(a, u[i], u[i + t]);
      }
    }
  }

  void buildInnerBlocks(const T *a, int n) {
    lookup.assign(n, 0);
    std::vector<int> stack(block_size);
    for (int i = 0; i < blocks; ++i) {
      int l = i * block_size;
      int r = std::min(n, l + block_size);
      uint32_t state = 0;
      for (int j = l, top = 0; j < r; ++j) {
        while (top && compare(a[j], a[stack[top - 1]])) {
          state ^= uint32_t(1) << (stack[top - 1] - l);
          --top;
        }
        stack[top++] = j;
        state ^= uint32_t(1) << (j - l);
        lookup[j] = state;
      }
    }
  }

  std::vector<uint32_t> lookup;
  std::vector<int> log;
  std::vector<int> table;
  int blocks;
  Compare compare;
};
