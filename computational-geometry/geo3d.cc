#include <cmath>
#include <cstdio>

typedef double flt;

const flt eps = 1e-8;
int sgn(flt x) {return x < -eps ? -1 : x > eps;}

struct Point {
  flt x, y, z;
  Point() {}
  Point(flt x, flt y, flt z): x(x), y(y), z(z) {}
  bool operator < (const Point &rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y) || (x == rhs.x && y == rhs.y && z < rhs.z);
  }
  Point operator + (const Point &rhs) const {
    return Point(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  Point operator - (const Point &rhs) const {
    return Point(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  Point operator * (const flt k) const {
    return Point(x * k, y * k, z * k);
  }
  Point operator / (const flt k) const {
    return Point(x / k, y / k, z / k);
  }
  flt dot(const Point &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }
  Point det(const Point &rhs) const {
    return Point(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
  }
  flt sqrlen() const {
    return x * x + y * y + z * z;
  }
  flt len() const {
    return sqrt(sqrlen());
  }
  void read() {
    scanf("%lf%lf%lf", &x, &y, &z);
  }
  void print() {
    printf("%.10f %.10f %.10f\n", x, y, z);
  }
} A, B, C, D;

// 判断点C是否在直线AB上
bool dotsInline(const Point &A, const Point &B, const Point &C) {
  return sgn((B - A).det(C - A).len()) == 0;
}

// 判断直线AB和CD是否平行
bool is_parallel(const Point &A, const Point &B, const Point &C, const Point &D) {
  return sgn((B - A).det(D - C).len()) == 0;
}

// 判断直线AB和CD的交点, 如果直线异面, 返回的是两直线的垂线在AB上的垂足
// 需要保证AB和CD不平行.
Point intersect(const Point &A, const Point &B, const Point &C, const Point &D) {
  Point p0 = (C - A).det(D - C), p1 = (B - A).det(D - C);
  return A + (B - A) * (p0.dot(p1) / p1.sqrlen());
}
