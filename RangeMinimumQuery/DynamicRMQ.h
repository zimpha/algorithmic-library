#include <vector>
#include <algorithm>

template <class T, T combine(T, T)>
class DynamicRMQ {
public:
  void build(const std::vector<T> &data) {
    build(&data[0], data.size());
  }
  void build(const T *data, int n) {
    u.resize(n * 2);
    for (int i = 0; i < n; ++i) {
      u[i + n] = data[i];
    }
    for (int i = n - 1; i > 0; --i) {
      u[i] = combine(u[i << 1], u[i << 1 | 1]);
    }
  }
  void modify(int p, const T& v) {
    for (u[p += u.size() / 2] = v; p > 1; p >>= 1) {
      u[p >> 1] = combine(u[p], u[p ^ 1]);
    }
  }
  T query(int l, int r, T res = T()) {// [l, r)
    const int n = u.size() >> 1;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) res = combine(res, u[l++]);
      if (r & 1) res = combine(res, u[--r]);
    }
    return res;
  }
private:
  std::vector<T> u;
};
