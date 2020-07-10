#include <vector>

/**
 * Sparse Table Algorithm for Range Minimum Query
 *
 * time complexity: O(n log n) ~ O(1)
 **/
template <class T, T combine(T, T)>
class SparseTable {
 public:
  SparseTable(T e = T()): e(e) {}

  void build(const std::vector<T> &a) {
    build(a.data(), a.size());
  }
  
  void build(const T a[], int n) {
    if (n == 0) return;
    int lg = 32 - __builtin_clz(n);
    u.assign(lg, std::vector<T>(n));
    for (int i = 0; i < n; ++i) u[0][i] = a[i];
    for (int s = 1, m = 1; m * 2 <= n; ++s, m <<= 1) {
      for (int i = 0; i + m < n; ++i) {
        u[s][i] = combine(u[s - 1][i], u[s - 1][i + m]);
      }
    }
  }
  // find the minimum value in range [l, r)
  T query(int l, int r) {
    if (l == r) return e;
    int k = 31 - __builtin_clz(r - l);
    return combine(u[k][l], u[k][r - (1 << k)]);
  }

 private:
  std::vector<std::vector<T>> u;
  T e; // unit value
};
