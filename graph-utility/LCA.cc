#include <vector>

using uint32 = unsigned int;

class SchieberVishkinLCA {
public:
  void build(const std::vector<int> &preorder, const std::vector<int> &parents, int root) {
    const int n = preorder.size();
    indices.resize(n);
    ascendant.resize(n);
    head.resize(n);

    for (int i = 0; i < n; ++i) indices[preorder[i]] = i + 1;
    inlabel.assign(indices.begin(), indices.end());
    for (int i = n - 1; i > 0; --i) {
      int v = preorder[i], p = parents[v];
      if (lowbit(inlabel[p]) < lowbit(inlabel[v])) {
        inlabel[p] = inlabel[v];
      }
    }

    ascendant[root] = 0;
    for (int i = 1; i < n; ++i) {
      int v = preorder[i], p = parents[v];
      ascendant[v] = ascendant[p] | lowbit(inlabel[v]);
    }

    head[0] = root;
    for (int i = 1; i < n; ++i) {
      int v = preorder[i], p = parents[v];
      if (inlabel[v] != inlabel[p]) head[indices[v] - 1] = p;
      else head[indices[v] - 1] = head[indices[p] - 1];
    }
  }

  int query(int u, int v) const {
    uint32 Iv = inlabel[v], Iu = inlabel[u];
		uint32 hIv = lowbit(Iv), hIu = lowbit(Iu);
		uint32 mask = highbit((Iv ^ Iu) | hIv | hIu) - 1;
		uint32 j = lowbit(ascendant[v] & ascendant[u] & ~mask);
		int x, y;
		if (j == hIv) x = v;
		else {
			mask = highbit(ascendant[v] & (j - 1)) - 1;
			x = head[(indices[v] & ~mask | (mask + 1)) - 1];
		}
		if (j == hIu) y = u;
		else {
			mask = highbit(ascendant[u] & (j - 1)) - 1;
			y = head[(indices[u] & ~mask | (mask + 1)) - 1];
		}
		return indices[x] < indices[y] ? x : y;
  }

private:
  static uint32 lowbit(uint32 x) {
    return x & ~x + 1; // x & (-x) or x & (x ^ (x - 1))
  }
  static uint32 highbit(uint32 x) {
    return 1u << (31 - __builtin_clz(x));
  }

  std::vector<uint32> indices;
  std::vector<uint32> inlabel;
  std::vector<uint32> ascendant;
  std::vector<int> head;
};
