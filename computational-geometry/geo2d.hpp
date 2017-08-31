#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <deque>

/****************************** Basic Geometry ********************************/

using flt = double;
const flt eps = 1e-12, inf = 1e30, PI = acos(-1.0);
template<typename T>
inline T sqr(T x) {return x * x;}
inline flt cmp(flt a, flt b, flt e = eps) {
  return fabs(a - b) >= e + fabs(a) * e ? a - b : 0;
}
inline int sgn(flt x, flt e = eps) {return x < -e ? -1 : (x > e);}
inline flt fix(flt x, flt e = eps) {return cmp(x, 0, e);}

struct point {
  flt x, y;
  point(flt x = 0, flt y = 0): x(x), y(y) {}
  bool operator < (const point &rhs) const {
    return cmp(x, rhs.x) < 0 || (cmp(x, rhs.x) == 0 && cmp(y, rhs.y) < 0);
  }
  bool operator == (const point &rhs) const {
    return cmp(x, rhs.x) == 0 && cmp(y, rhs.y) == 0;
  }
  point operator + (const point &rhs) const {
    return point(x + rhs.x, y + rhs.y);
  }
  point operator - (const point &rhs) const {
    return point(x - rhs.x, y - rhs.y);
  }
  point operator * (const flt k) const {
    return point(x * k, y * k);
  }
  point operator / (const flt k) const {
    return point(x / k, y / k);
  }
  point operator ~ () const {// counter clockwise rotate 90 degree
    return point(-y, x);
  }
  flt dot(const point &rhs) const {
    return x * rhs.x + y * rhs.y;
  }
  flt det(const point &rhs) const {
    return x * rhs.y - y * rhs.x;
  }
  flt norm2() const {
    return x * x + y * y;
  }
  flt norm() const {
    return hypot(x, y);
  }
  point rot(flt a) const {// counter clockwise rotate A rad
    return point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
  }
  point rot(flt cosa, flt sina) const {// counter clockwise rotate using cos/sin
    return point(x * cosa - y * sina, x * sina + y * cosa);
  }
  point trunc(flt a = 1.0) const {
    return (*this) * (a / this->norm());
  }
};

using poly_t = std::vector<point>;

// check if point $O$ is on segment $AB$
bool OnSeg(const point &A, const point &B, const point &O) {
  return sgn((A - O).det(B - O)) == 0 && sgn((A - O).dot(B - O)) <= 0;
}

// check if line $AB$ is parallel to line $CD$
bool IsParallel(const point &A, const point &B, const point &C, const point &D) {
  return sgn((B - A).det(D - C)) == 0;
}

// check intersection of segments $AB$ and $CD$, store intersection in $P$
bool SegInter(const point &A, const point &B, const point &C, const point &D, point &P) {
  if (OnSeg(A, B, C)) {P = C; return true;}
  if (OnSeg(A, B, D)) {P = D; return true;}
  if (OnSeg(C, D, A)) {P = A; return true;}
  if (OnSeg(C, D, B)) {P = B; return true;}
  point AB(B - A), CD(D - C);
  if (sgn(AB.det(CD)) == 0) return false; // parallel
  int d1 = sgn(AB.det(C - A)) * sgn(AB.det(D - A));
  int d2 = sgn(CD.det(A - C)) * sgn(CD.det(B - C));
  P = A + AB * (CD.det(C - A) / CD.det(AB));
  return d1 < 0 && d2 < 0;
}

// check intersection of lines $AB$ and $CD$, store intersection in $P$
bool LineInter(const point &A, const point &B, const point &C, const point &D, point &P) {
  point AB(B - A), CD(D - C);
  if (sgn(AB.det(CD)) == 0) return false; // parallel
  P = A + AB * (CD.det(C - A) / CD.det(AB));
  return true;
}

// find the minimum distance from point $O$ to segment $AB$
flt Dis2Seg(const point &A, const point &B, const point &O) {
  if (sgn((O - A).dot(B - A)) < 0) return (O - A).norm();
  if (sgn((O - B).dot(A - B)) < 0) return (O - B).norm();
  return fabs((O - A).det(B - A) / (A - B).norm());
}

// find the minimum distance from segment $AB$ to segment $CD$
flt DisSeg2Seg(const point &A, const point &B, const point &C, const point &D) {
  point O;
  if (SegInter(A, B, C, D, O)) return 0;
  else return std::min(std::min(Dis2Seg(A, B, C), Dis2Seg(A, B, D)),
                       std::min(Dis2Seg(C, D, A), Dis2Seg(C, D, B)));
}

// move line AB along normal vector
void move_d(point &A, point &B, const flt len) {
  point D = ~(B - A).trunc();
  A = A + (D * len), B = B + (D * len);
}

// project point O on line AB
point project(const point &A, const point &B, const point &O) {
  point AB = B - A;
  return A + AB * (AB.dot(O - A) / AB.norm2());
}

point reflect(const point &A, const point &B, const point &O) {
  point AB = B - A;
  return (A + AB * (AB.dot(O - A) / AB.norm2())) * 2 - O;
}

/************************************ Circle Utilities *****************************************/
// check if two circles intersect, intersection will store in P and Q
bool intersect(const point &A, flt ra, const point &B, flt rb, point &P, point &Q) {
  point AB(B - A);
  double dis = AB.norm();
  if (sgn(dis - (ra + rb)) > 0 || sgn(dis - fabs(ra - rb)) < 0) return false;
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
