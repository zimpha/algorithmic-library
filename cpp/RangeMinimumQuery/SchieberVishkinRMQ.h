#include <vector>
#include <cassert>
#include <functional>

template <class T, class Compare = std::less<T>>
class SchieberVishkinRMQ {
public:
  SchieberVishkinRMQ() = default;

  void build(const std::vector<T> &a) {
    build(a.data(), a.size());
  }

  void build(const T *a, int n) {
    std::vector<int> left(n, -1), right(n, -1);
    std::vector<int> stk(n);
    // build Cartesian Tree
    for (int i = 0, top = 0; i < n; ++i) {
      int last = -1;
      while (top && compare(a[i], a[stk[top - 1]])) {
        last = stk[--top];
      }
      if (top) right[stk[top - 1]] = i;
      left[i] = last;
      stk[top++] = i;
    }

    // find preorder
    int root = stk[0];
    std::vector<int> parents(n, -1), order;
    indices.resize(n), inlabel.resize(n);
    for (int top = 1; top; ) {
      int u = stk[--top];
      order.push_back(u);
      indices[u] = inlabel[u] = order.size();
      if (left[u] != -1) {
        stk[top++] = left[u];
        parents[left[u]] = u;
      }
      if (right[u] != -1) {
        stk[top++] = right[u];
        parents[right[u]] = u;
      }
    }

    // calc helper structures for Schieber-Vishkin LCA
    ascendant.resize(n), head.resize(n);
    for (int i = n - 1; i > 0; --i) {
      int v = order[i], p = parents[v];
      if (lowbit(inlabel[p]) < lowbit(inlabel[v])) {
        inlabel[p] = inlabel[v];
      }
    }
    ascendant[root] = 0;
    for (int i = 1; i < n; ++i) {
      int v = order[i], p = parents[v];
      ascendant[v] = ascendant[p] | lowbit(inlabel[v]);
    }
    head[0] = root;
    for (int i = 1; i < n; ++i) {
      int v = order[i], p = parents[v];
      if (inlabel[v] != inlabel[p]) head[indices[v] - 1] = p;
      else head[indices[v] - 1] = head[indices[p] - 1];
    }
  }

  // return the index of the minimum value in [u, v] in O(1)
  int query(int u, int v) const {
    uint Iv = inlabel[v], Iu = inlabel[u];
    uint hIv = lowbit(Iv), hIu = lowbit(Iu);
    uint mask = highbit((Iv ^ Iu) | hIv | hIu);
    uint j = lowbit(ascendant[v] & ascendant[u] & ~mask);
    int x, y;
    if (j == hIv) x = v;
    else {
      mask = highbit(ascendant[v] & (j - 1));
      x = head[((indices[v] & ~mask) | (mask + 1)) - 1];
    }
    if (j == hIu) y = u;
    else {
      mask = highbit(ascendant[u] & (j - 1));
      y = head[((indices[u] & ~mask) | (mask + 1)) - 1];
    }
    return indices[x] < indices[y] ? x : y;
  }

private:
  using uint = unsigned int;
  static uint lowbit(uint x) {
    return x & (~x + 1); // x & (-x) or x & (x ^ (x - 1))
  }
  static uint highbit(uint v) {
    v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
		return v >> 1;
  }

  Compare compare;
  std::vector<uint> indices;
  std::vector<uint> inlabel;
  std::vector<uint> ascendant;
  std::vector<int> head;
};
