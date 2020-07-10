#include <memory>
#include <cstring>
#include <vector>
#include <functional>

template <class T, class Compare = std::less<T>>
class DirectRMQ {
public:
  void build(const std::vector<T> &a) {
    build(a.data(), a.size());
  }

  void build(const T *a, int n) {
    block_size = 32 - __builtin_clz(n);
    blocks = (n + block_size - 1) / block_size;
    buildSparseTable(a, n);
    buildInnerBlocks(a, n);
  }

  // return the index of the minimum value in [u, v] in O(1)
  int query(const T *a, int l, int r) const {
    const auto &table = sparse_table;
    int x = l / block_size + 1, y = r / block_size - 1, z, mask;
    int ret = l;
    if (x <= y) {
      z = block_size - lookup[y - x + 1] - 1;
      ret = leftMinIndex(a, table[x + z * blocks], table[y - (1 << z) + 1 + z * blocks]);
    }
    z = x * block_size - 1 < r ? x * block_size - 1 : r;
    mask = ~(((1 << (l - (x - 1) * block_size)) - 1) << (x * block_size - l));
    ret = leftMinIndex(a, (x - 1) * block_size + lookup[blocks_type[z] & mask], ret);
    z = r, l = l > (y + 1) * block_size ? l : (y + 1) * block_size;
    mask = ~(((1 << (l - (y + 1) * block_size)) - 1) << ((y + 2) * block_size -l));
    ret = leftMinIndex(a, ret, (y + 1) * block_size + lookup[blocks_type[z] & mask]);
    return ret;
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
    sparse_table.resize(blocks * height);
    int *u = &sparse_table[0];
    for (int i = 0, idx = 0; height && i < n; i += block_size, ++idx) {
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
    lookup.assign(1 << block_size, 0);
    blocks_type.assign(n, 0);
    std::vector<int> stack(block_size);
    for (int i = 0; i < blocks; ++i) {
      int l = i * block_size;
      int r = std::min(n, l + block_size);
      for (int j = l, top = 0; j < r; ++j) {
        while (top && compare(a[j], a[stack[top - 1]])) --top;
        blocks_type[j] = 1 << (l + block_size - j - 1);
        if (top) {
          blocks_type[j] |= blocks_type[stack[top - 1]];
        }
        stack[top++] = j;
      }
    }
    for (int i = 1, res = block_size - 1; i < (1 << block_size); ++i) {
      if ((1 << (block_size - res)) <= i) --res;
      lookup[i] = res;
    }
  }

  std::vector<int> lookup;
  std::vector<int> blocks_type;
  std::vector<int> sparse_table;
  int block_size, blocks;
  Compare compare;
};
