#include "geo2d.hpp"
#include <tuple>

// check point o in polygon p; 0:outside, 2:border, 1:inside
int in_polygon(const poly_t &p, const point &o) {
  int cnt = 0, n = p.size();
  for (int i = 0; i < n; ++i) {
    const auto &a = p[i], &b = p[(i + 1) % n];
    if (on_seg(a, b, o)) return 2;
    int k = sgn((b - a).det(o - a));
    int d1 = sgn(a.y - o.y), d2 = sgn(b.y - o.y);
    cnt += (k > 0 && d1 <= 0 && d2 > 0);
    cnt -= (k < 0 && d2 <= 0 && d1 > 0);
  }
  return cnt != 0;
}

// find the area of the polygon and store the centroid in o
flt area(const poly_t &p, point &o) {
  flt sum = 0;
  o = {0, 0};
  int n = p.size();
  for (int i = 0; i < n; ++i) {
    const auto &a = p[i], &b = p[(i + 1) % n];
    sum += b.det(a);
    o = o + (a + b) * b.det(a);
  }
  sum = std::abs(sum);
  o = o / (3.0 * sum);
  return sum * 0.5;
}

// area of union of polygons
flt polygon_union(poly_t poly[], int n) {
  auto ratio = [](const point &A, const point &B, const point &O) {
    if (sgn(A.x - B.x) == 0) return (O.y - A.y) / (B.y - A.y);
    else return (O.x - A.x) / (B.x - A.x);
  };
  flt ret = 0;
  for (int i = 0; i < n; ++i) {
    for (size_t v = 0; v < poly[i].size(); ++v) {
      point A = poly[i][v], B = poly[i][(v + 1) % poly[i].size()];
      std::vector<std::pair<flt, int> > segs;
      segs.push_back(std::make_pair(0.0, 0));
      segs.push_back(std::make_pair(1.0, 0));
      for (int j = 0; j < n; ++j) if (i != j) {
        for (size_t u = 0; u < poly[j].size(); ++u) {
          point C = poly[j][u], D = poly[j][(u + 1) % poly[j].size()];
          int sc = sgn((B - A).det(C - A)), sd = sgn((B - A).det(D - A));
          if (sc == 0 && sd == 0) {
            if (sgn((B - A).dot(D - C)) > 0 && i > j) {
              segs.push_back(std::make_pair(ratio(A, B, C), +1));
              segs.push_back(std::make_pair(ratio(A, B, D), -1));
            }
          } else {
            flt sa = (D - C).det(A - C), sb = (D - C).det(B - C);
            if (sc >= 0 && sd < 0) segs.push_back(std::make_pair(sa / (sa - sb), 1));
            else if (sc < 0 && sd >= 0) segs.push_back(std::make_pair(sa / (sa - sb), -1));
          }
        }
      }
      std::sort(segs.begin(), segs.end());
      flt pre = std::min(std::max(segs[0].first, 0.0), 1.0), now, sum = 0;
      int cnt = segs[0].second;
      for (size_t j = 1; j < segs.size(); ++j) {
        now = std::min(std::max(segs[j].first, 0.0), 1.0);
        if (!cnt) sum += now - pre;
        cnt += segs[j].second;
        pre = now;
      }
      ret += A.det(B) * sum;
    }
  }
  return ret / 2;
}

struct ConvexHull {
  poly_t ps;
  // build the convex from a set of point
  // Time Complexity: O(n log n)
  void build(poly_t &u) {
    std::sort(u.begin(), u.end());
    u.erase(std::unique(u.begin(), u.end()), u.end());
    if (u.size() < 3u) {
      ps = u;
      return;
    }
    for (size_t i = 0, o = 1, m = 1; ~i; i += o) {
      while (ps.size() > m) {
        point A = ps.back() - ps[ps.size() - 2];
        point B = ps.back() - u[i];
        if (sgn(A.det(B)) < 0) break;
        ps.pop_back();
      }
      ps.push_back(u[i]);
      if (i + 1 == u.size()) m = ps.size(), o = -1;
    }
    ps.pop_back();
  }
  // return the diameter of convex, also the ending points.
  // Time Complexity: O(n)
  flt diameter(int &first, int &second) const {
    int si = 0, sj = 0, n = ps.size();
    for (int i = 0; i < n; ++i) {
      if (ps[si].x > ps[i].x) si = i;
      if (ps[sj].x < ps[i].x) sj = i;
    }
    first = si, second = sj;
    flt mx = 0;
    for (int i = si, j = sj; i != sj || j != si; ) {
      flt tmp = (ps[i] - ps[j]).norm();
      if (tmp > mx) mx = tmp, first = i, second = j;
      if ((ps[(i + 1) % n] - ps[i]).det(ps[(j + 1) % n] - ps[j]) < 0) i = (i + 1) % n;
      else j = (j + 1) % n;
    }
    return mx;
  }
  // return the area of the convex and the mass center.
  // Time Complexity: O(n)
  flt area(point &center) const {
    return ::area(ps, center);
  }
  // check whether P is inside the convex, {0, 2, 1} : {outside, border, inside}.
  // Time Complexity: O(log n)
  int contain(const point &P) const {
  }
  // return the index of two tangent points looking from P
  // Time Complexity: O(log n)
  std::pair<int, int> tangent(const point &P) const {
  }
  // return the length of the intersection with line $AB$
  // Time Complexity: O(log n)
  flt intersection(const point &A, const point &B) {
  }
  // return the intersection of two convex hulls
  // Time Complexity: O(n)
  ConvexHull intersection(const ConvexHull &other) const {
  }
};

// intersection of half plane
struct halfplane_t {
  point a, b; // left side of vector \vec{ab}, i.e. \vec{ab} \times \vec{ap} > 0
  halfplane_t() {}
  halfplane_t(const point &a, const point &b): a(a), b(b) {}
  int cmp(const halfplane_t &l) const {
    auto u = b - a, v = l.b - l.a;
    int suy = sgn(u.y), svy = sgn(v.y), du = 1, dv = 1;
    if (suy < 0 || (suy == 0 && sgn(u.x) > 0)) du = -1;
    if (svy < 0 || (svy == 0 && sgn(v.x) > 0)) dv = -1;
    if (du == dv) return sgn(v.det(u));
    else return du < dv ? -1 : 1;
  }
  bool operator < (const halfplane_t &l) const {
    int res = cmp(l);
    return res == 0 ? l.side(a) > 0: res < 0;
  }
  int side(const point &p) const {// 1: left, 0: on, -1:right
    return sgn((b - a).det(p - a));
  }
  point inter(const halfplane_t &l) const {
    flt k = (l.a - l.b).det(a - l.b);
    k = k / (k - (l.a - l.b).det(b - l.b));
    return a + (b - a) * k;
  }
};

poly_t halfplane(std::vector<halfplane_t> v) {
  std::sort(v.begin(), v.end());
  std::deque<halfplane_t> q;
  q.push_back(v[0]);
  std::deque<point> ans;
  for (size_t i = 1; i < v.size(); ++ i) {
    if (v[i].cmp(v[i - 1]) == 0) continue;
    while (ans.size() && v[i].side(ans.back()) < 0) ans.pop_back(), q.pop_back();
    while (ans.size() && v[i].side(ans.front()) < 0) ans.pop_front(), q.pop_front();
    ans.push_back(q.back().inter(v[i]));
    q.push_back(v[i]);
  }
  while (ans.size() && q.front().side(ans.back()) < 0) ans.pop_back(), q.pop_back();
  while (ans.size() && q.back().side(ans.front()) < 0) ans.pop_front(), q.pop_front();
  if (q.size() <= 2) return {};
  std::vector<point> pt(ans.begin(), ans.end());
  pt.push_back(q.front().inter(q.back()));
  return pt;
}

template <class P>
std::vector<std::tuple<int, int, int>> triangulation(const std::vector<P> &poly) {
  int n = poly.size();
  if (n == 3) return {std::make_tuple(0, 1, 2)};
  std::vector<int> eras, reflex;
  std::vector<int> prev(n), next(n);
  std::vector<bool> is_era(n), del(n);
  for (int i = 0; i < n; ++i) {
    int a = (i - 1 + n) % n, b = i, c = (i + 1) % n;
    prev[i] = a, next[i] = c;
    if ((poly[b] - poly[a]).det(poly[c] - poly[a]) < 0) reflex.push_back(i);
  }

  auto check = [&] (int a, int b, int c) {
    auto &A = poly[a], &B = poly[b], &C = poly[c];
    auto AB = B - A, BC = C - B, CA = A - C;
    for (auto &v: reflex) if (v != a && v != b && v != c) {
      auto AV = poly[v] - A, BV = poly[v] - B, CV = poly[v] - C;
      if (AB.det(AV) >= 0 && BC.det(BV) >= 0 && CA.det(CV) >= 0) return false;
    }
    return true;
  };

  for (int i = 0; i < n; ++i) {
    int a = (i - 1 + n) % n, b = i, c = (i + 1) % n;
    if ((poly[b] - poly[a]).det(poly[c] - poly[a]) < 0) continue;
    if (check(a, b, c)) {
      is_era[i] = true;
      eras.push_back(i);
    }
  }

  std::vector<std::tuple<int, int, int>> ret;
  while (!eras.empty()) {
    int b = eras.back(); eras.pop_back();
    if (!is_era[b] || del[b]) continue;
    int a = prev[b], c = next[b];
    next[a] = c; prev[c] = a;
    del[b] = true; --n;
    is_era[a] = is_era[c] = false;
    ret.emplace_back(a, b, c);
    if (n == 3) {
      ret.emplace_back(prev[a], a, c);
      break;
    }
    int pa = prev[a], nc = next[c];
    if ((poly[a] - poly[pa]).det(poly[c] - poly[pa]) > 0 && check(pa, a, c)) {
      eras.push_back(a); is_era[a] = true;
    }
    if ((poly[c] - poly[a]).det(poly[nc] - poly[a]) > 0 && check(a, c, nc)) {
      eras.push_back(c); is_era[c] = true;
    }
  }
  return ret;
}
