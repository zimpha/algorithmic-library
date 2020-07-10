#include <vector>
#include <functional>

template <class T, class Compare = std::less<T>>
class OfflineRMQ {
public:
  // solve a set of queries for minimum value in [l, r]
  std::vector<T> solve(const std::vector<T> &a, const std::vector<int> &L, const std::vector<int> &R) {
    const int n = a.size(), q = L.size();
    std::vector<int> prev(q), head(n, -1), parent(n, -1);
    std::function<int(int)> root = [&](int x) {
      return parent[x] == -1 ? x : parent[x] = root(parent[x]);
    };

    for (int i = 0; i < q; ++i) {
      prev[i] = head[R[i]];
      head[R[i]] = i;
    }
    std::vector<int> stack(n);
    std::vector<T> result(q);
    for (int i = 0, top = 0; i < n; ++i) {
      while (top && compare(a[i], a[stack[top - 1]])) {
        parent[stack[--top]] = i;
      }
      stack[top++] = i;
      for (int it = head[i]; ~it; it = prev[it]) {
        result[it] = a[root(L[it])];
      }
    }
    return result;
  }
private:
  Compare compare;
};
