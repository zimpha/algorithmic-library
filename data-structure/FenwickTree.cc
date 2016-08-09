#include <vector>

template<class T>
struct FenwickTree : std::vector<T> {// [0, n)
  typedef std::vector<T> S;
  int n;
  FenwickTree(int _n = 0): S(_n), n(_n) {}
  void clr(int _n) {n = _n; assign(n, T());}
  void add(int i, T v) {
    for (; i < n; i |= i + 1) S::operator[](i) += v;
  }
  inline T sum(int r) {// [0, r)
    T s = T();
    for (; r; r &= r - 1) s += S::operator[](r - 1);
    return s;
  }
  T sum(int l, int r) {// [l, r)
    return sum(r) - sum(l);
  }
  int lower_bound(T x) { // argmin_i x <= [0, i]
    int i = 1, j = 0;
    T s = T();
    while (i < n) i <<= 1;
    for (; i; i >>= 1) {
      if (i + j <= n && s + S::operator[](i + j - 1) < x) {
        s += S::operator[](i + j - 1);
        j += i;
      }
    }
    return j;
  }
};

