#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <deque>

/****************************** Basic Geometry ********************************/

using flt = double;
using int64 = long long;

const flt eps = 1e-8, pi = acos(-1.0);

template <class T>
inline int sgn(T x, T e = eps) { return x < -e ? -1 : x > e; }
template <class T>
inline T sqr(T x) { return x * x; }
template <class T>
inline T cmp(T a, T b, T e = eps) {
  return std::abs(a - b) >= e + std::abs(a) * e ? a - b : 0;
}
inline flt fix(flt x, flt e = eps) {return cmp(x, 0., e);}

template <class T>
struct Point {
  T x, y;
  Point(T x = 0, T y = 0): x(x), y(y) {}
  bool operator < (const Point &o) const { return x < o.x || (x == o.x && y < o.y); }
  bool operator == (const Point &o) const { return x == o.x && y == o.y; }
  Point operator + (const Point &o) const { return Point(x + o.x, y + o.y); }
  Point operator - (const Point &o) const { return Point(x - o.x, y - o.y); }
  Point operator * (T k) const { return Point(x * k, y * k); }
  Point operator / (T k) const { return Point(x / k, y / k); }
  T dot(const Point &o) const { return x * o.x + y * o.y; }
  T det(const Point &o) const { return x * o.y - y * o.x; }
  T norm2() const { return x * x + y * y; }
  flt norm() const { return hypot(x, y); }
  flt ang() const { return atan2(y, x); }
  Point perp() const { return Point(-y, x); } // rotate 90 degrees
  Point unit() const { return *this / norm(); }
  Point trunc(flt k) const { return unit() * k; }
  // counter clockwise rotate a rad
  Point rot(flt a) const {
    return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
  }
  // counter clockwise rotate using cos/sin
  Point rot(flt cosa, flt sina) const {
    return Point(x * cosa - y * sina, x * sina + y * cosa);
  }
};

using point = Point<flt>;
using poly_t = std::vector<point>;

template <class P>
bool on_seg(const P &a, const P &b, const P &o) {
  return sgn((a - o).det(b - o)) == 0 && sgn((a - o).dot(b - o)) <= 0;
}

template <class P>
bool is_parallel(const P &a, const P &b, const P &c, const P &d) {
  return sgn((b - a).det(d - c)) == 0;
}

// find intersection of segments ab and cd, stored in p
template <class P>
bool seg_inter(const P &a, const P &b, const P &c, const P &d, P &p) {
  if (on_seg(a, b, c)) return p = c, true;
  if (on_seg(a, b, d)) return p = d, true;
  if (on_seg(c, d, a)) return p = a, true;
  if (on_seg(c, d, b)) return p = b, true;
  P ab{b - a}, cd{d - c};
  if (sgn(ab.det(cd)) == 0) return false; // parallel
  int d1 = sgn(ab.det(c - a)) * sgn(ab.det(d - a));
  int d2 = sgn(cd.det(a - c)) * sgn(cd.det(b - c));
  p = a + ab * (cd.det(c - a) / cd.det(ab));
  return d1 < 0 && d2 < 0;
}

// find intersection of lines ab and cd, stored in p
template <class P>
bool line_inter(const P &a, const P &b, const P &c, const P &d, P &p) {
  P ab{b - a}, cd{d - c};
  if (sgn(ab.det(cd)) == 0) return false; // parallel
  p = a + ab * (cd.det(c - a) / cd.det(ab));
  return true;
}

// minimum distance from o to segment ab
template <class P>
flt dis2seg(const P &a, const P &b, const P &o) {
  P ao{o - a}, bo{o - b}, ab{b - a};
  if (sgn(ao.dot(ab)) < 0) return ao.norm();
  if (sgn(-bo.dot(ab)) < 0) return bo.norm();
  return std::abs(ao.det(ab)) / ab.norm();
}

// find the minimum distance from segment ab to segment cd
template <class P>
flt dis_seg2seg(const P &a, const P &b, const P &c, const P &d) {
  P o;
  if (seg_inter(a, b, c, d, o)) return 0;
  else return std::min(std::min(dis2seg(a, b, c), dis2seg(a, b, d)),
                       std::min(dis2seg(c, d, a), dis2seg(c, d, b)));
}

// move line AB along normal vector
void move_d(point &a, point &b, const flt len) {
  auto d = (b - a).perp().trunc(len);
  a = a + d, b = b + d;
}

// project point o on line ab
point project(const point &a, const point &b, const point &o) {
  auto ab = b - a;
  return a + ab * (ab.dot(o - a) / ab.norm2());
}

// find the reflect point of o with respect to line ab
point reflect(const point &a, const point &b, const point &o) {
  auto ab = b - a;
  return (a + ab * (ab.dot(o - a) / ab.norm2())) * 2 - o;
}

/************************************ Circle Utilities *****************************************/
// check if two circles intersect, intersection will store in P and Q
bool intersect(const point &A, flt ra, const point &B, flt rb, point &P, point &Q) {
  point AB(B - A);
  double dis = AB.norm();
  if (sgn(dis) == 0 || sgn(dis - (ra + rb)) > 0 || sgn(dis - fabs(ra - rb)) < 0) return false;
  flt cosa = (dis * dis + ra * ra - rb * rb) / (2 * dis * ra);
  flt sina = sqrt(fix(1.0 - cosa * cosa));
  AB = AB * (ra / dis);
  P = AB.rot(cosa, +sina) + A;
  Q = AB.rot(cosa, -sina) + A;
  return true;
}
// check if line AB intesect circle O, intersection will store in P and Q
bool intersect(const point &A, const point &B, const point &O, flt r, point &P, point &Q) {
  point AB(B - A), AO(O - A);
  double dis = fabs(AB.det(AO)) / AB.norm();
  if (sgn(dis - r) > 0) return false;
  point M = A + AB * (AB.dot(AO) / AB.norm2());
  dis = sqrt(fix(r * r - dis * dis));
  AB = AB / AB.norm() * dis;
  P = M + AB, Q = M - AB;
  return true;
}
