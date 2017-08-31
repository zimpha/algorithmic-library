#include "geo2d.hpp"

// check point O in polygon P; 0:outside, 2:border, 1:inside
int InPolygon(const poly_t &P, const point &O) {
  int cnt = 0, n = P.size();
  for (int i = 0; i < n; ++i) {
    const point &A = P[i], &B = P[(i + 1) % n];
    if (OnSeg(A, B, O)) return 2;
    int k = sgn((B - A).det(O - A));
    int d1 = sgn(A.y - O.y), d2 = sgn(B.y - O.y);
    cnt += (k > 0 && d1 <= 0 && d2 > 0);
    cnt -= (k < 0 && d2 <= 0 && d1 > 0);
  }
  return cnt != 0;
}

flt Area(const poly_t &P, point &res) {
  flt sum = 0;
  res = point(0, 0);
  int n = P.size();
  for (int i = 0; i < n; ++i) {
    const point &A = P[i], &B = P[(i + 1) % n];
    sum += B.det(A);
    res = res + (A + B) * B.det(A);
  }
  sum = abs(sum);
  res = res / (3.0 * sum);
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
    return Area(ps, center);
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
  flt ang;
  halfplane_t() {}
  halfplane_t(const point &a, const point &b): a(a), b(b) {
    ang = atan2(b.y - a.y, b.x - a.x);
  }
  bool operator < (const halfplane_t &l) const {
    int res = sgn(ang - l.ang);
    return res == 0 ? l.side(a) >= 0: res < 0;
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

poly_t half_plane(std::vector<halfplane_t> v) {
  std::sort(v.begin(), v.end());
  std::deque<halfplane_t> q;
  q.push_back(v[0]);
  std::deque<point> ans;
  for (size_t i = 1; i < v.size(); ++ i) {
    if (sgn(v[i].ang - v[i - 1].ang) == 0) continue;
    while (ans.size() && v[i].side(ans.back()) < 0) ans.pop_back(), q.pop_back();
    while (ans.size() && v[i].side(ans.front()) < 0) ans.pop_front(), q.pop_front();
    ans.push_back(q.back().inter(v[i]));
    q.push_back(v[i]);
  }
  while (ans.size() && q.front().side(ans.back()) < 0) ans.pop_back(), q.pop_back();
  while (ans.size() && q.back().side(ans.front()) < 0) ans.pop_front(), q.pop_front();
  if (q.size() <= 2) return poly_t();
  std::vector<point> pt(ans.begin(), ans.end());
  pt.push_back(q.front().inter(q.back()));
  // these two lines are used to erase duplicated points
  // but the order of point will be changed
  std::sort(pt.begin(), pt.end());
  pt.erase(std::unique(pt.begin(), pt.end()), pt.end());
  return pt;
}
